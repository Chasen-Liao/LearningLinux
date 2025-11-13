// gcc 2-4.c -o color_clock

#include <stdio.h>
#include <time.h> 
#include <signal.h>
#include <unistd.h> // alarm

// 信号处理函数：每秒更新并打印时钟
void update_clock(int signo) {
    time_t now;
    struct tm *local_time;

    // 获取当前时间
    time(&now);
    local_time = localtime(&now);  // 转换为本地时间

    // ANSI转义序列：清除当前行并将光标移至行首（避免多行输出）
    printf("\033[K");       // 清除从光标到行尾的内容
    printf("\033[0G");      // 将光标移至当前行首

    // 彩色输出时钟（格式：HH:MM:SS）
    // 颜色代码：31(红)、32(绿)、34(蓝)、33(黄)，0m重置颜色
    printf("当前时间: "
           "\033[1;31m%02d\033[0m"   // 小时（红色加粗）
           "\033[1;33m:\033[0m"      // 冒号（黄色加粗）
           "\033[1;32m%02d\033[0m"   // 分钟（绿色加粗）
           "\033[1;33m:\033[0m"      // 冒号（黄色加粗）
           "\033[1;34m%02d\033[0m",  // 秒（蓝色加粗）
           local_time->tm_hour,
           local_time->tm_min,
           local_time->tm_sec);

    fflush(stdout);  // 刷新输出缓冲区（无换行时需手动刷新）
    alarm(1);        // 重新设置1秒后触发下一次信号
    // alarm 函数功能是让内核在指定时间（秒数）后向当前进程发送一个 SIGALRM 信号，从而实现 “定时触发” 的效果
}

int main() {
    // 注册SIGALRM信号的处理函数
    if (signal(SIGALRM, update_clock) == SIG_ERR) {
        perror("无法注册信号处理函数");
        return 1;
    }

    printf("彩色文字时钟启动中...（按Ctrl+C退出）\n");
    alarm(1);  // 启动第一个1秒定时器

    // 主循环：阻塞等待信号（不占用CPU）
    while (1) {
        pause();  // 暂停进程，直到收到信号
    }

    return 0;
}