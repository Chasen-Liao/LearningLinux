#include "myls.h"
#include <stdio.h>
#include <stdlib.h>

/* 简单版本：用法 ./myls 或 ./myls <path>
 * 默认不显示隐藏文件（与 ls 默认行为一致）。
 * 如果你想要 -a 支持，可在此加入 getopt 解析（可按需扩展）。
 */

int main(int argc, char *argv[]) {
    const char *path = ".";
    if (argc == 1) {
        path = ".";
    } else if (argc == 2) {
        path = argv[1];
    } else {
        fprintf(stderr, "Usage: %s [path]\n", argv[0]);
        return 2;
    }

    int ret = print_path_or_file(path, /*show_hidden=*/0, stdout);
    return ret == 0 ? 0 : 1;
}

