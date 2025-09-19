#include <stdio.h>

int main()
{
	int n; // 输入n个数字
	double min, num; // 当前的最小值，当前的数字
	scanf("%d", &n);

	scanf("%lf", &min);

	for (int i = 1; i < n; i++) {
		scanf("%lf", &num); // 输入当前数字
		if (num < min) {
			min = num;
		}
	}

	printf("min=%.2lf\n", min);

	return 0;
}
