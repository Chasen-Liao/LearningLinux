#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int dayOfYear(char* date) {
    int year, month, day;
    sscanf(date, "%d-%d-%d", &year, &month, &day); // 格式化的输入sscanf
    
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeapYear(year)) {
        daysInMonth[1] = 29;
    }
    
    int totalDays = 0;
    for (int i = 0; i < month - 1; i++) {
        totalDays += daysInMonth[i];
    }
    totalDays += day;
    
    return totalDays;
}

int main() {
    char date[11];
    printf("请输入日期（YYYY-MM-DD）: ");
    scanf("%s", date);
    
    int result = dayOfYear(date);
    printf("%d\n", result);
    
    return 0;
}
