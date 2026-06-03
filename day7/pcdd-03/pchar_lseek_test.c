#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
	int fd, cnt;
	char buf[32];
	fd = open("dev/pchar0", O_RDWR);
	cnt = write(fd, "ABCDEFGHIJKLMNOPQRSTUVWZYZ",32);

	printf("device write: %d bytes.\n", cnt);

	lseek(fd, -6, SEEK_END); // fpos=26
	memset(buf, 0, sizeof(buf));
	read(fd, buf, 4); // 1234 // fpos=30
	printf("device read: %s\n", buf);

	lseek(fd, 0, SEEK_SET); //fpos=0
	memset(buf, 0, sizeof(buf));
	read(fd, buf, 4); //ABCD // fpos=4
	printf("device read: %s\n", buf);

	lseek(fd, +10, SEEK_CUR); // fpos=14
	memset(buf, 0, sizeof(buf));
	read(fd, buf, 4);  //OPQR // fpos=18
	printf("device read: %s\n", buf);

	close(fd);
	return 0;
}

