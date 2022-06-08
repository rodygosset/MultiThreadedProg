#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include <pthread.h>



pthread_t t[4];

pthread_cond_t  thread_sync_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t count_access = PTHREAD_MUTEX_INITIALIZER;

int thread_nb = 3;

int count = 0;

// start with defining the functions


void f1(int *no) {
    printf("Thread number %d, identity %d is starting up...\n", *no + 1, pthread_self());
}

void f2(int *no) {
    printf("Thread number %d, identity %d is starting up...\n", *no + 1, pthread_self());
}

void f3(int *no) {
    printf("Thread number %d, identity %d is starting up...\n", *no + 1, pthread_self());
}

void f4(int *no) {
    printf("Thread number %d, identity %d is done.\n", *no + 1, pthread_self());
}

void f5(int *no) {
    printf("Thread number %d, identity %d is done.\n", *no + 1, pthread_self());
}

void f6(int *no) {
    printf("Thread number %d, identity %d is done.\n", *no + 1, pthread_self());
}

void f7() {
    printf("-- DONE --\n");
}


void thread(int *no) {
    switch(*no) {
        case 0:
            f1(no);
            break;
        case 1:
            f2(no);
            break;
        case 2:
            f3(no);
            break;
    }
    // lock access to the counter
    pthread_mutex_lock(&count_access);
    count++;
    // printf("-- count --> %d, thread_nb --> %d.\n", count, thread_nb);
    if(count == thread_nb) {
        pthread_cond_broadcast(&thread_sync_cond);
        // once we're done accessing it, free it
        // so it's accessible to the other thread
        pthread_mutex_unlock(&count_access);
    }  else {
        pthread_cond_wait(&thread_sync_cond, &count_access);
        pthread_mutex_unlock(&count_access);
    }
    switch(*no) {
        case 0:
            f4(no);
            break;
        case 1:
            f5(no);
            break;
        case 2:
            f6(no);
            break;
    }
}


void last_thread(int *no) {
    int i = 0;
    while(i < thread_nb) {
        // printf("Thread %d joined thread %d.\n", *no + 1, i + 1);
        pthread_join(t[i], NULL);
        i++;
    }
    f7();
}

int main(int argc, char *argv[]) {
    int i = 0;
    int ivec[4];
    while(i < thread_nb) {
        pthread_create(&t[i], NULL, (void *)thread, &ivec[i]);
        ivec[i] = i;
        i++;
    }
    ivec[thread_nb] = thread_nb;
    pthread_create(&t[thread_nb], NULL, (void *)last_thread, &ivec[thread_nb]);
    pthread_join(t[thread_nb], NULL);
}