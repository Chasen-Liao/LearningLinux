#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    int i;
    
    for (i = 0; i < 2; i++) {   // 创建 2 次，形成 A->B->C
        pid = fork();

        if (pid < 0) {
            perror("fork 失败");
            exit(1);
        } 
        else if (pid == 0) {
            // 子进程执行
            printf("这是第 %d 代进程，PID = %d, 父进程 = %d\n", i + 1, getpid(), getppid());
            // 除最后一个（C进程）外继续循环创建下一级
            if (i == 1) {
                sleep(1);
                printf("孙进程(C) 结束\n");
            }
        } 
        else {
            // 父进程等待子进程结束，防止僵尸
            
            // printf("这是 %d 代进程, PID = %d, 父进程 = %d\n",i, getpid(), getppid());
            wait(NULL);
            // 创建完一个子进程后父进程退出循环
            break;
        }
    }

    

    return 0;
}
