/*************************************************************************
	> File Name: parseFromJson.c
	> Author: 
	> Mail: 
	> Created Time: Tue 31 Aug 2021 05:08:10 PM CST
 ************************************************************************/

#include "../1.headfiles/head.h"
#include "../1.headfiles/cJSON.h"
#include "../1.headfiles/DataStructure.h"

void json_err_handler() {
    const char *err_ptr = cJSON_GetErrorPtr();
    if(err_ptr != NULL) {
        fprintf(stderr, "Error Before: %s\n", err_ptr);
    }
    exit(-1);
}

void cJSON_parseSys(char *sys_json_str, struct Sys_Data *sys_data) {
    cJSON *sys = cJSON_Parse(sys_json_str);
    if(sys == NULL) {
        json_err_handler();
    }
    cJSON *nowtime = cJSON_GetObjectItem(sys, "nowtime");
    if(cJSON_IsString(nowtime) && (nowtime->valuestring != NULL)) {
        strcpy(sys_data->nowtime, nowtime->valuestring);
    }
    cJSON *hostname = cJSON_GetObjectItem(sys, "hostname");
    if(cJSON_IsString(hostname) && (hostname->valuestring != NULL)) {
        strcpy(sys_data->hostname, hostname->valuestring);
    }
    cJSON *os_version = cJSON_GetObjectItem(sys, "os_version");
    if(cJSON_IsString(os_version) && (os_version->valuestring != NULL)) {
        strcpy(sys_data->os_version, os_version->valuestring);
    }
    cJSON *kernal_version = cJSON_GetObjectItem(sys, "kernal_version");
    if(cJSON_IsString(kernal_version) && (kernal_version->valuestring != NULL)) {
        strcpy(sys_data->kernal_version, kernal_version->valuestring);
    }
    cJSON *runtime = cJSON_GetObjectItem(sys, "runtime");
    if(cJSON_IsString(runtime) && (runtime->valuestring != NULL)) {
        strcpy(sys_data->runtime, runtime->valuestring);
    }
    cJSON *loadavg = cJSON_GetObjectItem(sys, "loadavg");
    int size = (int)cJSON_GetArraySize(loadavg);
    for(int i = 0; i < size; i++) {
        cJSON *tmp = (cJSON *)cJSON_GetArrayItem(loadavg, i);
        if(cJSON_IsNumber(tmp)) {
            sys_data->loadavg[i] = tmp->valuedouble;
        }
    }
    cJSON *disk_total = cJSON_GetObjectItem(sys, "disk_total");
    if(cJSON_IsString(disk_total) && (disk_total->valuestring != NULL)) {
        strcpy(sys_data->disk_total, disk_total->valuestring);
    }
    cJSON *disk_usage = cJSON_GetObjectItem(sys, "disk_usage");
    if(cJSON_IsString(disk_usage) && (disk_usage->valuestring != NULL)) {
        strcpy(sys_data->disk_usage, disk_usage->valuestring);
    }
    cJSON *mem_total = cJSON_GetObjectItem(sys, "mem_total");
    if(cJSON_IsString(mem_total) && (mem_total->valuestring != NULL)) {
        strcpy(sys_data->mem_total, mem_total->valuestring);
    }
    cJSON *mem_usage = cJSON_GetObjectItem(sys, "mem_usage");
    if(cJSON_IsString(mem_usage) && (mem_usage->valuestring != NULL)) {
        strcpy(sys_data->mem_usage, mem_usage->valuestring);
    }
    cJSON *cpu_temp = cJSON_GetObjectItem(sys, "cpu_temp");
    if(cJSON_IsNumber(cpu_temp)) {
        sys_data->cpu_temp = cpu_temp->valuedouble;
    }
    cJSON *disk_warning = cJSON_GetObjectItem(sys, "disk_warning");
    if(cJSON_IsString(disk_warning) && (disk_warning->valuestring != NULL)) {
        strcpy(sys_data->disk_warning, disk_warning->valuestring);
    }
    cJSON *mem_warning = cJSON_GetObjectItem(sys, "mem_warning");
    if(cJSON_IsString(mem_warning) && (mem_warning->valuestring != NULL)) {
        strcpy(sys_data->mem_warning, mem_warning->valuestring);
    }
    cJSON *cpu_warning = cJSON_GetObjectItem(sys, "cpu_warning");
    if(cJSON_IsString(cpu_warning) && (cpu_warning->valuestring != NULL)) {
        strcpy(sys_data->cpu_warning, cpu_warning->valuestring);
    }
    return ;
}

void cJSON_parseCpu(char *cpu_json_str, struct Cpu_Data *cpu_data) {
    cJSON *cpu = cJSON_Parse(cpu_json_str);
    if(cpu == NULL) {
        json_err_handler();
    }
    cJSON *nowtime = cJSON_GetObjectItem(cpu, "nowtime");
    if(cJSON_IsString(nowtime) && (nowtime->valuestring != NULL)) {
        strcpy(cpu_data->nowtime, nowtime->valuestring);
    }
    cJSON *loadavg = cJSON_GetObjectItem(cpu, "loadavg");
    int size = (int)cJSON_GetArraySize(loadavg);
    for(int i = 0; i < size; i++) {
        cJSON *tmp = (cJSON *)cJSON_GetArrayItem(loadavg, i);
        if(cJSON_IsNumber(tmp)) {
            cpu_data->loadavg[i] = tmp->valuedouble;
        }
    }
    cJSON *usage = cJSON_GetObjectItem(cpu, "usage");
    if(cJSON_IsString(usage) && (usage->valuestring != NULL)) {
        strcpy(cpu_data->usage, usage->valuestring);
    }
    cJSON *curr_temp = cJSON_GetObjectItem(cpu, "curr_temp");
    if(cJSON_IsNumber(curr_temp)) {
        cpu_data->curr_temp = curr_temp->valuedouble;
    }
    cJSON *warning = cJSON_GetObjectItem(cpu, "warning");
    if(cJSON_IsString(warning) && (warning->valuestring != NULL)) {
        strcpy(cpu_data->warning, warning->valuestring);
    }
    return ;
}

void cJSON_parseMem(char *mem_json_str, struct Mem_Data *mem_data) {
    cJSON *mem = cJSON_Parse(mem_json_str);
    if(mem == NULL) {
        json_err_handler();
    }
    cJSON *nowtime = cJSON_GetObjectItem(mem, "nowtime");
    if(cJSON_IsString(nowtime) && (nowtime->valuestring != NULL)) {
        strcpy(mem_data->nowtime, nowtime->valuestring);
    }
    cJSON *total = cJSON_GetObjectItem(mem, "total");
    if(cJSON_IsString(total) && (total->valuestring != NULL)) {
        strcpy(mem_data->total, total->valuestring);
    }
    cJSON *free = cJSON_GetObjectItem(mem, "free");
    if(cJSON_IsString(free) && (free->valuestring != NULL)) {
        strcpy(mem_data->free, free->valuestring);
    }
    cJSON *usage = cJSON_GetObjectItem(mem, "usage");
    if(cJSON_IsString(usage) && (usage->valuestring != NULL)) {
        strcpy(mem_data->usage, usage->valuestring);
    }
    cJSON *dynamic_avg = cJSON_GetObjectItem(mem, "dynamic_avg");
    if(cJSON_IsString(dynamic_avg) && (dynamic_avg->valuestring != NULL)) {
        strcpy(mem_data->dynamic_avg, dynamic_avg->valuestring);
    }
    return ;
}

void cJSON_parseDisk(char *disk_json_str, struct Disk_Data *disk_data) {
    cJSON *disk = cJSON_Parse(disk_json_str);
    if(disk == NULL) {
        json_err_handler();
    }
    cJSON *nowtime = cJSON_GetObjectItem(disk, "nowtime");
    if(cJSON_IsString(nowtime) && (nowtime->valuestring != NULL)) {
        strcpy(disk_data->nowtime, nowtime->valuestring);
    }
    cJSON *swap_name = cJSON_GetObjectItem(disk, "swap_name");
    if(cJSON_IsString(swap_name) && (swap_name->valuestring != NULL)) {
        strcpy(disk_data->swap_name, swap_name->valuestring);
    }
    cJSON *total = cJSON_GetObjectItem(disk, "total");
    if(cJSON_IsNumber(total)) {
        disk_data->total = total->valueint;
    }
    cJSON *free = cJSON_GetObjectItem(disk, "free");
    if(cJSON_IsNumber(free)) {
        disk_data->free = free->valueint;
    }
    cJSON *usage = cJSON_GetObjectItem(disk, "usage");
    if(cJSON_IsString(usage) && (usage->valuestring != NULL)) {
        strcpy(disk_data->usage, usage->valuestring);
    }
    return ;
}

void cJSON_parseUser(char *user_json_str, struct User_Data *user_data) {
    cJSON *user = cJSON_Parse(user_json_str);
    if(user == NULL) {
        json_err_handler();
    }
    cJSON *nowtime = cJSON_GetObjectItem(user, "nowtime");
    if(cJSON_IsString(nowtime) && (nowtime->valuestring != NULL)) {
        strcpy(user_data->nowtime, nowtime->valuestring);
    }
    cJSON *sum = cJSON_GetObjectItem(user, "sum");
    if(cJSON_IsNumber(sum)) {
        user_data->sum = sum->valueint;
    }
    cJSON *active_user = cJSON_GetObjectItem(user, "active_user");
    if(cJSON_IsString(active_user) && (active_user->valuestring != NULL)) {
        strcpy(user_data->active_user, active_user->valuestring);
    }
    cJSON *user_with_root = cJSON_GetObjectItem(user, "user_with_root");
    if(cJSON_IsString(user_with_root) && (user_with_root->valuestring != NULL)) {
        strcpy(user_data->user_with_root, user_with_root->valuestring);
    }
    cJSON *curr_user = cJSON_GetObjectItem(user, "curr_user");
    if(cJSON_IsString(curr_user) && (curr_user->valuestring != NULL)) {
        strcpy(user_data->curr_user, curr_user->valuestring);
    }
    return ;
}

void cJSON_parseProc(char *proc_json_str, struct Proc_Data *proc_data) {
    cJSON *proc = cJSON_Parse(proc_json_str);
    if(proc == NULL) {
        json_err_handler();
    }
    cJSON *nowtime = cJSON_GetObjectItem(proc, "nowtime");
    if(cJSON_IsString(nowtime) && (nowtime->valuestring != NULL)) {
        strcpy(proc_data->nowtime, nowtime->valuestring);
    }
    cJSON *proc_name = cJSON_GetObjectItem(proc, "proc_name");
    if(cJSON_IsString(proc_name) && (proc_name->valuestring != NULL)) {
        strcpy(proc_data->proc_name, proc_name->valuestring);
    }
    cJSON *proc_id = cJSON_GetObjectItem(proc, "proc_id");
    if(cJSON_IsNumber(proc_id)) {
        proc_data->proc_id = proc_id->valueint;
    }
    cJSON *owner = cJSON_GetObjectItem(proc, "owner");
    if(cJSON_IsString(owner) && (owner->valuestring != NULL)) {
        strcpy(proc_data->owner, owner->valuestring);
    }
    cJSON *cpu_usage = cJSON_GetObjectItem(proc, "cpu_usage");
    if(cJSON_IsString(cpu_usage) && (cpu_usage->valuestring != NULL)) {
        strcpy(proc_data->cpu_usage, cpu_usage->valuestring);
    }
    cJSON *mem_usage = cJSON_GetObjectItem(proc, "mem_usage");
    if(cJSON_IsString(mem_usage) && (mem_usage->valuestring != NULL)) {
        strcpy(proc_data->mem_usage, mem_usage->valuestring);
    }
    return ;
}

//测试使用
/*
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

int main() {
    struct Sys_Data sys_data;
    memset(&sys_data, 0, sizeof(sys_data));
    char buff[100] = {0};
    scanf("%[^\n]", buff);
    char *tmp = cJSON_PackageSysData(buff);
    printf("%s\n", tmp);
    cJSON_parseSys(tmp, &sys_data);
    printf("%s %s %s %s %s %lf %lf %lf %s %s %s %s %lf %s %s %s\n"
    , sys_data.nowtime, sys_data.hostname, sys_data.os_version, sys_data.kernal_version
    , sys_data.runtime, sys_data.loadavg[0], sys_data.loadavg[1], sys_data.loadavg[2]
    , sys_data.disk_total, sys_data.disk_usage, sys_data.mem_total, sys_data.mem_usage
    , sys_data.cpu_temp, sys_data.disk_warning, sys_data.mem_warning, sys_data.cpu_warning);
    return 0;
}
*/
