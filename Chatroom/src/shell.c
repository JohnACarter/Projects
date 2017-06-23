/* CSci4061 S2016 Assignment 2
* section: 1
* date: 03/11/2016
* name: John Carter, James Anthony Bruno, Alexander Vo
* id: 4436856, 4515423, 4950571 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include "util.h"
#define MAX_BUFFER 256
/*
 * Read a line from stdin.
 */
char *sh_read_line(void)
{
	char *line = NULL;
	ssize_t bufsize = 0;

	getline(&line, &bufsize, stdin);
	return line;
}

void segfault(void) {
	int *ptr = NULL;
	*ptr = 1;
}

/*
 * Do stuff with the shell input here.
 */
int sh_handle_input(char *line, int fd_toserver)
{
	char *line_copy = (char*)malloc(MSG_SIZE);
	strcpy(line_copy, line);
	char *command = strtok(line,"  ");
	if(starts_with(command, CMD_SEG)) {
		segfault();
	} else {
		if(write(fd_toserver, (void *)line_copy, strlen(line_copy)) < 0){
			perror("Failed to write");
		}
	}
	
	/* Write message to server for processing */
	free((void *)line_copy);
	return 0;
}

int main(int argc, char **argv)
{	char *buf;
	user_chat_box_t user;
	strcpy(user.name,argv[1]);
	int c_read = atoi(argv[2]);
	int p_write = atoi(argv[3]);
	if(fcntl(c_read, F_SETFL, O_NONBLOCK) < 0) {
		perror("Nonblock pipe not set");
	}	
	user.child_pid = fork();
	if(user.child_pid == -1){
		perror("fork");
		exit(1);
	} else if(user.child_pid == 0){
		close(c_read);
		while(1) {
			print_prompt(user.name);
			buf = sh_read_line();
			usleep(1000);
			if(!is_empty(buf)){	
				sh_handle_input(buf, p_write);
			}
		}
	} else {
		char msg[MSG_SIZE];
		sprintf(msg, "\\child_pid %d\n", user.child_pid); // Let the server know the pid of the child within shell
		sh_handle_input(msg, p_write);
		close(p_write);
		int status;
		while(1) {
			usleep(1000);
			if(waitpid(user.child_pid, &status, WNOHANG) > 0) { // I go to seek a Great Perhaps
				exit(1);
			}
			char rbuf[1024] = "";
			while(read(c_read,rbuf,MSG_SIZE) > 0) {
				printf("%s\n",rbuf);
			}
		}
	}
}
