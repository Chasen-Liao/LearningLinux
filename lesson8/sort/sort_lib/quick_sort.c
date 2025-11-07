//quick_sort.c

// 快速排序实现

void quick_sort(int arr[], int low, int high)
{   // 子数组至少有两个元素
    if (low < high) {
        
        int pivot = arr[high]; // 随便找个枢轴元素, 也叫基准点
        int i = low - 1; // 初始化左边界
        
        // 处理除了基准点以外的其他元素
        // 将小于基准点的元素交换到左边
        for (int j = low; j < high;j++){
            if (arr[j]<pivot) {
                i++;
                int temp = arr[j];
                arr[i] = arr[i];
                arr[i] = temp;
            }
        }
        // 将基准点放到正确位置
        int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;
        int pivot_index = i + 1; // 记录基准点位置
        // 递归处理基准点左右两侧子数组
        quick_sort(arr, low, pivot_index - 1);
        quick_sort(arr, pivot_index + 1, high);
    }
}