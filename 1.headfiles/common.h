/*************************************************************************
	> File Name: common.h
	> Author: 
	> Mail: 
	> Created Time: Wed 25 Aug 2021 03:40:58 PM CST
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H

void get_conf_val(char *filepath, char *key, char *val);
void write_running_log(FILE *fp, char *fmt, ...);
void make_nonblock(int fd);
void make_block(int fd);

#endif
