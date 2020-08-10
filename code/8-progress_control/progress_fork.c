#include <stdio.h>
#include <unistd.h>


void print_pid()
{
   printf("progress id:%d\n", getpid());
}

void print_parent_pid()
{
    printf("parent progress id:%d\n", getppid());
}

static void prgress_func(const char* append_str)
{
    printf("%s\n",append_str);
    print_pid();
    print_parent_pid();
    printf("%s\n",append_str);
}

int main()
{
    pid_t id = fork();
    
    if(0 == id)
    {
        prgress_func("**********In Child 1***********");
        _exit(0);
    }
    sleep(2);
    
    id = fork();
    if(0 == id)
    {
        printf("This is in the second fork\n");
        prgress_func("**********In Child 2***********");
        _exit(0);
    }
    sleep(2);
    prgress_func("**********In Parent***********");
    
    return 0;
}
