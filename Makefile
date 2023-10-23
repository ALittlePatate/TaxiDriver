# Makefile for Linux Kernel Driver
CONFIG_MODULE_SIG=n

# Source files and object files
SRC_DIR := src
SOURCE_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := src/TaxiDriver.o 
obj-m := $(OBJ_FILES) 
# KERNELDIR ?= /home/maxime/Downloads/linux-6.5.7-arch1/
KERNELDIR ?= /lib/modules/6.5.8-arch1-1/build/
# Kernel module name
MODULE_NAME := TaxiDriver

all: default

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	mv src/$(MODULE_NAME).ko .
	$(MAKE) clean
	cd src/client && $(MAKE)

clean:
	find src/ -maxdepth 1 -type f ! -name "*.h" ! -name "*.c" -exec rm {} \;

fclean: clean
	find . -maxdepth 1 -type f ! -name "Makefile" -exec rm {} \;

load:
	sudo insmod $(MODULE_NAME).ko
	sudo mknod /dev/$(MODULE_NAME) c 511 0

unload:
	sudo rmmod $(MODULE_NAME)
	sudo rm /dev/$(MODULE_NAME)

.PHONY: all clean fclean load unload
