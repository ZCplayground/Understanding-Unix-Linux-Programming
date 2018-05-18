/*  ls -- list directory contents

Three questions when you build ls:
1. How to read entry and show directory contents?
2. How to get file properties and how to show them?
3. Give you a name, how to tell whether it is a file or a directory? 

The directory is a special kind of file. It contains a lot of 
records, each with a uniform standard format. The content of 
each record represents a file or directory. The directory can 
be opened by a standard file function. The directory is not 
an unformatted text but contains a certain data structure.

step1: man -k direct | grep read  # learn how to read dir file
step2: read mannul of readdir and get other information.
step3: learn about opendir/readdir/closedir/dirent.h and so on.
step4: Write your own ls command!
*/

/*
    open directory
+-> read entry            - end of dir? -+
|_  display file info                    |
    close directory     <----------------+
*/

#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>

void do_ls(char []);

int main(int argc, char *argv[])
{
    if(argc == 1)
        do_ls(".");
    else
        while(--argc){
            printf("%s:\n",*++argv);
            do_ls(*argv);
        }
}

void do_ls(char dirname[])
{
    DIR * dir_ptr;
    struct dirent * pdirent;
    if((dir_ptr = opendir(dirname)) == NULL)
        fprintf(stderr, "ls: cannot open %s\n", dirname);
    else
    {
        while((pdirent = readdir(dir_ptr))!=NULL)
        {
            printf("%s\n",pdirent->d_name);
        }
        closedir(dir_ptr);
    }
}
