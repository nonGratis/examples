ifneq ($(KERNELRELEASE),)
# kbuild part of the makefile
obj-m := hello1.o
ccflags-y += -g -DDEBUG
else

ARCH := arm
CROSS_COMPILE := arm-linux-gnueabihf-
KDIR ?= /lib/modules/$(shell uname -r)/build

default:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=$(PWD)
clean:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=$(PWD) clean
endif
