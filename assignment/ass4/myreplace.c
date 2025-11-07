#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    const char *filename = "a.txt";
    const char *old = "student";
    const char *neww = "teacher";
    size_t len_old = strlen(old);

    FILE *fp = fopen(filename, "r+"); // 可读可写，不截断
    if (!fp) {
        perror("fopen");
        return 1;
    }

    // 读取整个文件到内存（方便查找匹配位置）
    if (fseek(fp, 0, SEEK_END) != 0) { perror("fseek"); fclose(fp); return 1; }
    long filesize = ftell(fp);
    if (filesize < 0) { perror("ftell"); fclose(fp); return 1; }
    rewind(fp);

    char *buf = malloc(filesize + 1);
    if (!buf) { perror("malloc"); fclose(fp); return 1; }
    if (fread(buf, 1, filesize, fp) != (size_t)filesize) {
        // 可能是空文件或读取错误
    }
    buf[filesize] = '\0';

    // 在内存中查找所有出现的位置
    char *p = buf;
    while ((p = strstr(p, old)) != NULL) {
        long offset = p - buf;           // 相对于文件开始的字节偏移
        if (fseek(fp, offset, SEEK_SET) != 0) { perror("fseek"); break; }
        if (fwrite(neww, 1, len_old, fp) != len_old) { perror("fwrite"); break; }
        fflush(fp);                      // 确保写入文件
        p += len_old;                    // 继续查找下一个（避免无限循环）
    }

    free(buf);
    fclose(fp);

    printf("替换完成。\n");
    return 0;
}

