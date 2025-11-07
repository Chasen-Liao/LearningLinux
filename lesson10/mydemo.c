// 守护进程

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // fork, chdir, close
#include <sys/types.h>
#include <fcntl.h> // open
#include <time.h>
// #include <sys/types.h>
#include <sys/stat.h> // umask
#include <sys/param.h> // NOFILE
#include <string.h> // strcat


#define LOG_FILE "demolog.txt"


int main()
{
    // 1创建子进程 关闭父进程
    pid_t pid = fork();
    if (pid < 0){
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // 2 创建新的会话
    // 使用 setsid 函数创建一个新的会话，并使当前进程成为该会话的首进程。 
    // 成功时，setsid 返回新会话的会话 ID。
    if (setsid() < 0){
        perror("setsid");
        exit(EXIT_FAILURE);
    }
    // 现在这个进程已经脱离终端控制了

    // 3 设置文件权限掩码为0
    // umask 0000
    umask(0);

    // 4 更改工作目录 /tmp
    if (chdir("/tmp") < 0){
        perror("chdir");
        exit(EXIT_FAILURE);
    }

    // 5 关闭不再需要的文件描述符
    // close(1); // 标准输出 close(2); // 标准错误 close(3); // 多余的文件描述符
    for (int i = 0; i < NOFILE; ++i){
        close(i);
    }
    open("/dev/null", O_RDWR); // 标准输入
    dup2(0, 1); // 标准输出
    dup2(0, 2); // 标准错误

    // 6 守护进程的工作, 业务逻辑
    // 每隔10秒钟向日志文件写入当前时间
    int fd;
    time_t t; // 时间戳
    while (1)
    {
        sleep(10);
        // 写入日志文件
        if (fd = open(LOG_FILE, O_WRONLY | O_CREAT | O_APPEND, 0666) > 0){
            // 没有error
            t = time(NULL); // 1970.1.1 00:00:00 到现在的秒数
            // char* time_str = ctime(&t); // 转换为字符串形式
            char s[100] = {0};
            strcat(s, "LOG: 当前时间是");
            strcat(s, asctime(localtime(&t))); // 本地时间字符串
            write(fd, s, strlen(s));
            close(fd);
        }

    }
    
    
    return 0;
}