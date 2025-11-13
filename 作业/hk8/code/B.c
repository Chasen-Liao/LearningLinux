// B.c

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

// 信号处理函数
void handle_signal(int signum) {
    if (signum == SIGINT) {
        printf("B: Receiving a SIGINT signal.\n");
        exit(0); // 接收到信号后退出程序
    }
}

int main() {
    // 注册SIGINT信号的处理函数
    if (signal(SIGINT, handle_signal) == SIG_ERR) {
        perror("无法设置信号处理函数");
        exit(1);
    }
    
    // 进入无限循环等待信号
    while (1) {
        pause(); // 暂停程序，等待信号
    }
    
    return 0;
}