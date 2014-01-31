KERNEL_DIR=/media/a852afe7-e4bb-48b6-a1da-7583947c84d4/linux-sunxi/
ifneq ($(KERNELRELEASE),)
	obj-m := char_dev.o
	char_dev-objs := bitOperations.o protocolOperations.o thermOperations.o
else
	KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
default:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-linux-eabi- -C ${KERNEL_DIR} M=$(PWD) modules
alex:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-linux-eabi- -C ${KERNEL_DIR} M=$(PWD) modules
	scp -P 2220 *.mod.* *.ko camsi6@camsi.ups-tlse.fr:~/AlexMattijs
	ssh -p 2220 camsi6@camsi.ups-tlse.fr
matt:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-linux-eabi- -C ${KERNEL_DIR} M=$(PWD) modules
	scp -P 2220 *.mod.* *.ko camsi5@camsi.ups-tlse.fr:~/AlexMattijs
	ssh -p 2220 camsi5@camsi.ups-tlse.fr
test:
	arm-linux-gnueabihf-gcc main.c
	scp -P 2220 a.out camsi5@camsi.ups-tlse.fr:~/AlexMattijs
	ssh -p 2220 camsi5@camsi.ups-tlse.fr
clean:
	rm -rf *.o *.ko.* *.mod.* *.ko .[a-z] modules.order Module.symvers .*.cmd
endif
