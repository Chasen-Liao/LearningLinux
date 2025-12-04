/*
### 2.2. 编写程序实现如下功能：创建父子进程，父子进程之间通过管道进行通信，父程向子进程发送英文字符串，子进程接收到该字符串后，将该字符串倒序，并附加上自己的进程pid传回给父进程。

例如：父进程发出Hello World!；子进程处理返回!dlroW olleH2709，2709是子进程号。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAXLINE 1024
int main(int argc, char *argv[])
{
	int p2c[2]; /* parent -> child */
	int c2p[2]; /* child -> parent */
	pid_t pid;
	char sendbuf[MAXLINE];
	char recvbuf[MAXLINE + 64];
	ssize_t n;

	if (pipe(p2c) == -1) {
		perror("pipe p2c");
		return 1;
	}
	if (pipe(c2p) == -1) {
		perror("pipe c2p");
		return 1;
	}

	pid = fork();
	if (pid < 0) {
		perror("fork");
		return 1;
	}

	if (pid == 0) {
		/* Child */
		close(p2c[1]); /* close write end of parent->child */
		close(c2p[0]); /* close read end of child->parent */

		/* read message from parent */
		n = read(p2c[0], sendbuf, sizeof(sendbuf)-1);
		if (n < 0) {
			perror("child read");
			_exit(1);
		}
		if (n == 0) {
			/* no data */
			close(p2c[0]);
			close(c2p[1]);
			_exit(0);
		}
		sendbuf[n] = '\0';

		/* reverse the string in-place */
		size_t len = strlen(sendbuf);
		for (size_t i = 0; i < len/2; ++i) {
			char t = sendbuf[i];
			sendbuf[i] = sendbuf[len-1-i];
			sendbuf[len-1-i] = t;
		}

		/* append child pid */
		snprintf(recvbuf, sizeof(recvbuf), "%s%ld", sendbuf, (long)getpid());

		/* write back to parent */
		size_t to_write = strlen(recvbuf);
		ssize_t written = write(c2p[1], recvbuf, to_write);
		if (written < 0) {
			perror("child write");
			_exit(1);
		}

		close(p2c[0]);
		close(c2p[1]);
		_exit(0);
	} else {
		/* Parent */
		close(p2c[0]); /* close read end of parent->child */
		close(c2p[1]); /* close write end of child->parent */

		if (argc > 1) {
			/* join argv[1..] as message */
			size_t pos = 0;
			sendbuf[0] = '\0';
			for (int i = 1; i < argc; ++i) {
				if (i > 1) {
					strncat(sendbuf, " ", sizeof(sendbuf)-strlen(sendbuf)-1);
				}
				strncat(sendbuf, argv[i], sizeof(sendbuf)-strlen(sendbuf)-1);
			}
		} else {
			strncpy(sendbuf, "Hello World!", sizeof(sendbuf)-1);
			sendbuf[sizeof(sendbuf)-1] = '\0';
		}

		/* write to child */
		size_t len = strlen(sendbuf);
		if (write(p2c[1], sendbuf, len) != (ssize_t)len) {
			perror("parent write");
			close(p2c[1]);
			close(c2p[0]);
			wait(NULL);
			return 1;
		}
		close(p2c[1]);

		n = read(c2p[0], recvbuf, sizeof(recvbuf)-1);
		if (n < 0) {
			perror("parent read");
			close(c2p[0]);
			wait(NULL);
			return 1;
		}
		recvbuf[n] = '\0';

		printf("Parent received: %s\n", recvbuf);

		close(c2p[0]);
		wait(NULL);
	}

	return 0;
}
