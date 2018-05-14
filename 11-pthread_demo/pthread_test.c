#include <stdio.h>
#include <pthread.h>


void print_message_function( void *ptr)
{
    int i = 0;
    for (i; i < 10; i++)
    {
        printf("%s: %d\n", (char *)ptr, i);
    }
}

int main(int argc, char *argv[])
{
    int tmp1, tmp2;
    void *retval;
    pthread_t th_rd1, th_rd2;
    char *message1 = "thread1";
    char *message2 = "thread2";
    int ret1, ret2;
    
    ret1 = pthread_create(&th_rd1, NULL, (void *)&print_message_function, (void *)message1);
    ret2 = pthread_create(&th_rd2, NULL, (void *)&print_message_function, (void *)message2);
    
    if(ret1 != 0)
        printf("create pthread1 failed!\n");
    
    if(ret2 != 0)
        printf("create pthread2 failed!\n");
    
    tmp1 = pthread_join(th_rd1, &retval);
    printf("thread1 return value(retval) is %d\n", (int)retval);
    printf("thread1 return value(tmp) is %d\n", tmp1);
    
    if (tmp1 != 0)
    {
        printf("cannot join with thread1\n");
    }
    printf("thread1 end\n");
    
    tmp2 = pthread_join(th_rd2, &retval);
    printf("thread2 return value(retval) is %d\n", (int)retval);
    printf("thread2 return value(tmp) is %d\n", tmp2);
    
    if (tmp2 != 0)
    {
        printf("cannot join with thread2\n");
    }
    printf("thread2 end\n");
    
    return 0;
}
