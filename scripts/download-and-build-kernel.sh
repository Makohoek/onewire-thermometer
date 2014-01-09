#!/bin/sh
# Written by Mattijs Korpershoek <mattijs.korpershoek@gmail.com>
#      and by Alexandre Montilla <alexandre.montillla@gmail.com>

# you will need mkimage for building the uimage
# sudo aptitude install uboot-mkimage

if [ $# -ne 2 ]; then
  echo "Download and build Public Linux Kernel for the sunxi family (Allwinner A10,  A13,  ...));"
  echo "usage: $0 <buildFile> <pathToArmCompiler>";
  exit 0;
fi

BUILDDIR=$1
PATHTOARMCOMPILER=$2

# Cloning latest stable image from github
echo "Creating installation directories..."

if [ ! -e ${BUILDDIR} ]; then
  mkdir ${PREFIX}
fi

#download sources
cd ${BUILDDIR}
git clone -b sunxi-3.4 https://github.com/linux-sunxi/linux-sunxi.git

# Note: to build this, you should have a gcc-arm compiler in your path
# build
cd linux-sunxi
PATH=$PATH:${PATHTOARMCOMPILER}
make ARCH=arm CROSS_COMPILE=arm-linux-eabi- uImage modules

# create a full module tree:
make ARCH=arm CROSS_COMPILE=arm-linux-eabi- INSTALL_MOD_PATH=output modules_install

echo "you have the following sitting in your kernel tree:
    arch/arm/boot/uImage
    utput/lib/modules/3.4.XXX/"


# u-boot
git clone https://github.com/linux-sunxi/u-boot-sunxi.git
cd u-boot-sunxi.git

# find out which is our TARGET board
grep sunxi boards.cfg | awk '{print $7}' | grep A13 }

make CROSS_COMPILE=arm-linux-eabi- A13-OLinuXino
