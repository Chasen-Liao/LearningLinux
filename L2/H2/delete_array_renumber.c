    
#include <stdio.h>
#define N 20 // 定义数组大小

int main() {
    int arr[N], result[N]; // 原始数组和结果数组
    int count = 0; // 结果数组中的元素计数
    
    // 输入数组
    for (int i = 0; i < N; i++) {
        scanf("%d", &arr[i]);
    }
    
    // 去重处理
    for (int i = 0; i < N; i++) {
        int j;
        // 检查当前元素是否已经在结果数组中
        for (j = 0; j < count; j++) {
            if (result[j] == arr[i]) break;
        }
        // 如果不在结果数组中，则添加
        if (j == count) {
            result[count++] = arr[i];
        }
    }
    
    // 输出去重后的结果
    for (int i = 0; i < count; i++) {
        printf("%d ", result[i]);
    }
    printf("\n");
    
    return 0;
}

