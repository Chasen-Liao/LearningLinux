#include <stdio.h>
#include <string.h>

// 定义学生结构体
struct Student {
    char name[21];  // 名字，长度不超过20
    int score;      // 成绩，不大于100的非负整数
};

int main() {
    int n;
    scanf("%d", &n);  // 读取学生数量
    
    struct Student students[n];  // 创建结构体数组
    
    // 读取每个学生的信息
    for (int i = 0; i < n; i++) {
        scanf("%s %d", students[i].name, &students[i].score);
    }
    
    // 使用冒泡排序进行排序
    for (int i = 0; i < n - 1; i++) {
       	for (int j = 0; j < n - i - 1; j++) {
            // 比较成绩，成绩高的排在前面
            if (students[j].score < students[j + 1].score) {
                // 交换两个学生的位置
                struct Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
            // 如果成绩相同，按名字字典序排序
            else if (students[j].score == students[j + 1].score) {
                if (strcmp(students[j].name, students[j + 1].name) > 0) {
                    // 交换两个学生的位置
                    struct Student temp = students[j];
                    students[j] = students[j + 1];
                    students[j + 1] = temp;
                }
            }
        }
    }
    
    // 输出排序后的结果
    for (int i = 0; i < n; i++) {
        printf("%s %d\n", students[i].name, students[i].score);
    }
    
    return 0;
}
