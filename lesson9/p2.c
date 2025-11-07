// 创建子进程
// p1.c

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> // fork() sleep()

int main() {
    // fork(); // 第一次创建子进程 父进程0创建子进程1

    // fork(); // 第二次创建子进程 父进程0创建子进程2 子进程1创建子进程3

    // fork(); // 第三次创建子进程 父进程0创建子进程4 子进程1创建子进程5 子进程2创建子进程6 子进程3创建子进程7

    // // 2^n - 1 = 2^3 - 1 = 8 - 1 = 7个子进程 + 1个父进程 = 8个进程 , 其中最小的是外壳程序

    // printf("我的PID是%d, 父进程PID是%d\n", getpid(), getppid());

    // sleep(5); // 确保所有进程都有时间完成输出

    for (int i = 0; i < 4; i++) {
        pid_t pid = fork(); // pid_t 是数据类型
        if (pid != 0) {
            break; // 只让父进程继续创建子进程
            // 不在走创建的过程, 防止自己复制自己
        }
    }

    printf("我的PID是%d, 父进程PID是%d\n", getpid(), getppid());
    sleep(5);

    return 0;

    // 这句话打印了几次? 
    // 进程树: 
}


