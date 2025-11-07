// 2-9.c
// 使用两个子进程交替判断一个范围内的数字是否为素数

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // fork, pipe, read, write, close
#include <sys/wait.h>
#include <string.h>
#include <math.h> 

// 判断一个数是否为素数
int is_prime(int num) {
    if (num <= 1) return 0;    // 小于等于1不是素数
    if (num == 2) return 1;    // 2是素数
    if (num % 2 == 0) return 0; // 偶数不是素数
    for (int i = 3; i*i <= num; i += 2) {
        if (num % i == 0) return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) { // argc: 参数个数, argv: 参数数组
    // 检查命令行参数
    if (argc != 3) {
        fprintf(stderr, "用法: %s <左边界> <右边界>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 解析参数为整数
    int left = atoi(argv[1]);
    int right = atoi(argv[2]);
    if (left < 0 || right < 0 || left > right) {
        fprintf(stderr, "错误: 无效的范围（左边界<=右边界，且均为非负数）\n");
        exit(EXIT_FAILURE);
    }

    // 创建两个管道用于进程同步（子进程1→子进程2，子进程2→子进程1）
    int pipe1[2], pipe2[2]; // pipe1[0]: 读端, pipe1[1]: 写端
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe创建失败");
        exit(EXIT_FAILURE);
    }

    // 创建第一个子进程（处理偶数索引的数字）
    pid_t child1 = fork();
    if (child1 == -1) {
        perror("fork子进程1失败");
        exit(EXIT_FAILURE);
    }

    if (child1 == 0) {  // 子进程1逻辑
        // 关闭不需要的管道端（只保留写pipe1，读pipe2）
        close(pipe1[0]);
        close(pipe2[1]);

        // 遍历范围内的数字
        for (int i = left; i <= right; i++) {
            int diff = i - left;
            if (diff % 2 == 0) {  // 偶数索引（0,2,4...）由子进程1处理
                // 判断素数并输出结果
                int prime = is_prime(i);
                printf("子进程1 (PID: %d): %d %s素数\n", 
                       getpid(), i, prime ? "是" : "不是");

                // 处理完后发送信号给子进程2，通知其可以处理下一个
                char buf = 'a';
                if (write(pipe1[1], &buf, 1) == -1) {
                    perror("子进程1写管道失败");
                    exit(EXIT_FAILURE);
                }
            } else {  // 等待子进程2处理完奇数索引后再继续
                char buf;
                if (read(pipe2[0], &buf, 1) == -1) {
                    perror("子进程1读管道失败");
                    exit(EXIT_FAILURE);
                }
            }
        }

        // 关闭管道并退出
        close(pipe1[1]);
        close(pipe2[0]);
        exit(EXIT_SUCCESS);
    }

    // 创建第二个子进程（处理奇数索引的数字）
    pid_t child2 = fork();
    if (child2 == -1) {
        perror("fork子进程2失败");
        exit(EXIT_FAILURE);
    }

    if (child2 == 0) {  // 子进程2逻辑
        // 关闭不需要的管道端（只保留读pipe1，写pipe2）
        close(pipe1[1]);
        close(pipe2[0]);

        // 遍历范围内的数字
        for (int i = left; i <= right; i++) {
            int diff = i - left;
            if (diff % 2 == 1) {  // 奇数索引（1,3,5...）由子进程2处理
                // 先等待子进程1的信号（确保交替执行）
                char buf;
                if (read(pipe1[0], &buf, 1) == -1) {
                    perror("子进程2读管道失败");
                    exit(EXIT_FAILURE);
                }

                // 判断素数并输出结果
                int prime = is_prime(i);
                printf("子进程2 (PID: %d): %d %s素数\n", 
                       getpid(), i, prime ? "是" : "不是");

                // 处理完后发送信号给子进程1，通知其可以处理下一个
                if (write(pipe2[1], &buf, 1) == -1) {
                    perror("子进程2写管道失败");
                    exit(EXIT_FAILURE);
                }
            }
        }

        // 关闭管道并退出
        close(pipe1[0]);
        close(pipe2[1]);
        exit(EXIT_SUCCESS);
    }

    // 父进程逻辑：关闭所有管道，等待子进程结束
    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    // 等待两个子进程完成
    waitpid(child1, NULL, 0);
    waitpid(child2, NULL, 0);

    printf("所有数字处理完毕\n");
    return 0;
}