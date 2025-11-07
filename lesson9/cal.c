// cal.c
// 并行计算器 

#include <stdio.h> // fopen fprinf
#include <stdlib.h> // exit
#include <unistd.h> // fork() getpid()
#include <sys/types.h>
#include <sys/wait.h> // wait

#define FILENAME "res.txt" 
// 保存结果的文件名

void perform(char operation, double a, double b, FILE *file){
    double result;

    switch (operation)
    {
        case '+':
            result = a + b;
            fprintf(file, "PID: %d: %lf + %lf = %lf\n", getpid(), a, b, result);
            break;
        case '-':
            result = a - b;
            fprintf(file, "PID: %d: %lf - %lf = %lf\n", getpid(), a, b, result);
            break;
        case '*':
            result = a * b;
            fprintf(file, "PID: %d: %lf * %lf = %lf\n", getpid(), a, b, result);
            break;
        case '/':
            if (b != 0) {
                result = a / b;
                fprintf(file, "PID: %d: %lf / %lf = %lf\n", getpid(), a, b, result);
            } else {
                fprintf(file, "PID: %d: Division by zero error for %lf / %lf\n", getpid(), a, b);
            }
            break;
        default:
            fprintf(file, "PID: %d: Unknown operation %c\n", getpid(), operation);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3){
        return 1;
    }

    double a = atof(argv[1]);
    double b = atof(argv[2]);

    char operations[] = {'+', '-', '*', '/'};
    pid_t pids[4];

    FILE *file = fopen(FILENAME, "w"); // 应该是个指针
    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    for (int i = 0; i < 4; i++) {
        pids[i] = fork();
        if (pids[i] < 0) {
            fclose(file);
            return 1;
        } else if (pids[i] == 0) {
            perform(operations[i], a, b, file);
            fclose(file);
            exit(0);
        }
    }

    for (int i = 0; i < 4; i++) {
        waitpid(pids[i], NULL, 0);
    }
    fclose(file);

    return 0;
}

