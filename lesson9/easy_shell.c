// cal.c
// 并行计算器 

#include <stdio.h> // fopen fprinf
#include <stdlib.h> // exit
#include <unistd.h> // fork() getpid()
#include <sys/types.h>
#include <sys/wait.h> // wait


int main(int argc, char *argv[])
{
    // 简单的外壳程序
    // 不断的循环
    while(1)
    {
        char command[256];
        char *args[10];
        pid_t pid;
        int status;

        printf("(~_~):");
        fgets(command, sizeof(command), stdin); // 从键盘中获取命令
        // 去掉换行符
        command[strlen(command - 1)] = '\0';
        if(strcmp("quit", command) == 0) // quit 表示退出shell
        {
            break;
        }

        char *s = NULL;
        s = strtok(command, " "); // 以空格为分隔符 strtok 分割字符串
        int i = 0;
        while(s!=NULL){
            args[i++] = s;
            s = strtok(NULL, " ");
        }
        args[i] = NULL; // 最后一个参数必须是NULL
        pid = fork(); // 创建子进程
        if(pid < 0){
            execvp(args[0], args); // 执行命令
        }
        else if(pid > 0) 
        {
            waitpid(pid, &status, 0); // 等待子进程结束
        }
        

    }

    return 0;
}

