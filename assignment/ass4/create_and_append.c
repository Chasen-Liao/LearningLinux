// create_and_append.c
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    const char *filename = "aa.txt";
    const char *initial = "Hello World!";
    const char *append = "Linux is an open source OS.";

    /* 以读写方式打开（若不存在则创建），并且截断已有文件，权限为用户读写 0600 */
    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    /* 写入初始内容 "Hello World!" */
    ssize_t written = write(fd, initial, strlen(initial));
    if (written < 0 || (size_t)written != strlen(initial)) {
        perror("write initial");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /*
     * 使用 fcntl 复制一个新的文件描述符（等价于 dup，但题目要求用 fcntl）
     * F_DUPFD 的第三个参数是从哪个描述符编号开始搜索可用描述符（通常传 0 或 3）
     */
    int newfd = fcntl(fd, F_DUPFD, 0);
    if (newfd < 0) {
        perror("fcntl(F_DUPFD)");
        close(fd);
        exit(EXIT_FAILURE);
    }

    /*
     * 确保写入发生在文件末尾。
     * 注意：通过 fcntl/dup 复制得到的描述符与原描述符共享同一个 open-file description，
     * 因此它们共享文件偏移。由于我们刚写完初始内容，偏移通常已经位于文件末尾。
     * 但为了明确起见，这里显式将偏移移动到文件末尾（等同于 append）。
     */
    if (lseek(newfd, 0, SEEK_END) == (off_t)-1) {
        perror("lseek");
        close(newfd);
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* 通过新文件描述符向文件末尾写入附加字符串 */
    written = write(newfd, append, strlen(append));
    if (written < 0 || (size_t)written != strlen(append)) {
        perror("write append");
        close(newfd);
        close(fd);
        exit(EXIT_FAILURE);
    }

    /* 关闭文件描述符 */
    close(newfd);
    close(fd);

    printf("完成：%s 已创建并追加内容。\n", filename);
    return 0;
}

