obj-m := mygpio.o

KDIR := /home/nilesh/linux/linux/
PWD := $(shell pwd)

all:
	make -C /lib/modules/`uname -r`/build M=$(PWD) modules
#	above is command for native compilation
#	make -C $(KDIR) M=$(PWD) ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- modules
#	above is command for cross compilation

clean:
	make -C /lib/modules/`uname -r`/build M=$(PWD) clean
#	make -C $(KDIR) M=$(PWD) clean

