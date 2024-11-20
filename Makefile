CXX := ~/opt/cross/bin/i686-elf-g++
CXXFLAGS := -T linker.ld -ffreestanding -O2 -nostdlib -Llibc/out

SRC_DIR := src
OUT_DIR := out

SRC_EXT := cpp
OUT_EXT := o

OS_NAME := NightOS
OS_BIN := $(OS_NAME).bin

all:
	$(MAKE) bootloader
	$(MAKE) libc
	$(MAKE) kernel
	$(MAKE) link

link:
	@mkdir -p $(OUT_DIR)
	$(CXX) $(CXXFLAGS) \
		$(wildcard bootloader/$(OUT_DIR)/*.$(OUT_EXT)) \
		$(wildcard kernel/$(OUT_DIR)/*.$(OUT_EXT)) \
		-o $(OUT_DIR)/$(OS_BIN) \
		-l:nightlibc.a

bootloader:
	$(MAKE) -C $@

libc:
	$(MAKE) -C $@

kernel:
	$(MAKE) -C $@

print:
	@echo "CXX: $(CXX)"
	@echo "CXXFLAGS: $(CXXFLAGS)"
	@echo "SRC_DIR: $(SRC_DIR)"
	@echo "OUT_DIR: $(OUT_DIR)"
	@echo "SRC_EXT: $(SRC_EXT)"
	@echo "OUT_EXT: $(OUT_EXT)"
	@echo "OS_NAME: $(OS_NAME)"
	@echo "OS_BIN: $(OS_BIN)"
	@echo "BOOT_FILES: $(BOOT_FILES)"
	@echo "KERNEL_FILES: $(KERNEL_FILES)"

clean:
	$(MAKE) -C bootloader clean
	$(MAKE) -C libc clean
	$(MAKE) -C kernel clean

.PHONY: all clean bootloader kernel libc print
