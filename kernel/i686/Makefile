ARCH_NAME := i686

# Change this to the path of your cross compiler toolchain
OPT_DIR := /opt/cross/bin

CXX := $(OPT_DIR)/$(ARCH_NAME)-elf-g++
AS := $(OPT_DIR)/$(ARCH_NAME)-elf-as
LD := $(OPT_DIR)/$(ARCH_NAME)-elf-ld

AS := nasm

CXXFLAGS := -march=$(ARCH_NAME) -std=c++2b -masm=intel -e __kernel_main__ \
	-fno-exceptions -fno-rtti -ffreestanding -fno-eliminate-unused-debug-types \
	-nostdlib -nostartfiles -nodefaultlibs -mno-red-zone \
	-I./include \
	-D__NOS_KERNEL_GDT_INCLUDE_32BIT_SEGMENT__ \
	-g

ASFLAGS := -f elf32 -g -F dwarf

SRC_DIR := src
OUT_DIR := out

SRC_EXT := cpp
OUT_EXT := o
ASM_EXT := asm

SRC_FILES := $(wildcard $(SRC_DIR)/*.$(SRC_EXT))
ASM_FILES := $(wildcard $(SRC_DIR)/*.$(ASM_EXT))
OUT_FILES := $(patsubst $(SRC_DIR)/%.$(SRC_EXT),$(OUT_DIR)/%.$(OUT_EXT),$(SRC_FILES))

compile: $(OUT_FILES)
	mv $(OUT_DIR)/crti.$(OUT_EXT) ../..
	mv $(OUT_DIR)/crtn.$(OUT_EXT) ../..

$(OUT_DIR)/%.$(OUT_EXT): $(SRC_DIR)/%.$(SRC_EXT)
	@mkdir -p $(OUT_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUT_DIR)/%.$(OUT_EXT): $(SRC_DIR)/%.$(ASM_EXT)
	@mkdir -p $(OUT_DIR)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -f ./$(OUT_DIR)/*

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