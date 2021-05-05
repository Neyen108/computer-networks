#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX 1000
#define PORT 8080

int main()
{
    char buff[MAX];

    //Creating socket fd
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    int len = sizeof(servaddr);

    while (1)
    {
        printf("\nYou (client) : ");
        scanf("%s", buff);

        if (strncmp("exit", buff, 4) == 0)
        {
            printf("Client Exit...\n");
            break;
        }

        //send message to udpServer
        if (sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&servaddr, len) == -1)
        {
            perror("Error in sendto()");
            exit(1);
        }

        //clear the buffer
        memset(buff, '\0', sizeof(buff));

        //get the client time
        if (recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&servaddr, &len) == -1)
        {
            perror("Error in recvfrom()");
            exit(1);
        }

        printf("Time: %s", buff);

        //clear the buffer
        memset(buff, '\0', sizeof(buff));

        //get the response and time
        if (recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&servaddr, &len) == -1)
        {
            perror("Error in recvfrom()");
            exit(1);
        }

        printf("\nServer : %s", buff);

        //clear the buffer
        memset(buff, '\0', sizeof(buff));
    }
    close(sockfd);
}