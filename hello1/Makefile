ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m := hello1.o
ccflags-y := -I$(M)/inc -DDEBUG
else
# Normal makefile
KDIR ?= /lib/modules/$(shell uname -r)/build

default:
	$(MAKE) -C $(KDIR) M=$(PWD) EXTRA_CFLAGS+=-DDEBUG
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
endif
