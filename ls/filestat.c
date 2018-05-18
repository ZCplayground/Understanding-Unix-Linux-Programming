/* After finishing ver1.0, some features needed to completed as below:
1. sort: store all filename in an array and qsort it.
2. The standard ls is output in columns. The solution: read the file name into an array, and then calculate the column width and number of rows.
3. ls -a: Make ls output hidden files only when accepting option -a.
4. ls -l: File size, file owner, etc.
*/

#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

int main()
{
    struct stat infobuf;
    if(stat("/etc/passwd",&infobuf) == -1)
        perror("/etc/passwd");
    else 
        printf("The size of /etc/passwd is %ld byte(s)\n", infobuf.st_size);
    
    return 0;
}
