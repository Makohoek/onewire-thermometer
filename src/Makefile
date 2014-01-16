ifneq ($(KERNELRELEASE),)
   	obj-m := therm.o
   therm-objs := char_dev.o
else
	KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) -C ${KERNEL_DIR} M=$(PWD) modules
clean:
	rm -rf *.o *.ko.* *.mod.* *.ko .[a-z] modules.order Module.symvers .*.cmd
endif
