#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdio.h>
#include <string.h>

#define MAX_CLIENTS 10      // 最大连接用户数
#define BUFFER_SIZE 1024    // 缓冲区大小
#define NAME_SIZE 20        // 用户名最大字符数
#define PORT 8088           // 服务器端口

// 消息类型定义
typedef enum {
    MSG_REGISTER = 1,       // 注册消息
    MSG_PUBLIC_CHAT = 2,    // 公共聊天
    MSG_PRIVATE_CHAT = 3,   // 私聊
    MSG_USER_LIST = 4,      // 用户列表
    MSG_DISCONNECT = 5      // 断开连接
} MessageType;

// 通信协议包结构
typedef struct {
    MessageType type;           // 消息类型
    int length;                 // 消息体长度
    char data[BUFFER_SIZE];     // 消息数据
} ChatPacket;

// 客户端结构
typedef struct {
    char name[NAME_SIZE];       // 客户端名称
    int sockfd;                 // 套接字文件描述符
} ClientInfo;

// 打包函数
void pack_message(ChatPacket *packet, MessageType type, const char *data);

// 解包函数
void unpack_message(ChatPacket *packet, MessageType *type, char *data);

#endif // PROTOCOL_H
