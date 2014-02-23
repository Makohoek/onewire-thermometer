#! /bin/bash

source ./settings.sh 

dmesg --clear
insmod ${MODULENAME}.ko GpioPort=2 || exit 1
#dmesg | less
#./unlinkDevice.sh

mknod /dev/ledAlexMatt c 247 0
mknod /dev/thermAlexMatt c 247 1
# mknod /dev/schrijven c 247 2

chgrp ${GRP} /dev/ledAlexMatt
chgrp ${GRP} /dev/thermAlexMatt
#  chgrp ${GRP} /dev/schrijven

chmod 644 /dev/ledAlexMatt
chmod 644 /dev/thermAlexMatt
#  chmod 644 /dev/schrijven
