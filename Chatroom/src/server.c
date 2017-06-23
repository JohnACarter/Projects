/* CSci4061 S2016 Assignment 2
 * name: John Carter, James Anthony Bruno
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>
#include "util.h"

user_chat_box_t users[MAX_USERS];

int total_users = 0;

int kill_if_exists(pid_t pid) {
	int k = kill(pid, 0); // dry run of kill
	if(k != -1) {
		return kill(pid, 9);
	}
	return 0;
}

/*
 * Shorthand to send user errors.
 */
void user_error(char *msg, int fd) {
	char send[MSG_SIZE];
	sprintf(send, "<ERROR: %s>\n", msg);
	
	if(dprintf(fd, send, strlen(msg) + 1) < 0)
		perror("Error message send failed");
}

/*
 * Utility function.
 * Find user index for given user name.
 */
int find_user_index(user_chat_box_t *users, char *name) {
	int i; 
	user_idx = -1;

	if (name == NULL) {
		fprintf(stderr, "NULL name passed.\n");
		return user_idx;
	}
	
	for (i = 0; i < MAX_USERS; i++) {
		if (users[i].status == SLOT_EMPTY)
			continue;
		if (strncmp(users[i].name, name, strlen(name)) == 0) {
			user_idx = i;
			break;
		}
	}

	return user_idx;
}

/*
 * Who's alive?
 */
user_chat_box_t * active_users(user_chat_box_t *users, user_chat_box_t *active) {
	int a = 0;
	int u = 0;
	
	for(; u < MAX_USERS; u++) {
		if(users[u].status == SLOT_FULL) {
			active[a] = users[u];
			a++;
		}
	}
	
	return active;
}

/*
 * List the existing users on the server shell
 */
int list_users(user_chat_box_t *users, int fd) {
	int status;
	
	if(total_users == 0) {
		char *msg = "<no users>\n";
		
	if((status = write(fd, msg, strlen(msg))) < 0) {
			perror("Unable to write");
		}
		
	return status;
	} 
	else {
		user_chat_box_t active[total_users];
		active_users(users, active);
		int status;
		int u = 0;
		int count = 0;
		
	for(; u < total_users; u++) {
			char msg [1024] = "";
			
			if(count == 0) {
				sprintf(msg,"\n\n%s",active[u].name);
				count++;
			} 
			else {
				sprintf(msg,"%s",active[u].name);
			}
			status = write(fd, msg, sizeof(msg));
		}
		
	return status;
	}
}

/*
 * Add a new user--check that max users(10) hasn't been reached, create non-blocking pipe for child process,
 * set child status to SLOT_FULL, keep track of total users, fork a shell process in a new xterm.
 */
int add_user(user_chat_box_t *users, char *buf, int server_fd)
{	
	char *add_u = (char*)malloc(MSG_SIZE);
	int wait_status;
	
	if (total_users >= MAX_USERS){
		user_error("Maximum Users Reached", server_fd);
		free((void *)add_u);
		
		return -1;
	}
	else {	
		if(find_user_index(users, buf) != -1) {
			user_error("User already exists", server_fd);
			free((void *)add_u);
			
			return -1;
		}
		
		int u = 0;	
		
		for(; u < MAX_USERS; u++) {
			if(users[u].status == SLOT_EMPTY) {
				break;
			}
		}
		
		if(pipe(users[u].ptoc) <0){
			perror("Pipe failed");
			exit(1);
		}
		if(pipe(users[u].ctop) <0){
			perror("Pipe failed");
			exit(1);
		}
		if(fcntl(users[u].ctop[0],F_SETFL,O_NONBLOCK) < 0){
			perror("Nonblock flag not set");
		}
		users[u].status = SLOT_FULL;
		total_users++;
		strcpy(users[u].name, buf);
		sprintf(add_u,"\nAdding user %s...",users[u].name);
		
		if(write(server_fd,add_u,MSG_SIZE) < 0){
			perror("Failed to write");
		}
		
		users[u].pid = fork();
		if (users[u].pid == -1){
			perror("fork");
			exit(1);
		} 
		else if (users[u].pid == 0){
			close(users[u].ptoc[1]);
			close(users[u].ctop[0]);
			char temp1[MSG_SIZE];
			char temp2[MSG_SIZE];
			snprintf(temp1,sizeof(temp1),"%d",users[u].ptoc[0]);
			snprintf(temp2,sizeof(temp2),"%d",users[u].ctop[1]);
			free((void *)add_u);
			execl(XTERM_PATH,XTERM,"+hold","-e","./shell",users[u].name,temp1,temp2,NULL);
			perror("Exec error");
			exit(1);
		}
		
		close(users[u].ptoc[0]);
		close(users[u].ctop[1]);
	}
	
	free((void *)add_u);
	return 1;
}

/*
 * Broadcast message to all users. Completed for you as a guide to help with other commands :-).
 */
int broadcast_msg(user_chat_box_t *users, char *buf, int fd, char *sender)
{
	int i;
	const char *msg = "Broadcasting...", *s;
	char text[MSG_SIZE];

	/* Notify on server shell */
	if (write(fd, msg, strlen(msg) + 1) < 0)
		perror("writing to server shell");
	
	/* Send the message to all user shells */
	s = strtok(buf, "\n");
	sprintf(text, "%s : %s", sender, s);
	
	for (i = 0; i < MAX_USERS; i++) {
		if (users[i].status == SLOT_EMPTY)
			continue;
		if (write(users[i].ptoc[1], text, strlen(text) + 1) < 0)
			perror("write to child shell failed");
	}
	
	return 1;
}

/*
 * Close all pipes for this user
 */
void close_pipes(int idx, user_chat_box_t *users)
{
	if(close(users[idx].ptoc[1]) < 0 || close(users[idx].ctop[0]) < 0){
		perror(("Failed to close pipes for User: %s",users[idx].name));
	}
}

/*
 * Cleanup single user: close all pipes, kill user's child process, kill user 
 * xterm process, free-up slot.
 */
void cleanup_user(int idx, user_chat_box_t *users)
{
	close_pipes(idx, users);
    
    if(kill_if_exists(users[idx].pid) < 0 || kill_if_exists(users[idx].child_pid) < 0){
	  perror(("Failed to kill process for User: %s",users[idx].name));
		}
    
    users[idx].status = SLOT_EMPTY;
    int status;
    total_users--;
		if(wait(&status) < 0){
			perror("wait");}
}

/*
 * Cleanup all users: given to you
 */
void cleanup_users(user_chat_box_t *users)
{
	int i;

	for (i = 0; i < MAX_USERS; i++) {
		if (users[i].status == SLOT_EMPTY)
			continue;
		cleanup_user(i, users);
	}
}

/*
 * Cleanup server process: close all pipes, kill the parent process and its 
 * children.
 */
void cleanup_server(server_ctrl_t server_ctrl)
{
	//cleanup_users(users);
	
  if(close(server_ctrl.ctop[0]) < 0 || close(server_ctrl.ptoc[1]) < 0 ){
		perror("Failed to close pipes for Server");
	}
    
  if(kill_if_exists(server_ctrl.pid) < 0 || kill_if_exists(server_ctrl.child_pid) < 0){
		perror("Failed to kill process for Server");
	}
}

/*
 * Utility function.
 * Given a command's input buffer, extract name.
 */
char *extract_name(int cmd, char *buf)
{
	char *s = NULL;

	s = strtok(buf, " ");
	s = strtok(NULL, " ");
	if (cmd == P2P)
		return s;	/* s points to the name as no newline after name in P2P */
	s = strtok(s, "\n");	/* other commands have newline after name, so remove it*/
	return s;
}

//extract P2P message
char *extract_p2p_msg(char *buf){
	char *s = NULL;
	s = strtok(buf," ");
	s = strtok(NULL," ");
	s = strtok(NULL,"\n");
	return s;	
}

/*
 * Send personal message. Print error on the user shell if user not found.
 */
void send_p2p_msg(char *sender, user_chat_box_t *users, char *buf, char *to_user, int from_fd)
{	
	char *no_user = "No user name given"; 
    int target_idx = find_user_index(users, to_user);
   	char *p2p_msg = extract_p2p_msg(buf);
   	if(p2p_msg == '\0' || is_empty(p2p_msg)) {
   		user_error("Empty message", from_fd);
   	} else {
		char *msg = (char*)malloc(MSG_SIZE);
		if (to_user[0] == '\0'){
			sprintf(msg,"%s\n",no_user);}		
		else{
			sprintf(msg,"%s : %s",sender,p2p_msg);}

	    if (target_idx < 0 ){ 
	        //need to print to user shell if user not found
	        user_error("User not found", from_fd);   
	    }
	    if(write(users[target_idx].ptoc[1] , msg, strlen(msg)+1) < 0)
	        user_error("Message send failed", from_fd);   
		free((void*)msg);
	}
}

/*
 * Kick a user
 */
int kick_user(user_chat_box_t *users, char *name, int fd) {
	int i = find_user_index(users, name);
	if(i < 0) {
		user_error("User not found", fd);
		return 0;
	} else {
		cleanup_user(i, users);
		return 1;
	}
}

/*
 * Incoming message handler
 */
void handle_message(char *line, int from_fd, char *sender, int is_server, server_ctrl_t *server) {
	char *shell_copy = (char*)malloc(MSG_SIZE);
	sprintf(shell_copy,"%s\n",line);
	char *trimmed;
	trim(line, &trimmed);
	char *command = strtok(trimmed, " ");
	server_cmd_type com = parse_command(command);
	char *username = extract_name(com, line);
	int took_action = 0;
	int user_id;
	int success;
	char msg[MSG_SIZE];
	switch(com) {
		case LIST_USERS:
			list_users(users, from_fd);
			break;
		default:
			if(is_server) { // Shell only commands
				switch(com) {
					case ADD_USER:
						took_action = 1;
						if(username == '\0'){
							user_error("No user name given", from_fd);
							break;}
						success = add_user(users, username, from_fd);
						break;
					case KICK:
						took_action = 1;
						if(username == '\0'){
							user_error("No user name given", from_fd);
							break;}
						else if(find_user_index(users,username) == -1){
							user_error("Not a valid user", from_fd);
							break;}
						success = kick_user(users, username, from_fd);
						if(success != 0) {
							strcpy(msg, "<kicked>");
							broadcast_msg(users, msg, server->ptoc[1], username);
						}
						break;
					case CHILD_PID:
						took_action = 1;
						break;
					case EXIT:
						strcpy(msg, "<shutting down>");
						broadcast_msg(users, msg, server->ptoc[1], sender);
						cleanup_users(users);
						cleanup_server(*server);
						break;
				}
			} else { // Child only commands
				switch(com) {
					case CHILD_PID:
						strcpy(msg, "<joined>");
						user_id = find_user_index(users, sender);
						users[user_id].child_pid = atoi(username);
						broadcast_msg(users, msg, server->ptoc[1], sender);
						took_action = 1;
						break;
					case EXIT:
						strcpy(msg, "<exited>");
						user_id = find_user_index(users, sender);
						broadcast_msg(users, msg, server->ptoc[1], sender);
						cleanup_user(user_id, users);
						took_action = 1;
						break;
					case P2P:
						took_action = 1;
						if(username == '\0'){
							user_error("No user name given", from_fd);
							break;}
						else if(find_user_index(users,username) == -1){
							user_error("Not a valid user", from_fd);
							break;}
						send_p2p_msg(sender,users,shell_copy,username,from_fd);
						break;
				}
			}
			if(!took_action) // default action
				broadcast_msg(users, shell_copy, server->ptoc[1], sender);
			break;
	}
	free((void *) shell_copy);
}

int main(int argc, char **argv)
{
// Create 2 pipes for communication between SERVER and shell process, initialize all potential users
// with SLOT_EMPTY status, fork a child process to act as the shell for server.
	char buf[MSG_SIZE];
	server_ctrl_t shell;
	shell.pid = getpid();
	int child;
	if(pipe(shell.ptoc) <0){
			perror("Pipe failed");
			exit(1);}
	if(pipe(shell.ctop) <0){
			perror("Pipe failed");
			exit(1);}
	char *server = "SERVER";
	if(fcntl(shell.ctop[0], F_SETFL, O_NONBLOCK) < 0){
		perror("Nonblock pipes not set");}
	
	int u = 0;
	for(; u < MAX_USERS; u++) {
		users[u].status = SLOT_EMPTY;
	}

	if ((shell.child_pid = fork()) == -1){
		perror("fork");
		exit(1);}
	else if (shell.child_pid == 0){
		close(shell.ptoc[1]);
		close(shell.ctop[0]);
		char temp1[20];
		char temp2[20];
		snprintf(temp1,sizeof(temp1),"%d",shell.ptoc[0]);
		snprintf(temp2,sizeof(temp2),"%d",shell.ctop[1]);
		execl("./shell","DUD",server,temp1,temp2,NULL);
		perror("Exec failure");
		exit(1);
	}
// END setup Server Shell

// In parent process continually loop to check if the server shell or a specific user shell has written any
// data for reading and executing by the server.
	else{
		close(shell.ptoc[0]);
		close(shell.ctop[1]);
		shell.pid = getpid();
		while (1) {
			/* Let the CPU breathe */
			usleep(1000);
			char *p_buf = (char*)malloc(MSG_SIZE);

// If server shell has sent data, server then reads and processes command in switch.
			
			if(read(shell.ctop[0],p_buf,MSG_SIZE) > 0){
				handle_message(p_buf, shell.ptoc[1], server, 1, &shell);
			}
//END Server Shell Read

//Now check to see if a specific user has data to be read, and if so the server executes command.
			
			int u = 0;
			for(; u < MAX_USERS; u++) {
				if(users[u].status == SLOT_EMPTY) continue;
				int status;
				if(waitpid(users[u].pid, &status, WNOHANG) > 0) {
					// Maybe include a little departing message here
					cleanup_user(u, users);
					continue;
				}
				usleep(1000);
				char *user_buf = (char*)malloc(MSG_SIZE);
				if (read(users[u].ctop[0],user_buf,MSG_SIZE)>0){
					handle_message(user_buf, users[u].ptoc[1], users[u].name, 0, &shell);
				}
				free((void *)user_buf);
			}
			free((void *)p_buf);
//END Server Users Read
		}	/* while loop ends when server shell sees the \exit command */
	}
	return 0;
}

