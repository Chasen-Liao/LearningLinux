#include <stdio.h>

// 汉诺塔递归函数
void hanoi(int n, char from, char to, char aux) {
    if (n == 1) {
        printf("%c->%c\n", from, to); // 只有一个盘子时直接移动
        return;
    }
    hanoi(n - 1, from, aux, to); // 将n-1个盘子从起始柱移动到辅助柱
    printf("%c->%c\n", from, to); // 将最大的盘子从起始柱移动到目标柱
    hanoi(n - 1, aux, to, from); // 将n-1个盘子从辅助柱移动到目标柱
}

int main() {
    int n;
    scanf("%d", &n); // 输入盘子数量
    hanoi(n, 'A', 'C', 'B'); // 调用汉诺塔函数，A是起始柱，C是目标柱，B是辅助柱
    return 0;
}    
