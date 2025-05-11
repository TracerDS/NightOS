ARCH_NAME := i686

# Change this to the path of your cross compiler toolchain
OPT_DIR := /opt/cross/bin

CXX := $(OPT_DIR)/$(ARCH_NAME)-elf-g++
AS := $(OPT_DIR)/$(ARCH_NAME)-elf-as
LD := $(OPT_DIR)/$(ARCH_NAME)-elf-ld

ASFLAGS := -march=$(ARCH_NAME)
LDFLAGS := -nostdlib -O2 -g -nostartfiles -nodefaultlibs -e __bootloader_start__ -r

AS := nasm
ASFLAGS := -f elf32 -g -F dwarf

SRC_DIR := src
OUT_DIR := out

SRC_EXT := asm
OUT_EXT := o

SRC_FILES := $(wildcard $(SRC_DIR)/*.$(SRC_EXT))
OUT_FILES := $(patsubst $(SRC_DIR)/%.$(SRC_EXT),$(OUT_DIR)/%.$(OUT_EXT),$(SRC_FILES))

compile: $(OUT_FILES)
	$(CXX) $(LDFLAGS) -o $(OUT_DIR)/bootloader.bin $(OUT_FILES)

$(OUT_DIR)/%.$(OUT_EXT): $(SRC_DIR)/%.$(SRC_EXT)
	@mkdir -p $(OUT_DIR)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf ./$(OUT_DIR)/*

info:
	@echo "Current architecture: $(ARCH_NAME)"
	@echo ""
	@echo "AS: $(AS)"
	@echo "ASFLAGS: $(ASFLAGS)"
	@echo ""
	@echo "SRC_DIR: $(SRC_DIR)"
	@echo "OUT_DIR: $(OUT_DIR)"
	@echo "SRC_EXT: $(SRC_EXT)"
	@echo "OUT_EXT: $(OUT_EXT)"
	@echo ""
	@echo "SRC_FILES: $(SRC_FILES)"
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