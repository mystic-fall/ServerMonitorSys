/*************************************************************************
	> File Name: epoll.h
	> Author: 
	> Mail: 
	> Created Time: Mon 23 Aug 2021 11:17:55 AM CST
 ************************************************************************/

#ifndef _EPOLL_H
#define _EPOLL_H

void epoll_add(int epfd, int sockfd, int events); 

void epoll_mod(int epfd, int sockfd, int events);

void epoll_del(int epfd, int sockfd, int events);

#endif
