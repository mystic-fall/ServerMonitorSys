/*************************************************************************
	> File Name: parseFromJson.h
	> Author: 
	> Mail: 
	> Created Time: Wed 01 Sep 2021 08:24:23 PM CST
 ************************************************************************/

#ifndef _PARSEFROMJSON_H
#define _PARSEFROMJSON_H

void json_err_handler();

void cJSON_parseSys(char *sys_json_str, struct Sys_Data *sys_data);

void cJSON_parseCpu(char *cpu_json_str, struct Cpu_Data *cpu_data);

void cJSON_parseMem(char *mem_json_str, struct Mem_Data *mem_data);

void cJSON_parseDisk(char *disk_json_str, struct Disk_Data *disk_data);

void cJSON_parseUser(char *user_json_str, struct User_Data *user_data);

void cJSON_parseProc(char *proc_json_str, struct Proc_Data *proc_data);

char *cJSON_PackageSysData(const char *sys_data_str);

#endif
