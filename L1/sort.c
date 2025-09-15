#include <stdio.h> // scanf printf
#include <stdlib.h>// malloc() free()
#include <string.h>// memcy
#include <unistd.h>// unix内核的头文件
		   //

void bubble_sort(double arr[], int n)
{
	double temp;
	for(int i=0;i < n-1;i++)
		for(int j = 0;i < n - 1 - i; j++){
			if (arr[j] > arr[j + 1]){
				temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;
			}
		}
}

void print_res(double arr[], int n){

	for(int i = 0;i < n; i++)
		printf("%lf", arr[i]);
	printf("\n");

}



int main()
{
	double *arr;
	int n;


	printf("input arrage n");
	

	scanf("%d", &n);
	
	
	arr1 = (double *)malloc(n * sizeof(double));
	
	
	printf("输入n个数组元素，空格分割");
	for (int i =0;i < n;i++){
		scanf("%lf", &arr[i]);
	}
	printf("冒泡排序的结果");
	bubble_sort(arr, n);

	print_res(arr, n);
	free(arr);

	return 0;
}


