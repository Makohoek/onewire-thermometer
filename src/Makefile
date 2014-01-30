KERNEL_DIR=/media/a852afe7-e4bb-48b6-a1da-7583947c84d4/linux-sunxi/
ifneq ($(KERNELRELEASE),)
   	obj-m := char_dev.o
	therm-objs := bitOperations.o protocolOperations.o thermOperations.o
else
	KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-linux-eabi- -C ${KERNEL_DIR} M=$(PWD) modules
clean:
	rm -rf *.o *.ko.* *.mod.* *.ko .[a-z] modules.order Module.symvers .*.cmd
endif
