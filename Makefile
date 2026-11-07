obj-m += lkdriver.o

KDIR ?= /lib/modules/`uname -r`/build

test: install
	lsmod | grep lkdriver

install: build
	sudo rmmod lkdriver.ko || true
	sudo insmod lkdriver.ko

build:
	$(MAKE) -C $(KDIR) M=$$PWD
