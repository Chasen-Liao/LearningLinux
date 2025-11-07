#ifndef MYLS_H
#define MYLS_H

#include <stdio.h>

/*
 * 列出 directory 路径下的文件名（每行一个），输出到 out（通常是 stdout）。
 * show_hidden: 非零则显示以 '.' 开头的隐藏文件；否则隐藏。
 * 返回 0 表示成功，非 0 表示失败（并在 stderr 输出错误信息）。
 */
int list_directory(const char *directory, int show_hidden, FILE *out);

/*
 * 如果 path 是目录则列出其内容（调用 list_directory），
 * 否则（普通文件）打印路径的最后一段（basename）。
 * show_hidden 同上，out 同上。
 */
int print_path_or_file(const char *path, int show_hidden, FILE *out);

#endif /* MYLS_H */

