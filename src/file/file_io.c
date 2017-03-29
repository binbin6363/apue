#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>

int show()
{
	fprintf(stdout, "apue chapter 2, file io\n");
}

// 获取当前时刻，毫秒
int get_ms()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	int time_ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return time_ms;
}

// 由open返回的文件描述符一定是最小的未用描述符数字。这一点被很多应用程序用来在标
// 准输入、标准输出或标准出错输出上打开一个新的文件。例如，一个应用程序可以先关闭标准
// 输出(通常是文件描述符1 )，然后打开另一个文件，事先就能了解到该文件一定会在文件描述
// 符1上打开。在3 . 1 2节说明dup2函数时，可以了解到有更好的方法来保证在一个给定的描述符
// 上打开一个文件
int fake_redirect()
{
	close(STDOUT_FILENO);
	int fd = open("test.txt", O_RDWR | O_APPEND | O_CREAT | O_SYNC);
	fprintf(stderr, "print info to stderr, close fd:%d, open fd:%d.\n", STDOUT_FILENO, fd);
	if (-1 == fd)
	{
		fprintf(stderr, "%s\n", "open file failed.");
		return -1;
	}
	fprintf(stdout, "print info to stdout.\n");
	close(fd);
	return 0;
}

// 将标准输出定向到文件test.txt
int test_dup2()
{
	int fd = open("test.txt", O_RDWR | O_APPEND | O_CREAT | O_SYNC);
	int newfd = dup2(fd, STDOUT_FILENO);
	if (-1 == newfd)
	{
		fprintf(stderr, "%s\n", "call dup2 failed.");
		return -1;
	}
	write(newfd, "call dup2, test print to stdout\n", 32);
	close(fd);
	return 0;
}

// f c n t l函数可以改变已经打开文件的性质
int test_fcntl()
{
	int fd = open("test.txt", O_RDWR | O_APPEND | O_CREAT | O_SYNC);
	if (-1 == fd)
	{
		fprintf(stderr, "%s\n", "open file failed.");
		return -1;
	}
	int fd1 = fcntl(fd, F_DUPFD, STDOUT_FILENO);
	if (-1 == fd1)
	{
		fprintf(stderr, "%s\n", "call fcntl failed.");
		return -2;
	}
	write(fd1, "print info after call fcntl(STDOUT_FILENO, F_FDUPFD, fd).\n", 59);
	close(fd1);
	return 0;
}

const static int BUFFSIZE = 8192;//(1<<20); // 1mb
// 同步写数据
// 8.2M 数据 同步写耗时9678ms，BUFFSIZE=8192
// 8.2M 数据 同步写耗时234ms，BUFFSIZE=(1<<20)
int test_sync_write()
{
	int ret = 0;
	int fdr = open("test_read.txt", O_RDONLY);
	if (-1 == fdr)
	{
		fprintf(stderr, "%s:%s\n", "open file failed, file", "test_read.txt");
		return -1;
	}
	int fd = open("test_write.txt", O_WRONLY | O_CREAT | O_SYNC);
	if (-1 == fd)
	{
		fprintf(stderr, "%s:%s\n", "open file failed, file", "test_write.txt");
		return -1;
	}

	int start = get_ms();
	int readn = 0;
	char buf[BUFFSIZE];
	while (0 != (readn = read(fdr, buf, BUFFSIZE)))
	{
		write(fd, buf, readn);
	}
	int end = get_ms();
	fprintf(stdout, "%s:%dms\n", "sync write file cost time", (end-start));
	close(fd);
	return 0;
}

// 异步写数据
// 8.2M 数据 异步写耗时9ms，BUFFSIZE=8192
// 8.2M 数据 异步写耗时6ms，BUFFSIZE=(1<<20)
int test_async_write()
{
	int ret = 0;
	int fdr = open("test_read.txt", O_RDONLY);
	if (-1 == fdr)
	{
		fprintf(stderr, "%s:%s\n", "open file failed, file", "test_read.txt");
		return -1;
	}
	int fd = open("test_write.txt", O_WRONLY | O_CREAT);
	if (-1 == fd)
	{
		fprintf(stderr, "%s:%s\n", "open file failed, file", "test_write.txt");
		return -1;
	}

	int start = get_ms();
	int readn = 0;
	char buf[BUFFSIZE];
	while (0 != (readn = read(fdr, buf, BUFFSIZE)))
	{
		write(fd, buf, readn);
	}
	int end = get_ms();
	fprintf(stdout, "%s:%dms\n", "async write file cost time", (end-start));
	close(fd);
	return 0;
}

int main(int argc, char const *argv[])
{
	show();

	if (0 != test_sync_write())
	{
		fprintf(stderr, "%s\n", "test test_sync_write failed.");
	}
	
	if (0 != test_async_write())
	{
		fprintf(stderr, "%s\n", "test test_async_write failed.");
	}
	
	return 0;
}
