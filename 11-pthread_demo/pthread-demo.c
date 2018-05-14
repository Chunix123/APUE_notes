#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


void *thread_zigbee(void *p)
{
    //pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    //pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
    
    while(1)
    {
        printf("thread_zigbee end...\n");
        sleep(1);
    }
    
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    pthread_t th_zigbee;
    
    if(pthread_create(&th_zigbee, NULL, (void *)thread_zigbee, NULL) != 0)
    {
        printf("Create the thread_zigbee error!\n");
    }
    
    sleep(3);
    pthread_cancel(th_zigbee);
    pthread_join(th_zigbee, NULL);
    printf("Main thread is exit\n");
    sleep(1);
    
    return 0;
}
