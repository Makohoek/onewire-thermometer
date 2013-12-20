#!/bin/sh
# Written by Uwe Hermann <uwe@hermann-uwe.de>, released as public domain.
# Modified by Piotr Esden-Tempski <piotr@esden.net>, released as public domain.

# Modified by Roel Verdult <roel@libnfc.org>, released as public domain.
# - Removed manufacture (STM) specific libraries
# - Added parameters for install location
# - Optimized for building smallest firmware size
# - Added windows (MinGW) build compatibility

# Modified by Mattijs Korpershoek <mattijs.korpershoek@gmail.com>
#      and by Alexandre Montilla <alexandre.montillla@gmail.com>
# to download mpfr, gmp and mpc which are requiered to build gcc
# removed multilib flag also, which was causing errors. multilib is 
# default in the recent (>4.8) gcc versions

if [ $# -ne 1 ]; then
  echo "Public GNU GCC ARM toolchain building script";
  echo "usage: $0 <prefix>";
  exit 0;
fi

# Version configuration
BINUTILS=binutils-2.24      # 2.18
GMP=gmp-5.1.3
MPFR=mpfr-3.1.2
MPC=mpc-1.0
GCC=gcc-4.8.2               # 4.8.2
NEWLIB=newlib-2.0.0        # 1.16.0
GDB=gdb-7.6                 # 6.8

# Advanced options
PARALLEL="-j 3"                 # PARALLEL = "-j 5" for 4 CPU's
DARWIN_OPT_PATH=/opt/local  # Path in which MacPorts or Fink is installed
MINGW_PATH=/mingw           # Path in which MacPorts or Fink is installed

# Command-line parameters
TARGET="arm-linux-eabi"     # TARGET = arm-elf | arm-none-eabi | arm-linux-eabi
PREFIX=$1/${TARGET}         # Install location of your toolchain (e.g. ${HOME})
INSTALL=${PREFIX}/install

export PATH="${PREFIX}/bin:${PATH}"

case "$(uname)" in
  Linux)
  echo "Found Linux OS."
  GCCFLAGS=
  GDBFLAGS=
  ;;
  Darwin)
  echo "Found Darwin OS."
  GCCFLAGS="--with-gmp=${DARWIN_OPT_PATH} \
            --with-mpfr=${DARWIN_OPT_PATH} \
            -with-libiconv-prefix=${DARWIN_OPT_PATH}"
  GDBFLAGS="--disable-werror"
  ;;
  MINGW*)
  echo "Found Windows OS."
  GCCFLAGS="--with-gmp=${MINGW_PATH} \
            --with-mpfr=${MINGW_PATH} \
            -with-libiconv-prefix=${MINGW_PATH}"
  GDBFLAGS="--disable-werror"
  ;;
  *)
  echo "Found unknown OS. Aborting!"
  exit 1
  ;;
esac

echo "Creating installation directories..."
if [ ! -e ${PREFIX} ]; then
  mkdir ${PREFIX}
fi
if [ ! -e ${INSTALL} ]; then
  mkdir ${INSTALL}
fi
cd ${INSTALL}
if [ ! -e sources ]; then
  mkdir sources
fi
if [ ! -e build ]; then
  mkdir build
fi

echo "Downloading binutils sources..."
wget -c -P sources http://ftp.gnu.org/gnu/binutils/${BINUTILS}.tar.bz2
echo "Downloading gmp sources..."
wget -c -P sources https://gmplib.org/download/gmp/${GMP}.tar.bz2
echo "Downloading mpfr sources..."
wget -c -P sources http://www.mpfr.org/mpfr-current/${MPFR}.tar.bz2
echo "Downloading mpc sources..."
wget -c -P sources http://www.multiprecision.org/mpc/download/${MPC}.tar.gz
echo "Downloading gcc sources..."
wget -c -P sources ftp://ftp.gnu.org/gnu/gcc/${GCC}/${GCC}.tar.bz2
echo "Downloading newlib sources..."
wget -c -P sources ftp://sources.redhat.com/pub/newlib/${NEWLIB}.tar.gz
echo "Downloading gdb sources..."
wget -c -P sources ftp://ftp.gnu.org/gnu/gdb/${GDB}.tar.bz2

if [ ! -e .${BINUTILS}.build ]; then
  echo "******************************************************************"
  echo "* Unpacking ${BINUTILS}"
  echo "******************************************************************"
  tar xfvj sources/${BINUTILS}.tar.bz2
  cd build
  echo "******************************************************************"
  echo "* Configuring ${BINUTILS}"
  echo "******************************************************************"
  ../${BINUTILS}/configure --target=${TARGET} \
                           --prefix=${PREFIX} \
                           --enable-interwork \
                           --with-gnu-as \
                           --with-gnu-ld \
                           --disable-werror || exit
  echo "******************************************************************"
  echo "* Building ${BINUTILS}"
  echo "******************************************************************"
  make MAKEINFO=makeinfo ${PARALLEL} || exit
  echo "******************************************************************"
  echo "* Installing ${BINUTILS}"
  echo "******************************************************************"
  make install || exit
  cd ..
  echo "******************************************************************"
  echo "* Cleaning up ${BINUTILS}"
  echo "******************************************************************"
  touch .${BINUTILS}.build
  rm -rf build/* ${BINUTILS}
fi

if [ ! -e .${GCC}-boot.build ]; then
  echo "******************************************************************"
  echo "* Unpacking ${GCC}-boot"
  echo "******************************************************************"
  tar xfvj sources/${GCC}.tar.bz2
  echo "******************************************************************"
  echo "* Unpacking ${GMP} library"
  echo "******************************************************************"
  tar xfvj sources/${GMP}.tar.bz2
  echo "******************************************************************"
  echo "* Moving ${GMP} library to ${GCC} build folder"
  echo "******************************************************************"
  mv ${GMP} ${GCC}/gmp || exit
  echo "******************************************************************"
  echo "* Unpacking ${MPFR} library"
  echo "******************************************************************"
  tar xfvj sources/${MPFR}.tar.bz2
  echo "******************************************************************"
  echo "* Moving ${MPFR} library to ${GCC} build folder"
  echo "******************************************************************"
  mv ${MPFR} ${GCC}/mpfr || exit
  echo "******************************************************************"
  echo "* Unpacking ${MPC} library"
  echo "******************************************************************"
  tar xfv sources/${MPC}.tar.gz
  echo "******************************************************************"
  echo "* Moving ${MPC} library to ${GCC} build folder"
  echo "******************************************************************"
  mv ${MPC} ${GCC}/mpc || exit
  cd build

  echo "******************************************************************"
  echo "* Configuring ${GCC}-boot"
  echo "******************************************************************"
  ../${GCC}/configure --target=${TARGET} \
                      --prefix=${PREFIX} \
                      --enable-interwork \
                      --with-newlib \
                      --enable-languages="c,c++" \
                      --without-headers \
                      --with-gnu-as \
                      --with-gnu-ld \
                      ${GCCFLAGS} || exit
  echo "******************************************************************"
  echo "* Building ${GCC}-boot"
  echo "******************************************************************"
  make ${PARALLEL} all-gcc || exit
  echo "******************************************************************"
  echo "* Installing ${GCC}-boot"
  echo "******************************************************************"
  make install-gcc || exit
  cd ..
  echo "******************************************************************"
  echo "* Cleaning up ${GCC}-boot"
  echo "******************************************************************"
  touch .${GCC}-boot.build
  rm -rf build/* ${GCC}
fi

if [ ! -e .${NEWLIB}.build ]; then
  echo "******************************************************************"
  echo "* Unpacking ${NEWLIB}"
  echo "******************************************************************"
  tar xfvz sources/${NEWLIB}.tar.gz
  cd build
  echo "******************************************************************"
  echo "* Configuring ${NEWLIB}"
  echo "******************************************************************"
  ../${NEWLIB}/configure --target=${TARGET} \
                         --prefix=${PREFIX} \
                         --enable-interwork \
                         --with-gnu-as \
                         --with-gnu-ld \
                         --enable-target-optspace \
                         --enable-newlib-reent-small \
                         --enable-newlib-io-c99-formats \
                         --enable-newlib-io-long-long \
                         --disable-newlib-multithread \
                         --disable-newlib-supplied-syscalls \
                         CFLAGS="-DPREFER_SIZE_OVER_SPEED -DSMALL_MEMORY" || exit
  echo "******************************************************************"
  echo "* Building ${NEWLIB}"
  echo "******************************************************************"
  make ${PARALLEL} || exit
  echo "******************************************************************"
  echo "* Installing ${NEWLIB}"
  echo "******************************************************************"
  make install || exit
  cd ..
  echo "******************************************************************"
  echo "* Cleaning up ${NEWLIB}"
  echo "******************************************************************"
  touch .${NEWLIB}.build
  rm -rf build/* ${NEWLIB}
fi


# Yes, you need to build gcc again!
if [ ! -e .${GCC}.build ]; then
  echo "******************************************************************"
  echo "* Unpacking ${GCC}"
  echo "******************************************************************"
  tar xfvj sources/${GCC}.tar.bz2
  echo "******************************************************************"
  echo "* Unpacking ${GMP} library"
  echo "******************************************************************"
  tar xfvj sources/${GMP}.tar.bz2
  echo "******************************************************************"
  echo "* Moving ${GMP} library to ${GCC} build folder"
  echo "******************************************************************"
  mv ${GMP} ${GCC}/gmp || exit
  echo "******************************************************************"
  echo "* Unpacking ${MPFR} library"
  echo "******************************************************************"
  tar xfvj sources/${MPFR}.tar.bz2
  echo "******************************************************************"
  echo "* Moving ${MPFR} library to ${GCC} build folder"
  echo "******************************************************************"
  mv ${MPFR} ${GCC}/mpfr || exit
  echo "******************************************************************"
  echo "* Unpacking ${MPC} library"
  echo "******************************************************************"
  tar xfv sources/${MPC}.tar.gz
  echo "******************************************************************"
  echo "* Moving ${MPC} library to ${GCC} build folder"
  echo "******************************************************************"
  mv ${MPC} ${GCC}/mpc || exit
  cd build
  echo "******************************************************************"
  echo "* Configuring ${GCC}"
  echo "******************************************************************"
  ../${GCC}/configure --target=${TARGET} \
                      --prefix=${PREFIX} \
                      --enable-interwork \
                      --enable-languages="c,c++" \
                      --enable-newlib \
                      --with-gnu-as \
                      --with-gnu-ld \
                      ${GCCFLAGS} || exit
  echo "******************************************************************"
  echo "* Building ${GCC}"
  echo "******************************************************************"
  make ${PARALLEL} || exit
  echo "******************************************************************"
  echo "* Installing ${GCC}"
  echo "******************************************************************"
  make install || exit
  cd ..
  echo "******************************************************************"
  echo "* Cleaning up ${GCC}"
  echo "******************************************************************"
  touch .${GCC}.build
  rm -rf build/* ${GCC}
fi

if [ ! -e .${GDB}.build ]; then
  echo "******************************************************************"
  echo "* Unpacking ${GDB}"
  echo "******************************************************************"
  tar xfvj sources/${GDB}.tar.bz2
  cd build
  echo "******************************************************************"
  echo "* Configuring ${GDB}"
  echo "******************************************************************"
  ../${GDB}/configure --target=${TARGET} \
                      --prefix=${PREFIX} \
                      --enable-interwork \
                      ${GDBFLAGS} || exit
  echo "******************************************************************"
  echo "* Building ${GDB}"
  echo "******************************************************************"
  make ${PARALLEL} || exit
  echo "******************************************************************"
  echo "* Installing ${GDB}"
  echo "******************************************************************"
  make install || exit
  cd ..
  echo "******************************************************************"
  echo "* Cleaning up ${GDB}"
  echo "******************************************************************"
  touch .${GDB}.build
  rm -rf build/* ${GDB}
fi
