/*
Show how to get terminal attributes use tcgetattr() and test with bit mask.
*/

#include<termios.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main()
{
    struct termios info;
    int rv;

    rv = tcgetattr(0, &info); // fd = 0 means stdin!
    if(rv == -1){
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    if(info.c_lflag & ECHO)
        printf("echo is on, since its bit is 1\n");
    else 
        printf("echo is off, since its bit is 0\n");
}

// run this program and try:
// stty -echo
// and run this program again.
