#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


void print_pid()
{
   printf("progress id:%d\n", getpid());
}

void print_parent_pid()
{
    printf("parent progress id:%d\n", getppid());
}

void print_uid()
{
   printf("progress user id:%d\n", getuid());
}

void print_euid()
{
   printf("progress efficient user id:%d\n", geteuid());
}

void print_gid()
{
  printf("progress group id:%d\n", getgid());
}

void print_egid()
{
    printf("progress efficient group id:%d\n", getegid());
}

int main()
{
    printf("--------  Begin test_progress_id()  --------\n");
    print_pid();
    print_parent_pid();
    print_uid();
    print_euid();
    print_gid();
    print_egid();
    printf("---------  End test_progress_id()  ---------\n\n");
    
    return 0;
}
