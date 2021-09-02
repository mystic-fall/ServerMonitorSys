/*************************************************************************
	> File Name: common.c
	> Author: 
	> Mail: 
	> Created Time: Sat 21 Aug 2021 02:18:42 PM CST
 ************************************************************************/

#include "head.h"
#include<inttypes.h>
#include<stdarg.h>

void write_running_log(FILE *fp, char *fmt, ...) {
    time_t timer = time(NULL);
    char *ct = ctime(&timer);
    ct[strlen(ct) - 1] = '\0';
    va_list args;
    va_start(args, fmt);
    flock(fp->_fileno, LOCK_EX);
    fprintf(fp, "[%s]\n", ct);
    vfprintf(fp, fmt, args);
    fflush(fp);
    flock(fp->_fileno, LOCK_UN);
    va_end(args);
    return ;
}

void get_conf_val(char *filepath, char *key, char *val) {
    if(key == NULL || val == NULL) {
        DBG(RED("invalid arguments!!!\n"));
        exit(1);
    }
    FILE *fp;
    if((fp = fopen(filepath, "r")) == NULL) {
        PERR("fopen");
    }
    int curr;
    size_t n;
    char *line = (char *)malloc(sizeof(char) * 100);
    while((curr = getline(&line, &n, fp)) != -1) {
        char *p = strstr(line, key);
        if(p == NULL || p[strlen(key)] != '=') continue;
        strncpy(val, p + strlen(key) + 1, (int)curr - strlen(key) - 2);
        val[(int)curr - strlen(key) - 2] = '\0';
        break;
    }
    free(line);
    if(curr == -1) {
        DBG(YELLOW("Key Not Found!!!\n"));
        exit(0);
    }
    return ;
}

void make_nonblock(int fd) {
    int raw_flag;
    if((raw_flag = fcntl(fd, F_GETFL, 0)) < 0) 
        PERR("fcntl get flag in make_nonblock");

    if(fcntl(fd, F_SETFL, raw_flag | O_NONBLOCK) < 0)
        PERR("fcntl set flag in make_nonblock");

    return ;
}

void make_block(int fd) {
    int raw_flag;
    if((raw_flag = fcntl(fd, F_GETFL, 0)) < 0) 
        PERR("fcntl get flag in make_block");

    if(fcntl(fd, F_SETFL, raw_flag & ~O_NONBLOCK) < 0)
        PERR("fcntl set flag in make_block");

    return ;
}

