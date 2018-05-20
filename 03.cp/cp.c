// cp: copy file 

/*
open sourcefile for reading
open copyfile for writing
+-> read from source -- eof?  --+
|_  write from buffer to copy   |
                                |
    close two files <-----------+

*/
#include<stdio.h>
#include<stdlib.h>
#define BUFFERSIZE 4096
void do_more(FILE * in, FILE * out);
int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "usage: %s <source> <destination>\n",*argv);
        exit(1);
    }
    FILE *in = NULL, *out = NULL;
    if((in = fopen(argv[1],"r")) == NULL)
    {
        fprintf(stderr, "Cannot open %s\n",argv[1]);
        exit(1);
    }
    if((out = fopen(argv[2],"w")) == NULL)
    {
        fprintf(stderr, "Cannot create %s\n",argv[2]);
        exit(1);
    }

    do_more(in,out);
    fclose(out);
    fclose(in);
    return 0;
}

void do_more(FILE * in, FILE * out)
{
    char buf[BUFFERSIZE];
    int num_of_char = 0;
    while(num_of_char = fread(buf,sizeof(char),BUFFERSIZE,in))
    {
        if(num_of_char == 0)
            break;
        fwrite(buf,sizeof(char),num_of_char,out);
        num_of_char = 0;
    }
}
