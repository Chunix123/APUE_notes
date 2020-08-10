#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#define MSG_SIZE 512

sem_t sem;


void *thread_func(void *msg)
{
    sem_wait(&sem);
    
    char *ptr = msg;
    
    while(strcmp("end\n", msg) != 0)
    {
        //sleep(1);
        printf("[thread_func] %s\n", ptr);
        sem_wait(&sem);
    }
    
    pthread_exit(NULL);
}

int main()
{
    pthread_t tid;
    char msg[MSG_SIZE];
    
    memset(msg, 0, sizeof(msg));
    sem_init(&sem, 0, 0);
    
    pthread_create(&tid, NULL, (void *)thread_func, msg);
    
    printf("Input some text. Enter 'end'to finish...\n");
    
    while(strcmp("end\n", msg) != 0)
    {
        fgets(msg, MSG_SIZE, stdin);
        sem_post(&sem);
        //sleep(2);
    }
    
    printf("Waiting for thread to finish...\n");
    pthread_join(tid, NULL);
    printf("Thread joined\n");
    sem_destroy(&sem);
    
    return 0;
}