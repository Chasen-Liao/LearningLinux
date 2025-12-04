#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_NAME "myfifo"

int main() {
    int fifo_fd, file_fd;
    char buffer[256];
    ssize_t n;

    // 打开文件 a.txt
    file_fd = open("a.txt", O_RDONLY);
    if (file_fd < 0) {
        perror("open a.txt");
        exit(1);
    }

    // 打开 FIFO（写端）
    fifo_fd = open(FIFO_NAME, O_WRONLY);
    if (fifo_fd < 0) {
        perror("open fifo");
        exit(1);
    }

    // 读取文件并写入 FIFO
    while ((n = read(file_fd, buffer, sizeof(buffer))) > 0) {
        write(fifo_fd, buffer, n);
    }

    close(file_fd);
    close(fifo_fd);
    return 0;
}
