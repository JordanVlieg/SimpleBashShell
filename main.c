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

struct Process 
{
	int pid;
	char command[1000];
};

void Exited_Process(int sig)
{
	pid_t pid;

  	pid = wait(NULL);

  	printf("Pid %d exited.\n", pid);
}

void Delete_Children(void)
{
	pid_t pid;
	int waitStatus;
	while((pid= waitpid(-1, &waitStatus, WNOHANG)) > 0)
	{
		printf("process completed");	
	}
}


int main ( void )
{
	void Delete_Children(void);
	struct Process bgList[5];
	int item = 0;
	for(;item<5;item++)
	{
		bgList[item].pid = -2;
	}
	//char* bgNameList[5];
	for (;;)
	{
		char* cmd = readline ("shell>");
		char command[1000];
		strncpy(command, cmd, 1000);
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
			printf("reached\n");
			bgFlag = 1;
		}

		if(strcmp(theArgs[bgFlag], "bglist") == 0)
		{
			int theJob = 0;
			for(; theJob < 5; theJob++)
			{
				if(bgList[theJob].pid != -2)
				{
					printf("%d: %s\n", theJob, bgList[theJob].command);
				}
			}
		}
		else if(strcmp(theArgs[bgFlag], "bgkill") == 0)
		{
			int process;
			char switcher = theArgs[1+bgFlag][0];
			if(switcher == '0')
			{
				process = 0;
			}
			else if(switcher == '1')
			{
				process = 1;
			}
			else if(switcher == '2')
			{
				process = 2;
			}
			else if(switcher == '3')
			{
				process = 3;
			}
			else if(switcher == '4')
			{
				process = 4;
			}
			else
			{
				process = -1;
				printf("Invalid process number %d\n", switcher);
			}

			if(process >= 0)
			{
				kill(bgList[process].pid, SIGTERM);
				printf("Killed process: %d\n", process);
				bgList[process].pid = -2;
			}
			
		}
		else if(strcmp(theArgs[bgFlag], "cd") == 0)
		{
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
				printf("This is hit now\nbg ");
				exit(EXIT_SUCCESS);
			}
			else if(childpid == -1)
			{
				// Fork was unsuccessful
				printf("Internal system error: Fork\n");
				exit(EXIT_FAILURE);
			}
			else
			{
				// This is the parent thread

				if(bgFlag == 0)
				{
					waitpid(-1, &waitStatus, WNOHANG);
				}
				else
				{
					int bgCounter = 0;
					for(; bgCounter < 5; bgCounter++)
					{
						if(bgList[bgCounter].pid == -2)
						{
							printf("Added a process to the list: %s\n", cmd);							bgList[bgCounter].pid = childpid;
							strncpy(bgList[bgCounter].command, command, 1000);
							break;
						}
						else
						{
							printf("PID is: %d\n", bgList[bgCounter].pid);
						}
					}
					printf("The child process ID is: %d\n", childpid);
					signal(SIGCHLD, Exited_Process);
				}

			}
			free (cmd);
		}
	}
}

