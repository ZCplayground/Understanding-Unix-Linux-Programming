#include<stdio.h>
#include<fcntl.h>

int main()
{
    int fd;
    char line[100];

    fgets(line, 100, stdin); printf("%s\n",line);
    fgets(line, 100, stdin); printf("%s\n",line);
    fgets(line, 100, stdin); printf("%s\n",line);

    // redirection

    close(0);
    fd = open("/etc/passwd", O_RDONLY);

    fgets(line, 100, stdin); printf("%s\n",line);
    fgets(line, 100, stdin); printf("%s\n",line);
    fgets(line, 100, stdin); printf("%s\n",line);

    return 0;
}
