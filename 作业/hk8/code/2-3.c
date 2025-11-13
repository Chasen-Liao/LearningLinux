#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>
#include <stdbool.h>

// 全局变量存储素数相关信息（volatile确保信号处理函数能获取最新值）
volatile int primes[10000];  // 存储找到的素数（足够容纳2~65535所有素数）
volatile int prime_count = 0;  // 已找到的素数数量
volatile int last_prime = 0;   // 最近找到的素数

// 处理Ctrl+C（SIGINT）信号：打印最近找到的素数并继续搜索
void handle_sigint(int signo) {
    if (signo == SIGINT) {
        printf("\n[收到Ctrl+C] 最近找到的素数: %d\n", last_prime);
        // 重新注册信号处理函数（部分系统会重置信号处理方式）
        signal(SIGINT, handle_sigint);
    }
}

// 处理Ctrl+\（SIGQUIT）信号：打印所有已找到的素数并退出
void handle_sigquit(int signo) {
    if (signo == SIGQUIT) {
        printf("\n[收到Ctrl+\\] 所有已找到的素数如下：\n");
        for (int i = 0; i < prime_count; i++) {
            printf("%d ", primes[i]);
            if ((i + 1) % 10 == 0)  // 每行打印10个素数，避免输出混乱
                printf("\n");
        }
        printf("\n共找到%d个素数，程序退出。\n", prime_count);
        exit(0);  // 退出程序
    }
}

// 判断一个数是否为素数
bool is_prime(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;  // 2是唯一偶数素数
    if (n % 2 == 0) return false;  // 偶数排除
    // 检查3到sqrt(n)的奇数是否能整除n
    for (int i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0)
            return false;
    }
    return true;
}

int main() {
    // 注册信号处理函数
    if (signal(SIGINT, handle_sigint) == SIG_ERR) { // signal函数的作用是如果收到SIGINT的话就只想handle_sigint这个函数
        perror("无法注册SIGINT信号处理函数");
        exit(1);
    }
    if (signal(SIGQUIT, handle_sigquit) == SIG_ERR) {
        perror("无法注册SIGQUIT信号处理函数");
        exit(1);
    }

    printf("开始搜索2~65535之间的素数...\n");
    printf("提示：按Ctrl+C打印最近找到的素数；按Ctrl+\\退出并打印所有素数\n");

    // 搜索2~65535之间的素数
    for (int num = 2; num <= 65535; num++) {
        if (is_prime(num)) {
            primes[prime_count] = num;
            last_prime = num;
            prime_count++;
        }
    }

    // 若未收到退出信号，搜索完成后打印所有素数
    printf("\n[搜索完成] 2~65535之间的所有素数如下：\n");
    for (int i = 0; i < prime_count; i++) {
        printf("%d ", primes[i]);
        if ((i + 1) % 10 == 0)
            printf("\n");
    }
    printf("\n共找到%d个素数，程序正常结束。\n", prime_count);

    return 0;
}