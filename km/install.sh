#!/bin/bash
make clean
make
sudo rmmod TaxiDriver
sudo rm /dev/TaxiDriver
# choose a unique MAJOR number; read out the used number and adjust in mknod for next use.
# MAJOR number has to match mknod to open device!
# 511, 250, 505
sudo mknod /dev/TaxiDriver c 506 0
sudo chmod 666 /dev/TaxiDriver
sudo insmod TaxiDriver.ko
cat /proc/devices | grep -i "TaxiDriver"
