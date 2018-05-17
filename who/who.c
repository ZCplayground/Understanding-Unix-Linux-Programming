/*
step1: man who     # Know who uses struct utmp
step2: man -k utmp # search utmp manual
step3: man utmp    # learn its structure
step4: Write your own who command!
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
    struct utmp current_record;
    int    utmpfd;
    int    reclen = sizeof(current_record);
    printf("size of reclen = %d\n",reclen);

    if( (utmpfd = open("/var/run/utmp",O_RDONLY) == -1) )
    {
        perror("/var/run/utmp");
        exit(1);
    }
    printf("open utmp successfully...\n");
    while(read(utmpfd, &current_record, reclen) == reclen)
    {
	printf("begin reading...\n");
        show_info(&current_record);
    }
    close(utmpfd);
    return 0;
}

void show_info(struct utmp * u)
{
    printf("%-8.8s",u->ut_name);
    printf(" ");
    printf("%-8.8s",u->ut_line);
    printf(" ");
    printf("%-8d",u->ut_tv.tv_sec); // ut_time
    printf(" ");
#ifdef SHOWHOST
    printf("%-8d",*(u->ut_addr_v6));
    printf(" ");
#endif
    printf("\n");
}
