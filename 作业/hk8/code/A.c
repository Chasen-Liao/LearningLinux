// A.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char *argv[]) {
    // 检查参数是否正确
    if (argc != 3) {
        fprintf(stderr, "用法: %s <B的进程号> <定时时间(秒)>\n", argv[0]);
        exit(1);
    }
    
    // 解析参数
    pid_t b_pid = atoi(argv[1]);
    int delay_seconds = atoi(argv[2]);
    
    // 等待指定的时间
    sleep(delay_seconds);
    
    // 打印信息
    printf("A: After %d seconds, send a SIGALRM signal myself.\n", delay_seconds);
    printf("A: Sending a signal to process %d...\n", b_pid);
    
    // 向进程B发送SIGINT信号
    if (kill(b_pid, SIGINT) == -1) {
        perror("发送信号失败");
        exit(1);
    }
    
    return 0;
}