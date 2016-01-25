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
#define ARGSLEN		10

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
