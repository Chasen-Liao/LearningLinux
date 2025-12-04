/*
### 2.4. 用popen实现无名管道，完成ps aux|grep root。
   实现说明：
   - 使用 popen() 函数直接执行管道命令字符串
   - popen() 返回一个 FILE* 指针，可以像读取文件一样读取命令的输出
   - 使用 pclose() 关闭管道
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *fp;
    char line[1024];
    int count = 0;

    /* 直接使用 popen 执行命令 */
    fp = popen("ps aux | grep root", "r");
    if (fp == NULL) {
        perror("popen failed");
        exit(1);
    }

    printf("=== ps aux | grep root ===\n");

    /* 逐行读取输出 */
    while (fgets(line, sizeof(line), fp) != NULL) {
        count++;
        printf("%s", line);
        fflush(stdout);
    }

    printf("=== 总共读取 %d 行 ===\n", count);

    /* 关闭管道 */
    pclose(fp);

    return 0;
}