# 简单 Makefile：把 main.c 和 myls.c 编译成当前目录下的可执行文件 "myls"
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -I. -O2
SRC = main.c myls.c
TARGET = myls

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC)

clean:
	rm -f $(TARGET)

