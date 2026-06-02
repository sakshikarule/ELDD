// user space program to test our device driver pchar.ko
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd, cnt;
	char ubuf[32]="",
	fd = open("/dev/pchar0", O_RDWR);
    printf("device file opened: fd=%d\n", fd);
	cnt = write(fd, "ABCD", 4);
	printf("device file written: %d bytes\n", cnt);
	cnt = read(fd, ubuf, 32);
	printf("device file read: %d bytes: %s\n", cnt, ubuf);
	close(fd);
	printf("device file close\n");
	return 0;
}

// > gcc pchar_test.c -o pchar_test.out
// > sudo ./pchar_test.out
