#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX 1000
#define PORT 8080

//TODO: calculate time delay
//check servaddr.sin_addr.s_addr

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
    servaddr.sin_addr.s_addr = inet_addr("10.0.2.15");
    servaddr.sin_port = htons(PORT);

    int len = sizeof(servaddr);

    printf("\n---------------------------------------\n");
    printf("This is the Client program. Enter your message. \n(Type 'exit' to exit the client)\n");
    while (1)
    {
        //Enter what to send to the server
        printf("\nYou (client) : ");
        scanf("%s", buff);

        //check exit condition
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
        //print client time
        printf("Time: %s", buff);

        //clear the buffer
        memset(buff, '\0', sizeof(buff));

        //get the response from server and server response time
        if (recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&servaddr, &len) == -1)
        {
            perror("Error in recvfrom()");
            exit(1);
        }
        //print response and response time
        printf("\nServer : %s", buff);

        //clear the buffer
        memset(buff, '\0', sizeof(buff));
    }
    printf("\n---------------------------------------\n");
    close(sockfd);
}