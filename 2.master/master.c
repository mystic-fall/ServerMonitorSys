/*************************************************************************
> File Name: master.c
> Author: 
> Mail: 
> Created Time: Sat 21 Aug 2021 04:51:10 PM CST
************************************************************************/

#include "../1.headfiles/head.h"
#include "../1.headfiles/common.h"
#include "../1.headfiles/tcp_server.h"
#include "../1.headfiles/udp_server.h"
#include "../1.headfiles/epoll.h"
#include "../1.headfiles/DataStructure.h"
#include "mysql.h"
#include "mysql_conn_pool.h"
#include "mysql_opt.h"
#include "parseFromJson.h"
#include "task_pool.h"
#include "master.h"

void exception_handler(int taskfd, struct Info_from_client *info) {
    int host_id = client[taskfd].host_id;
    switch(info->itype) {
        case 0: break;
        case 1: 
        case 2: {
            struct Sys_Data sys_data;
            bzero(&sys_data, sizeof(sys_data));
            cJSON_parseSys(info->info_text, &sys_data);
            if(sys_insert_opt(host_id, sp, &sys_data)) {
                DBG(GREEN_HL("Insert 1 row into sys successfully\n"));
            } else {
                DBG(RED_HL("failed to Insert 1 row into sys\n"));
                write_running_log(fp, "failed to Insert 1 row into sys\n");
            }
        } break;
        case 3: {
            struct Cpu_Data cpu_data;
            bzero(&cpu_data, sizeof(cpu_data));
            cJSON_parseCpu(info->info_text, &cpu_data);
            if(cpu_insert_opt(host_id, sp, &cpu_data)) {
                DBG(GREEN_HL("Insert 1 row into cpu successfully\n"));
            } else {
                DBG(RED_HL("failed to Insert 1 row into cpu\n"));
                write_running_log(fp, "failed to Insert 1 row into cpu\n");
            }
        } break;
        case 4: {
            struct Mem_Data mem_data;
            bzero(&mem_data, sizeof(mem_data));
            cJSON_parseMem(info->info_text, &mem_data);
            if(mem_insert_opt(host_id, sp, &mem_data)) {
                DBG(GREEN_HL("Insert 1 row into mem successfully\n"));
            } else {
                DBG(RED_HL("failed to Insert 1 row into mem\n"));
                write_running_log(fp, "failed to Insert 1 row into mem\n");
            }
        } break;
        case 5: {
            struct Disk_Data disk_data;
            bzero(&disk_data, sizeof(disk_data));
            cJSON_parseDisk(info->info_text, &disk_data);
            if(disk_insert_opt(host_id, sp, &disk_data)) {
                DBG(GREEN_HL("Insert 1 row into disk successfully\n"));
            } else {
                DBG(RED_HL("failed to Insert 1 row into disk\n"));
                write_running_log(fp, "failed to Insert 1 row into disk\n");
            }
        } break;
        case 6: {
            struct User_Data user_data;
            bzero(&user_data, sizeof(user_data));
            cJSON_parseUser(info->info_text, &user_data);
            if(user_insert_opt(host_id, sp, &user_data)) {
                DBG(GREEN_HL("Insert 1 row into user successfully\n"));
            } else {
                DBG(RED_HL("failed to Insert 1 row into user\n"));
                write_running_log(fp, "failed to Insert 1 row into user\n");
            }
        } break;
        case 7: {
            struct Proc_Data proc_data;
            bzero(&proc_data, sizeof(proc_data));
            cJSON_parseProc(info->info_text, &proc_data);
            if(proc_insert_opt(host_id, sp, &proc_data)) {
                DBG(GREEN_HL("Insert 1 row into proc successfully\n"));
            } else {
                DBG(RED_HL("failed to Insert 1 row into proc\n"));
                write_running_log(fp, "failed to Insert 1 row into proc\n");
            }
        } break;
    }
    return ;
}

void *db_test(void *arg) {
    pthread_detach(pthread_self());
    SQL_CONN_POOL *sp = (SQL_CONN_POOL *)arg;
    while(1) {
        if(sp->disconn_num > CONN_MAX_NUM / 3) {
            sql_pool_destroy(sp);
            sp = sql_pool_init(atoi(_db_conn_num), _db_ip, _db_name, _db_user, _db_passwd);
        }
        sleep(20); //20s一次db test
    }
    return NULL;
}

//收到客户端的警告后写日志，使用udp
void *recv_warning(void *arg) {
    int udp_sockfd = *(int *)arg;
    struct sockaddr_in clieaddr;
    socklen_t clieaddr_len = sizeof(clieaddr);
    bzero(&clieaddr, sizeof(clieaddr));
    fd_set rset;
    while(1) {
        FD_ZERO(&rset);
        FD_SET(udp_sockfd, &rset);
        if(select(udp_sockfd + 1, &rset, NULL, NULL, NULL) <= 0) {
            perror("select");
            write_running_log(fp, "master err: select");
        }
        bzero(warning, sizeof(warning));
        ssize_t n, package_size = 1024;
        while(package_size) {
            n = recvfrom(udp_sockfd, warning, package_size, 0, (struct sockaddr *)&clieaddr, &clieaddr_len);
            if(n <= 0) {
                perror("recvfrom");
                write_running_log(fp, "master err: recvfrom");
            }
            package_size -= n;
        }
        char log_buff[1200] = {0}, str[20];
        sprintf(log_buff, "recvfrom client ip: %s, port: %d\n%s", inet_ntop(AF_INET
        , &clieaddr.sin_addr, str, sizeof(str)), ntohs(clieaddr.sin_port), warning);
        DBG(YELLOW_HL("%s\n"), log_buff);
        write_running_log(fp, "%s\n", log_buff);
    }
    return NULL;
}

//心跳工具
void do_addone(int taskfd) {
    pthread_mutex_lock(&client[taskfd].lock);
    client[taskfd].heartbeat_times += 1;
    pthread_mutex_unlock(&client[taskfd].lock);
    return ;
}

void do_reset(int taskfd) {
    pthread_mutex_lock(&client[taskfd].lock);
    client[taskfd].heartbeat_times = 0;
    pthread_mutex_unlock(&client[taskfd].lock);
    return ;
}

void do_clear(int taskfd) {
    close(taskfd);
    epoll_del(epfd, taskfd, 0);
    DBG(YELLOW_HL("client<%d> is closed\n"), taskfd);
    write_running_log(fp, "client<%d> is closed\n", taskfd);
    bzero(&client[taskfd], sizeof(client[taskfd]));
    return ;
}

void *do_heartbeat() {
    pthread_detach(pthread_self());
    while(1) {
        for(int i = 3; i < 10050; i++) {
            if(client[i].is_online == 0) continue;
            if(client[i].heartbeat_times == 5) {
                DBG("client<%d> hasn't responded for a long time\n", i);
                write_running_log(fp, "client<%d> hasn't responded for a long time\n", i);
                do_clear(i);
                continue;
            }
            int val = 0;
            if(send(i, &val, sizeof(int), 0) <= 0){
                perror("send");
                DBG(RED("heartbeat: err occured on client<%d>\n"), i);
                write_running_log(fp, "heartbeat: err occured on client<%d>\n", i);
                do_clear(i);
            } else {
                DBG(GREEN_HL("send heartbeat data to client<%d> successfully\n"), i);
                do_addone(i);
            }
        }
        sleep(5); //5s心跳一次
    }
    return NULL;
}

//主接收线程 tcp 收到的数据解析后插入数据库中 同时接受心跳包
void *do_work() {
    pthread_detach(pthread_self());
    while(1) {
        Task task = task_pool_pop(tp);
        struct Info_from_client info;
        int taskfd = task.taskfd;
        memset(&info, 0, sizeof(info));
        while(1) {
            ssize_t n = recv(taskfd, &info, sizeof(int), 0);
            if(n > 0) {
                do_reset(taskfd);
                if(info.itype == 0) {
                    DBG(YELLOW_HL("recv heartbeat data from client<%d>\n"), taskfd);
                    continue;
                }
                n = recv(taskfd, (void *)&info + 4, sizeof(info) - 4, 0);
                DBG("recv task from client<%d>, itype=%d\n%s\n", taskfd, info.itype, info.info_text);
                exception_handler(taskfd, &info);
            } else if(n < 0) {
                if(errno == EAGAIN || errno == EWOULDBLOCK) {
                    DBG(BLUE("client<%d> recv buffer is empty, work done!\n"), taskfd);
                } else {
                    perror("recv");
                    DBG(RED("do_work: client<%d> other err happend\n"), taskfd);
                    write_running_log(fp, "do_work: client<%d> other err happend\n", taskfd);
                    do_clear(taskfd);
                }
                break;
            } else {
                DBG(YELLOW("client<%d> closed the connection, we should contact him to restart!!\n") , taskfd);
                write_running_log(fp, "client<%d> closed the connection, we should contact him to restart!!\n", taskfd);
                do_clear(taskfd);
                break;
            }
        }
    }
    return NULL;
}

int main() {
    do_master_conf();
    FILE *fp = fopen("./master.log", "a+");
    fprintf(fp, "ashjjashdajd");
    write_running_log(fp, "heheheheheh\n");
    struct epoll_event events[256];
    if((epfd = epoll_create(1)) < 0) PERR("epoll_create");
    //创建tcp
    listenfd = tcp_socket_create(master_ip, atoi(master_port), 10);
    epoll_add(epfd, listenfd, EPOLLIN);
    //创建udp
    udp_sockfd = create_udp_sockfd(master_ip, atoi(master_udpport));
    
    //数据库线程，管理数据库连接，运行在子进程中
    pthread_create(&db_tid, NULL, db_test, (void *)sp);
    //udp线程，用来处理警告信息，运行在主进程中
    pthread_create(&warning_tid, NULL, recv_warning, (void *)&udp_sockfd);
    //创建子进程时并没有复制线程？？
    pid_t pid[5];
    for(int i = 0; i < 5; i++) {
        pid[i] = fork();
        if(pid[i] == 0) {
            //心跳线程，用来给客户端发送心跳包，运行在子进程中
            pthread_create(&hb_tid, NULL, do_heartbeat, NULL);
            pthread_t tid[4];
            for(int i = 0; i < 4; i++) {
                pthread_create(&tid[i], NULL, do_work, NULL);
            }
            while(1) {
                int nready = epoll_wait(epfd, events, 256, -1);
                if(nready < 0) {
                    perror("epoll_wait");
                    DBG("pid[%d] epoll is wrong\n", i);
                    write_running_log(fp, "pid[%d] epoll is wrong\n", i);
                    exit(-1);
                }
                for(int i = 0; i < nready; i++) {
                    int taskfd = events[i].data.fd;
                    if(taskfd == listenfd) {
                        char clnt_ip[20] = {0};
                        int clnt_port;
                        int connfd = tcp_con_accept(listenfd, clnt_ip, &clnt_port);
                        DBG(BLUE_HL("Accept connection from client<fd=%d> ip = %s,port = %d\n"), connfd, clnt_ip, clnt_port);
                        write_running_log(fp, "Accept connection from client<fd=%d> ip = %s,port = %d\n", connfd, clnt_ip, clnt_port);
                        //设置为 nonblock io
                        make_nonblock(connfd);

                        pthread_mutex_init(&client[connfd].lock, NULL);
                        client[connfd].host_id = host_id_distributor();
                        strcpy(client[connfd].host_ip, clnt_ip);
                        client[connfd].host_port = clnt_port;
                        client[connfd].is_online = 1;

                        epoll_add(epfd, connfd, EPOLLIN | EPOLLET);
                    } else if(events[i].events & EPOLLIN) {
                        task_pool_push(tp, taskfd);
                    }
                }
            }
            close(listenfd);
            close(udp_sockfd);
            close(epfd);
            exit(0);
        }
    }
    for(int i = 0; i < 10; i++)
        wait(NULL);
    close(listenfd);
    close(udp_sockfd);
    close(epfd);
    return 0;
}

