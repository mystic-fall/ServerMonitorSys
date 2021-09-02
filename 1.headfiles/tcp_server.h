/*************************************************************************
	> File Name: tcp_server.h
	> Author: 
	> Mail: 
	> Created Time: Sat 21 Aug 2021 02:28:14 PM CST
 ************************************************************************/

#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H

int tcp_socket_create(char *ip, int port, int backlog);
int tcp_con_accept(int listenfd, char *ip, int *port);
int set_tcp_keepalive(int connfd, int intvl, int probes, int time);

#endif
