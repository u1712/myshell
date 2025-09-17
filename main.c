#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

int main() {
	char *user_input;
	char *args[64];
	while (1) {
		char *user_input = readline("  > ");
		char* token = strtok(user_input, " ");
		int i = 0;
		while (token != NULL) {
			args[i] = token;
			i++;
			token = strtok(NULL, " ");
		}
		args[i] = NULL;

		if (args[0] == NULL) {
			continue;
		}
		
		if (strcmp(args[0], "cd") == 0) {
			chdir(args[1]);
			if (chdir(args[1]) != 0) {	
				perror("directory doesn't exist");
			}
			continue;
		} else if (strcmp(args[0], "cd") == 0 && args[1] == NULL) {
			chdir(getenv("HOME"));
		}
		
		if (strcmp(args[0], "exit") == 0) {
			break; // exit the shell loop
		}
	
		int p = fork();
		if (p < 0) {
			printf("fork failed");
			exit(1);
		} else if (p == 0) {
			execvp(args[0], args);
			perror("execvp failed");
			exit(1);
		} else {
			wait(NULL);
		}
		if (user_input && *user_input) {
		 add_history(user_input);
		}
	}
	free(user_input);
	return 0;
}
