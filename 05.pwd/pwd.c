#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>

// Complete workflow of pwd instead of simple API getcwd.

/*
Tracing up from the current directory and continuing into the parent directory.
Each time you enter the parent directory, check the inode number of "."
And store the name of the inode in the struct `dirent` of the parent directory.
Until it reaches the root `/`
*/
void printpathto(ino_t);
ino_t get_inode(char *);
void inum_to_name(ino_t, char *, int);

int main()
{
    printpathto(get_inode("."));
    putchar('\n');
    return 0;
}

void printpathto(ino_t this_inode) // good function name and args name can convey the meanings.
{
    ino_t my_inode;
    char its_name[BUFSIZ];
    if(get_inode("..")!= this_inode){ // Recursive termination: get root dir
        chdir(".."); // get to parent dir
        inum_to_name(this_inode, its_name, BUFSIZ); // get child dir's name
        my_inode = get_inode("."); // parent dir's inode
        printpathto(my_inode); // recursively
        printf("/%s",its_name);
    } 
}

void inum_to_name(ino_t inode_to_find, char * namebuf, int buflen) 
{   // get dir name according to its inode number, store it in namebuf
    DIR * dir_ptr;
    struct dirent * pdirent;
    dir_ptr = opendir(".");
    if(dir_ptr==NULL){
        perror(".");
        exit(EXIT_FAILURE);
    }

    /*serch dir for a file with specific inode num*/
    while((pdirent = readdir(dir_ptr))!=NULL){
        if(pdirent->d_ino == inode_to_find)
        {
            strncpy(namebuf, pdirent->d_name, buflen);
            namebuf[buflen-1] = '\0';
            closedir(dir_ptr);
            return;
        }
    }
    fprintf(stderr,"error looking for inum %ld\n", inode_to_find);
    exit(EXIT_FAILURE);
}

ino_t get_inode(char *fname)
{
    // stat fname to get inode
    struct stat info;
    if(stat(fname, &info) == -1){
        fprintf(stderr, "Cannot stat");
        exit(EXIT_FAILURE);
    }
    return info.st_ino;
}
