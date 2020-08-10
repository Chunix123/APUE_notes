#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>


#define N 10
#define MAX 100


int child_work(int pfd, char *fname)
{
    char buf[MAX];
    int n = 0, fd;
    
    if((fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0)
    {
        fprintf(stderr, "Fail to open %s : %s.\n", fname, strerror(errno));
        return -1;
    }
    
    while(n = read(fd, buf, sizeof(buf)))
    {
        write(fd, buf, n);
    }
    
    close(pfd);
    
    return 0;
}

int father_work(int pfd, char *fname)
{
    int fd,n;
    char buf[MAX];
    
    if((fd = open(fname, O_RDONLY)) < 0)
    {
        fprintf(stderr,"Fail to open %s : %s.\n", fname, strerror(errno));
        return -1;
    }
    
    while(n = read(fd, buf, sizeof(buf)))
    {
        write(pfd, buf, n);
    }
    
    close(pfd);
    
    return 0;
}

int main(int argc, char *argv[])
{
    int fd[2];
    pid_t pid;
    
    if(argc < 3)
    {
        fprintf(stderr, "usage %s argv[1] argv[2]. \n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
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
        child_work(fd[0], argv[2]);
    }
    else
    {
        //printf("Parant \n");
        //sleep(2);
        close(fd[0]);
        father_work(fd[1], argv[1]);
        wait(NULL);
    }
    
    return 0;
}

