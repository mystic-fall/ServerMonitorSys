/*************************************************************************
	> File Name: task_pool.c
	> Author: 
	> Mail: 
	> Created Time: Mon 23 Aug 2021 11:48:26 AM CST
 ************************************************************************/
/*
 * simple task_pool ...
 */
#include "../1.headfiles/head.h"
#include "task_pool.h"

Task_Pool *task_pool_init() {
    Task_Pool *tp = (Task_Pool *)malloc(sizeof(Task_Pool));
    tp->head = tp->tail = NULL;
    pthread_mutex_init(&tp->lock, NULL);
    pthread_cond_init(&tp->cond, NULL);
    return tp;
}

Task *getNewTask(int taskfd) {
    Task *task = (Task *)malloc(sizeof(Task));
    task->taskfd = taskfd;
    task->next = NULL;
    return task;
}

void task_pool_push(Task_Pool *tp, int taskfd) {
    Task *task = getNewTask(taskfd);
    pthread_mutex_lock(&tp->lock);
    if(tp->head == NULL) {
        tp->head = tp->tail = task;
    } else {
        tp->tail->next = task;
        tp->tail = task;
    }
    pthread_mutex_unlock(&tp->lock);
    pthread_cond_signal(&tp->cond);
    return ;
}

Task task_pool_pop(Task_Pool *tp) {
    pthread_mutex_lock(&tp->lock);
    while(tp->head == NULL) {
        pthread_cond_wait(&tp->cond, &tp->lock);
    }
    Task *task = tp->head;
    tp->head = tp->head->next;
    if(tp->head == NULL) {
        tp->tail = NULL;
    }
    Task t = *task;
    free(task);
    pthread_mutex_unlock(&tp->lock);
    return t;
}

void task_pool_clear(Task_Pool *tp) {
    pthread_mutex_lock(&tp->lock);
    Task *curr = tp->head;
    tp->head = tp->tail = NULL;
    while(curr) {
        Task *del = curr;
        curr = curr->next;
        free(del);
    }
    pthread_mutex_unlock(&tp->lock);
    pthread_mutex_destroy(&tp->lock);
    pthread_cond_destroy(&tp->cond);
    free(tp);
    return ;
}
