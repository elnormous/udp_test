#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

int main(int argc, char* argv[])
{
    int fd;
    short port = 7000;
    struct sockaddr_in myaddr;

    if (argc > 1)
    {
        port = (short)atoi(argv[1]);
    }

    printf("Port: %d\n", (int)port);

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Failed to create socket\n");
        return 1;
    }

    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(port);

    if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
    {
        perror("Failed to bind\n"); return 0;
    }

    printf("Waiting for data\n");

    char buffer[1024];
    struct sockaddr_in sender;
    socklen_t sendsize = sizeof(sender);

    while (1)
    {
        ssize_t ret = recvfrom(fd, buffer, sizeof(buffer), 0,
                               (struct sockaddr*)&sender, &sendsize);

        if (ret < 0)
        {
            perror("recvfrom failed\n");
            break;
        }
        else if (ret == 0)
        {
            printf("shutdown\n");
        }
        else
        {
            char* ip = (char*)&sender.sin_addr.s_addr;
            printf("Received %d bytes from %d.%d.%d.%d\n", (int)ret, (int)ip[0], (int)ip[1], (int)ip[2], (int)ip[3]);
        }
    }

    close(fd);

    return 0;
}
