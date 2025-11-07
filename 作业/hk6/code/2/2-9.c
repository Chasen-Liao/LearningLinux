// 创建两个子进程交替判断范围内的数是不是素数

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <math.h>




int main(int argc, char *argv[]) // argc 表示参数的个数, argv 表示参数数组(字符串)
{
    // 检查参数是否异常
    if (argc != 3){
        fprintf(stderr, "用法: %s <左边界> <右边界>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 解析左右边界
    int left = atoi(argv[1]);
    int right = atoi(argv[2]);
    if (left < 0 || right < 0 || left > right) {
        fprintf(stderr, "错误: 无效的范围（左边界<=右边界，且均为非负数）\n");
        exit(EXIT_FAILURE);
    }

    // 创建两个管道用于进程通信
    

    return 0;
}