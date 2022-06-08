#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include <pthread.h>

// start with defining the functions

int a = 8;
int b = 2;
int c;
int d;
float e;
int f;


pthread_t t[3];
pthread_mutex_t thread_sync = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t count_access = PTHREAD_MUTEX_INITIALIZER;

int count = 0;



void f1() {
    // sleep a random amount of time
    // between 1 and 3 seconds
    sleep(rand() % 3 + 1);
    c = a + b;
}

void f2() {
    // sleep a random amount of time
    // between 1 and 3 seconds
    sleep(rand() % 3 + 1);
    d = a - b;
}

void f3() {
    // sleep a random amount of time
    // between 1 and 3 seconds
    sleep(rand() % 3 + 1);
    e = (float) c / d;
}

void f4() {
    // sleep a random amount of time
    // between 1 and 3 seconds
    sleep(rand() % 3 + 1);
    f = c * d;
}

void f5() {
    printf("e => %f\n", e);
    printf("f => %d\n", f);
}

void thread_one(int *no) {
    printf(" Thread One - Identité : %d.%u \n", getpid(), pthread_self());
    f1();
    // lock access to the counter
    pthread_mutex_lock(&count_access);
    count++;
    if(count == 2) {
        // once we're done accessing it, free it
        // so it's accessible to the other thread
        pthread_mutex_unlock(&count_access);
        pthread_mutex_unlock(&thread_sync);
    } else {
        pthread_mutex_unlock(&count_access);
        pthread_mutex_lock(&thread_sync);
    }
    f3();
    printf(" Thread One -- DONE \n");
    // pthread_exit(0);
}

void thread_two(int *no) {
    printf(" Thread Two - Identité : %d.%u \n", getpid(), pthread_self());
    f2();
    // lock access to the counter
    pthread_mutex_lock(&count_access);
    count++;
    if(count == 2) {
        // once we're done accessing it, free it
        // so it's accessible to the other thread
        pthread_mutex_unlock(&count_access);
        pthread_mutex_unlock(&thread_sync);
    } else {
        pthread_mutex_unlock(&count_access);
        pthread_mutex_lock(&thread_sync);
    }
    f4();
    printf(" Thread Two -- DONE \n");
    // pthread_exit(0);
}

void thread_three(int *no) {
    printf(" Thread Three - Identité : %d.%u \n", getpid(), pthread_self());
    pthread_join(t[0], NULL);
    pthread_join(t[1], NULL);
    f5();
    // pthread_exit(0);
}

int main(int argc, char *argv[]) {
    int ivec[3] = { 0, 1, 2 };
    //pthread_mutex_lock(&thread_sync);
    // init the random generator
    srand(time(NULL)+getpid());
    printf("count --> %d\n", count);
    pthread_create(&t[0], NULL, (void *)thread_one, &ivec[0]);
    pthread_create(&t[1], NULL, (void *)thread_two, &ivec[1]);
    pthread_create(&t[2], NULL, (void *) thread_three, &ivec[2]);
    pthread_join(t[2], NULL);
    printf("count --> %d\n", count);
}