// 测试重定向标准输出

#include <unistd.h>
#include <fcntl.h> // open
#include <stdio.h>
#include <stdlib.h> // exit


int main()
{
    int fd = open("screen.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666); // 0666 -> rw-rw-rw-
    

    if (fd == -1){
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 复制文件描述符
    dup2(fd, 1);  // 1 是标准输出的文件描述符, 0 是标准输入, 2 是标准错误
                   // 把 fd 复制到 标准输出上
    // fopen 是文件流指针
    execlp("ps", "ps", "-uax", NULL);
    
        
    return 0;
}
