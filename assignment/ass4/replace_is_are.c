// replace_is_are.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define SRC_FILENAME "word.txt"
#define TMP_TEMPLATE  "word.txt.tmp.XXXXXX"
#define PAT "is"
#define REPL "are"

int is_word_boundary(char ch) {
    /* 把字母和数字视为“单词字符”，其他视为边界 */
    return (ch == '\0') || (!isalnum((unsigned char)ch));
}

int main(void) {
    int fd = -1, tmpfd = -1;
    struct stat st;
    char *buf = NULL;
    size_t size = 0;
    ssize_t r;
    size_t i, count = 0;
    size_t patlen = strlen(PAT);
    size_t repllen = strlen(REPL);

    /* 打开源文件读入 */
    fd = open(SRC_FILENAME, O_RDONLY);
    if (fd < 0) {
        perror("open source file");
        return 1;
    }

    if (fstat(fd, &st) < 0) {
        perror("fstat");
        close(fd);
        return 1;
    }

    /* 如果是空文件直接退出 */
    if (st.st_size == 0) {
        printf("%s is empty. nothing to do.\n", SRC_FILENAME);
        close(fd);
        return 0;
    }

    size = (size_t)st.st_size;
    buf = malloc(size + 1);
    if (!buf) {
        perror("malloc");
        close(fd);
        return 1;
    }

    /* 读取全部内容 */
    {
        size_t off = 0;
        while (off < size) {
            r = read(fd, buf + off, size - off);
            if (r < 0) {
                if (errno == EINTR) continue;
                perror("read");
                free(buf);
                close(fd);
                return 1;
            }
            if (r == 0) break;
            off += (size_t)r;
        }
    }
    buf[size] = '\0';
    close(fd);
    fd = -1;

    /* 统计满足“单词 is”条件的出现次数（为了分配新缓冲区大小） */
    for (i = 0; i + patlen <= size; ++i) {
        if (buf[i] == 'i' && buf[i+1] == 's') {
            char prev = (i == 0) ? '\0' : buf[i-1];
            char next = (i + patlen >= size) ? '\0' : buf[i + patlen];
            if (is_word_boundary(prev) && is_word_boundary(next)) {
                ++count;
                i += patlen - 1; /* 跳过已匹配部分 */
            }
        }
    }

    if (count == 0) {
        printf("没有找到单词 \"is\"，不做修改。\n");
        free(buf);
        return 0;
    }

    /* 分配新缓冲区并构造替换后的内容 */
    {
        size_t new_size = size + count * (repllen - patlen);
        char *newbuf = malloc(new_size + 1);
        if (!newbuf) {
            perror("malloc newbuf");
            free(buf);
            return 1;
        }
        size_t ni = 0;
        for (i = 0; i < size; ++i) {
            if (i + patlen <= size && buf[i] == 'i' && buf[i+1] == 's') {
                char prev = (i == 0) ? '\0' : buf[i-1];
                char next = (i + patlen >= size) ? '\0' : buf[i + patlen];
                if (is_word_boundary(prev) && is_word_boundary(next)) {
                    /* 替换 */
                    memcpy(newbuf + ni, REPL, repllen);
                    ni += repllen;
                    i += patlen - 1; /* 跳过匹配 */
                    continue;
                }
            }
            /* 否则原样拷贝当前字符 */
            newbuf[ni++] = buf[i];
        }
        newbuf[ni] = '\0';

        /* 将结果写入临时文件，再重命名覆盖原文件 */
        char tmpname[] = TMP_TEMPLATE;
        tmpfd = mkstemp(tmpname);
        if (tmpfd < 0) {
            perror("mkstemp");
            free(newbuf);
            free(buf);
            return 1;
        }

        /* 保持临时文件权限与原文件相同 */
        if (fchmod(tmpfd, st.st_mode & 07777) < 0) {
            /* 不致命，记录警告 */
            perror("fchmod");
        }

        /* 写入临时文件 */
        size_t written = 0;
        while (written < ni) {
            ssize_t w = write(tmpfd, newbuf + written, ni - written);
            if (w < 0) {
                if (errno == EINTR) continue;
                perror("write tmp");
                close(tmpfd);
                unlink(tmpname);
                free(newbuf);
                free(buf);
                return 1;
            }
            written += (size_t)w;
        }

        /* 确保数据刷入磁盘（可选但更安全） */
        fsync(tmpfd);
        close(tmpfd);
        tmpfd = -1;

        /* 用临时文件替换原文件 */
        if (rename(tmpname, SRC_FILENAME) < 0) {
            perror("rename");
            unlink(tmpname);
            free(newbuf);
            free(buf);
            return 1;
        }

        printf("已替换 %zu 处 \"is\" -> \"are\"，并已保存到 %s\n", count, SRC_FILENAME);

        free(newbuf);
    }

    free(buf);
    return 0;
}

