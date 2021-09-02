/*************************************************************************
	> File Name: mysql_op.c
	> Author: 
	> Mail: 
	> Created Time: Tue 31 Aug 2021 08:50:11 PM CST
 ************************************************************************/

#include "../1.headfiles/head.h"
#include "../1.headfiles/DataStructure.h"
#include "mysql.h"
#include "mysql_conn_pool.h"

int create_db_conn(SQL_CONN_POOL *sp, SQL_NODE *node) {
    if(sp == NULL || node == NULL) {
        DBG(RED("sp or node is NULL, exit\n"));
        exit(-1);
    }
    node->mysql_conn = mysql_init(NULL);
    if(node->mysql_conn == NULL) {
        DBG(RED("errno in init, errno = %d\n"), mysql_errno(node->mysql_conn));
        node->sql_state = DB_DISCONN;
        return 0;
    }
    if(mysql_real_connect(node->mysql_conn, sp->ip, sp->user, sp->passwd
    , sp->db_name, 0, NULL, 0) == NULL) {
        DBG(RED("can not connect to mysql\n"));
        node->sql_state = DB_DISCONN;
        return 0;
    }
    node->used = 0;
    node->sql_state = DB_CONN;
    int opt = 1; //开启自动连接
    mysql_options(node->mysql_conn, MYSQL_OPT_RECONNECT, &opt);
    opt = 3; //连接超时时间为3s
    mysql_options(node->mysql_conn, MYSQL_OPT_CONNECT_TIMEOUT, &opt);
    return 1;
}

SQL_CONN_POOL *sql_pool_init(int connect_pool_size                               
 , char *ip, char *db_name, char *user, char *passwd) {
    if(connect_pool_size < 1) {
        connect_pool_size = 1;
    } else if(connect_pool_size > CONN_MAX_NUM) {
        connect_pool_size = CONN_MAX_NUM;
    } 
    SQL_CONN_POOL *sp = (SQL_CONN_POOL *)malloc(sizeof(SQL_CONN_POOL));
    sp->start = 1;
    sp->disconn_num = 0;
    sp->pool_size = connect_pool_size;
    pthread_mutex_init(&sp->lock, NULL);
    sem_init(&sp->free_num, 0, connect_pool_size);
    strcpy(sp->ip, ip);
    strcpy(sp->db_name, db_name);
    strcpy(sp->user, user);
    strcpy(sp->passwd, passwd);
    for(int i = 0; i < connect_pool_size; i++) {
        sp->sql_pool[i].index = i;
        if(create_db_conn(sp, &sp->sql_pool[i]) == 0) {
            DBG(RED("node<%d> connection failed, please recreate the pool\n"), i);
            sp->disconn_num += 1;
        } else {
            DBG(YELLOW("node<%d> connection success\n"), i);  
        }
    }
    return sp;
}

void sql_pool_destroy(SQL_CONN_POOL *sp) {
    pthread_mutex_lock(&sp->lock);
    sp->start = 0;
    sleep(3);
    for(int i = 0; i < sp->pool_size; i++) {
        if(sp->sql_pool[i].sql_state == DB_CONN) {
            mysql_close(sp->sql_pool[i].mysql_conn);
            sp->sql_pool[i].mysql_conn = NULL;
        }
        sp->sql_pool[i].used = 0;
        sp->sql_pool[i].sql_state = DB_DISCONN;
    }
    pthread_mutex_unlock(&sp->lock);
    pthread_mutex_destroy(&sp->lock);
    sem_destroy(&sp->free_num);
    return ;
}

void release_conn(SQL_CONN_POOL *sp, int index) {
    if(sp == NULL) {
        printf("sql pool stopped\n");
        exit(-1);
    }
    pthread_mutex_lock(&sp->lock);
    sp->sql_pool[index].used = 0;
    sem_post(&sp->free_num);
    pthread_mutex_unlock(&sp->lock);
    return ;
}

SQL_NODE *get_conn(SQL_CONN_POOL *sp) {
    if(sp == NULL || sp->start == 0) {
        DBG("sql pool stopped\n");
        exit(-1);
    }
    sem_wait(&sp->free_num);
    pthread_mutex_lock(&sp->lock);
    srand(time(NULL));
    int start_index = rand() % sp->pool_size;
    for(int i = 0; i < sp->pool_size; i++) {
        int index = (start_index + i) % sp->pool_size;
        if(sp->sql_pool[index].used == 0) {
            if(sp->sql_pool[index].sql_state == DB_DISCONN) { //重连
                if(create_db_conn(sp, &sp->sql_pool[index]) == 0) {
                    DBG(RED("node<%d> reconnect failed\n"), index);
                    continue;
                } else { //重连成功，返回连接使用
                    DBG(GREEN("node<%d> reconnect succeed\n"), index);
                    sp->disconn_num -= 1;
                    sp->sql_pool[index].sql_state = DB_CONN;
                    sp->sql_pool[index].used = 1;
                    pthread_mutex_unlock(&sp->lock);
                    return &sp->sql_pool[index]; 
                }
            }
            int ping_res = mysql_ping(sp->sql_pool[index].mysql_conn);
            if(ping_res != 0) { //服务器关闭了连接
                DBG(RED("ping mysql error\n"));
                sp->sql_pool[index].sql_state = DB_DISCONN;
                sp->disconn_num += 1;
                continue;
            }
            //返回可用链接
            sp->sql_pool[index].used = 1;
            pthread_mutex_unlock(&sp->lock);
            return &sp->sql_pool[index];
        }
    }
    pthread_mutex_unlock(&sp->lock);
    sem_post(&sp->free_num);
    return NULL;
}

int sys_insert_opt(int host_id, SQL_CONN_POOL *sp, struct Sys_Data *sys_data) {
    SQL_NODE *node;
    while((node = get_conn(sp)) == NULL) {
        sleep(1);
    }
    MYSQL *conn = node->mysql_conn;
    char sys_insert_sql[2000] = {0};
    sprintf(sys_insert_sql, "INSERT INTO sys(host_id,nowtime,hostname,os_version,kernal_version,runtime,loadavg_1min,loadavg_5min,loadavg_15min,disk_total,disk_usage,mem_total,mem_usage,cpu_temp,disk_warning,mem_warning,cpu_warning) VALUES(%d,'%s','%s','%s','%s','%s',%.2lf,%.2lf,%.2lf,'%s','%s','%s','%s',%.2f,'%s','%s','%s')", host_id, sys_data->nowtime, sys_data->hostname, sys_data->os_version, sys_data->kernal_version, sys_data->runtime, sys_data->loadavg[0], sys_data->loadavg[1], sys_data->loadavg[2], sys_data->disk_total, sys_data->disk_usage, sys_data->mem_total, sys_data->mem_usage, sys_data->cpu_temp, sys_data->disk_warning, sys_data->mem_warning, sys_data->cpu_warning);
    //printf("%s\n", sys_insert_sql);
    int ret = mysql_query(conn, sys_insert_sql);
    //if(!ret) {
    //    printf("Insert %d rows into sys\n", (int)mysql_affected_rows(conn));
    //}
    release_conn(sp, node->index);
    return !ret; //1:success 2:failure
}

int cpu_insert_opt(int host_id, SQL_CONN_POOL *sp, struct Cpu_Data *cpu_data) {
    SQL_NODE *node;
    while((node = get_conn(sp)) == NULL) {
        sleep(1);
    }
    MYSQL *conn = node->mysql_conn;
    char cpu_insert_sql[2000] = {0};
    sprintf(cpu_insert_sql, "INSERT INTO cpu(host_id,nowtime,loadavg_1min,loadavg_5min,loadavg_15min,`usage`,curr_temp,warning) VALUES(%d,'%s',%.2lf,%.2lf,%.2lf,'%s',%.2f,'%s')", host_id, cpu_data->nowtime, cpu_data->loadavg[0], cpu_data->loadavg[1], cpu_data->loadavg[2], cpu_data->usage, cpu_data->curr_temp, cpu_data->warning);
    int ret = mysql_query(conn, cpu_insert_sql);
    release_conn(sp, node->index);
    return !ret;
}

int mem_insert_opt(int host_id, SQL_CONN_POOL *sp, struct Mem_Data *mem_data) {
    SQL_NODE *node;
    while((node = get_conn(sp)) == NULL) {
        sleep(1);
    }
    MYSQL *conn = node->mysql_conn;
    char mem_insert_sql[2000] = {0};
    sprintf(mem_insert_sql, "INSERT INTO mem(host_id,nowtime,total,free,`usage`,dynamic_avg) VALUES(%d,'%s','%s','%s','%s','%s')", host_id, mem_data->nowtime, mem_data->total, mem_data->free, mem_data->usage, mem_data->dynamic_avg);
    int ret = mysql_query(conn, mem_insert_sql);
    release_conn(sp, node->index);
    return !ret;
}

int disk_insert_opt(int host_id, SQL_CONN_POOL *sp, struct Disk_Data *disk_data) {
    SQL_NODE *node;
    while((node = get_conn(sp)) == NULL) {
        sleep(1);
    }
    MYSQL *conn = node->mysql_conn;
    char disk_insert_sql[2000] = {0};
    sprintf(disk_insert_sql, "INSERT INTO disk(host_id,nowtime,flag,swap_name,total,free,`usage`) VALUES(%d,'%s',%d,'%s',%d,%d,'%s')", host_id, disk_data->nowtime, disk_data->flag, disk_data->swap_name, disk_data->total, disk_data->free, disk_data->usage);
    int ret = mysql_query(conn, disk_insert_sql);
    //printf("%s\n", disk_insert_sql);
    release_conn(sp, node->index);
    return !ret;
}

int user_insert_opt(int host_id, SQL_CONN_POOL *sp, struct User_Data *user_data) {
    SQL_NODE *node;
    while((node = get_conn(sp)) == NULL) {
        sleep(1);
    }
    MYSQL *conn = node->mysql_conn;
    char user_insert_sql[2000] = {0};
    sprintf(user_insert_sql, "INSERT INTO user(host_id,nowtime,sum,active_user,user_with_root,curr_user) VALUES(%d,'%s',%d,'%s','%s','%s')", host_id, user_data->nowtime, user_data->sum, user_data->active_user, user_data->user_with_root, user_data->curr_user);
    int ret = mysql_query(conn, user_insert_sql);
    //printf("%s\n", user_insert_sql);
    release_conn(sp, node->index);
    return !ret;
}

int proc_insert_opt(int host_id, SQL_CONN_POOL *sp, struct Proc_Data *proc_data) {
    SQL_NODE *node;
    while((node = get_conn(sp)) == NULL) {
        sleep(1);
    }
    MYSQL *conn = node->mysql_conn;
    char proc_insert_sql[2000] = {0};
    sprintf(proc_insert_sql, "INSERT INTO proc(host_id,nowtime,proc_name,proc_id,owner,cpu_usage,mem_usage) VALUES(%d,'%s','%s',%d,'%s','%s','%s')", host_id, proc_data->nowtime, proc_data->proc_name, proc_data->proc_id, proc_data->owner, proc_data->cpu_usage, proc_data->mem_usage);
    int ret = mysql_query(conn, proc_insert_sql);
    //printf("%s\n", proc_insert_sql);
    release_conn(sp, node->index);
    return !ret;
}
