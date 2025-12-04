/*
### 2.3. 利用无名管道pipe()函数、创建进程fork()函数，实现 ps aux | grep root | wc -l 命令。
   实现说明：
   - 使用两个无名管道，创建三个子进程或三个进程链路：
	   1) 执行 `ps aux`，其 stdout 重定向到 pipe1 的写端
	   2) 执行 `grep root`，其 stdin 来自 pipe1 的读端，stdout 重定向到 pipe2 的写端
	   3) 执行 `wc -l`，其 stdin 来自 pipe2 的读端
   - 父进程负责创建管道、fork 子进程并在最后等待子进程结束
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	int pipe1[2]; /* ps -> grep */
	int pipe2[2]; /* grep -> wc */
	pid_t pid1, pid2, pid3;

	if (pipe(pipe1) == -1) {
		perror("pipe1");
		return 1;
	}

	/* 第一个子进程：执行 ps aux，输出到 pipe1 写端 */
	pid1 = fork();
	if (pid1 < 0) {
		perror("fork pid1");
		return 1;
	}

	if (pid1 == 0) {
		/* child 1: ps aux */
		/* 将 stdout 重定向到 pipe1[1] */
		if (dup2(pipe1[1], STDOUT_FILENO) == -1) {
			perror("dup2 ps");
			_exit(1);
		}
		close(pipe1[0]);
		close(pipe1[1]);
		/* exec ps aux */
		execlp("ps", "ps", "aux", (char *)NULL);
		perror("execlp ps");
		_exit(1);
	}

	/* 父继续，关闭 pipe1 的写端（父不再写）
	   注意不要关闭 pipe1[0]，因为需要给下一个子进程作为 stdin */
	close(pipe1[1]);

	if (pipe(pipe2) == -1) {
		perror("pipe2");
		return 1;
	}

	/* 第二个子进程：执行 grep root，stdin 来自 pipe1[0]，stdout 到 pipe2[1] */
	pid2 = fork();
	if (pid2 < 0) {
		perror("fork pid2");
		return 1;
	}

	if (pid2 == 0) {
		/* child 2: grep root */
		if (dup2(pipe1[0], STDIN_FILENO) == -1) {
			perror("dup2 grep stdin");
			_exit(1);
		}
		if (dup2(pipe2[1], STDOUT_FILENO) == -1) {
			perror("dup2 grep stdout");
			_exit(1);
		}
		/* 关闭父中打开的端 */
		close(pipe1[0]);
		close(pipe2[0]);
		close(pipe2[1]);
		execlp("grep", "grep", "root", (char *)NULL);
		perror("execlp grep");
		_exit(1);
	}

	/* 父进程不再需要 pipe1 的读端，也不需要 pipe2 的写端 */
	close(pipe1[0]);
	close(pipe2[1]);

	/* 第三个子进程：执行 wc -l，stdin 来自 pipe2[0] */
	pid3 = fork();
	if (pid3 < 0) {
		perror("fork pid3");
		return 1;
	}

	if (pid3 == 0) {
		/* child 3: wc -l */
		if (dup2(pipe2[0], STDIN_FILENO) == -1) {
			perror("dup2 wc stdin");
			_exit(1);
		}
		close(pipe2[0]);
		execlp("wc", "wc", "-l", (char *)NULL);
		perror("execlp wc");
		_exit(1);
	}

	/* 父进程关闭剩余的 pipe2 读端 */
	close(pipe2[0]);

	/* 等待三个子进程结束 */
	for (int i = 0; i < 3; ++i) {
		wait(NULL);
	}

	return 0;
}