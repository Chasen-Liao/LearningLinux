/* mycopy.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "用法: %s 源文件 目标文件\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *src = argv[1];
    const char *dst = argv[2];

    /* 打开源文件 */
    int in_fd = open(src, O_RDONLY);
    if (in_fd < 0) {
        fprintf(stderr, "打开源文件 '%s' 失败: %s\n", src, strerror(errno));
        return EXIT_FAILURE;
    }

    /* 尝试获取源文件权限，用于创建目标文件 */
    struct stat st;
    mode_t mode = 0666; /* 默认权限（被 umask 修改）*/
    if (fstat(in_fd, &st) == 0) {
        mode = st.st_mode & 0777;
    } else {
        /* 获取权限失败，但仍可继续，记录错误信息 */
        fprintf(stderr, "获取源文件 '%s' 属性失败: %s\n", src, strerror(errno));
        /* 不返回，继续使用默认 mode */
    }

    /* 打开/创建目标文件 */
    int out_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC, mode);
    if (out_fd < 0) {
        fprintf(stderr, "打开/创建目标文件 '%s' 失败: %s\n", dst, strerror(errno));
        if (close(in_fd) < 0) /* 关闭源文件并报告可能的错误 */
            fprintf(stderr, "关闭源文件 '%s' 失败: %s\n", src, strerror(errno));
        return EXIT_FAILURE;
    }

    /* 读取并写入 */
    ssize_t nread;
    char buf[8192];
    while (1) {
        nread = read(in_fd, buf, sizeof(buf));
        if (nread > 0) {
            ssize_t written = 0;
            while (written < nread) {
                ssize_t w = write(out_fd, buf + written, nread - written);
                if (w >= 0) {
                    written += w;
                } else if (errno == EINTR) {
                    /* 被信号中断，重试写 */
                    continue;
                } else {
                    fprintf(stderr, "写入到目标文件 '%s' 出错: %s\n", dst, strerror(errno));
                    /* 尝试关闭文件描述符并报告错误 */
                    if (close(in_fd) < 0)
                        fprintf(stderr, "关闭源文件 '%s' 失败: %s\n", src, strerror(errno));
                    if (close(out_fd) < 0)
                        fprintf(stderr, "关闭目标文件 '%s' 失败: %s\n", dst, strerror(errno));
                    return EXIT_FAILURE;
                }
            }
        } else if (nread == 0) {
            /* 到达 EOF */
            break;
        } else { /* nread < 0 */
            if (errno == EINTR) {
                /* 被信号中断，重试读 */
                continue;
            }
            fprintf(stderr, "从源文件 '%s' 读取失败: %s\n", src, strerror(errno));
            if (close(in_fd) < 0)
                fprintf(stderr, "关闭源文件 '%s' 失败: %s\n", src, strerror(errno));
            if (close(out_fd) < 0)
                fprintf(stderr, "关闭目标文件 '%s' 失败: %s\n", dst, strerror(errno));
            return EXIT_FAILURE;
        }
    }

    /* 关闭文件描述符并检查错误 */
    if (close(in_fd) < 0) {
        fprintf(stderr, "关闭源文件 '%s' 失败: %s\n", src, strerror(errno));
        /* 仍尝试关闭目标文件 */
    }
    if (close(out_fd) < 0) {
        fprintf(stderr, "关闭目标文件 '%s' 失败: %s\n", dst, strerror(errno));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
