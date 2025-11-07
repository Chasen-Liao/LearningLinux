// selection_sort.c

// 导入总的头文件

#include "sort.h"

void selection_sort(double arr[], int n) {
  // O(n^2)
  /* 算法流程:
  先选定一个元素,往后找一个最小的元素,之后和当前元素交换位置*/

  for (int i = 0; i < n - 1; i++) {
    int min_idx = i; // 假设一开始往后最小的下标是自己
                     // 往后遍历所有的元素
    for (int j = i + 1; j < n; j++) {
      if (arr[min_idx] > arr[j]) {
        min_idx = j;
      }
    }
    // 找到了最小的下标，现在进行交换
    double temp = arr[i];
    arr[i] = arr[min_idx];
    arr[min_idx] = temp;
  }
}
