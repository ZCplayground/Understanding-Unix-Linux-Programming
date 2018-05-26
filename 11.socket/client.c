
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUFFER_SIZE 1024
int main()
{
    char buffer[BUFFER_SIZE];
    while(1)
    {
        scanf("%s",buffer); // message to send out

        /*
          Step 1: ask kernel for a socket
        */
        int sock = socket(AF_INET, SOCK_STREAM, 0);

        /*
          Step 2: connect to server
        */
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        serv_addr.sin_port = htons(1234);
        connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

        /*
          Step 3: transfer data, then hangup
        */
        write(sock,buffer,sizeof(buffer));
        if(buffer[0]=='e' && buffer[1]=='x' && buffer[2]=='i' && buffer[3]=='t')
        {
            printf("The End.\n");
            break;
        }
    close(sock);
    }
    return 0;
}
