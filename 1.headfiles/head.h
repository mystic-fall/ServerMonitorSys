/*************************************************************************
	> File Name: head.h
	> Author: 
	> Mail: 
	> Created Time: Mon 16 Aug 2021 03:41:26 PM CST
 ************************************************************************/

#ifndef _HEAD_H
#define _HEAD_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dirent.h>
#include<errno.h>
#include<sys/select.h>
#include<sys/time.h>
#include<sys/wait.h>
#include<sys/file.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/msg.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/epoll.h>
#include<string.h>
#include<strings.h>
#include "color.h"

#ifdef _D 
#define DBG(fmt, args...) printf(fmt, ##args)
#else 
#define DBG(fmt, args...) 
#endif

#define PERR(msg) perror(msg), exit(1)

#endif
