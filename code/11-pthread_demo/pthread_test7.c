#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>


#define MSG_SIZE 512

sem_t sem;
sem_t sem_add;

void *thread_func(void *msg);

int main()
{
    int res = -1;
    pthread_t thread;
    void *thread_result = NULL;
    char msg[MSG_SIZE];
    
    res = sem_init(&sem, 0, 0);
    if(res == -1)
    {
        perror("semaphore initializtion failed!\n");
        exit(EXIT_FAILURE);
    }
    
    res = sem_init(&sem_add, 0, 1);
    if(res == -1)
    {  
        perror("semaphore intitialization failed\n");
        exit(EXIT_FAILURE);
    }
    
    res = pthread_create(&thread, NULL, thread_func, msg);
    if(res != 0)
    {
        perror("pthread_create failed!\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Input some text. Enter 'end' to finish...\n");
    sem_wait(&sem_add);
    while(strcmp("end\n", msg) != 0)
    {
        if(strncmp("TEST", msg, 4) == 0)
        {
            strcpy(msg, "copy_data\n");
            sem_post(&sem);
            sem_wait(&sem_add);
        }
        fgets(msg, MSG_SIZE, stdin);
        sem_post(&sem);
        sem_wait(&sem_add);
    }
    
    printf("Waiting for thread to finish...\n");
    
    res = pthread_join(thread, &thread_result);
    if(res != 0)
    {
        perror("pthread_join failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Thread joined!\n");
    
    sem_destroy(&sem);
    sem_destroy(&sem_add);
    exit(EXIT_FAILURE);
    
    return 0;
}

void *thread_func(void *msg)
{
    char *ptr = msg;
    sem_wait(&sem);
    
    while(strcmp("end\n", msg) != 0)
    {
        int i = 0;
        for(; ptr[i] != '\0'; ++i)
        {
            if(ptr[i] >= 'a' && ptr[i] <= 'z')
            {
                ptr[i] -= 'a' - 'A';
            }
        }
        printf("You input %d characters\n", i-1);
        printf("To Uppercase: %s\n", ptr);
        sem_post(&sem_add);
        sem_wait(&sem);
    }
    sem_post(&sem_add);
    pthread_exit(NULL);
}
