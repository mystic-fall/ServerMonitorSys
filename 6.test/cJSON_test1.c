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

int main() {
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
    printf("%s\n", string);
end:
    cJSON_Delete(monitor);
    free(string);
    return 0;
}

