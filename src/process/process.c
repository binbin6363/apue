#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>


void show()
{
	fprintf(stdout, "apue chapter 8, process control\n");
}

// v f o r k与f o r k一样都创建一个子进程，
// 但是它并不将父进程的地址空间完全复制到子进程中，因为子进程会立即调用e x e c (或e x i t )，于
// 是也就不会存访该地址空间。不过在子进程调用e x e c或e x i t之前，它在父进程的空间中运行。
// 这种工作方式在某些U N I X的页式虚存实现中提高了效率（与上节中提及的，在f o r k之后跟随
// e x e c，并采用在写时复制技术相类似）。
// v f o r k和f o r k之间的另一个区别是： v f o r k保证子进程先运行，在它调用e x e c或e x i t之后父进
// 程才可能被调度运行。（如果在调用这两个函数之前子进程依赖于父进程的进一步动作，则会
// 导致死锁。）

int glob = 6;
char buf[] = "hello, test fork and vfork.\n";

int test_fork1()
{
	int var;
	pid_t pid;

	var = 88;

	if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1)
	{
		printf("write to stdout failed.");
		exit(1);
	}

	printf("%s\n", "before fork");

	if ((pid = fork()) < 0)
	{
		printf("fork error");
		exit(1);
	}
	else if (0 == pid)
	{
		glob++;
		var++;
	}
	else
	{
		sleep(2);
	}

	printf("pid=%d, glob=%d, var=%d\n", getpid(), glob, var);

	return 0;
}


int main(int argc, char const *argv[])
{
	show();

	if (0 != test_fork1())
	{
		printf("test fork1 failed\n");
		return 0;
	}
	return 0;
}
