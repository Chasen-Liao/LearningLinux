#include <stdio.h>
#include "sort.h"

int main() {
    double arr[20];  // 用于存放输入的20个double类型的数
    int n = 20;      // 数组大小
    int choice;      // 用户选择的排序算法

    // 输入20个double类型的数
    printf("请输入20个浮点数：\n");
    for (int i = 0; i < n; i++) {
        scanf("%lf", &arr[i]);
    }

    // 输入选择的排序算法
    printf("选择排序算法:\n");
    printf("0 - 选择排序\n1 - 冒泡排序\n2 - 快速排序\n");
    scanf("%d", &choice);

    // 根据用户选择调用不同的排序算法
    switch (choice) {
        case 0:
            selection_sort(arr, n);  // 选择排序
            break;
        case 1:
            bubble_sort(arr, n);  // 冒泡排序
            break;
        case 2:
            quick_sort(arr, 0, n - 1);  // 快速排序
            break;
        default:
            printf("无效的选择!\n");
            return 1;
    }

    // 输出排序后的结果
    printf("排序后的结果为:\n");
    for (int i = 0; i < n; i++) {
        printf("%.2lf ", arr[i]);
    }
    printf("\n");

    return 0;
}

