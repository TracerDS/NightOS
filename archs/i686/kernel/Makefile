# Change this to the path of your cross compiler toolchain
OPT_DIR := /opt/cross/bin

SRC_DIR := src
HPP_DIR := include
OUT_DIR := out

ARCH_NAME := i686

SRC_EXT := cpp
ASM_EXT := asm
OUT_EXT := o
PRE_EXT := po

ARCH_NAME := $(notdir $(realpath $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))/..))
CROSS_PREFIX := $(ARCH_NAME)-elf

CXX := $(OPT_DIR)/$(ARCH_NAME)-elf-g++
AS := nasm
LD := $(OPT_DIR)/$(ARCH_NAME)-elf-ld
AR := $(OPT_DIR)/$(ARCH_NAME)-elf-ar

CXXFLAGS := -march=$(ARCH_NAME) -std=c++2b -masm=intel \
	-fno-exceptions -fno-rtti -ffreestanding \
	-nostdlib -nostartfiles -nodefaultlibs -mno-red-zone \
	-Wall -Wextra -Wpedantic \
	-I$(HPP_DIR) \
	-D__MULTIBOOT__ -D_DEBUG \
	-g -fno-eliminate-unused-debug-types
ASFLAGS := -f elf32 -g -F dwarf
LDFLAGS := -L$(OPT_DIR)/../lib/gcc/$(CROSS_PREFIX)/$(GCC_VERSION) -r
ARFLAGS := rcs

SRC_FILES := $(shell find $(SRC_DIR) -type f -name *.$(SRC_EXT))
ASM_FILES := $(shell find $(SRC_DIR) -type f -name *.$(ASM_EXT))
OUT_FILES := $(patsubst $(SRC_DIR)/%.$(SRC_EXT),$(OUT_DIR)/%.$(OUT_EXT),$(SRC_FILES))

compile: $(OUT_FILES)
	mv $(OUT_DIR)/crt.$(OUT_EXT) ..

$(OUT_DIR)/%.$(OUT_EXT): $(SRC_DIR)/%.$(SRC_EXT)
	@mkdir -p $(patsubst $(SRC_DIR)/%,$(OUT_DIR)/%,$(dir $<))
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUT_DIR)/%.$(OUT_EXT): $(SRC_DIR)/%.$(ASM_EXT)
	@mkdir -p $(patsubst $(SRC_DIR)/%,$(OUT_DIR)/%,$(dir $<))
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -fr ./$(OUT_DIR)/*

info:
	@echo "Current architecture: $(ARCH_NAME)"
	@echo ""
	@echo "CXX: $(CXX)"
	@echo "CXXFLAGS: $(CXXFLAGS)"
	@echo ""
	@echo "SRC_DIR: $(SRC_DIR)"
	@echo "OUT_DIR: $(OUT_DIR)"
	@echo "SRC_EXT: $(SRC_EXT)"
	@echo "OUT_EXT: $(OUT_EXT)"
	@echo ""
	@echo "SRC_FILES: $(SRC_FILES)"
	@echo "ASM_FILES: $(ASM_FILES)"
	@echo "OUT_FILES: $(OUT_FILES)"

.PHONY: compile clean info


##############################################

ifeq ($(CXX),)
$(error Compiler not found. Please set the CXX variable to the compiler command.)
endif
ifeq ($(AS),)
$(error Assembler not found. Please set the AS variable to the assembler command.)
endif
ifeq ($(LD),)
$(error Linker not found. Please set the LD variable to the linker command.)
endif
ifeq ($(CXXFLAGS),)
$(error CXXFLAGS is empty! Please set the CXXFLAGS variable to \
include current architecture and to compile for a freestanding environment.)
endif
ifeq ($(SRC_EXT),)
$(error SRC_EXT is empty! Please set the SRC_EXT variable to the source file extension.)
endif
ifeq ($(OUT_EXT),)
$(warning OUT_EXT is empty! This may cause issues. \
Please set the OUT_EXT variable to the output file extension.)
endif

##############################################