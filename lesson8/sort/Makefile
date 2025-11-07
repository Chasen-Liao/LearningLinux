SRCS = sort_lib/select_sort.c sort_lib/bubble_sort.c sort_lib/quick_sort.c
SORT_OBJS = $(SRCS:.c=.o)
CC = gcc
CFLAGS = -Wall -fPIC

# 普通编译
# 如果这些变量存在的话就执行下面的语句
main_normal: main.o $(SORT_OBJS)
	$(CC) -o $@ $< $(SORT_OBJS)

#静态库编译
main_static: main.o libmysort.a
		$(CC) -o $@ $<  -L. -lmysort -static
# 静态库（使用 .o 文件）
libmysort.a: $(SORT_OBJS)
	ar rcs $@ $(SORT_OBJS)
# 动态库编译
main_dynamic: main.o libmysort.so
		$(CC) -o $@ $< -L. -lmysort
libmysort.so: $(SORT_OBJS)
	$(CC) -shared -fPIC -o libmysort.so $(SORT_OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 伪规则
.PHONY: all clean

all: main_normal main_static main_dynamic

clean:
	rm -rf *.o main_* lib* sort_lib/*.o
