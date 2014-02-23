#! /bin/bash

source ./settings.sh 

dmesg --clear
insmod ${MODULENAME}.ko GpioPort=2 || exit 1
dmesg | less
./unlinkDevice.sh

mknod /dev/ledAlexMatt c 249 0
# mknod /dev/lezenKapot c 249 1
# mknod /dev/schrijven c 249 2

chgrp ${GRP} /dev/ledAlexMatt
#  chgrp ${GRP} /dev/lezenKapot
#  chgrp ${GRP} /dev/schrijven

chmod 644 /dev/ledAlexMatt
#  chmod 644 /dev/lezenKapot
#  chmod 644 /dev/schrijven
