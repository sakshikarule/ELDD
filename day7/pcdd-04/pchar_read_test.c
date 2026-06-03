// user space program to test our device driver pchar.ko
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd, cnt;
    char ubuf[32];
    fd = open("/dev/pchar0", O_RDWR);
    printf("device file opened: fd=%d\n", fd);

    memset(ubuf, 0, sizeof(ubuf));
    cnt = read(fd, ubuf, 26);
    printf("device file read: %d bytes => %s\n", cnt, ubuf); // 26, A...Z

    memset(ubuf, 0, sizeof(ubuf));
    cnt = read(fd, ubuf, 10);
    printf("device file read: %d bytes => %s\n", cnt, ubuf); // 6, 1...6

    memset(ubuf, 0, sizeof(ubuf));
    cnt = read(fd, ubuf, 3);
    printf("device file read: %d bytes => %s\n", cnt, ubuf); // 0, 

    close(fd);
    printf("device file closed\n");
    return 0;
}

// > gcc pchar_read_test.c -o pchar_read_test.out
// > sudo ./pchar_read_test.out
