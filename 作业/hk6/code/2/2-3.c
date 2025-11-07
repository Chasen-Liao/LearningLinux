// 2-3.c 
// 通过子进程调用exec家族函数

#include <stdio.h> // 标准库
#include <stdlib.h> 
#include <unistd.h> // fork(), execlp()
#include <sys/wait.h> // wait()


int main()
{
    pid_t pid = fork();

    if (pid < 0) {
        // 如果进程创建失败
        perror("失败");
        exit(EXIT_FAILURE); // failure
    } else if (pid == 0) {
        // ls -l -t -R /home
        /* 
         * execlp参数说明：
         * 1. 第一个参数"ls"：要执行的程序名（依赖PATH环境变量查找）
         * 2. 后续参数：命令行参数列表，格式与终端输入一致
         *    - "ls"：第一个参数通常为程序名自身（对应argv[0]）
         *    - "-l"、"-t"、"-R"：ls的选项（详细列表、按时间排序、递归显示）
         *    - "/home"：要查询的目录路径
         * 3. 最后必须以NULL结尾，标记参数列表结束
         */
        execlp("ls", "ls", "-l", "-t", "-R", "/home/chasen/lcz", NULL);

        perror("execlp执行失败");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
        printf("\n子进程已完成：ls -l -t -R /home 命令执行\n");
    }

    return 0;
}


