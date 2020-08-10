#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>


#define MAX 100
#define N 5

char share_buf[MAX];
sem_t rsem, wsem;


void *read_share_buf(void *arg)
{
    int num = *((int *)arg);
    
    printf("Create pthread %d success. \n", *((int *)arg));
    
    while(1)
    {
        if(sem_wait(&rsem) < 0)
        {
            fprintf(stderr, "%d fail to sem wait : %s . \n", num, strerror(errno));
            exit(EXIT_FAILURE);
        }
        
        printf("pthread %d read buf : %s\n", num, share_buf);
        memset(share_buf, 0, sizeof(share_buf));
        
        if(sem_post(&wsem) < 0)
        {
            fprintf(stderr, "%d fail to sem post : %s . \n", num, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
}

int do_work()
{
    while(1)
    {
        if(sem_wait(&wsem) < 0)
        {
            perror("Fail to sem wait");
            exit(EXIT_FAILURE);
        }
        
        printf(">");
        fgets(share_buf, MAX, stdin);
        share_buf[strlen(share_buf) - 1] = '\0';
        
        if(strncmp(share_buf,"quit",4) == 0)
        {
            break;
        }
        
        if(sem_post(&rsem) < 0)
        {
            perror("Fail to sem post");
            exit(EXIT_FAILURE);
        }
    }
    
    return 0;
}

int main()
{
    int res, i;
    pthread_t tid[N];
    
    if(sem_init(&rsem, 0, 0) < 0)
    {
        perror("Fail to sem_init");
        exit(EXIT_FAILURE);
    }
    
    if(sem_init(&wsem, 0, 1) < 0)
    {
        perror("Fail to sem_init");
        exit(EXIT_FAILURE);
    }
    
    for(i = 0; i < N; i++)
    {
        res = pthread_create(&tid[i], NULL, read_share_buf, (void *)&i);
        usleep(500);
        if(res != 0)
        {
            perror("Fail to create pthread");
            exit(EXIT_FAILURE);
        }
    }
    
    do_work();
    
    for(i = 0; i < N; i++)
    {
        pthread_cancel(tid[i]);
    }
    
    if(sem_destroy(&rsem) < 0)
    {
        perror("Fail to sem destory rsem");
        exit(EXIT_FAILURE);
    }
    
    if(sem_destroy(&wsem) < 0)
    {
        perror("Fail to sem destory wsem");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}
