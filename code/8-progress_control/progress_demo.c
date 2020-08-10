#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>


#define N 10
#define MAX 100


int child_read_pipe(int fd)
{
    char buf[N];
    int n = 0;
    
    while(1)
    {
        n = read(fd, buf, sizeof(buf));
        buf[n] = '\0';
        
        printf("Read %d bytes : %s\n", n, buf);
        
        if(strncmp(buf, "quit", 4) == 0)
            break;
    }
    
    return 0;
}

int father_write_pipe(int fd)
{
    char buf[MAX] = {0};
    
    while(1)
    {
        printf(">");
        fgets(buf, sizeof(buf), stdin);
        write(fd, buf, strlen(buf));
        usleep(500);
        
        if(strncmp(buf, "quit", 4) == 0)
            break;
    }
    
    return 0;
}

#if 0
int main()
{
    int fd[2];
    int count = 0;
    pid_t pid;
    
    if(pipe(fd) < 0)
    {
        perror("Failed to create pipe");
        exit(EXIT_FAILURE);
    }
    
    if((pid = fork()) < 0)
    {
        perror("fork error\n");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
        //printf("Child \n");
        close(fd[1]);
        child_read_pipe(fd[0]);
    }
    else
    {
        //printf("Parant \n");
        //sleep(2);
        close(fd[0]);
        father_write_pipe(fd[1]);
    }
    
    return 0;
}
#endif

int main()
{
    int pid;
    int n;
    int fd[2];
    char buf[1000 * 6] = {0};

    if(pipe(fd) < 0)
    {
        perror("Fail to pipe");
        exit(EXIT_FAILURE);
    }

    if((pid = fork()) < 0)
    {
        perror("Fail to fork");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
        close(fd[1]);
        sleep(5);
        close(fd[0]);
        printf("Read port close.\n");
        sleep(3);
    }
    else
    {
        close(fd[0]);
        while(1)
        {
            n = write(fd[1],buf,sizeof(buf));
            printf("Write %d bytes to pipe.\n",n);
        }
    }

    exit(EXIT_SUCCESS);
}
