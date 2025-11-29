// 2-1.c dup/dup2 向文件中写入数据
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd, saved_stdout;

    // 1. 打开文件 c.txt，用于写入（不存在则创建）
    fd = open("c.txt", O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // 2. 保存当前的标准输出文件描述符（屏幕）
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout < 0) {
        perror("dup");
        return 1;
    }

    // 3. 用 dup2 将文件描述符 fd 覆盖到标准输出 stdout
    if (dup2(fd, STDOUT_FILENO) < 0) {
        perror("dup2");
        return 1;
    }

    // 4. 此时 printf 输出会写入 c.txt
    printf("Hello Linux\n");
    fflush(NULL);    // 刷新所有标准 IO 缓冲

    // 5. 关闭文件 c.txt
    close(fd);

    // 6. 恢复标准输出到屏幕
    if (dup2(saved_stdout, STDOUT_FILENO) < 0) {
        perror("dup2 restore");
        return 1;
    }

    close(saved_stdout);

    // 7. 再次打印，此时输出到屏幕
    printf("Hello Linux\n");
    fflush(NULL);

    return 0;
}
