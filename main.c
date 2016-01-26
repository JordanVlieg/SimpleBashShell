/*
 * main.c
 *
 * A simple program to illustrate the use of the GNU Readline library
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define ARGSLEN		100


int main ( void )
{
	for (;;)
	{
		char* cmd = readline ("shell>");
		printf ("Got: [%s]\n", cmd);
		int waitStatus;
		char* theArgs[ARGSLEN];
		const char delimiter[2] = " ";
		char* token;
		token  = strtok(cmd, delimiter);
		int counter;
		for(counter = 0; token != NULL && counter < ARGSLEN - 1; counter++)
		{
			theArgs[counter] = token;
			token = strtok(NULL, delimiter);
		}
		theArgs[counter] = NULL;

		if(strcmp(theArgs[0], "pwd") == 0)
		{
			char theDirectory[100];
			getcwd(theDirectory, 100);
			printf("Printed by me: %s\n", theDirectory);
		}

		else if(strcmp(theArgs[0], "cd") == 0)
		{
			printf("REACHED THE GODDAMN FUNCTION \n");
			if(theArgs[1] == NULL)
			{
				printf("No directory given\n");
			}
			else
			{
				char theDirectory[100];
				getcwd(theDirectory, 100);
				strcat(theDirectory, "/");
				strcat(theDirectory, theArgs[1]);
				int returnVal = chdir(theDirectory);
				if(returnVal == -1)
				{
					printf("Error changing directory\n");
				}
			}
		}
		else
		{
			int childpid = fork();
			if(childpid == 0)
			{
				// This is the child thread
				execvp(theArgs[0], theArgs);
				exit(0);
			}
			else if(childpid == -1)
			{
				// Fork was unsuccessful
				printf("Internal system error: Fork");
				exit(0);
			}
			else
			{
				// This is the parent thread
				wait(&waitStatus);
			}
			free (cmd);
		}
	}
}
