#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

/*
 * Print shell prompt. Pass the username as argument.
 */
void print_prompt(char *name)
{
	printf("%s >> ", name);
}

/*
 * Checks if the first string starts with the second. Return 1 if true.
 */
int starts_with(const char *a, const char *b)
{
	if (strncmp(a, b, strlen(b)) == 0)
		return 1;
	else
		return 0;
}


/*
 * Identify the command used at the shell 
 */
int parse_command(char *buf)
{
	int cmd;

	if (starts_with(buf, CMD_CHILD_PID))
		cmd = CHILD_PID;
	else if (starts_with(buf, CMD_P2P))
		cmd = P2P;
	else if (starts_with(buf, CMD_LIST_USERS))
		cmd = LIST_USERS;
	else if (starts_with(buf, CMD_ADD_USER))
		cmd = ADD_USER;
	else if (starts_with(buf, CMD_EXIT))
		cmd = EXIT;
	else if (starts_with(buf, CMD_KICK))
		cmd = KICK;
	else
		cmd = BROADCAST;

	return cmd;
}

void trim(char *str, char **ret) {
	int trim = 0;
	int i;
	int len = strlen(str);
	for(i = strlen(str) - 1; i >= 0; i--) {
		if(isspace(str[i])) {
			trim++;
		} else {
			break;
		}
	}
	*ret = (char*)malloc(sizeof(char) * (len - trim + 1));
	strncpy(*ret, str, len - trim);
	// *ret[len - trim] = '\0';
	// return ret;
}

/*
 * Check if the line is empty (no data; just spaces or return)
 */
int is_empty(char *line)
{
	while (*line != '\0') {
		if (!isspace(*line))
			return 0;
		line++;
	}
	return 1;
}
