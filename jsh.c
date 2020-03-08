#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define TRUE 1
#define BUFFER 300
#define TAM_P 30 

void read_command(char *command, char *parameters[]);
void show_prompt();

int main(int argc, char *argv[], char *envp[]) {

	int status;
	int i;
	char *token;
	FILE *fp;
	char path[BUFFER] = "\0";	
	char command[BUFFER] = "\0";
    	char *parameters[BUFFER];
	

	while (TRUE) {
        for (int i = 0; i < BUFFER; i++) {
            	parameters[i] = (char *) malloc(TAM_P * sizeof(char));
        }
        
		show_prompt();
		read_command(command, parameters);
		if (fork() != 0) {
			waitpid(-1, &status, 0);
		} else {
			for (i = 0; i < 99; i++) {
				if (!strncasecmp("PATH", envp[i], 4)) {
					break;
				}
			}

			strtok(envp[i], "=");

			while ((token = strtok(NULL, ":")) != NULL) {
				strcpy(path, token);
				strcat(path, "/");
				strcat(path, command);
				if ((fp = fopen(path, "r")) != NULL) {
					fclose(fp);
					break;
				}
			}
			
			execve(path, parameters, envp);
		}
	}

	return 0;
}

void show_prompt() {
	printf("$ ");
}

void read_command(char *command, char *parameters[]) {
	int i;
	char *token;
	char tok[2] = " ";
	char complete_command[BUFFER] = "\0";
	scanf(" %[^\n]s", complete_command);
	token = strtok(complete_command, tok);
	strcpy(command, token);

	if (!strcmp(command, "exit")) {
		exit(0);
	}

	i = 0;
	strcpy(parameters[i++], token);
	while ((token = strtok(NULL, tok)) != NULL) {
		strcpy(parameters[i++], token);
	}
	
	parameters[i] = (char *)0;
}
