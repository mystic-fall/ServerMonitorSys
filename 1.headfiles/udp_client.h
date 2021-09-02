/*************************************************************************
	> File Name: udp_client.h
	> Author: 
	> Mail: 
	> Created Time: Wed 01 Sep 2021 08:07:05 PM CST
 ************************************************************************/

#ifndef _UDP_CLIENT_H
#define _UDP_CLIENT_H

int get_udp_socket(char *ip, int port, struct sockaddr_in *servaddr);

#endif
