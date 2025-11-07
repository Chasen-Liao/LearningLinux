// cal_fixed.c
// 简单的 shell（修复与增强版）

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>

#define BUF_SIZE 1024
#define MAX_ARGS 100

// SIGCHLD 处理函数：回收后台子进程，避免产生僵尸进程
void sigchld_handler(int sig)
{
    int saved_errno = errno;
    while (1) {
        pid_t pid = waitpid(-1, NULL, WNOHANG);
        if (pid <= 0) break;
        // 可选：打印已结束的后台进程信息
        // printf("[info] child %d exited\n", pid);
    }
    errno = saved_errno;
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP; // 自动重启被中断的 syscalls
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    char command[BUF_SIZE];
    char *args[MAX_ARGS];

    while (1) {
        printf("(~_~):");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            // Ctrl-D 或 输入流结束
            printf("\n");
            break;
        }

        // 去掉末尾换行
        size_t len = strlen(command);
        if (len > 0 && command[len-1] == '\n') command[len-1] = '\0';

        // 跳过前导空白
        char *cmd = command;
        while (*cmd && isspace((unsigned char)*cmd)) cmd++;
        if (*cmd == '\0') continue; // 空命令

        // 内建命令退出
        if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0) break;

        // 检查是否后台运行（以 & 结尾，或单独包含 &）
        int background = 0;
        len = strlen(cmd);
        // 去掉尾部空白
        while (len > 0 && isspace((unsigned char)cmd[len-1])) cmd[--len] = '\0';
        if (len > 0 && cmd[len-1] == '&') {
            background = 1;
            cmd[--len] = '\0';
            while (len > 0 && isspace((unsigned char)cmd[len-1])) cmd[--len] = '\0';
        }

        // 分词
        int i = 0;
        char *token = strtok(cmd, " \t");
        while (token != NULL && i < MAX_ARGS-1) {
            if (strcmp(token, "&") == 0) { background = 1; break; }
            args[i++] = token;
            token = strtok(NULL, " \t");
        }
        args[i] = NULL;
        if (i == 0) continue;

        // 内建命令：cd
        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                char *home = getenv("HOME");
                if (home == NULL) home = "/";
                if (chdir(home) != 0) perror("cd");
            } else {
                if (chdir(args[1]) != 0) perror("cd");
            }
            continue;
        }

        // 创建子进程执行外部命令
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        } else if (pid == 0) {
            // 子进程：执行命令
            execvp(args[0], args);
            // execvp 返回说明出错
            perror("execvp");
            exit(EXIT_FAILURE);
        } else {
            // 父进程
            if (!background) {
                int status;
                if (waitpid(pid, &status, 0) == -1) perror("waitpid");
            } else {
                printf("[background] pid %d\n", pid);
                // SIGCHLD 处理器会回收后台结束的子进程
            }
        }
    }

    return 0;
}
