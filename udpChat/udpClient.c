#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX 1000
#define PORT 8080

int main()
{
    char buff[MAX];

    //Creating socket fd
    int sockfd;
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(1);
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("10.0.0.1");
    servaddr.sin_port = htons(PORT);

    int len = sizeof(servaddr);
    long double rtt_msec = 0, total_msec = 0;
    struct timespec time_start, time_end;

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
        clock_gettime(CLOCK_MONOTONIC, &time_start);
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
        clock_gettime(CLOCK_MONOTONIC, &time_end);

        //print response and response time
        printf("\n%s", buff);

        //clear the buffer
        memset(buff, '\0', sizeof(buff));

        //print delay
        double timeElapsed = ((double)(time_end.tv_nsec - time_start.tv_nsec)) / 1000000.0;
        rtt_msec = (time_end.tv_sec - time_start.tv_sec) * 1000.0 + timeElapsed;
        printf("\n------------------------------------\n");
        printf("Delay = %.3Lfms", rtt_msec);
        printf("\n------------------------------------\n");
    }
    printf("\n---------------------------------------\n");
    close(sockfd);
}