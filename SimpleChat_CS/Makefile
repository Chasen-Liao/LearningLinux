# Makefile for SimpleChat_CS

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pthread
INCLUDES = -I./include

# 源文件
SERVER_SRCS = server/server.c server/protocol.c
CLIENT_SRCS = client/client.c

# 目标可执行文件
SERVER_BIN = server/server
CLIENT_BIN = client/client

# 对象文件
SERVER_OBJS = $(SERVER_SRCS:.c=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)

# 默认目标
all: server client

# 编译服务器
server: $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $(SERVER_BIN) $(SERVER_OBJS)
	@echo "✓ 服务器编译完成: $(SERVER_BIN)"

# 编译客户端
client: $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $(CLIENT_BIN) $(CLIENT_OBJS)
	@echo "✓ 客户端编译完成: $(CLIENT_BIN)"

# 编译对象文件规则
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# 清理编译生成的文件
clean:
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS) $(SERVER_BIN) $(CLIENT_BIN)
	@echo "✓ 清理完成"

# 清理并重新编译
rebuild: clean all

# 打印变量值（用于调试）
debug:
	@echo "SERVER_SRCS: $(SERVER_SRCS)"
	@echo "CLIENT_SRCS: $(CLIENT_SRCS)"
	@echo "SERVER_OBJS: $(SERVER_OBJS)"
	@echo "CLIENT_OBJS: $(CLIENT_OBJS)"

.PHONY: all server client clean rebuild debug
