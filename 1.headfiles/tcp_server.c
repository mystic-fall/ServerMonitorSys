/*************************************************************************
	> File Name: tcp_server.c
	> Author: 
	> Mail: 
	> Created Time: Sat 21 Aug 2021 01:51:15 PM CST
 ************************************************************************/

#include "head.h"
#include<netinet/tcp.h>

int tcp_socket_create(char *ip, int port, int backlog) {
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd < 0) PERR("socket");
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //设置为端口复用
    int optval = 1;
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
        PERR("setsockopt");
    
    if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        PERR("bind");
    
    if(listen(listenfd, backlog) < 0) 
        PERR("listen");
    DBG(YELLOW_HL("server started !!!\n"));
    return listenfd;
}

int tcp_con_accept(int listenfd, char *ip, int *port) {
    struct sockaddr_in clntaddr;
    socklen_t clntaddr_len = sizeof(clntaddr);
    int connfd = accept(listenfd, (struct sockaddr*)&clntaddr, &clntaddr_len);
    if(connfd < 0) {
        PERR("accept");
    }
    char str[20] = {0};
    strcpy(ip, inet_ntop(AF_INET, &clntaddr.sin_addr, str, sizeof(str)));
    *port = ntohs(clntaddr.sin_port);
    //DBG("receive connection client<%d> from ip = %s, port = %d\n", 
    //connfd, inet_ntop(AF_INET, &clntaddr.sin_addr, str, sizeof(str)), 
    //ntohs(clntaddr.sin_port));
    return connfd;
}

void set_tcp_keepalive(int connfd, int intvl, int probes, int time) {
    int tcp_keepalive_intvl = intvl;
    int tcp_keepalive_probes = probes;
    int tcp_keepalive_time = time;
    int tcp_keepalive_on = 1;

    if(setsockopt(connfd, SOL_SOCKET, SO_KEEPALIVE
    , &tcp_keepalive_on, sizeof(tcp_keepalive_on)) < 0) 
        perror("setsockopt 1"), exit(1);
    if(setsockopt(connfd, SOL_TCP, TCP_KEEPINTVL
    , &tcp_keepalive_on, sizeof(tcp_keepalive_on)) < 0) 
        perror("setsockopt 1"), exit(1);
    if(setsockopt(connfd, SOL_TCP, TCP_KEEPCNT
    , &tcp_keepalive_on, sizeof(tcp_keepalive_on)) < 0) 
        perror("setsockopt 1"), exit(1);
    if(setsockopt(connfd, SOL_TCP, TCP_KEEPIDLE
    , &tcp_keepalive_on, sizeof(tcp_keepalive_on)) < 0) 
        perror("setsockopt 1"), exit(1);

    DBG(YELLOW_HL("set keepalive successfully!!\n"));
    return ;
}
