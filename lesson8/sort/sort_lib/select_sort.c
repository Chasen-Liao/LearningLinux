// select_sort.c

// 选择排序实现

void select_sort(int arr[], int n)
{
    int i, j, min_idx, temp;

    for (int i = 0; i < n-1; i++) {
        min_idx = i;
        for (int j = i+1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        // 交换找到的最小元素和当前位置元素
        if (min_idx != i) {
            temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
// gcc -fsyntax-only select_sort.c 不编译只检查语法   
}