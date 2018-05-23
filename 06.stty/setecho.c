/*
If command line arguments begin with 'y', echo flag will be turned up,
otherwise it will be turned down.
*/

#include<termios.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#define oops(s,x) {perror(s);exit(x);}

int main(int argc, char *argv[])
{
    struct termios info;
    if(argc == 1)
        exit(0);
    if(tcgetattr(0, &info) == -1)
        oops("tcgetattr", 1);
    if(argv[1][0]=='y')
        info.c_lflag |= ECHO;
    else
        info.c_lflag &= ~ECHO;

    if(tcsetattr(0, TCSANOW, &info) == -1)
        oops("tcsetattr",2);
}

/*
$ gcc echostate.c -o echostate
$ gcc setecho.c -o setecho
$ cp setecho /bin
$ cp echostate /bin
$ echostate; setecho n; echostate; stty echo
$ stty -echo; echostate; setecho y; echostate
*/
