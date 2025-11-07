#include <stdio.h>
#include <stdlib.h>

/*
问题描述：假设你是一名学生，
正在准备一份作业。你已经在一个文件中写下了 "`This is my homework.`" 这个字符串，
并将这个文件命名为 file1。但是，你发现你还需要添加一些内容，于是你再创建了一个新文件 file2，并在其中写入了 "`We should finish it!`" 这个字符串。
为了避免把两个文件的内容混淆，你希望将 file1 中的内容追加到 file2 中，使得 file2 的内容为 "`This is my homework.\nWe should finish it!`"（\n表示换行）。
为了实现这个目标，你需要先读取 file1 中的内容，然后将其写入 file2 中。
*/

int main()
{
    // 通过fopen标准库函数打开 file1
    FILE *file1 = fopen("file1.txt", "r"); // r是只读模式
    //? FILE 是一个结构体类型，表示文件流，用于文件操作
    if (file1 == NULL){
        perror("Failed to open file1"); // perror用于输出错误信息
        //? perror 的英文意思是 "print error"
        return 1;
    }

    // 通过fopen标准库函数以追加模式打开 file2
    FILE *file2 = fopen("file2.txt", "w"); // a是追加模式, 会保留原来的内容; w是写入模式, 会清除内容
    if (file2 == NULL){ // 如果file2不存在的话要有错误处理
        perror("Failed to open file2");
        fclose(file1); // 关闭 file1
        return 1;
    }

    // 读取 file1 的内容并且追加到file2

    char buffer[256]; // 定义一个缓冲区
    while (fgets(buffer, sizeof(buffer), file1) != NULL){ // 
        //? fgets用于从文件中读取一行内容, 函数参数依次是: 存储内容的缓冲区, 缓冲区大小, 文件指针
        fputs(buffer, file2); // 将读取到的内容写入 file2
        //? fputs用于将字符串写入文件, 函数参数依次是: 要写入的字符串, 文件指针
    }

    // 因为是要在file1后面追加，我们现在把file1的内容已经写好了
    // 要加入 '\n' 换行符
    // fputc('\n', file2); // fputc -- put char -- 单个字符

    // 把"we" should finish it!" 也写入 file2
    fputs("We should finish it!\n", file2);

    // 关闭文件
    fclose(file1);
    fclose(file2);

    printf("completed.\n");


    return 0;
}
