/*************************************************************************
	> File Name: ../1.headfiles/udp_client.c
	> Author: 
	> Mail: 
	> Created Time: Tue 31 Aug 2021 02:48:48 PM CST
 ************************************************************************/

#include "head.h"

int get_udp_socket(char *ip, int port, struct sockaddr_in *servaddr) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0) perror("udp socket");
    bzero(servaddr, sizeof(*servaddr));
    servaddr->sin_family = AF_INET;
    servaddr->sin_port = htons(port);
    inet_pton(AF_INET, ip, &servaddr->sin_addr);
    return sockfd;
}

