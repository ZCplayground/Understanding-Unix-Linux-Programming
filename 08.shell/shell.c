#include <sys/types.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#define MAXARGS 20
#define ARGLEN 100

int get_arglist(char * line);
char *makestring(char * buf);
void execute(int argc, char * arglist[]);

char *arglist[MAXARGS + 1];

/*
ls -l dir

arglist ---> □--> "ls\0"
             □--> "-l\0"
             □--> "dir\0"
             NULL
*/

int main()
{
	char line[ARGLEN];
	while (1)
	{
		memset(line, 0, sizeof(line));
		fgets(line, ARGLEN, stdin);
		line[strlen(line) - 1] = '\0'; // trim new line
		if (line[0] == '\0') break;    // empty command line
		int argc = get_arglist(line);
		execute(argc, arglist);
	}
	return 0;
}

int get_arglist(char * line)
{   /* Get several args from a line.
	   Allocate memory for them dynamically. Store their pointers in arglist*/

	char argbuf[ARGLEN];  // one arg
	int numargs = 0;      // pointer to array
	int i = 0, j, k, start, end;

	while (numargs < ARGLEN) {
		start = i; // start position of an arg
		for (; ; i++)
			if (line[i] == '\0' || line[i] == ' ') { // Delimiter between args
				end = i;  // end postion of an arg
				break;
			}
		memset(argbuf, 0, sizeof(argbuf));
		int arglen = end - start;
		for (j = 0, k = start; j < arglen; j++, k++) {
			argbuf[j] = line[k];
		}
		argbuf[j] = '\0';
		arglist[numargs++] = makestring(argbuf);
		if (line[i] == '\0') break; // end of command line
		while (line[i] == ' ') i++; // proceed until not space
	}
	arglist[numargs] = NULL;  // close list. It's important for the exec system call.
	return numargs;
}

/* A sample of get_arglist

command line: ls  -l dir\0
position:     0123456789
              i i i i  i
			  s e s es  e
arglist[0]    ls
arglist[1]        -l
arglist[2]           dir

Tips:
'start' indicates the beginning of each arg.
'i' will stop at the first space after each arg. At this time
'end' is assigned to 'i'. And then
'i' will skip the subsequent space until it meets another args or
get the end of input.
*/

char * makestring(char * buf)
{
	char *cp;
	cp = (char *)malloc(sizeof(char) * (strlen(buf) + 1));
	if (cp == NULL) {
		printf("error: failed to malloc.\n");
		return NULL;
	}
	strcpy(cp, buf);
	return cp;
}

void execute(int argc, char * arglist[])
{
	int i;
	for(i = 0; i < argc; i++)
		printf("arg[%d]: %s\t", i, arglist[i]);
	printf("\n");
	int pid, exitstatus;

	pid = fork();
	switch (pid)
	{
	case -1:
		perror("fork failed");
		exit(1);
	case 0:
		execvp(arglist[0], arglist);
		perror("execvp failed");
		exit(1);
	default:
		while (wait(&exitstatus) != pid);
		printf("child exited with status %d\n", exitstatus);
	}
}
