# Change this to the path of your cross compiler toolchain.
OPT_DIR := /opt/cross/bin

SRC_DIR := src
HPP_DIR := include
OUT_DIR := out
BIN_DIR := build

SRC_EXT := cpp
ASM_EXT := asm
OUT_EXT := o
BIN_EXT := bin
LNK_EXT := ld

ARCH_NAME := $(notdir $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST)))))
CROSS_PREFIX := $(ARCH_NAME)-elf

CXX := $(OPT_DIR)/$(CROSS_PREFIX)-g++
AS := nasm
LD := $(OPT_DIR)/$(CROSS_PREFIX)-ld

GCC_VERSION := $(shell $(CXX) -dumpfullversion)

CXXFLAGS := 
ASFLAGS := 
LDFLAGS := -T linker.$(LNK_EXT) -O2 -nostdlib -L$(OPT_DIR)/../lib/gcc/$(CROSS_PREFIX)/$(GCC_VERSION)

BOOTLOADER_DIR := bootloader
KERNEL_DIR := kernel

# ------------------------------------------------ #

all: link

compile:
	$(MAKE) -C $(BOOTLOADER_DIR) compile
	$(MAKE) -C $(KERNEL_DIR) compile

link: compile
	@mkdir -p $(BIN_DIR)

	$(LD) $(LDFLAGS) -o $(BIN_DIR)/$(ARCH_NAME).$(BIN_EXT) \
		$(BOOTLOADER_DIR)/$(OUT_DIR)/bootloader.$(BIN_EXT) \
		$(KERNEL_DIR)/$(OUT_DIR)/kernel.$(BIN_EXT) \
	-lgcc
	
#	$(LD) $(LDFLAGS) -o $(BIN_DIR)/$(ARCH_NAME).$(BIN_EXT) \
		$(shell find $(BOOTLOADER_DIR) -type f -name '*.$(OUT_EXT)') \
		$(shell find $(KERNEL_DIR) -type f -name '*.$(OUT_EXT)') \
	-lgcc

clean:
	$(MAKE) -C $(BOOTLOADER_DIR) clean
	$(MAKE) -C $(KERNEL_DIR) clean
	rm -f -r $(BIN_DIR)

info:
	@echo "ARCH_NAME: $(ARCH_NAME)"
	@echo ""
	@echo "OPT_DIR: $(OPT_DIR)"
	@echo "SRC_DIR: $(SRC_DIR)"
	@echo "HPP_DIR: $(HPP_DIR)"
	@echo "OUT_DIR: $(OUT_DIR)"
	@echo "BIN_DIR: $(BIN_DIR)"
	@echo ""
	@echo "SRC_EXT: $(SRC_EXT)"
	@echo "ASM_EXT: $(ASM_EXT)"
	@echo "OUT_EXT: $(OUT_EXT)"
	@echo "BIN_EXT: $(BIN_EXT)"
	@echo "LNK_EXT: $(LNK_EXT)"
	@echo ""
	@echo "OS_NAME: $(OS_NAME)"
	@echo "OS_PATH: $(OS_PATH)"
	@echo "CROSS_PREFIX: $(CROSS_PREFIX)"
	@echo ""
	@echo "CXX: $(CXX)"
	@echo "AS: $(AS)"
	@echo "LD: $(LD)"
	@echo ""
	@echo "CXXFLAGS: $(CXXFLAGS)"
	@echo "ASFLAGS: $(ASFLAGS)"
	@echo "LDFLAGS: $(LDFLAGS)"
	@echo ""
	@echo "GCC_VERSION: $(GCC_VERSION)"
	@echo "BOOTLOADER_DIR: $(BOOTLOADER_DIR)"
	@echo "KERNEL_DIR: $(KERNEL_DIR)"

.PHONY: all compile link clean info
	