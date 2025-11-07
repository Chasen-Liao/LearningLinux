#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    // 获取父进程PID
    pid_t father_pid = getpid();
    int i;

    // 创建3个子进程
    for (i = 0; i < 3; i++) {
        pid_t pid = fork();
        if (pid == -1) {  // 错误处理
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {  // 子进程逻辑
            // 子进程获取自身PID
            pid_t child_pid = getpid();
            // 打开文件（追加模式，避免覆盖）
            FILE *fp = fopen("a.txt", "a");
            if (fp == NULL) {
                perror("fopen failed");
                exit(EXIT_FAILURE);
            }
            // 写入子进程信息（表明身份和PID）
            fprintf(fp, "I am child process %d, mypid is %d\n", i + 1, child_pid);
            fclose(fp);  // 关闭文件
            exit(EXIT_SUCCESS);  // 子进程退出，避免继续执行父进程代码
        }
        // 父进程继续创建下一个子进程
    }

    // 父进程等待所有子进程结束,避免僵尸进程
    for (i = 0; i < 3; i++) {
        wait(NULL);  // 每次等待一个子进程回收
    }

    // 父进程写入自身信息
    FILE *fp = fopen("a.txt", "a");
    if (fp == NULL) {
        perror("fopen failed");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "I am father process, mypid is %d\n", father_pid);
    fclose(fp);

    return 0;
}