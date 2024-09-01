CXX := ~/opt/cross/bin/i686-elf-g++
CXXFLAGS := -T linker.ld -ffreestanding -O2 -nostdlib -Llibc/out

SRC_DIR := src
OUT_DIR := out

SRC_EXT := cpp
OUT_EXT := o

OS_NAME := NightOS
OS_BIN := $(OS_NAME).bin

all:
	$(MAKE) -C bootloader
	$(MAKE) -C libc
	$(MAKE) -C kernel
	$(CXX) $(CXXFLAGS) bootloader/$(OUT_DIR)/bootloader.$(OUT_EXT) kernel/$(OUT_DIR)/kernel.$(OUT_EXT) -o $(OUT_DIR)/$(OS_BIN) -l:nightlibc.a

bootloader:
	$(MAKE) -C $@

libc:
	$(MAKE) -C $@

kernel:
	$(MAKE) -C $@

clean:
	$(MAKE) -C bootloader clean
	$(MAKE) -C libc clean
	$(MAKE) -C kernel clean

.PHONY: all clean bootloader libc kernel
