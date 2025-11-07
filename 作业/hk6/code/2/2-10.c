#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <string.h>
#include <pwd.h>  // 用于getpwuid获取用户信息

// 日志文件路径，选择/var/log需要root权限，此处用/tmp更通用
#define LOG_FILE "/tmp/user_login.log"
// 时间间隔(秒)
#define INTERVAL 30

// 获取当前登录用户名的函数
const char* get_current_username() {
    static char unknown[] = "unknown_user";
    char* username = NULL;

    // 方法1：使用getlogin()获取登录用户名
    username = getlogin();
    if (username != NULL) {
        return username;
    }

    // 方法2：尝试从环境变量获取
    username = getenv("USER");
    if (username == NULL) {
        username = getenv("LOGNAME");
    }
    if (username != NULL) {
        return username;
    }

    // 方法3：通过用户ID获取（最可靠的方式）
    uid_t uid = getuid();
    struct passwd* pw = getpwuid(uid);
    if (pw != NULL && pw->pw_name != NULL) {
        return pw->pw_name;
    }

    // 所有方法失败
    return unknown;
}

int main() {
    // 1. 创建子进程，父进程退出
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // 父进程退出，子进程成为孤儿进程由init/systemd接管
        exit(EXIT_SUCCESS);
    }

    // 2. 创建新会话，脱离终端控制
    if (setsid() < 0) {
        perror("setsid failed");
        exit(EXIT_FAILURE);
    }

    // 3. 再次fork，确保进程不是会话首进程，无法打开终端
    pid = fork();
    if (pid < 0) {
        perror("second fork failed");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // 4. 设置文件权限掩码
    umask(0022);  // 允许日志文件被同组用户读取

    // 5. 更改工作目录到/tmp（具有写入权限的通用目录）
    if (chdir("/tmp") < 0) {
        perror("chdir failed");
        exit(EXIT_FAILURE);
    }

    // 6. 关闭所有不必要的文件描述符
    for (int i = 0; i < NOFILE; i++) {
        close(i);
    }

    // 7. 重定向标准输入、输出、错误到/dev/null
    int fd = open("/dev/null", O_RDWR);
    if (fd < 0) {
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    if (fd > STDERR_FILENO) {
        close(fd);
    }

    // 8. 守护进程核心工作：定时记录用户信息
    while (1) {
        sleep(INTERVAL);

        // 获取当前时间
        time_t now;
        time(&now);
        struct tm* local_time = localtime(&now);
        if (local_time == NULL) {
            continue;
        }

        // 格式化时间字符串
        char time_str[64];
        strftime(time_str, sizeof(time_str), 
                 "%Y-%m-%d %H:%M:%S", local_time);

        // 获取用户名
        const char* username = get_current_username();

        // 构建日志信息
        char log_msg[256];
        snprintf(log_msg, sizeof(log_msg), 
                 "[%s] Current logged in user: %s\n", 
                 time_str, username);

        // 写入日志文件
        int log_fd = open(LOG_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (log_fd >= 0) {
            write(log_fd, log_msg, strlen(log_msg));
            close(log_fd);
        }
    }

    return 0;
}