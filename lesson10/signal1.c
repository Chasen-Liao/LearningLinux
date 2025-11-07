// 测试kill发送信号

#include <stdio.h>
#include <stdlib.h> // exit
#include <signal.h> // kill
#include <unistd.h> // sleep
#include <sys/types.h> // pid_t

int main()
{
    pid_t pid = fork();
    if (pid < 0){
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0)
    {
        while (1)
        {
            sleep(1);
            printf("没有收到信号...\n");
        }
    } else {
        int i = 0;
        while (i++ < 3)
        {
            printf("没有发送信号...\n");
            sleep(1);
        }
        // 发送信号
        if (kill(pid, SIGINT) < 0) // 写int和宏都可
        {
            perror("kill");
            exit(EXIT_FAILURE);
        }
        printf("发送信号成功\n");   
    }
    return 0;
}