ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m := hello2.o
ccflags-y := -I$(PWD)/../hello1/inc
else
# Normal makefile
KDIR ?= /lib/modules/$(shell uname -r)/build
default:
	$(MAKE) -C $(KDIR) M=$(PWD)
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
endif
