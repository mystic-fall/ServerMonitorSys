/*************************************************************************
> File Name: client.c
> Author: 
> Mail: 
> Created Time: Sat 21 Aug 2021 09:20:12 PM CST
************************************************************************/

#include "../1.headfiles/head.h"
#include "../1.headfiles/common.h"
#include "../1.headfiles/tcp_client.h"
#include "../1.headfiles/udp_client.h"
#include "./packageToJson.h"
#include "client.h"

char *IPC_FILE = "./output";

//进程间通信，使用消息队列
void sendmsg_to_parent(unsigned long mtype, char *msg_text) {
    int msqid = msgget((key_t)1234, IPC_CREAT | 0666);
    if(msqid < 0) PERR("msgget in send");
    struct MSG msg;
    memset(&msg, 0, sizeof(msg));
    msg.mtype = mtype;
    strncpy(msg.msg_text, msg_text, strlen(msg_text));
    //DBG("mtype = %ld\n%s\n", mtype, msg.msg_text);
    if(msgsnd(msqid, (void *)&msg, MSG_LEN, 0) < 0) 
    PERR("msgsnd");
    return ;
}

int recvmsg_from_child(unsigned long mtype, char *msg_buff) {
    int msqid = msgget((key_t)1234, IPC_CREAT | 0666);
    if(msqid < 0) PERR("msgget in recv");
    struct MSG msg;
    memset(&msg, 0, sizeof(msg));
    if(msgrcv(msqid, (void *)&msg, MSG_LEN, mtype, IPC_NOWAIT) < 0) {
        if(errno == ENOMSG) return 0;
        PERR("msgrcv");
    }
    //DBG("mtype = %ld\n%s\n", mtype, msg.msg_text);
    strncpy(msg_buff, msg.msg_text, strlen(msg.msg_text));
    return 1;
}
//<-------------------------------->

void do_reconnect() {
    tcp_sockfd = tcp_connect(master_ip, atoi(master_port));
    if(tcp_sockfd < 0) {
        if(kill(0, 9) < 0) PERR("kill");
        DBG(RED("failed to reconnect the server GG\n"));
        write_running_log(fp, "failed to reconnect the server, proc GG\n");
        exit(1);
    }
    DBG(GREEN_HL("reconnect successfully\n"));
    write_running_log(fp, "reconnect successfully\n");
    make_block(tcp_sockfd);
    return ;
}

//子线程监听入口，阻塞io来接收master的心跳信息
void *recvfrom_master(void *arg) {
    int sockfd = *(int *)arg;
    fd_set rset;
    while(1) { //客户端心跳，断线重连
        FD_ZERO(&rset);
        FD_SET(sockfd, &rset);
        struct timeval tv;
        tv.tv_sec = 30;
        tv.tv_usec = 0;
        int ret = select(sockfd + 1, &rset, NULL, NULL, &tv);
        if(ret > 0) { //收到心跳
            int recval;
            ssize_t n;  
            if(recv(sockfd, &recval, sizeof(int), 0) <= 0) {
                DBG(RED("server socket err\n"));
                write_running_log(fp, "server socket err\n");
                close(sockfd);
                do_reconnect();
                continue;
            } else if(send(sockfd, &recval, sizeof(int), 0) <= 0) {
                DBG(RED("server socket err\n"));
                write_running_log(fp, "server socket err\n");
                close(sockfd);
                do_reconnect();
                continue;
            }
            DBG(PURPLE_HL("recv and send heartbeat successfully\n"));
        } else if(ret == 0) { //30s没收到心跳，重连
            DBG(YELLOW("it's been long time without info from server\n"));
            write_running_log(fp, "it's been long time without info from server\n");
            close(sockfd);
            do_reconnect();
        } else { //error
            PERR("select");
        }
    }
    return NULL;
}

//警告信息通过udp发送
void send_warning(char *warning) {
    DBG(RED("%s\n"), warning);
    write_running_log(fp, "%s\n", warning);
    char str[20];
    char warn_info[1024] = {0};
    strncpy(warn_info, warning, strlen(warning));
    if(sendto(udp_sockfd, warn_info, sizeof(warn_info), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        perror("sendto");
    else {
        DBG(YELLOW_HL("warning msg is sent to server\n"));
        write_running_log(fp, "warning msg is sent to server\n");
    }
    return ;
}

void msg_handler(char *tmp_buff, int mtype) {
    struct MSG msg;
    memset(&msg, 0, sizeof(msg));
    msg.mtype = mtype;
    //printf("mtype = %ld\n", msg.mtype);
    char *json_str;
    switch(msg.mtype) {
        case 1: {
            send_warning(tmp_buff); 
            json_str = cJSON_PackageSysData(tmp_buff);
        } break;
        case 2: {
            json_str = cJSON_PackageSysData(tmp_buff);
        } break;
        case 3: {
            json_str = cJSON_PackageCpuData(tmp_buff);
        } break;
        case 4: {
            json_str = cJSON_PackageMemData(tmp_buff);
        } break;
        case 5: {
            json_str = cJSON_PackageDiskData(tmp_buff);    
        } break;
        case 6: {
            json_str = cJSON_PackageUserData(tmp_buff);    
        } break;
        case 7: {
            send_warning(tmp_buff);
            json_str = cJSON_PackageProcData(tmp_buff);    
        } break;
        default: {
            DBG(RED("invalid mtype\n"));
            exit(-1);
        }
    }
    strcpy(msg.msg_text, json_str);
    sendmsg_to_parent(msg.mtype, msg.msg_text);
    free(json_str);
    return ;
}

//工具人，相当于strstr("\n")
int get_offset(const char *info_buff) {
    for(int i = 0; info_buff[i]; i++) {
        if(info_buff[i] == '\n') return i + 1;
    }
    return 0;
}

#define BUFF_LEN 4096
#define READ_ONCE 512
//子进程定时执行shell脚本，并把信息传递给父进程
void run_child_main() {
    int pfd[2];
    if(pipe(pfd) < 0) PERR("pipe");
    pid_t pid[7];
    for(int i = 1; i <= 6; i++) {
        //if(i != 6) continue;
        pid[i] = fork();
        if(pid[i] < 0) PERR("fork");
        if(pid[i] == 0) {
            close(pfd[0]);
            int cnt = -1;
            while(1) {
                //每个脚本的执行时间间隔不同
                switch(i) {
                    case 1: sleep(3); break; //cpu
                    case 2: sleep(4); break; //mem
                    case 3: sleep(10); break; //disk
                    case 4: sleep(7); break; //user
                    case 5: sleep(8); break; //proc
                    case 6: sleep(5); break; //sys
                }
                pid_t work;
                if((work = fork()) < 0) PERR("fork in child");
                cnt++;
                if(work == 0) {
                    //DBG(YELLOW_HL("child proc<%d : %d> is created!\n"), i, cnt);
                    dup2(pfd[1], 1); //脚本的标准输出重定向到管道的写端
                    lockf(pfd[1], 1, 0); //上锁
                    execl("/bin/bash", "bash", ssp[i], NULL);
                    //出错
                    write_running_log(fp, "script running err\n");
                    PERR("execl");
                    exit(1);
                }
            }
            exit(0);
        }
    }
    close(pfd[1]);
    char read_buff[BUFF_LEN] = {0};
    ssize_t n, offset = 0;
    int ind;
    while((n = read(pfd[0], read_buff + offset, BUFF_LEN)) > 0) {
        offset += n;
        read_buff[offset] = '\0';
        while(ind = get_offset(read_buff)) {
            char tmp_buff[MSG_LEN] = {0};
            sscanf(read_buff, "%[^\n]\n", tmp_buff);
            msg_handler(tmp_buff + 2, tmp_buff[0] - '0');
            //DBG(GREEN_HL("%s\n"), tmp_buff);
            strcpy(read_buff, read_buff + ind);
            offset -= ind;
        }
    }
    return ;
}

int real_main() {
    do_client_conf();
    //udp client
    udp_sockfd = get_udp_socket(master_ip, atoi(master_udpport), &servaddr);
    pid_t pid = fork();
    if(pid < 0) PERR("fork in parent");
    if(pid == 0) {
        run_child_main();
        exit(0);
    } else {
        //tcp client
        tcp_sockfd = tcp_connect(master_ip, atoi(master_port));
        if(tcp_sockfd < 0) { //连接失败带走子进程
            if(kill(0, 9) < 0) PERR("kill");
            DBG(GREEN_HL("failed to connect the server GG\n"));
            write_running_log(fp, "failed to connect the server, proc GG\n");
            exit(1);
        } 
        DBG(BLUE_HL("succeed to connect the server\n"));
        write_running_log(fp, "succeed to connect the server\n");
        make_block(tcp_sockfd);
        //心跳线程，接收线程
        pthread_t hb_tid;
        pthread_create(&hb_tid, NULL, recvfrom_master, &tcp_sockfd);
        while(1) {
            memset(sys_data, 0, sizeof(sys_data));
            for(int i = 1; i <= 7; i++) {
                int flag = recvmsg_from_child(i, sys_data[i].info_text);
                if(flag == 0) continue;
                DBG(GREEN_HL("Info<itype = %d>:\n%s\n"), i, sys_data[i].info_text);
                struct Info_to_master info;
                info.itype = i;
                strcpy(info.info_text, sys_data[i].info_text);
                if(send(tcp_sockfd, &info, sizeof(info), 0) < 0) {
                    perror("send");
                    do_reconnect();
                }
            }
            sleep(1); //隔1s从消息队列中收一次消息
        }
    }
    return 0;
}

int main() {
    real_main();
    return 0;
    /*
    pid_t pid = fork();
    if(pid < 0) PERR("fock");
    if(pid == 0) {
        setsid();
        close(0), close(1), close(2);
        real_main();
    }
    exit(0);
    */
}

