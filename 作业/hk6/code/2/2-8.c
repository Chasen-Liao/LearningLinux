#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

// 打印休息信息：身份、PID、当前时间
void print_rest(const char* role, pid_t pid) {
    time_t now = time(NULL);
    printf("%s (pid: %d) is resting at %s", role, pid, ctime(&now));
}

int main() {
    time_t start_time = time(NULL);  // 记录程序开始时间（基准时间）
    printf("Starts at %s", ctime(&start_time));
    pid_t pid;

    // 爷爷进程创建爸爸进程
    pid = fork();
    if (pid == -1) {
        perror("fork dad failed");
        exit(1);
    } else if (pid == 0) {  // 爸爸进程
        // 爸爸进程创建孙子进程
        pid = fork();
        if (pid == -1) {
            perror("fork son failed");
            exit(1);
        } else if (pid == 0) {  // 孙子进程
            const char* role = "grandson";
            pid_t my_pid = getpid();
            int interval = 6;  // 孙子每6秒休息一次
            time_t next_rest = start_time + 2;  // 前2秒后首次休息

            // 工作时间18秒内，循环休息
            while (next_rest - start_time <= 18) {
                time_t now = time(NULL);
                if (now < next_rest) {
                    sleep(next_rest - now);  // 等待到下一次休息时间
                }
                print_rest(role, my_pid);
                next_rest += interval;  // 更新下一次休息时间
            }
            exit(0);  // 孙子进程结束
        } else {  // 爸爸进程（等待孙子结束）
            const char* role = "father";
            pid_t my_pid = getpid();
            int interval = 4;  // 爸爸每4秒休息一次
            time_t next_rest = start_time + 2;

            while (next_rest - start_time <= 18) {
                time_t now = time(NULL);
                if (now < next_rest) {
                    sleep(next_rest - now);
                }
                print_rest(role, my_pid);
                next_rest += interval;
            }
            wait(NULL);  // 回收孙子进程，避免僵尸
            exit(0);
        }
    } else {  // 爷爷进程（等待爸爸结束）
        const char* role = "grandpa";
        pid_t my_pid = getpid();
        int interval = 2;  // 爷爷每2秒休息一次
        time_t next_rest = start_time + 2;

        while (next_rest - start_time <= 18) {
            time_t now = time(NULL);
            if (now < next_rest) {
                sleep(next_rest - now);
            }
            print_rest(role, my_pid);
            next_rest += interval;
        }
        wait(NULL);  // 回收爸爸进程，避免僵尸
        exit(0);
    }

    return 0;
}