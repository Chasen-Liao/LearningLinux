//main.c

#include "sort_lib/sort.h"

#define SIZE 10

int main()
{
    int arr[SIZE], choice;
    int size = SIZE;
    
    printf("请输入%d个整数:\n", size);
    for (int i = 0; i < SIZE; i++){
        scanf("%d", &arr[i]);
    }
    printf("请选择算法 1.冒泡排序 2.选择排序 3.快速排序:\n");
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            bubble_sort(arr, size);
            break;
        case 2:
            select_sort(arr, size);
            break;
        case 3:
            quick_sort(arr, 0, size - 1);
            break;
        default:
            printf("无效选择!\n");
            return 1;
    }
    printf("排序后的数组为:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}
