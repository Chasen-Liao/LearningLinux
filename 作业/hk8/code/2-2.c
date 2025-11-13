// 2.2 


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// 全局变量保存两个子进程的PID
pid_t child_pids[2];

// 父进程处理SIGQUIT信号的函数
void parent_quit_handler(int signo) {
    if (signo == SIGQUIT) {
        printf("父进程 %d 收到SIGQUIT信号，开始向子进程发送35号信号\n", getpid());
        // 向两个子进程分别发送35号信号
        for (int i = 0; i < 2; i++) {
            if (kill(child_pids[i], 35) == -1) {
                perror("发送35号信号失败");
            } else {
                printf("父进程已向子进程 %d 发送35号信号\n", child_pids[i]);
            }
        }
    }
}

// 子进程处理35号信号的函数
void child_signal_handler(int signo) {
    if (signo == 35) {
        printf("子进程 %d 收到35号信号，即将终止\n", getpid());
        exit(0); // 子进程终止
    }
}

int main(int argc, char argv[]) {
    // 创建两个子进程
    for (int i = 0; i < 2; i++) {
        pid_t pid = fork(); // 创建两个子进程
        if (pid == -1) {
            perror("创建子进程失败");
            exit(1);
        } else if (pid == 0) { // 子进程逻辑
            // 子进程注册35号信号的处理函数
            if (signal(35, child_signal_handler) == SIG_ERR) {
                perror("子进程注册信号处理函数失败");
                exit(1);
            }
            printf("子进程 %d 创建成功，等待35号信号...\n", getpid());
            // 子进程进入无限等待（等待信号）
            while (1) {
                pause(); // 暂停进程，直到收到信号
            }
        } else { // 父进程逻辑
            child_pids[i] = pid; // 保存子进程PID
        }
    }

    // 父进程注册SIGQUIT信号的处理函数（对应Ctrl+\）
    if (signal(SIGQUIT, parent_quit_handler) == SIG_ERR) {
        perror("父进程注册信号处理函数失败");
        exit(1);
    }

    printf("父进程 %d 创建完成，等待Ctrl+\\触发SIGQUIT信号...\n", getpid());

    // 父进程等待两个子进程终止
    for (int i = 0; i < 2; i++) {
        wait(NULL); // 等待子进程结束
        printf("父进程已回收子进程 %d\n", child_pids[i]);
    }

    printf("所有子进程已终止，父进程 %d 即将退出\n", getpid());
    return 0;
}