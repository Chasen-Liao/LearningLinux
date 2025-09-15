#include <stdio.h>

int main() {
    double d = -21.6666;
    
    printf("正确输出(double): %f\n", d);
    printf("错误输出(用%%d): %d\n", d);  // 未定义行为！
    printf("正确的:%d\n", (int)d);

    return 0;
}

