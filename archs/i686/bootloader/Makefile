# Change this to the path of your cross compiler toolchain
OPT_DIR := /opt/cross/bin

SRC_DIR := src
OUT_DIR := out

SRC_EXT := asm
OUT_EXT := o
BIN_EXT := bin

ARCH_NAME := $(notdir $(realpath $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))/..))
CROSS_PREFIX := $(ARCH_NAME)-elf

CXX := $(OPT_DIR)/$(CROSS_PREFIX)-g++
AS := nasm
LD := $(OPT_DIR)/$(CROSS_PREFIX)-ld
AR := $(OPT_DIR)/$(CROSS_PREFIX)-ar

GCC_VERSION := $(shell $(CXX) -dumpfullversion)

CXXFLAGS :=
ASFLAGS := -f elf32 -g -F dwarf -D__MULTIBOOT__
LDFLAGS := -L$(OPT_DIR)/../lib/gcc/$(CROSS_PREFIX)/$(GCC_VERSION) -r
ARFLAGS := rcs

SRC_FILES := $(shell find $(SRC_DIR) -type f -name *.$(SRC_EXT))
OUT_FILES := $(patsubst $(SRC_DIR)/%.$(SRC_EXT),$(OUT_DIR)/%.$(OUT_EXT),$(SRC_FILES))

compile: $(OUT_FILES)
	$(LD) $(LDFLAGS) $^ -o ./$(OUT_DIR)/bootloader.$(BIN_EXT)

$(OUT_DIR)/%.$(OUT_EXT): $(SRC_DIR)/%.$(SRC_EXT)
	@mkdir -p $(OUT_DIR)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf ./$(OUT_DIR)/*

info:
	@echo "ARCH_NAME: $(ARCH_NAME)"
	@echo "CROSS_PREFIX: $(CROSS_PREFIX)"
	@echo "GCC_VERSION: $(GCC_VERSION)"
	@echo ""
	@echo "OPT_DIR: $(OPT_DIR)"
	@echo "SRC_DIR: $(SRC_DIR)"
	@echo "OUT_DIR: $(OUT_DIR)"
	@echo ""
	@echo "SRC_EXT: $(SRC_EXT)"
	@echo "OUT_EXT: $(OUT_EXT)"
	@echo "LIB_EXT: $(LIB_EXT)"
	@echo ""
	@echo "CXX: $(CXX)"
	@echo "AS: $(AS)"
	@echo "LD: $(LD)"
	@echo "AR: $(AR)"
	@echo ""
	@echo "CXXFLAGS: $(CXXFLAGS)"
	@echo "ASFLAGS: $(ASFLAGS)"
	@echo "LDFLAGS: $(LDFLAGS)"
	@echo "ARFLAGS: $(ARFLAGS)"
	@echo ""
	@echo "SRC_FILES: $(SRC_FILES)"
	@echo "OUT_FILES: $(OUT_FILES)"
	@echo ""

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
ifeq ($(ASFLAGS),)
$(warning ASFLAGS is empty! This may cause issues. \
Please set the ASFLAGS variable to at least include the current architecture.)
endif
ifeq ($(SRC_EXT),)
$(error SRC_EXT is empty! Please set the SRC_EXT variable to the source file extension.)
endif
ifeq ($(OUT_EXT),)
$(warning OUT_EXT is empty! This may cause issues. \
Please set the OUT_EXT variable to the output file extension.)
endif

##############################################