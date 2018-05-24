/*
more03.c will change terminal stty mode through a shell command.
It will influences subsequent programs and commands.
This version gives a solution uses tcgetattr() and tcsetattr()
*/

#include<stdio.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>
#define PAGELEN 24
#define LINELEN 512
void do_more(FILE *);
int see_more(FILE *);
void tty_save_and_restore(int);
void set_noncanonical();

int main(int argc, char *argv[])
{
    tty_save_and_restore(0);
    set_noncanonical();
    FILE *fp;
    if(argc == 1)
        do_more(stdin);
    else
        while(--argc)
        {
            if((fp = fopen(* ++argv,"r")) != NULL)
            {
                do_more(fp);
                fclose(fp);
            }
        }
    tty_save_and_restore(1);
    return 0;
}

void tty_save_and_restore(int option)
{
    static struct termios original_mode;
    if(option == 0)
        tcgetattr(0, &original_mode);
    else
        tcsetattr(0, TCSANOW, &original_mode);
}

void set_noncanonical()
{
    struct termios ttystate;
    tcgetattr(0, &ttystate);
    ttystate.c_lflag &= ~ICANON;
    ttystate.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &ttystate);
}

void do_more(FILE * fp)
{
    char line[LINELEN];
    int num_of_lines = 0;
    int see_more(), reply;
    FILE *fp_tty;
    fp_tty = fopen("/dev/tty","r");
    if(fp_tty == NULL)
        exit(1);
    while(fgets(line,LINELEN,fp))
    {
        if(num_of_lines == PAGELEN) // full screen?
        {
            reply = see_more(fp_tty); // 1, 24, or exit
            if(reply == 0)
                break;
            num_of_lines -= reply;
        }
        if(fputs(line, stdout) == EOF)
            exit(1);
        num_of_lines++;
    }
}

int see_more(FILE *cmd)
{
    int c;
    printf("\033[7m more?\033[m");
    //system("stty -icanon");
    while((c=getc(cmd))!=EOF)
    {
        if(c == 'q')
            return 0;
        if(c == ' ')
            return PAGELEN;
        if(c =='\n')
            return 1;
    }
    return 0;
}
