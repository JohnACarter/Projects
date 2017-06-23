/* csci4061 S2016 Assignment 4 
* date: 05/02/16 
* names:
* 	John Carter, carte668, 4436856
*	James Anthony Bruno, bruno049, 4515423
*	Alexander Vo, voxxx188, 4950571
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "util.h"

#define MAX_THREADS 100
#define MAX_QUEUE_SIZE 100
#define MAX_REQUEST_LENGTH 64
#define BUFF_SIZE 1024
#define MAX_CACHE_SIZE 100

int port, num_dispatchers, num_workers, queue_length, cache_size, active_dispatchers;
char *path;
pthread_t *dispatchers;
pthread_t *workers;
FILE *logfile;

//Structure for queue.
typedef struct request_queue {
	int m_socket;
	char m_szRequest[MAX_REQUEST_LENGTH];
} request_queue_t;

/*
 * An actual queue. The last one is a misnomer
 */
typedef struct request_queue_proto {
	request_queue_t *requests;
	int *slots;
	int start_pointer;
	int end_pointer;
	int length;
	int full;
	int empty;
} request_q;

/*
 * Cache slot
 */
typedef struct cache_slot_proto {
	char path[MAX_REQUEST_LENGTH];
	long size;
	char *mime;
	char *buff;
	time_t last_requested;
	time_t mtime;
	int deleted;
} cache_slot;

typedef struct cache_proto {
	int empty_slot;
	int length;
	cache_slot *slots;
} request_cache;
request_cache cache;

pthread_mutex_t queue_access = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t active_dispatchers_l = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t log_write = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cache_access = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t queue_empty = PTHREAD_COND_INITIALIZER;
request_q queue;

/*
 * Add request to queue. Will wait until queue isn't full.
 * Returns:
 *	index of added request
 */
int queue_add(request_q *q, request_queue_t req) {
	pthread_mutex_lock(&queue_access);
	while(q->full) {
		pthread_cond_wait(&queue_full, &queue_access);
	}
	int index = q->end_pointer;
	q->requests[index] = req;
	q->slots[index] = 1;
	q->end_pointer = (q->end_pointer + 1) % q->length;
	if(q->slots[q->end_pointer]) {
		q->full = 1;
	}
	q->empty = 0;
	pthread_cond_signal(&queue_empty);
	pthread_mutex_unlock(&queue_access);
	return index;
}

/*
 * Remove a request from queue. Will wait until queue isn't empty.
 * Returns:
 *	former index of removed request
 *	-1 if the dispatcher should exit
 */
int queue_remove(request_q *q, request_queue_t *req) {
	pthread_mutex_lock(&queue_access);
	while(q->empty && active_dispatchers > 0) {
		pthread_cond_wait(&queue_empty, &queue_access);
	}
	if(active_dispatchers == 0 && q->empty) {
		pthread_mutex_unlock(&queue_access);
		return -1;
	}
	int index = q->start_pointer;
	*req = q->requests[index];
	q->slots[index] = 0;
	q->full = 0;
	q->start_pointer = (q->start_pointer + 1) % q->length;
	if(!q->slots[q->start_pointer]) {
		q->empty = 1;
	}
	pthread_cond_signal(&queue_full);
	pthread_mutex_unlock(&queue_access);
	return index;
}

/*
 * Grabs the cache slot corresponding to the path.
 * Returns:
 * 	index of slot
 *	-1 if slot not found
 */
int cache_grab(request_cache *c, char *path, cache_slot *slot) {
	pthread_mutex_lock(&cache_access);
	int i;
	int m = (c->empty_slot == -1) ? c->length : c->empty_slot;
	for(i = 0; i < m; i++) {
		if(c->slots[i].deleted) {
			continue;
		}
		if(strcmp(c->slots[i].path, path) == 0) {
			c->slots[i].last_requested = time(NULL);
			*slot = c->slots[i];
			pthread_mutex_unlock(&cache_access);
			return i;
		}
	}
	pthread_mutex_unlock(&cache_access);
	return -1;
}

/*
 * Places cache slot in cache.
 * If there are empty slots, replaces one.
 * If not, replaces the slot that was requested the longest time ago.
 * Returns:
 * 	index of new slot
 */
int cache_place(request_cache *c, cache_slot slot) {
	pthread_mutex_lock(&cache_access);
	int index;
	if(c->empty_slot != -1) {
		index = c->empty_slot;
		c->slots[index] = slot;
		c->empty_slot++;
		if(c->empty_slot == c->length) {
			c->empty_slot = -1;
		}
	} else {
		time_t oldest;
		int i;
		for(i = 0; i < c->length; i++) {
			if(c->slots[i].deleted) {
				index = i;
				break;
			}
			if(oldest > c->slots[i].last_requested) {
				oldest = c->slots[i].last_requested;
				index = i;
			}
		}
		free((void *)c->slots[index].buff);
		c->slots[index] = slot;
	}
	pthread_mutex_unlock(&cache_access);
	return index;
}

/*
 * Shorthand to create a relative path
 */
void relative_path(char *dest, char *src) {
	if(src[0] != '/') {
		strcpy(dest, src);
	} else {
		strcpy(dest, ".");
		strcat(dest, src);
	}
}

/*
 * Threadsafe logging. If size is -1, will output error in its place.
 */
void safe_log(FILE *logfile, int thread_id, int req_id, int fd, char *path, int size, char *error, time_t ms, int hit) {
	pthread_mutex_lock(&log_write);
	if(size == -1) {
		fprintf(logfile, "[%d][%d][%d][%s][%s]\n", thread_id, req_id, fd, path, error);
	} else {
		fprintf(logfile, "[%d][%d][%d][%s][%d][%dms][%s]\n", thread_id, req_id, fd, path, size, ms, hit ? "HIT" : "MISS");
	}
	fflush(logfile);
	pthread_mutex_unlock(&log_write);
}

/*
 * Grab the thread id from the thread array
 */
int get_id(pthread_t t_id, pthread_t *search, int search_len) {
	int id;
	for(id = 0; id < search_len; id++) {
		if(search[id] == t_id) {
			return id;
		}
	}
	return -1;
}

/*
 * Adds incoming requests to the queue.
 */
void * dispatch(void * arg) {
	pthread_t t_id = pthread_self();
	int id = get_id(t_id, dispatchers, num_dispatchers);
	while(1) {
		request_queue_t req;
		if((req.m_socket = accept_connection()) < 0) {
			fprintf(stderr, "Closing dispatcher #%d\n", id);
			pthread_mutex_lock(&active_dispatchers_l);
			active_dispatchers--;
			if(active_dispatchers == 0) {
				pthread_cond_broadcast(&queue_empty);
			}
			pthread_mutex_unlock(&active_dispatchers_l);
			pthread_exit(NULL);
			return NULL;
		}
		char *buff = malloc(BUFF_SIZE);
		get_request(req.m_socket, buff);
		strcpy(req.m_szRequest, buff);
		queue_add(&queue, req);
		printf("+ [%d][%d][%s]\n", id, req.m_socket, buff);
		free((void *)buff);
	}
	return NULL;
}

/*
 * Processes and serves requests
 */
void * worker(void * arg) {
	char *error;
	pthread_t t_id = pthread_self();
	int id = get_id(t_id, workers, num_workers);
	int count = 0;
	while(active_dispatchers > 0 || !queue.empty) {
		usleep(300);
		request_queue_t req;
		int status = queue_remove(&queue, &req);
		if(status == -1) {
			printf("Closing worker #%d\n", id);
			pthread_exit(NULL);
			break;
		}
		struct timeval stop, start;
		gettimeofday(&start, NULL);
		count++;
		int cache_index = -1;
		cache_slot slot;
		char *fpath = malloc(sizeof(char) * (MAX_REQUEST_LENGTH + 1));
		relative_path(fpath, req.m_szRequest);
		struct stat req_stat;
		int stat_status = stat(fpath, &req_stat);
		int slot_index;
		if(cache_size != 0 && (slot_index = cache_grab(&cache, req.m_szRequest, &slot)) >= 0) {
			if(stat_status != 0 || !S_ISREG(req_stat.st_mode)) {
				cache.slots[slot_index].deleted = 1;
				free((void *)fpath);
				error = "Invalid file";
				fprintf(stderr, "- [%d][%d][%d][%s][%s]\n", id, count, req.m_socket, req.m_szRequest, error);
				safe_log(logfile, id, count, req.m_socket, req.m_szRequest, -1, error, 0, 0);
				return_error(req.m_socket, error);
				continue;
			}
			if(req_stat.st_mtime > slot.mtime) {
				FILE *freq;
				if((freq = fopen(fpath, "rb")) == NULL) {
					cache.slots[slot_index].deleted = 1;
					free((void *)fpath);
					fclose(freq);
					error = "Invalid file";
					fprintf(stderr, "- [%d][%d][%d][%s][%s]\n", id, count, req.m_socket, req.m_szRequest, error);
					safe_log(logfile, id, count, req.m_socket, req.m_szRequest, -1, error, 0, 0);
					return_error(req.m_socket, error);
					continue;
				}
				cache.slots[slot_index].deleted = 1;
				cache_slot new_slot;
				strcpy(new_slot.mime, slot.mime);
				strcpy(new_slot.path, slot.path);
				new_slot.last_requested = slot.last_requested;
				new_slot.mtime = req_stat.st_mtime;
				new_slot.deleted = 0;
				fseek(freq, 0L, SEEK_END);
				new_slot.size = ftell(freq);
				new_slot.buff = malloc(sizeof(char) * (new_slot.size + 1));
				rewind(freq);
				fread(new_slot.buff, sizeof(char), new_slot.size, freq);
				fclose(freq);
				new_slot.buff[new_slot.size] = 0;
				cache_place(&cache, new_slot);
				if(return_result(req.m_socket, new_slot.mime, new_slot.buff, new_slot.size) != 0) {
					free((void *)fpath);
					error = "Can't return result";
					fprintf(stderr, "- [%d][%d][%d][%s][%s]\n", id, count, req.m_socket, req.m_szRequest, error);
					safe_log(logfile, id, count, req.m_socket, req.m_szRequest, -1, error, 0, 0);
					return_error(req.m_socket, error);
					continue;
				}
				gettimeofday(&stop, NULL);
				printf("- [%d][%d][%d][%s][%d][%d][MISS]\n", id, count, req.m_socket, req.m_szRequest, new_slot.size, stop.tv_usec - start.tv_usec);
				safe_log(logfile, id, count, req.m_socket, req.m_szRequest, new_slot.size, NULL, stop.tv_usec - start.tv_usec, 0);
				free((void *)fpath);
			} else {
				if(return_result(req.m_socket, slot.mime, slot.buff, slot.size) != 0) {
					free((void *)fpath);
					error = "Can't return result";
					fprintf(stderr, "- [%d][%d][%d][%s][%s]\n", id, count, req.m_socket, req.m_szRequest, error);
					safe_log(logfile, id, count, req.m_socket, req.m_szRequest, -1, error, 0, 0);
					return_error(req.m_socket, error);
					continue;
				}
				gettimeofday(&stop, NULL);
				printf("- [%d][%d][%d][%s][%d][%d][HIT]\n", id, count, req.m_socket, req.m_szRequest, slot.size, stop.tv_usec - start.tv_usec);
				safe_log(logfile, id, count, req.m_socket, req.m_szRequest, slot.size, NULL, stop.tv_usec - start.tv_usec, 1);
				free((void *)fpath);
			}
		} else {
			FILE *freq;
			if(stat_status != 0 || !S_ISREG(req_stat.st_mode) || (freq = fopen(fpath, "rb")) == NULL) {
				free((void *)fpath);
				error = "Invalid file";
				fprintf(stderr, "- [%d][%d][%d][%s][%s]\n", id, count, req.m_socket, req.m_szRequest, error);
				safe_log(logfile, id, count, req.m_socket, req.m_szRequest, -1, error, 0, 0);
				return_error(req.m_socket, error);
				continue;
			}
			slot.deleted = 0;
			strcpy(slot.path, req.m_szRequest);
			slot.last_requested = time(NULL);
			slot.mtime = req_stat.st_mtime;
			fseek(freq, 0L, SEEK_END);
			slot.size = ftell(freq);
			slot.buff = malloc(sizeof(char) * (slot.size + 1));
			rewind(freq);
			fread(slot.buff, sizeof(char), slot.size, freq);
			fclose(freq);
			slot.buff[slot.size] = 0;
			char *ext = strrchr(fpath, '.');
			if(strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) {
				slot.mime = "text/html";
			} else if(strcmp(ext, ".jpg") == 0) {
				slot.mime = "image/jpeg";
			} else if(strcmp(ext, ".gif") == 0) {
				slot.mime = "image/gif";
			} else {
				slot.mime = "text/plain";
			}
			if(cache_size > 0) {
				cache_place(&cache, slot);
			}
			if(return_result(req.m_socket, slot.mime, slot.buff, slot.size) != 0) {
				free((void *)fpath);
				error = "Can't return result";
				fprintf(stderr, "- [%d][%d][%d][%s][%s]\n", id, count, req.m_socket, req.m_szRequest, error);
				safe_log(logfile, id, count, req.m_socket, req.m_szRequest, -1, error, 0, 0);
				return_error(req.m_socket, error);
				continue;
			}
			gettimeofday(&stop, NULL);
			printf("- [%d][%d][%d][%s][%d][%d][MISS]\n", id, count, req.m_socket, req.m_szRequest, slot.size, stop.tv_usec - start.tv_usec);
			safe_log(logfile, id, count, req.m_socket, req.m_szRequest, slot.size, NULL, stop.tv_usec - start.tv_usec, 0);
			free((void *)fpath);
		}
	}
	return NULL;
}

/*
 * Is the path a valid directory?
 * Returns:
 *	0 if invalid
 *	1 if valid
 */
int is_valid_directory(char *path) {
	struct stat pathStat;
	return stat(path, &pathStat) >= 0 && S_ISDIR(pathStat.st_mode);
}

/*
 * Is the path a valid file?
 * Returns:
 *	0 if invalid
 *	1 if valid
 */
int is_valid_file(char *path) {
	struct stat pathStat;
	return stat(path, &pathStat) >= 0 && S_ISREG(pathStat.st_mode);
}

int main(int argc, char **argv) {
	//Error check first.
	if(argc != 7) {
		printf("usage: %s port path num_dispatchers num_workers queue_length cache_size\n", argv[0]);
		return -1;
	}
	// Grab arguments and check values
	port = atoi(argv[1]);
	if(port < 1025 || 65535 < port) {
		fprintf(stderr, "port must be within the range 1025 to 65535\n");
		return -1;
	}
	if((logfile = fopen("web_server_log", "w")) == NULL) {
		fprintf(stderr, "Error creating web server log\n");
		return -1;
	}
	fflush(logfile);
	path = argv[2];
	if(!is_valid_directory(path) || chdir(path) != 0) {
		fclose(logfile);
		fprintf(stderr, "%s not a valid directory\n", path);
		return -1;
	}
	num_dispatchers = atoi(argv[3]);
	if(num_dispatchers < 1 || MAX_THREADS < num_dispatchers) {
		fclose(logfile);
		fprintf(stderr, "num_dispatchers must be at least 1 and at most %d\n", MAX_THREADS);
		return -1;
	}
	active_dispatchers = num_dispatchers;
	num_workers = atoi(argv[4]);
	if(num_workers < 1 || MAX_THREADS < num_workers) {
		fclose(logfile);
		fprintf(stderr, "num_workers must be at least 1 and at most %d\n", MAX_THREADS);
		return -1;
	}
	queue_length = atoi(argv[5]);
	if(queue_length < 1 || MAX_QUEUE_SIZE < queue_length) {
		fclose(logfile);
		fprintf(stderr, "queue_length must be within the range 1 to %d\n", MAX_QUEUE_SIZE);
		return -1;
	}
	cache_size = atoi(argv[6]);
	if(cache_size < 0 || MAX_CACHE_SIZE < cache_size) {
		fclose(logfile);
		fprintf(stderr, "cache_size must be within the range 0 to %d\n", MAX_CACHE_SIZE);
		return -1;
	}
	if(cache_size != 0) {
		cache.slots = malloc(sizeof(cache_slot) * cache_size);
		cache.empty_slot = 0;
		cache.length = cache_size;
	}
	queue.requests = malloc(sizeof(request_queue_t) * queue_length);
	queue.slots = malloc(sizeof(int) * queue_length);
	int j;
	for(j = 0; j < queue_length; j++) {
		queue.slots[j] = 0;
	}
	queue.start_pointer = 0;
	queue.end_pointer = 0;
	queue.full = 0;
	queue.empty = 1;
	queue.length = queue_length;
	// printf("Call init() first and make a dispather and worker threads\n");
	init(port);
	int i;
	// Create dispatcher threads
	dispatchers = malloc(sizeof(pthread_t) * num_dispatchers);
	workers = malloc(sizeof(pthread_t) * num_workers);
	printf("Starting web server...\n");
	for(i = 0; i < num_dispatchers; i++) {
		if(pthread_create(&(dispatchers[i]), NULL, dispatch, NULL) != 0){
			free((void *)queue.requests);
			free((void *)dispatchers);
			free((void *)workers);
			free((void *)queue.slots);
			fclose(logfile);
			fprintf(stderr, "Error creating dispatcher thread\n");
			return -1;
		}
	}
	// Create worker threads
	for(i = 0; i < num_workers; i++) {
		if(pthread_create(&(workers[i]), NULL, worker, NULL) != 0){
			free((void *)queue.requests);
			free((void *)dispatchers);
			free((void *)workers);
			free((void *)queue.slots);
			fclose(logfile);
			fprintf(stderr, "Error creating worker thread\n");
			return -1;
		}
	}
	printf("Worker syntax:\n\t+ [worker id][fd][path]\nDispatcher syntax:\n\t- [dispatcher id][request number][fd][path][bytes or error message][ms][HIT or MISS]\n");
	// Join dispatchers
	for(i = 0; i < num_dispatchers; i++) {
		if(pthread_join(dispatchers[i], NULL) != 0) {
			free((void *)queue.requests);
			free((void *)dispatchers);
			free((void *)workers);
			free((void *)queue.slots);
			fclose(logfile);
			fprintf(stderr, "Error joining dispatcher thread\n");
			return -1;
		}
	}
	// Join workers
	for(i = 0; i < num_workers; i++) {
		if(pthread_join(workers[i], NULL) != 0) {
			free((void *)queue.requests);
			free((void *)dispatchers);
			free((void *)workers);
			free((void *)queue.slots);
			fclose(logfile);
			fprintf(stderr, "Error joining worker thread\n");
			return -1;
		}
	}
	free((void *)queue.requests);
	free((void *)dispatchers);
	free((void *)workers);
	free((void *)queue.slots);
	fclose(logfile);
	return 0;
}
