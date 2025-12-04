#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_NAME "myfifo"

int main() {
    int fifo_fd;
    char buffer[256];
    ssize_t n;

    // 打开 FIFO（读端）
    fifo_fd = open(FIFO_NAME, O_RDONLY);
    if (fifo_fd < 0) {
        perror("open fifo");
        exit(1);
    }

    // 读取并打印
    while ((n = read(fifo_fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, n);
    }

    close(fifo_fd);
    return 0;
}
