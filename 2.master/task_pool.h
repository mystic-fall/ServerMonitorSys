/*************************************************************************
	> File Name: task_pool.h
	> Author: 
	> Mail: 
	> Created Time: Wed 25 Aug 2021 04:54:57 PM CST
 ************************************************************************/

#ifndef _TASK_POOL_H
#define _TASK_POOL_H

typedef struct Task {
    int taskfd;
    struct Task *next;
} Task;

typedef struct Task_Pool {
    Task *head, *tail;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} Task_Pool;

Task_Pool *task_pool_init();

Task *getNewTask(int taskfd);

void task_pool_push(Task_Pool *tp, int taskfd);

Task task_pool_pop(Task_Pool *tp);

void task_pool_clear(Task_Pool *tp);

#endif
