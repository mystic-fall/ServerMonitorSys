/*************************************************************************
> File Name: cJSON_test1.c
> Author: 
> Mail: 
> Created Time: Sun 29 Aug 2021 08:26:44 PM CST
************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "../1.headfiles/cJSON.h"
/*
{
    "name": "Awesome 4K",
    "resolutions": [
        {
            "width": 1280,
            "height": 720
        },
        {
            "width": 1920,
            "height": 1080
        },
        {
            "width": 3840,
            "height": 2160
        }
    ]
}
*/

char *get_cJSON_string() {
    const unsigned int resolution_numbers[3][2] = {
        {1280, 720},
        {1920, 1080},
        {3840, 2160}
    };
    cJSON *monitor = NULL;
    cJSON *name = NULL;
    cJSON *resolutions = NULL;
    cJSON *resolution = NULL;
    cJSON *height = NULL;
    cJSON *width = NULL;

    monitor = cJSON_CreateObject();
    
    //name = cJSON_CreateString("Awesome 4K");
    //cJSON_AddItemToObject(monitor, "name", name);
    if(cJSON_AddStringToObject(monitor, "name", "Awesome 4K") == NULL) {
        printf("AddStringToObject\n");
        goto end;
    }

    //resolutions = cJSON_CreateArray();
    //cJSON_AddItemToObject(monitor, "resolutions", resolutions);
    resolutions = cJSON_AddArrayToObject(monitor, "resolutions");
    if(resolutions == NULL) {
        printf("AddArrayToObject\n");
        goto end;
    }
    for(int i = 0; i < 3; i++) {
        resolution = cJSON_CreateObject();
        cJSON_AddItemToArray(resolutions, resolution);

        //height = cJSON_CreateNumber(resolution_numbers[i][0]);
        //cJSON_AddItemToObject(resolution, "height", height);
        height = cJSON_AddNumberToObject(resolution, "height", resolution_numbers[i][0]);
        if(height == NULL) {
            printf("AddNumberToObject\n");
            goto end;
        }

        //width = cJSON_CreateNumber(resolution_numbers[i][1]);
        //cJSON_AddItemToObject(resolution, "width", width);
        width = cJSON_AddNumberToObject(resolution, "width", resolution_numbers[i][1]);
        if(width == NULL) {
            printf("AddNumberToObject\n");
            goto end;
        }
    }
    char *string = cJSON_Print(monitor);
    cJSON_Delete(monitor);
    return string;
end:
    return NULL;
}

int main() {
    char *string = get_cJSON_string();
    if(string == NULL) return -1;
    const cJSON *name;
    const cJSON *resolutions;
    const cJSON *resolution;
    const cJSON *height;
    const cJSON *width;
    cJSON *monitor_json = cJSON_Parse(string);
    if(monitor_json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if(error_ptr != NULL) {
            fprintf(stderr, "Error Before: %s\n", error_ptr);
        }
        goto end;
    }
    name = cJSON_GetObjectItem(monitor_json, "name");
    if(cJSON_IsString(name) && (name->valuestring != NULL)) {
        printf("name = %s\n", name->valuestring);
    }
    resolutions = cJSON_GetObjectItem(monitor_json, "resolutions");
    int size = (int)cJSON_GetArraySize(resolutions);
    for(int i = 0; i < size; i++) {
        resolution = (cJSON *)cJSON_GetArrayItem(resolutions, i);
        height = cJSON_GetObjectItem(resolution, "height");
        width = cJSON_GetObjectItem(resolution, "width");
        if(!cJSON_IsNumber(height) || !cJSON_IsNumber(width)) {
            goto end;
        } 
        printf("height = %.2lf width = %d\n", height->valuedouble, width->valueint);
    }
end:
    cJSON_Delete(monitor_json);
    return 0;
}
