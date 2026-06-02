
obj-m := splitfile.o
splitfile-objs := init.o exit.o file.o

modules:
	make -C /lib/modules/`uname -r`/build M=`pwd` modules

clean:	
	make -C /lib/modules/`uname -r`/build M=`pwd` clean

.phony:	modules clean

