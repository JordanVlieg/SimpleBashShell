#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main (void)
{
	int counter = 0;
	for(;counter<5;counter++)
	{
		printf("Completed a loop\n");
		sleep (2);
	}
}