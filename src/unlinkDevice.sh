#! /bin/bash

source ./settings.sh 


rmmod ${MODULENAME} || exit 1

rm -rf /dev/lezen
rm -rf /dev/lezenKapot
rm -rf /dev/schrijven
