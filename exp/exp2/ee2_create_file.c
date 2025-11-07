/*
 * mycreate.c
 *
 * 用法: ./mycreate a.txt b.txt c.txt
 *
 * 对每个命令行给出的文件名：
 *  - 创建（或截断已有文件）并以 0644 权限写入
 *  - 在文件里写入 "this is a file\n"
 *  - 成功则打印 "Created <filename>"
 *  - 出错则打印 "Error creating <filename>: <原因>"
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define CONTENT "this is a file\n"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> [file2 ...]\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        const char *fname = argv[i];

        /* 使用 open 创建/截断文件，权限 0644 */
        int fd = open(fname, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (fd < 0) {
            fprintf(stderr, "Error creating %s: %s\n", fname, strerror(errno));
            continue; /* 继续处理下一个文件 */
        }

        ssize_t to_write = (ssize_t)strlen(CONTENT);
        ssize_t written = write(fd, CONTENT, (size_t)to_write);
        if (written != to_write) {
            if (written < 0)
                fprintf(stderr, "Error writing to %s: %s\n", fname, strerror(errno));
            else
                fprintf(stderr, "Error writing to %s: short write (%zd/%zd)\n", fname, written, to_write);
            close(fd);
            continue;
        }

        if (close(fd) < 0) {
            fprintf(stderr, "Error closing %s: %s\n", fname, strerror(errno));
            continue;
        }

        printf("Created %s\n", fname);
    }

    return 0;
}

