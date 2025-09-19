#include <stdio.h>
#include <string.h>
int main()
{
	char str[105];
	printf("输入字符串:");
	scanf("%s", str);
	// 采用反向输出的实现
	printf("反转:");

	for (int i = strlen(str) - 1; i >= 0; i--) {
		printf("%c", str[i]);
	}
	printf("\n");

	return 0;
}
