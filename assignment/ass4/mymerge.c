#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp1, *fp2;
    char buffer[100];
    size_t n;

    // 1. 创建并写入 file1
    fp1 = fopen("file1", "w");
    if (fp1 == NULL) {
        perror("无法创建 file1");
        exit(1);
    }
    fputs("abcdefghijklmn", fp1);
    fclose(fp1);

    // 2. 创建并写入 file2
    fp2 = fopen("file2", "w");
    if (fp2 == NULL) {
        perror("无法创建 file2");
        exit(1);
    }
    fputs("ABCDEFGHIJKLMN", fp2);
    fclose(fp2);

    // 3. 读取 file1 内容并追加写入 file2
    fp1 = fopen("file1", "r");
    if (fp1 == NULL) {
        perror("无法打开 file1");
        exit(1);
    }

    fp2 = fopen("file2", "r+");
    if (fp2 == NULL) {
        perror("无法打开 file2");
        fclose(fp1);
        exit(1);
    }

    // 移动 file2 文件指针到文件末尾
    fseek(fp2, 0, SEEK_END);

    // 从 file1 读取并写入到 file2
    while ((n = fread(buffer, 1, sizeof(buffer), fp1)) > 0) {
        fwrite(buffer, 1, n, fp2);
    }

    fclose(fp1);
    fclose(fp2);

    printf("操作完成！file2 内容已更新为 abcdefghijklmnABCDEFGHIJKLMN\n");
    return 0;
}

