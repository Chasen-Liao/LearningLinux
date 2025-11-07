#include <stdio.h>    // 提供输入输出函数
#include <stdlib.h>   // 提供进程退出等函数
#include <unistd.h>   // 提供fork()等系统调用
#include <sys/wait.h> // 提供wait()等进程等待函数

int main() {
    // 创建第一个子进程（输出B）
    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("fork创建第一个子进程失败");
        exit(EXIT_FAILURE); // 退出程序并返回错误状态
    } else if (pid1 == 0) { // 第一个子进程执行逻辑
        for (int i = 0; i < 10; i++) {
            printf("B");
            fflush(stdout); // 立即刷新缓冲区，确保字符及时输出
        }
        exit(EXIT_SUCCESS); // 子进程完成任务后退出
    }

    // 创建第二个子进程（输出C）
    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("fork创建第二个子进程失败");
        exit(EXIT_FAILURE);
    } else if (pid2 == 0) { // 第二个子进程执行逻辑
        for (int i = 0; i < 10; i++) {
            printf("C");
            fflush(stdout);
        }
        exit(EXIT_SUCCESS);
    }

    // 父进程逻辑：等待两个子进程完成后输出A
    wait(NULL);  // 等待第一个子进程结束
    wait(NULL);  // 等待第二个子进程结束

    // 输出10个A
    for (int i = 0; i < 10; i++) {
        printf("A");
    }
    printf("\n"); // 最后换行，使输出更规范

    return 0;
}