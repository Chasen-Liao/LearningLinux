#define _POSIX_C_SOURCE 200809L
#include "myls.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

/* 如果系统没有 strdup，可用此替代 */
#ifndef HAVE_STRDUP
static char *local_strdup(const char *s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char *p = malloc(len);
    if (p) memcpy(p, s, len);
    return p;
}
#define strdup local_strdup
#endif

static int cmp_cstring(const void *a, const void *b) {
    const char * const *pa = a;
    const char * const *pb = b;
    return strcmp(*pa, *pb);
}

int list_directory(const char *directory, int show_hidden, FILE *out) {
    if (!directory || !out) {
        fprintf(stderr, "list_directory: invalid arguments\n");
        return -1;
    }

    DIR *dp = opendir(directory);
    if (!dp) {
        fprintf(stderr, "opendir(%s) failed: %s\n", directory, strerror(errno));
        return -1;
    }

    struct dirent *entry;
    char **names = NULL;
    size_t cap = 0, n = 0;

    while ((entry = readdir(dp)) != NULL) {
        const char *name = entry->d_name;
        /* 跳过 . 和 .. */
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) continue;
        /* 根据 show_hidden 决定是否跳过以 '.' 开头的隐藏文件 */
        if (!show_hidden && name[0] == '.') continue;

        if (n + 1 > cap) {
            size_t newcap = cap == 0 ? 32 : cap * 2;
            char **tmp = realloc(names, newcap * sizeof(char *));
            if (!tmp) {
                perror("realloc");
                for (size_t i = 0; i < n; ++i) free(names[i]);
                free(names);
                closedir(dp);
                return -1;
            }
            names = tmp;
            cap = newcap;
        }

        names[n] = strdup(name);
        if (!names[n]) {
            perror("strdup");
            for (size_t i = 0; i < n; ++i) free(names[i]);
            free(names);
            closedir(dp);
            return -1;
        }
        n++;
    }

    closedir(dp);

    if (n > 0) {
        qsort(names, n, sizeof(char *), cmp_cstring);
        for (size_t i = 0; i < n; ++i) {
            fprintf(out, "%s\n", names[i]);
            free(names[i]);
        }
    }

    free(names);
    return 0;
}

int print_path_or_file(const char *path, int show_hidden, FILE *out) {
    if (!path || !out) {
        fprintf(stderr, "print_path_or_file: invalid arguments\n");
        return -1;
    }

    struct stat st;
    if (stat(path, &st) != 0) {
        fprintf(stderr, "stat(%s) failed: %s\n", path, strerror(errno));
        return -1;
    }

    if (S_ISDIR(st.st_mode)) {
        return list_directory(path, show_hidden, out);
    } else {
        const char *p = strrchr(path, '/');
        if (p) fprintf(out, "%s\n", p + 1);
        else fprintf(out, "%s\n", path);
        return 0;
    }
}

