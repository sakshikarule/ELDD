// user space program to test our device driver pchar.ko
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd, cnt;
    char ubuf[32]="";
    fd = open("/dev/pchar0", O_RDWR);
    printf("device file opened: fd=%d\n", fd);
    cnt = write(fd, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26);
    printf("device file written: %d bytes\n", cnt); // 26
    cnt = write(fd, "1234567890", 10);
    printf("device file written: %d bytes\n", cnt); // 6
    cnt = write(fd, "ESD", 3);
    printf("device file written: %d bytes\n", cnt); // Error
    //cnt = read(fd, ubuf, 32);
    //printf("device file read: %d bytes: %s\n", cnt, ubuf);
    close(fd);
    printf("device file closed\n");
    return 0;
}

// > gcc pchar_write_test.c -o pchar_write_test.out
// > sudo ./pchar_write_test.out
