#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutey = PTHREAD_MUTEX_INITIALIZER;

void th_funb(void *p)
{
    if (pthread_mutex_lock(&mutex) != 0) 
    {
       perror("pthread_mutex_lock");
       exit(EXIT_FAILURE);
    }
    sleep(2);
    if (pthread_mutex_lock(&mutex) != 0) 
    {
       perror("pthread_mutex_lock");
       exit(EXIT_FAILURE);
    }
    /*
    if (pthread_mutex_unlock(&mutex) != 0) 
    {
        perror("pthread_mutex_unlock");
        exit(EXIT_FAILURE);
    }
    sleep(2);
    */
    /*
    if (pthread_mutex_lock(&mutey) != 0) 
    {
       perror("pthread_mutex_lock");
       exit(EXIT_FAILURE);
    }
    */
    /*
    sleep(5);
    if (pthread_mutex_unlock(&mutey) != 0) 
    {
        perror("pthread_mutex_unlock");
        exit(EXIT_FAILURE);
    }
    */
}

void th_func(void *p)
{
    if (pthread_mutex_lock(&mutey) != 0) 
    {
       perror("pthread_mutex_lock");
       exit(EXIT_FAILURE);
    }
    sleep(2);
    /*
    if (pthread_mutex_unlock(&mutey) != 0) 
    {
        perror("pthread_mutex_unlock");
        exit(EXIT_FAILURE);
    }
    sleep(2);
    */
    /*
    if (pthread_mutex_lock(&mutex) != 0) 
    {
       perror("pthread_mutex_lock");
       exit(EXIT_FAILURE);
    }
    */
    /*
    sleep(5);
    if (pthread_mutex_unlock(&mutex) != 0) 
    {
        perror("pthread_mutex_unlock");
        exit(EXIT_FAILURE);
    }
    */
}

#if 0
int main()
{
    int tmp1, tmp2;
    int ret1, ret2;
    void *retval;
    pthread_t pid_t, pid_r;
    
    ret1 = pthread_create(&pid_t, NULL, (void *)th_funb, NULL);
    ret2 = pthread_create(&pid_r, NULL, (void *)th_func, NULL);
    
    if(ret1 != 0)
        printf("create pthread1 failed!\n");
    
    if(ret2 != 0)
        printf("create pthread2 failed!\n");
    
    tmp1 = pthread_join(pid_t, &retval);
    printf("thread1 return value(retval) is %d\n", (int)retval);
    printf("thread1 return value(tmp) is %d\n", tmp1);
    
    if (tmp1 != 0)
    {
        printf("cannot join with thread1\n");
    }
    printf("thread1 end\n");
    
    tmp2 = pthread_join(pid_r, &retval);
    printf("thread2 return value(retval) is %d\n", (int)retval);
    printf("thread2 return value(tmp) is %d\n", tmp2);
    
    if (tmp2 != 0)
    {
        printf("cannot join with thread2\n");
    }
    printf("thread2 end\n");
    
    pthread_mutex_destroy(mutex);
    pthread_mutex_destroy(mutey);
    
    return 0;
}
#endif
///////////////////////////////////////////////



pthread_cond_t cond;

void hander(void *arg)
{
    free(arg);
    (void)pthread_mutex_unlock(&mutex);
}

void *thread1(void *arg)
{
    pthread_cleanup_push(hander, &mutex);
    
    while(1)
    {
        printf("thread1 is running!\n");
        /*
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        printf("thread1 applied the conition\n");
        pthread_mutex_unlock(&mutex);
        */
        pthread_cond_signal(&cond);
        sleep(1);
    }
    
    pthread_cleanup_pop(0);
}

void *thread2(void *arg)
{
    while(1)
    {
        printf("thread2 is running\n");
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        printf("thread2 applied the condition\n");
        pthread_mutex_unlock(&mutex);
        //sleep(2);
    }
}

int main()
{
    pthread_t thid1, thid2;
    printf("conition variaable study!\n");
    
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_create(&thid1, NULL, thread1, NULL);
    pthread_create(&thid2, NULL, thread2, NULL);
    /*
    sleep(1);
    do
    {
        pthread_cond_signal(&cond);
    }
    while(1);
    */
    while(1)
    {
        sleep(10);
    }
    sleep(10);
    
    pthread_exit(0);
    
    return 0;
}

