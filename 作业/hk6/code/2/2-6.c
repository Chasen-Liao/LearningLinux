// 2-4.c
// 父进程创建两个子进程，子进程执行完成之后父进程再执行

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    // 通过for循环创建两个子进程
    // 子进程输出I am child process X.
    // 父进程输出I am parent process.

    for (int i = 0; i < 2; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // 子进程
            printf("I am child process %d.\n", i + 1);
            exit(EXIT_SUCCESS);
        } else {
            // 等待子进程结束
            // wait(NULL);
            waitpid(pid, NULL, 0);
            // wait 和 waitpid 的区别在于 wait 会等待任意一个子进程结束，
            // 而 waitpid 可以指定等待特定的子进程结束。
        }
    }
    printf("I am parent process.\n");


    return 0;
}
