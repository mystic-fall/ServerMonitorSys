/*************************************************************************
	> File Name: mysql_opt.h
	> Author: 
	> Mail: 
	> Created Time: Wed 01 Sep 2021 08:10:18 PM CST
 ************************************************************************/

#ifndef _MYSQL_OPT_H
#define _MYSQL_OPT_H

//数据库连接池
int create_db_conn(SQL_CONN_POOL *sp, SQL_NODE *node);

SQL_CONN_POOL *sql_pool_init(int connect_pool_size                               
 , char *ip, char *db_name, char *user, char *passwd);

void sql_pool_destroy(SQL_CONN_POOL *sp);

void release_conn(SQL_CONN_POOL *sp, int index);

SQL_NODE *get_conn(SQL_CONN_POOL *sp);

//数据插入操作
int sys_insert_opt(int host_id, SQL_CONN_POOL *sp, struct Sys_Data *sys_data);

int cpu_insert_opt(int host_id, SQL_CONN_POOL *sp, struct Cpu_Data *cpu_data);

int mem_insert_opt(int host_id, SQL_CONN_POOL *sp, struct Mem_Data *mem_data);

int disk_insert_opt(int host_id, SQL_CONN_POOL *sp, struct Disk_Data *disk_data);

int user_insert_opt(int host_id, SQL_CONN_POOL *sp, struct User_Data *user_data);

int proc_insert_opt(int host_id, SQL_CONN_POOL *sp, struct Proc_Data *proc_data);

#endif
