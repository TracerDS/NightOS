# APPS #
CXX := i686-elf-g++
AS := i686-elf-as
LD := i686-elf-ld

# FLAGS #
LD_FLAGS := -T linker.ld -lgcc -ffreestanding -O2 -nostdlib 

OUT_DIR := obj
BIN_DIR := isodir

OUT_EXT := o

OS_NAME := NightOS
OS_EXT := bin
OS_PATH := $(OS_NAME).$(OS_EXT)
OS_BIN_PATH := $(basename $(OS_PATH)).iso

# Compilation targets # 

all: boot kernel
	$(CXX) $(LD_FLAGS) -o $(OS_PATH) $(shell find boot/$(OUT_DIR) kernel/$(OUT_DIR) -name '*.$(OUT_EXT)')
	cp $(OS_PATH) $(BIN_DIR)/boot/$(OS_PATH)
	grub-mkrescue -o $(OS_BIN_PATH) $(BIN_DIR)

boot:
	$(MAKE) -C boot
kernel:
	$(MAKE) -C kernel

info:
	@echo "LD           = $(LD)"
	@echo ""
	@echo "LD_FLAGS     = $(LD_FLAGS)"
	@echo ""
	@echo "OUT_DIR      = $(OUT_DIR)"
	@echo ""
	@echo "BOOT_FILES   = $(BOOT_FILES)"
	@echo "KERNEL_FILES = $(KERNEL_FILES)"
	@echo "OUT_FILES    = $(OUT_FILES)"
	@echo ""
	@echo "OUT_EXT      = $(OUT_EXT)"
	@echo "OS_NAME      = $(OS_NAME)"
	@echo "OS_EXT       = $(OS_EXT)"
	@echo "OS_PATH      = $(OS_PATH)"

clean:
	$(MAKE) clean -C boot
	$(MAKE) clean -C kernel
	rm -f $(OS_PATH) $(OS_BIN_PATH)

.PHONY: boot kernel all clean