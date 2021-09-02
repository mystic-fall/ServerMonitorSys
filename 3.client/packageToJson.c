/*************************************************************************
	> File Name: packageToJson.c
	> Author: 
	> Mail: 
	> Created Time: Mon 30 Aug 2021 02:21:14 PM CST
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "../1.headfiles/DataStructure.h"
#include "../1.headfiles/cJSON.h"

char *cJSON_PackageCpuData(const char *cpu_data_str) {
    struct Cpu_Data cpu_data;
    sscanf(cpu_data_str, "%s%lf%lf%lf%s%lf%s", cpu_data.nowtime, &cpu_data.loadavg[0]
    , &cpu_data.loadavg[1], &cpu_data.loadavg[2], cpu_data.usage, &cpu_data.curr_temp
    , cpu_data.warning);
    cJSON *cpu = cJSON_CreateObject();
    cJSON_AddStringToObject(cpu, "nowtime", cpu_data.nowtime);
    cJSON *loadavg = cJSON_AddArrayToObject(cpu, "loadavg");
    if(loadavg == NULL) exit(-1);
    for(int i = 0; i < 3; i++) {
        cJSON_AddItemToArray(loadavg, cJSON_CreateNumber(cpu_data.loadavg[i]));
    }
    cJSON_AddStringToObject(cpu, "usage", cpu_data.usage);
    cJSON_AddNumberToObject(cpu, "curr_temp", cpu_data.curr_temp);
    cJSON_AddStringToObject(cpu, "warning", cpu_data.warning);
    char *cpu_json_str = cJSON_Print(cpu);
    cJSON_Delete(cpu);
    return cpu_json_str;
}

char *cJSON_PackageMemData(const char *mem_data_str) {
    struct Mem_Data mem_data;
    sscanf(mem_data_str, "%s%s%s%s%s", mem_data.nowtime, mem_data.total
    , mem_data.free, mem_data.usage, mem_data.dynamic_avg);
    cJSON *mem = cJSON_CreateObject();
    cJSON_AddStringToObject(mem, "nowtime", mem_data.nowtime);
    cJSON_AddStringToObject(mem, "total", mem_data.total);
    cJSON_AddStringToObject(mem, "free", mem_data.free);
    cJSON_AddStringToObject(mem, "usage", mem_data.usage);
    cJSON_AddStringToObject(mem, "dynamic_avg", mem_data.dynamic_avg);
    char *mem_json_str = cJSON_Print(mem);
    cJSON_Delete(mem);
    return mem_json_str;
}

char *cJSON_PackageDiskData(const char *disk_data_str) {
    struct Disk_Data disk_data;
    sscanf(disk_data_str, "%s%d%s%d%d%s", disk_data.nowtime, &disk_data.flag
    , disk_data.swap_name, &disk_data.total, &disk_data.free, disk_data.usage);
    cJSON *disk = cJSON_CreateObject();
    cJSON_AddStringToObject(disk, "nowtime", disk_data.nowtime);
    cJSON_AddNumberToObject(disk, "flag", disk_data.flag);
    cJSON_AddStringToObject(disk, "swap_name", disk_data.swap_name);
    cJSON_AddNumberToObject(disk, "total", disk_data.total);
    cJSON_AddNumberToObject(disk, "free", disk_data.free);
    cJSON_AddStringToObject(disk, "usage", disk_data.usage);
    char *disk_json_str = cJSON_Print(disk);
    cJSON_Delete(disk);
    return disk_json_str;
}

char *cJSON_PackageUserData(const char *user_data_str) {
    struct User_Data user_data;
    sscanf(user_data_str, "%s%d%s%s%s", user_data.nowtime, &user_data.sum
    , user_data.active_user, user_data.user_with_root, user_data.curr_user);
    cJSON *user = cJSON_CreateObject();
    cJSON_AddStringToObject(user, "nowtime", user_data.nowtime);
    cJSON_AddNumberToObject(user, "sum", user_data.sum);
    cJSON_AddStringToObject(user, "active_user", user_data.active_user);
    cJSON_AddStringToObject(user, "user_with_root", user_data.user_with_root);
    cJSON_AddStringToObject(user, "curr_user", user_data.curr_user);
    char *user_json_str = cJSON_Print(user);
    cJSON_Delete(user);
    return user_json_str;
}

char *cJSON_PackageProcData(const char *proc_data_str) {
    struct Proc_Data proc_data;
    sscanf(proc_data_str, "%s%s%d%s%s%s", proc_data.nowtime, proc_data.proc_name
    , &proc_data.proc_id, proc_data.owner, proc_data.cpu_usage, proc_data.mem_usage);
    cJSON *proc = cJSON_CreateObject();
    cJSON_AddStringToObject(proc, "nowtime", proc_data.nowtime);
    cJSON_AddStringToObject(proc, "proc_name", proc_data.proc_name);
    cJSON_AddNumberToObject(proc, "proc_id", proc_data.proc_id);
    cJSON_AddStringToObject(proc, "owner", proc_data.owner);
    cJSON_AddStringToObject(proc, "cpu_usage", proc_data.cpu_usage);
    cJSON_AddStringToObject(proc, "mem_usage", proc_data.mem_usage);
    char *proc_json_str = cJSON_Print(proc);
    cJSON_Delete(proc);
    return proc_json_str;
}

char *cJSON_PackageSysData(const char *sys_data_str) {
    struct Sys_Data sys_data;
    sscanf(sys_data_str, "%s%s%s%s%s%lf%lf%lf%s%s%s%s%lf%s%s%s"
    , sys_data.nowtime, sys_data.hostname, sys_data.os_version, sys_data.kernal_version
    , sys_data.runtime, &sys_data.loadavg[0], &sys_data.loadavg[1], &sys_data.loadavg[2]
    , sys_data.disk_total, sys_data.disk_usage, sys_data.mem_total, sys_data.mem_usage
    , &sys_data.cpu_temp, sys_data.disk_warning, sys_data.mem_warning, sys_data.cpu_warning);
    cJSON *sys = cJSON_CreateObject();
    cJSON_AddStringToObject(sys, "nowtime", sys_data.nowtime);
    cJSON_AddStringToObject(sys, "hostname", sys_data.hostname);
    cJSON_AddStringToObject(sys, "os_version", sys_data.os_version);
    cJSON_AddStringToObject(sys, "kernal_version", sys_data.kernal_version);
    cJSON_AddStringToObject(sys, "runtime", sys_data.runtime);
    cJSON *loadavg = cJSON_AddArrayToObject(sys, "loadavg");
    if(loadavg == NULL) exit(-1);
    for(int i = 0; i < 3; i++) {
        cJSON_AddItemToArray(loadavg, cJSON_CreateNumber(sys_data.loadavg[i]));
    }
    cJSON_AddStringToObject(sys, "disk_total", sys_data.disk_total);
    cJSON_AddStringToObject(sys, "disk_usage", sys_data.disk_usage);
    cJSON_AddStringToObject(sys, "mem_total", sys_data.mem_total);
    cJSON_AddStringToObject(sys, "mem_usage", sys_data.mem_usage);
    cJSON_AddNumberToObject(sys, "cpu_temp", sys_data.cpu_temp);
    cJSON_AddStringToObject(sys, "disk_warning", sys_data.disk_warning);
    cJSON_AddStringToObject(sys, "mem_warning", sys_data.mem_warning);
    cJSON_AddStringToObject(sys, "cpu_warning", sys_data.cpu_warning);
    char *sys_json_str = cJSON_Print(sys);
    cJSON_Delete(sys);
    return sys_json_str;
}
/*
int main() {
    char cpu_data_str[100];
    char mem_data_str[100];
    char disk_data_str[200];
    char user_data_str[200];
    char proc_data_str[100];
    char sys_data_str[200];
    scanf("%[^\n]", cpu_data_str);
    getchar();
    scanf("%[^\n]", mem_data_str);
    getchar();
    scanf("%[^\n]", disk_data_str);
    getchar();
    scanf("%[^\n]", user_data_str);
    getchar();
    scanf("%[^\n]", proc_data_str);
    getchar();
    scanf("%[^\n]", sys_data_str);
    char *cpu_json_str = cJSON_PackageCpuData(cpu_data_str);
    char *mem_json_str = cJSON_PackageMemData(mem_data_str);
    char *disk_json_str = cJSON_PackageDiskData(disk_data_str);
    char *user_json_str = cJSON_PackageUserData(user_data_str);
    char *proc_json_str = cJSON_PackageProcData(proc_data_str);
    char *sys_json_str = cJSON_PackageSysData(sys_data_str);
    printf("%ld\n%s\n", strlen(cpu_json_str), cpu_json_str);
    printf("%ld\n%s\n", strlen(mem_json_str), mem_json_str);
    printf("%ld\n%s\n", strlen(disk_json_str), disk_json_str);
    printf("%ld\n%s\n", strlen(user_json_str), user_json_str);
    printf("%ld\n%s\n", strlen(proc_json_str), proc_json_str);
    printf("%ld\n%s\n", strlen(sys_json_str), sys_json_str);
    return 0;
}
*/
