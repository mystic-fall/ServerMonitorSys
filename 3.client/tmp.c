/*************************************************************************
	> File Name: tmp.c
	> Author: 
	> Mail: 
	> Created Time: Wed 01 Sep 2021 06:29:04 PM CST
 ************************************************************************/

#include<stdio.h>

    //处理粘包
    while(left > 0) {
        n = recv(sockfd, (void *)&info + offset, left, 0);
        left -= n;
        offset += n;
        if(n > 0) {
            printf("left = %d\n", left);
        } else if(n == 0) {
            DBG(RED("connection failed!\n"));
            exit(1);
        } else {
            DBG(RED("connection is wrong\n"));
            exit(1);
        }
    }
