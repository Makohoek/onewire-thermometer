#KERNEL_DIR=/media/storage/linux-sunxi/
KERNEL_DIR=/media/ExtendedLinux/linux-sunxi
#KERNEL_DIR=/media/a852afe7-e4bb-48b6-a1da-7583947c84d4/sunxi-jorda/linux-sunxi
ifneq ($(KERNELRELEASE),)
	obj-m := therm.o
	therm-objs := char_dev.o bitOperations.o OneWire.o thermOperations.o led.o DiscoveryProtocol.o SensorID.o LinkedList.o SensorOperations.o
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
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- -Wall -C ${KERNEL_DIR} M=$(PWD) modules
	scp -r *.o *.mod.* *.ko *.sh modules.order Module.symvers .*.cmd .tmp_versions Makefile testioctl.c testtemperature.c root@192.168.0.1:~/AlexMattijs
clean:
	rm -rf *.o *.ko.* *.mod.* *.ko .[a-z] modules.order Module.symvers .*.cmd
endif

testioctl:
	gcc -Wall testioctl.c -o testioctl
testtemperature:
	gcc -Wall testtemperature.c -o testtemperature
