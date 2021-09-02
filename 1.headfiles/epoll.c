/*************************************************************************
	> File Name: epoll.c
	> Author: 
	> Mail: 
	> Created Time: Mon 23 Aug 2021 11:13:36 AM CST
 ************************************************************************/

#include "head.h"

void epoll_add(int epfd, int sockfd, int events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = sockfd;
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev) < 0)
        PERR("epoll add");
    return ;
}

void epoll_mod(int epfd, int sockfd, int events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = sockfd;
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev) < 0)
        PERR("epoll mod");
    return ;
}

void epoll_del(int epfd, int sockfd, int events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = sockfd;
    if(epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, &ev) < 0) 
        perror("epoll del");
    return ;
}
