/*************************************************************************
	> File Name: mysql_connect_pool.h
	> Author: 
	> Mail: 
	> Created Time: Tue 31 Aug 2021 10:20:35 PM CST
 ************************************************************************/

#ifndef _MYSQL_CONNECT_POOL_H
#define _MYSQL_CONNECT_POOL_H

#define CONN_MAX_NUM 15
typedef struct SQL_NODE {
    MYSQL *mysql_conn;
    int used;
    int index;
    enum {
        DB_DISCONN, DB_CONN
    } sql_state;
} SQL_NODE;

typedef struct SQL_CONN_POOL {
    int start;
    int pool_size;
    int disconn_num;
    char ip[20];
    //int port;
    char db_name[20];
    char user[20];
    char passwd[20]; 
    SQL_NODE sql_pool[CONN_MAX_NUM];
    pthread_mutex_t lock;
    sem_t free_num;
} SQL_CONN_POOL;

#endif
