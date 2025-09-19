#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    int n;
    scanf("%d", &n); // 输入n
    
    if (n < 2) {
        return 0; // 小于2没有素数
    }
    
    // 创建筛子数组，isPrime[i]为true表示i是素数
    bool *isPrime = (bool *)malloc((n + 1) * sizeof(bool));
    
    // 初始化筛子，假设所有数都是素数
    for (int i = 0; i <= n; i++) {
        isPrime[i] = true;
    }
    
    isPrime[0] = false; // 0不是素数
    isPrime[1] = false; // 1不是素数
    
    // 埃氏筛算法核心
    for (int i = 2; i * i <= n; i++) {
        if (isPrime[i]) {
            // 将i的所有倍数标记为非素数
            for (int j = i * i; j <= n; j += i) {
                isPrime[j] = false;
            }
        }
    }
    
    // 输出所有素数
    for (int i = 2; i <= n; i++) {
        if (isPrime[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");
    
    free(isPrime); // 释放内存
    return 0;
}

