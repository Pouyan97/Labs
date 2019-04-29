/* buffer.h */
typedef int buffer_item;
#define BUFFER_SIZE 5

/* main.c */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
//#include "buffer.h"

#define RAND_DIVISOR 100000000
#define TRUE 1

int x=0;


void *function() {
    for(int i=0; i<100000; i++){
        x = x+1;
    }
}

int main(int argc, char *argv[]) {

    /* Create threads */
    pthread_t thread_id[5];
    printf("Before x - %d\n", x);
    for(int i = 0; i < 5; i++) {
        /* Create the thread */
        pthread_create(&thread_id[i],NULL,function,NULL);
    }
    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);
    pthread_join(thread_id[2], NULL);
    pthread_join(thread_id[3], NULL);
    pthread_join(thread_id[4], NULL);

    printf("After x - %d\n", x);

    /* Exit the program */
    printf("Exit the program\n");
    exit(0);
}
