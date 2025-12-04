// client.c
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../include/protocol.h"

#define SEVER_IP "127.0.0.1"    // 服务器端IP地址（改为本地localhost）

// 全局变量：连接状态标志
volatile int connection_closed = 0;  // 0=连接中，1=连接已断开
pthread_mutex_t connection_mutex = PTHREAD_MUTEX_INITIALIZER;

// 函数：接收消息的线程函数
void *receive_messages(void *socketfd_p);
 
int main(void)
{
    char buf[BUFFER_SIZE]; // 用于存储接收到的消息
    char name[NAME_SIZE];  // 用于存储用户名
 
    printf("请输入用户名:\n");
    fgets(name, NAME_SIZE, stdin);
    if (strlen(name) > 0 && name[strlen(name) - 1] == '\n') {
        name[strlen(name) - 1] = '\0'; // 去除名称末尾的换行符
    }
 
    // printf("name: %s\n", name);
 
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {                                      
        perror("Failed to create socket"); 
        return -1;                
    }
 
    struct sockaddr_in addr;
    addr.sin_port = htons(PORT);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(SEVER_IP);
 
    // 连接到服务器
    printf("正在连接到服务器 %s:%d...\n", SEVER_IP, PORT);
    fflush(stdout);  // 确保输出立即显示
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("❌ Connection Failed");
        close(sockfd);
        return -1;
    }
    printf("✓ 已连接到服务器！\n");
    
    pthread_t recv_thread;
    // 创建线程用于接收消息
    pthread_create(&recv_thread, NULL, receive_messages, &sockfd);
    pthread_detach(recv_thread);  // 分离线程
    
    // 发送用户名到服务器
    // 发送用户名到服务器
    printf("发送用户名: %s\n", name);
    fflush(stdout);
    int sent = send(sockfd, name, strlen(name), 0);
    if (sent < 0) {
        perror("Failed to send username");
        close(sockfd);
        return -1;
    }
    
    printf("💬 请输入消息 (输入 'exit' 退出):\n");
    fflush(stdout);
 
    // 循环获取用户输入并发送消息到服务器
    while (1)
    {
        // 检查连接是否已断开
        pthread_mutex_lock(&connection_mutex);
        if (connection_closed) {
            pthread_mutex_unlock(&connection_mutex);
            break;
        }
        pthread_mutex_unlock(&connection_mutex);
        
        memset(buf, 0, BUFFER_SIZE);
        fgets(buf, BUFFER_SIZE, stdin);
        if (buf[strlen(buf) - 1] == '\n')
        {
            buf[strlen(buf) - 1] = '\0';
        }
        
        if (strlen(buf) == 0) {
            continue;  // 跳过空消息
        }
        
        if (strcmp(buf, "exit") == 0)
        {
            printf("👋 正在退出...\n");
            break;
        }
        
        int sent = send(sockfd, buf, strlen(buf), 0);
        if (sent < 0) {
            perror("Failed to send message");
            break;
        }
    }
    close(sockfd);
    return 0; // 程序正常退出
}
 
// 函数：接收消息的线程函数
void *receive_messages(void *socketfd_p)
{
    int sockfd = *((int *)socketfd_p); 
    char buffer[BUFFER_SIZE];          // 用于存储接收到的消息
    int len;                           // 接收消息的长度
    
    // 循环接收消息直到连接关闭
    while ((len = recv(sockfd, buffer, BUFFER_SIZE, 0)) > 0)
    {
        buffer[len] = '\0';     // 添加字符串终止符
        printf("%s\n", buffer); // 在控制台打印接收到的消息
    }
    
    // 连接已断开
    if (len == 0) {
        // 服务器主动关闭连接
        printf("\n❌ 服务器已断开连接！\n");
        fflush(stdout);
    } else if (len < 0) {
        // 接收错误
        perror("❌ 接收数据失败");
        fflush(stdout);
    }
    
    // 设置连接断开标志
    pthread_mutex_lock(&connection_mutex);
    connection_closed = 1;
    pthread_mutex_unlock(&connection_mutex);
    
    printf("💤 程序即将退出...\n");
    fflush(stdout);
    
    pthread_exit(NULL); // 退出线程
}