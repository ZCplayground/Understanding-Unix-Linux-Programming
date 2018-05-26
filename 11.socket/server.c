#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define BUFFER_SIZE 1024

int main()
{
    char buffer[BUFFER_SIZE];

     /*
      Step 1: ask kernel for a socket
     */
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    /*
      Step 2: bind address to socket.
    */
    struct sockaddr_in serv_addr; // IPv4 address format. See: man 7 ip
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(1234); // htons: convert to network byte order

    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    printf("Server Socket Port:%d\n",ntohs(serv_addr.sin_port));

    /*
      Step 3: allow incoming connection. Queue size = 20.
    */
    listen(serv_sock, 20);

    /*
      main loop: accept(), write(), close()
    */
    while(1)
    {
        struct sockaddr_in cli_addr; // wait for call
        socklen_t cli_addr_size = sizeof(cli_addr);
        int cli_sock = accept(serv_sock, (struct sockaddr*)&cli_addr, &cli_addr_size);
        memset(buffer, 0, sizeof(buffer));
        read(cli_sock, buffer, sizeof(buffer)-1);   			//接受客户端发送的数据
        if(buffer[0]=='e' && buffer[1]=='x' && buffer[2]=='i' && buffer[3]=='t')
        {
            printf("The End.\n");
            break;
        }
        else
            printf("%s\n",buffer);
        close(cli_sock);
    }
    close(serv_sock);
    return 0;
}
