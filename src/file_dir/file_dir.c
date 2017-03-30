#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int show()
{
	fprintf(stdout, "apue chapter 4, file and directory\n");
}


int main(int argc, char const *argv[])
{
	show();

	return 0;
}