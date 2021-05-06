#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000
#define PORT 8080

int main()
{
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    //bind socket
    if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }

    time_t rawtime;
    struct tm *timeinfo;
    char currTime[100];
    char buff[MAX];
    char response[200] = "Hi, How may I help you?\nPress 1 to know more about me.\nPress 2 for a joke.\0";
    char knowMore[200] = "I'm Alan. A chat-bot.\0";
    char joke[200] = "What do you call a fake noodle?\nAn impasta.\0";

    struct sockaddr_in client;
    int len = sizeof(client);

    while (1)
    {
        //get message from client
        if (recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&client, &len) == -1)
        {
            perror("Error in recvfrom()");
            exit(1);
        }

        //get the time
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strcpy(currTime, asctime(timeinfo));

        //send the time-stamp
        if (sendto(sockfd, currTime, sizeof(currTime), 0, (struct sockaddr *)&client, len) == -1)
        {
            perror("Error in sendto()");
            exit(1);
        }

        //clear the time buffer
        memset(currTime, '\0', sizeof(currTime));

        //prepare the response
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strcpy(currTime, asctime(timeinfo));
        if (buff[0] == '1')
        {
            memset(buff, '\0', sizeof(buff));
            sprintf(buff, "Server : %s\nTime : %s", knowMore, currTime);
        }
        else if (buff[0] == '2')
        {
            memset(buff, '\0', sizeof(buff));
            sprintf(buff, "Server : %s\nTime : %s", joke, currTime);
        }
        else
        {
            memset(buff, '\0', sizeof(buff));
            sprintf(buff, "Server : %s\nTime : %s", response, currTime);
        }

        //send the response
        if (sendto(sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&client, len) == -1)
        {
            perror("Error in sendto()");
            exit(1);
        }
    }
}