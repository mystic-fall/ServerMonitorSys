/*************************************************************************
	> File Name: tcp_client.c
	> Author: 
	> Mail: 
	> Created Time: Sat 21 Aug 2021 02:04:10 PM CST
 ************************************************************************/

#include "head.h"
#include "common.h"

int tcp_connect(char *ip, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) perror("socket");
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &servaddr.sin_addr);
    printf("%s %d\n", ip, port);
    
    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        perror("connect");

    return sockfd;
}
