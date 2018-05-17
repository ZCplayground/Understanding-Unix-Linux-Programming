/*
step1: man who     # Know who uses struct utmp
step2: man -k utmp # search utmp manual
step3: man utmp    # learn its structure, in see also, find related functions
step4: man getutent
step5: Write your own who command!
*/

/* who -- show users currently logged in

1. read the data structrue from utmp file
2. display information in a suitable way
*/

#include<stdio.h>
#include<stdlib.h>
#include<utmp.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define SHOWHOST
void show_info();
int main()
{
    struct utmp * current_record;
    setutent();
    while((current_record = getutent())!=NULL)
    {
        if(current_record->ut_type != USER_PROCESS) // only display user process
            continue;
        show_info(current_record);
    }
    endutent();
    return 0;
}


void show_info(struct utmp * u)
{
    printf("%-8.8s",u->ut_user); // Username
    printf(" ");
    printf("%-8.8s",u->ut_line); // Device name of tty
    printf(" ");
    printf("%-8d",u->ut_tv.tv_sec); // ut_time
    printf(" ");
#ifdef SHOWHOST
    printf("%-8s",u->ut_host); // host name for remote login
    printf(" ");
#endif
    printf("\n");
}


