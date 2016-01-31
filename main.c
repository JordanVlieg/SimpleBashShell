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
	int bgList[] = [0, 0, 0, 0, 0];
	char* bgNameList[5];
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
		int bgFlag = 0;
		for(counter = 0; token != NULL && counter < ARGSLEN - 1; counter++)
		{
			theArgs[counter] = token;
			token = strtok(NULL, delimiter);
		}
		theArgs[counter] = NULL;

		if(strcmp(theArgs[0], "bg") == 0)
		{
			bgFlag = 1;
		}
		if(strcmp(theArgs[0], "cd") == 0)
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
				execvp(theArgs[bgFlag], theArgs); //A very dirty dirty trick
				printf("This is hit now");
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
				int bgCounter = 0;
				for(; bgCounter < 5; bgCounter++)
				{
					if(bgList[bgCounter] == 0)
					{
						bgList[bgCounter] = childpid;
						bgNameList[bgCounter] = theArgs;
					}
				}
				if(bgFlag == 0)
				{
					wait(&waitStatus);
				}
				else
				{
					int returnedID = waitpid(-1, &waitStatus, WNOHANG);
					printf("returned id is %d\n", returnedID);
				}

			}
			free (cmd);
		}
	}
}
