#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char* argv[])
{
    const char *host = "127.0.0.1";
    
    int fd;
    short port = 7000;
    struct sockaddr_in hostaddr;
    struct hostent *hp;
    
    if (argc > 1)
    {
        host = argv[1];
    }
    
    if (argc > 2)
    {
        port = (short)atoi(argv[2]);
    }

    printf("Host: %s, port: %d\n", host, (int)port);

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Failed to create socket");
        return 1;
    }
    
    hp = gethostbyname(argv[1]);

    if (hp == 0)
    {
        perror("Host lookup failed\n");
        return 1;
    }

    memset((char *)&hostaddr, 0, sizeof(hostaddr));
    memcpy((char *)&hostaddr.sin_addr, (char *)hp->h_addr, hp->h_length);
    hostaddr.sin_family = AF_INET;
    hostaddr.sin_port = htons(port);

    printf("Sending data\n");

    char buffer[] = "ping";
    
    ssize_t ret = sendto(fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&hostaddr, sizeof(hostaddr));
    
    if (ret < 0)
    {
        perror("sendto failed\n");
    }
    else if (ret == 0)
    {
        printf("shutdown\n");
    }
    else
    {
        char* ip = (char*)&hostaddr.sin_addr.s_addr;
        printf("Sent %d bytes to %d.%d.%d.%d\n", (int)ret, (int)ip[0], (int)ip[1], (int)ip[2], (int)ip[3]);
    }

    return 0;
}
