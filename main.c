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
	char cwd[1024];
	char previous[1024] = " ";
	while (1) {
		if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd failed");
            strcpy(cwd, "?"); // fallback
        }
		char prompt[1050];
		snprintf(prompt, sizeof(prompt), " %s > ", getcwd(cwd, sizeof(cwd)));
		char *user_input = readline(prompt);
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
			char temp[1024];
			strcpy(cwd, temp);
			
			if (args[1] == NULL) {	
				chdir(getenv("HOME"));
			} else if (strcmp(args[1], "-") == 0){
				chdir(temp);
			} else {
				chdir(args[1]);
			}
			strcpy(previous, temp);
			continue;
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
		free(user_input);
	}
	return 0;
}
