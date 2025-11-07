#include <unistd.h>
#include <stdio.h>

void main()
{
        for(int i=0;i<4;i++) {
                pid_t pid=fork();
                if(pid>0)
                        sleep(1);
        }
        printf("My pid is %d, and my parent pid is %d\n",getpid(),getppid());
}
