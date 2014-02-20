#KERNEL_DIR=/media/storage/linux-sunxi/
KERNEL_DIR=/media/ExtendedLinux/linux-sunxi
ifneq ($(KERNELRELEASE),)
	obj-m := therm1.o
	#therm1-objs := char_dev.o bitOperations.o protocolOperations.o thermOperations.o
	therm1-objs := toggleLed.o
else
	KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-eabi- -C ${KERNEL_DIR} M=$(PWD) modules
alexmac:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-eabi- -C ${KERNEL_DIR} M=$(PWD) modules
	scp -P 2220 -r *.o *.mod.* *.ko *.sh modules.order Module.symvers .*.cmd .tmp_versions camsi6@camsi.ups-tlse.fr:~/AlexMattijs
	/media/storage/linux-sunxissh -p 2220 camsi6@camsi.ups-tlse.fr
alexfac:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-eabi- -C ${KERNEL_DIR} M=$(PWD) modules
	scp -r *.o *.mod.* *.ko *.sh modules.order Module.symvers .*.cmd .tmp_versions root@olinuxino:~/AlexMattijs
	ssh root@olinuxino
matt:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-eabi- -C ${KERNEL_DIR} M=$(PWD) modules
	scp -r *.o *.mod.* *.ko *.sh modules.order Module.symvers .*.cmd .tmp_versions root@olinuxino:~/AlexMattijs
matthome:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -C ${KERNEL_DIR} M=$(PWD) modules
	scp -r *.o *.mod.* *.ko *.sh modules.order Module.symvers .*.cmd .tmp_versions root@192.168.0.37:~/AlexMattijs
clean:
	rm -rf *.o *.ko.* *.mod.* *.ko .[a-z] modules.order Module.symvers .*.cmd
endif
