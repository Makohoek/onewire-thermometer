KERNEL_DIR=/media/storage/linux-sunxi/
ifneq ($(KERNELRELEASE),)
	obj-m := therm.o
	therm-objs := char_dev.o bitOperations.o protocolOperations.o thermOperations.o
else
	KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-eabi- -C ${KERNEL_DIR} M=$(PWD) modules
alexmac:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-eabi- -C ${KERNEL_DIR} M=$(PWD) modules
	scp -P 2220 -r *.o *.mod.* *.ko modules.order Module.symvers .*.cmd .tmp_versions camsi6@camsi.ups-tlse.fr:~/AlexMattijs
	ssh -p 2220 camsi6@camsi.ups-tlse.fr
alexfac:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-eabi- -C ${KERNEL_DIR} M=$(PWD) modules
	scp -r *.o *.mod.* *.ko modules.order Module.symvers .*.cmd .tmp_versions root@olinuxino:~/AlexMattijs
	ssh root@olinuxino
matt:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-eabi- -C ${KERNEL_DIR} M=$(PWD) modules
	scp -r *.o *.mod.* *.ko modules.order Module.symvers .*.cmd .tmp_versions root@olinuxino:~/AlexMattijs
	ssh root@olinuxino
clean:
	rm -rf *.o *.ko.* *.mod.* *.ko .[a-z] modules.order Module.symvers .*.cmd
endif
