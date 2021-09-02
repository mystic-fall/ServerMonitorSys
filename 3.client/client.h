/*************************************************************************
	> File Name: client.h
	> Author: 
	> Mail: 
	> Created Time: Sat 21 Aug 2021 09:40:45 PM CST
 ************************************************************************/

#ifndef _CLIENT_H
#define _CLIENT_H

#define MSG_LEN 1024
#define INFO_LEN 1024

char *client_conf_path = "./client.conf";
char master_ip[30];
char master_port[30];
char master_udpport[30];
char client_log_path[40];
char ssp[7][30];  //shellscript path
FILE *fp; //client_log_path
int tcp_sockfd, udp_sockfd;
struct sockaddr_in servaddr;

//用于发送消息队列
struct MSG {
    unsigned long mtype;
    char msg_text[MSG_LEN];
};

//用于接收消息队列
struct {
    unsigned long mtype;
    char info_text[MSG_LEN];
} sys_data[7];

struct Info_to_master {
    unsigned int itype;
    char info_text[INFO_LEN];
};

void do_client_conf() {
    get_conf_val(client_conf_path, "MASTER_IP", master_ip);
    get_conf_val(client_conf_path, "MASTER_PORT", master_port);
    get_conf_val(client_conf_path, "MASTER_UDPPORT", master_udpport);
    get_conf_val(client_conf_path, "CLIENT_LOG_PATH", client_log_path);
    get_conf_val(client_conf_path, "CPU_SS_PATH", ssp[1]);
    get_conf_val(client_conf_path, "MEM_SS_PATH", ssp[2]);
    get_conf_val(client_conf_path, "DISK_SS_PATH", ssp[3]);
    get_conf_val(client_conf_path, "USER_SS_PATH", ssp[4]);
    get_conf_val(client_conf_path, "PROC_SS_PATH", ssp[5]);
    get_conf_val(client_conf_path, "SYSINFO_SS_PATH", ssp[6]);
    fp = fopen(client_log_path, "a+");
    if(fp == NULL) PERR("fopen");
    setbuf(fp, NULL);
    return ;
}

#endif
