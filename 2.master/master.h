/*************************************************************************
	> File Name: master.h
	> Author: 
	> Mail: 
	> Created Time: Wed 25 Aug 2021 03:03:07 PM CST
 ************************************************************************/

#ifndef _MASTER_H
#define _MASTER_H

#define INFO_LEN 1024

char *master_conf_path = "./master.conf";
char master_ip[20];
char master_port[20];
char master_udpport[20];
char master_log_path[40];

char _db_conn_num[10];
char _db_ip[20];
//char _db_port[20];
char _db_name[20];
char _db_user[20];
char _db_passwd[20];

FILE *fp; //master_log_path
int listenfd, udp_sockfd; //tcp udp
int epfd; //epoll
Task_Pool *tp; //任务池
SQL_CONN_POOL *sp; //数据库连接池
pthread_t db_tid, warning_tid, hb_tid; //工作线程
char warning[1024]; //警告信息

struct Info_from_client {
    unsigned int itype;
    char info_text[INFO_LEN];
};

struct {
    int host_id;
    char host_ip[20];
    int host_port;
    int is_online;
    int heartbeat_times;
    pthread_mutex_t lock;
} client[10050]; //hash结构存储用户，用于心跳

//分发host_id
int host_id_distributor() {
    static int host_id = 0;
    host_id += 1;
    return host_id;
}

void do_master_conf() {
    get_conf_val(master_conf_path, "MASTER_IP", master_ip);
    get_conf_val(master_conf_path, "MASTER_PORT", master_port);
    get_conf_val(master_conf_path, "MASTER_UDPPORT", master_udpport);
    get_conf_val(master_conf_path, "MASTER_LOG_PATH", master_log_path);
    get_conf_val(master_conf_path, "_DB_CONN_NUM", _db_conn_num);
    get_conf_val(master_conf_path, "_DB_IP", _db_ip);
    get_conf_val(master_conf_path, "_DB_NAME", _db_name);
    get_conf_val(master_conf_path, "_DB_USER", _db_user);
    get_conf_val(master_conf_path, "_DB_PASSWD", _db_passwd);
    
    fp = fopen(master_log_path, "a+");
    if(fp == NULL) PERR("fopen");
    setbuf(fp, NULL);
    tp = task_pool_init();
    sp = sql_pool_init(atoi(_db_conn_num), _db_ip, _db_name, _db_user, _db_passwd);
    memset(&client, 0, sizeof(client));
    return ;
}

#endif
