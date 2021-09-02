/*************************************************************************
	> File Name: DataStructure.h
	> Author: 
	> Mail: 
	> Created Time: Mon 30 Aug 2021 02:26:22 PM CST
 ************************************************************************/

#ifndef _DATASTRUCTURE_H
#define _DATASTRUCTURE_H

struct Cpu_Data {
    char nowtime[24];
    double loadavg[3];
    char usage[12];
    double curr_temp;
    char warning[8];
};

struct Mem_Data {
    char nowtime[24];
    char total[12];
    char free[12];
    char usage[12];
    char dynamic_avg[12];
};

struct Disk_Data {
    char nowtime[24];
    int flag;
    char swap_name[20];
    int total;
    int free;
    char usage[12];
};

struct User_Data {
    char nowtime[24];
    int sum;
    char active_user[200];
    char user_with_root[200];
    char curr_user[200];
};

struct Proc_Data {
    char nowtime[24];
    char proc_name[40];
    int proc_id;
    char owner[20];
    char cpu_usage[12];
    char mem_usage[12];
};

struct Sys_Data {
    char nowtime[24];
    char hostname[20];
    char os_version[40];
    char kernal_version[40];
    char runtime[60];
    double loadavg[3];
    char disk_total[12];
    char disk_usage[12];
    char mem_total[12];
    char mem_usage[12];
    double cpu_temp;
    char disk_warning[8];
    char mem_warning[8];
    char cpu_warning[8];
};

#endif
