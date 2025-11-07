/* myrm.c */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "用法: %s 文件1 [文件2 ...]\n", argv[0]);
        return 2;
    }

    int exit_code = 0; /* 最终返回码：0 表示全部成功，1 表示有错误 */
    for (int i = 1; i < argc; ++i) {
        const char *path = argv[i];
        if (remove(path) == 0) {
            printf("removed '%s'\n", path);
        } else {
            /* 打印错误信息到 stderr，包含 errno 描述 */
            fprintf(stderr, "cannot remove '%s': %s\n", path, strerror(errno));
            exit_code = 1;
        }
    }

    return exit_code;
}

