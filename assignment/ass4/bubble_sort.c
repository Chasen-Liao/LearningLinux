// bubble_sort.c

#include "sort.h"

void bubble_sort(double arr[], int n) {
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i + 1; j++) {
      if (arr[j] > arr[j + 1]) {
        // 如果前面的元素大于后面的元素，就交换
        double temp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = temp;
      }
    }
  }
}
