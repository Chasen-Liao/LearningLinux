// quick_sort.c

#include "sort.h"

void quick_sort(double arr[], int low, int high) {
  /*
   */
  if (low < high) {
    double pivot = arr[high]; // 以最后一个元素最为枢轴
    int i = low - 1;

    for (int j = low; j < high; j++) {
      if (arr[j] < pivot) {
        i++;
        double temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
      }
    }
    // 交换枢轴元素
    double temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    int partition_idx = i + 1;

    // 递归
    quick_sort(arr, low, partition_idx - 1);
    quick_sort(arr, partition_idx + 1, high);
  }
}
