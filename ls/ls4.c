/*
This version fix:
when you input ./ls3 /tmp  the program did not display filename but path name.
because of bug in show_file_info()

Fix some pattern in show_file_info()
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
void do_ls(char []);
void do_stat(char * filename);
void show_file_info(char * filename, struct stat * infobuf);
void mode_to_letter(int mode, char str[]);
char * uid_to_name(uid_t uid);
char * gid_to_name(gid_t gid);

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
        char path[4096]; // maximum path length
        while((pdirent = readdir(dir_ptr))!=NULL)
        {
            // do_stat(pdirent->d_name); bug: error to ./ls2 /
            memset(path, 0, sizeof(path));
            strcat(path, dirname);
            int len = strlen(path);
            if(path[len-1] != '/'){
                path[len] = '/';
                path[len+1] = '\0';
            }
            strcat(path, pdirent->d_name);
            do_stat(path);
        }
        closedir(dir_ptr);
    }
}

void do_stat(char * pathname)
{
    struct stat infobuf;
    if(stat(pathname, &infobuf) == -1){
        perror("stat");
        exit(EXIT_FAILURE);
    }
    else
        show_file_info(pathname, &infobuf);

}

void show_file_info(char * pathname, struct stat * buf)
{
    char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
    void mode_to_letter();
    char modestr[11];

    mode_to_letter(buf->st_mode, modestr);

    printf("%-11s", modestr);
    printf(" %3ld ", buf->st_nlink);
    printf("%6s ", uid_to_name(buf->st_uid));
    printf("%6s ", gid_to_name(buf->st_gid));
    printf("%10ld  ", buf->st_size);
    printf("%.12s", 4 + ctime(&buf->st_mtime));
    // printf(" %s\n", filename); bug!
    // we want to display filename here instead of pathname.
    // Finding the position of the first slash / from the end of the string, followed by the file name
    char filename[255];
    int len = strlen(pathname);
    int i, j;
    for(i = len - 1; i >= 0; i--){
        if(pathname[i] == '/')
            break;
    }
    for(j = 0, i++; pathname[i]!='\0';i++,j++){
        filename[j] = pathname[i];
    }
    filename[j] = '\0';
    printf(" %s\n", filename);
}

void mode_to_letter(int mode, char str[])
{
    strcpy(str,"----------");

    if(S_ISDIR(mode)) str[0] = 'd'; // dir
    if(S_ISCHR(mode)) str[0] = 'c'; // char devices
    if(S_ISBLK(mode)) str[0] = 'b'; // block devices

    if(mode & S_IRUSR) str[1] = 'r'; // 3 bits for user
    if(mode & S_IWUSR) str[2] = 'w';
    if(mode & S_IXUSR) str[3] = 'x';

    if(mode & S_IRGRP) str[4] = 'r'; // 3 bits for group
    if(mode & S_IWGRP) str[5] = 'w';
    if(mode & S_IXGRP) str[6] = 'x';

    if(mode & S_IROTH) str[7] = 'r'; // 3 bits for other
    if(mode & S_IWOTH) str[8] = 'w';
    if(mode & S_IXOTH) str[9] = 'x';
}

#include<pwd.h>

char * uid_to_name(uid_t uid)
{
    struct passwd * getpwuid(), *pw_ptr;
    static char numstr[10];

    if((pw_ptr = getpwuid(uid))==NULL){ // no username according to uid
        sprintf(numstr,"%d",uid);
        return numstr;
    }
    else
        return pw_ptr->pw_name;
}

#include<grp.h>

char * gid_to_name(gid_t gid)
{
    struct group * getgrgid(), *grp_ptr;
    static char numstr[10];

    if((grp_ptr = getgrgid(gid))==NULL){ // no groupname according to gid
        sprintf(numstr, "%d", gid);
        return numstr;
    }
    else
        return grp_ptr->gr_name;
}

