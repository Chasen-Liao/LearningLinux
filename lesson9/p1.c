// 创建子进程
// p1.c

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> // fork() sleep()

int main() {
    pid_t pid = fork(); // 创建子进程
    // 在父进程返回>0的子进程PID，在子进程返回0，创建失败返回-1

    if (pid < 0) {
        // 创建失败
        perror("fork failed");
        return 1;
    } 
    else if (pid == 0) {
        // 现在在子进程
        printf("(In child process)This is the child process. PID: %d\n", getpid());
        printf("father process PID: %d\n", getppid()); // getppid() 获取父进程ID
        printf("Child process exiting.\n");
        sleep(2); // 模拟子进程工作
        
    } 
    else {
        // 现在在父进程
        printf("(In father process)This is the parent process. PID: %d, Child PID: %d\n", getpid(), pid);
        printf("Parent process exiting.\n");
        sleep(1); // 确保父进程先打印
    }
}


