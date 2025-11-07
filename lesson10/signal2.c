// 测试kill发送信号

#include <stdio.h>
#include <stdlib.h> // exit
#include <signal.h> // kill
#include <unistd.h> // sleep
#include <sys/types.h> // pid_t

int main(int argc, char *argv[])
{
    int pgid = atoi(argv[1]);

    int res = kill(pgid*(-1), SIGTERM); // pgid*(-1)是什么？变成负数, tm的是乘-1

    if (res < 0)
    {
        perror("kill");
        exit(EXIT_FAILURE);
    }
    printf("发送信号成功\n");
    
    return 0;
}