/*************************************************************************
	> File Name: packageToJson.h
	> Author: 
	> Mail: 
	> Created Time: Wed 01 Sep 2021 08:02:43 PM CST
 ************************************************************************/

#ifndef _PACKAGETOJSON_H
#define _PACKAGETOJSON_H

char *cJSON_PackageCpuData(const char *cpu_data_str);

char *cJSON_PackageMemData(const char *mem_data_str);

char *cJSON_PackageDiskData(const char *disk_data_str);

char *cJSON_PackageUserData(const char *user_data_str);

char *cJSON_PackageProcData(const char *proc_data_str);

char *cJSON_PackageSysData(const char *sys_data_str);


#endif
