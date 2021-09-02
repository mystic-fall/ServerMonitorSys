/*************************************************************************
	> File Name: udp_server.c
	> Author: 
	> Mail: 
	> Created Time: Tue 31 Aug 2021 02:54:03 PM CST
 ************************************************************************/

#include "head.h"

int create_udp_sockfd(char *ip, int port) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0) PERR("socket");
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //设置为端口复用
    int optval = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        PERR("setsockopt");
    
    if(bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        PERR("bind in udp");
    return sockfd;
}

