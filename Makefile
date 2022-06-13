#
# Linux Kernel Module Makefile
# MPF: monitor process flag
# DPF: decode process flag
#
obj-m += mpf.o
obj-m += dpf.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
