// Sever.c
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include "../include/protocol.h"
 
// 处理与客户端通信的函数，在单独的线程中运行
void *handle_client(void *client_socket);
// 广播消息给所有连接的客户端
void broadcast_message(const char *message);
 
// 结构体表示连接到服务器的客户端
struct client
{
    char name[NAME_SIZE]; // 客户端的名称
    int sockfd;           // 与客户端通信的套接字文件描述符
};
 
// 存储指向客户端结构体的指针的数组
struct client *clients[MAX_CLIENTS];
int client_count = 0;         // 连接的客户端数量
pthread_mutex_t client_mutex; // 互斥锁
 
// 主函数
int main(void)
{
    struct sockaddr_in addr;
    int client_sockfd;               // 套接字文件描述符
    int addrlen = sizeof(addr);              // 服务器地址结构长度
    pthread_mutex_init(&client_mutex, NULL); // 初始化互斥锁
    // 创建连接套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("create socket");
        return -1;
    }
    
    // 设置套接字选项，允许地址立即重用
    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
    {
        perror("setsockopt");
        close(sockfd);
        return -1;
    }
    
    // 设置服务器地址
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    // 将套接字绑定到地址
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Bind failed");
        close(sockfd);
        return -1;
    }
    // 监听客户端连接
    if (listen(sockfd, MAX_CLIENTS) < 0)
    {
        perror("Listen failed");
        close(sockfd);
        return -1;
    }
    printf("🔗 服务器启动成功，监听端口 %d\n", PORT); 
    printf("⏳ 等待客户端连接......\n"); 
    // 接受传入的客户端连接
    while (1)
    {
        struct sockaddr_in client_addr;
        client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen);
        if (client_sockfd < 0)
        {
            perror("Accept failed");
            break;
        }
        pthread_mutex_lock(&client_mutex); // 锁定互斥锁以安全地访问客户端数据
        if (client_count < MAX_CLIENTS)
        {
            // 为客户端结构体分配内存
            struct client *client = malloc(sizeof(struct client));
            // 接收客户端的名称
            recv(client_sockfd, client->name, NAME_SIZE, 0);
            // 初始化客户端结构体并添加到数组中
            client->sockfd = client_sockfd;
            clients[client_count] = client;
            printf("%s 进入聊天室\n", client->name); // 打印消息表示客户端已加入
            client_count++;                          // 增加客户端数量
            // 创建线程来处理客户端通信
            pthread_t thread;
            int pthread_id = pthread_create(&thread, NULL, handle_client, client);
            if (pthread_id < 0)
            {
                perror("pthread_create");
                close(client_sockfd);
                return -1;
            }
            pthread_detach(thread); // 分离线程以允许其独立运行
        }
        else
        {
            // 如果达到最大客户端数量则打印消息
            printf("Maximum clients conneted. Connection refused.\n"); 
            close(client_sockfd);
        }
        pthread_mutex_unlock(&client_mutex); 
    }
    // 关闭服务器套接字
    close(sockfd);
    return 0;
}
 
// 广播消息给所有连接的客户端
void broadcast_message(const char *message)
{
    pthread_mutex_lock(&client_mutex); // 锁定互斥锁以防止并发访问客户端数据
    for (int i = 0; i < client_count; i++)
    {
        send(clients[i]->sockfd, message, strlen(message), 0); // 向每个客户端发送消息
    }
    pthread_mutex_unlock(&client_mutex); // 广播消息后解锁互斥锁
}
 
// 处理与客户端通信的函数，在单独的线程中运行
void *handle_client(void *client_socket)
{
    // 将传入的套接字文件描述符转换为客户端结构体
    struct client *client = (struct client *)client_socket;
    char buffer[BUFFER_SIZE];
    int bytes_read; // 用于存储每次读取的字节数
 
    // 接收来自客户端的消息，直到连接关闭
    while ((bytes_read = recv(client->sockfd, buffer, BUFFER_SIZE, 0)) > 0)
    {
        buffer[bytes_read] = '\0';                // 给接收到的消息添加空字符终止符
        printf("[%s] %s\n", client->name, buffer); // 在服务器控制台上打印客户端的名称和消息
        // 创建要广播给所有客户端（包括发送者）的消息
        char *message = malloc(strlen(client->name) + strlen(buffer) + 16);
        sprintf(message, "[%s]: %s", client->name, buffer);
        broadcast_message(message); // 广播消息给所有客户端
        free(message);              // 释放为消息分配的内存
    }
    printf("%s 已退出聊天室\n", client->name); // 当客户端断开连接时打印消息
    // 关闭客户端套接字并从数组中移除客户端
    pthread_mutex_lock(&client_mutex); // 锁定互斥锁以安全地访问客户端数据
    close(client->sockfd);             // 关闭客户端套接字
    for (int i = 0; i < client_count; i++)
    {
        if (clients[i]->sockfd == client->sockfd)
        {
            // 通过移动后续元素来从数组中移除客户端
            for (int j = i; j < client_count - 1; j++)
            {
                clients[j] = clients[j + 1];
            }
            client_count--; // 减少客户端数量
            break;
        }
    }
    free(client);                        // 释放为客户端结构体分配的内存
    pthread_mutex_unlock(&client_mutex); // 修改客户端数据后解锁互斥锁
    pthread_exit(NULL); // 退出线程
}