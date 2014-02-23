#! /bin/bash

source ./settings.sh 

rmmod ${MODULENAME} || exit 1

rm -rf /dev/ledAlexMatt
rm -rf /dev/thermAlexMatt
# rm -rf /dev/schrijven
