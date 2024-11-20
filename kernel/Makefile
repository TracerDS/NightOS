CXX := ~/opt/cross/bin/i686-elf-g++
AS := ~/opt/cross/bin/i686-elf-as
LD := ~/opt/cross/bin/i686-elf-ld

CXXFLAGS := -ffreestanding -nostdlib -O2 -Wall -Wextra -fno-exceptions -fno-rtti -g -std=c++2b -Wno-deprecated-enum-enum-conversion
ASFLAGS :=

SRC_DIR := src
HPP_DIR := include
OUT_DIR := out

ASM_SRC_EXT := s
SRC_EXT := cpp
OUT_EXT := o

SRC_FILES := $(wildcard $(SRC_DIR)/*.$(SRC_EXT))
OUT_SRC_FILES := $(patsubst $(SRC_DIR)/%.$(SRC_EXT),$(OUT_DIR)/%.$(OUT_EXT),$(SRC_FILES))

ASM_FILES := $(wildcard $(SRC_DIR)/*.$(ASM_SRC_EXT))
OUT_ASM_FILES := $(patsubst $(SRC_DIR)/%.$(ASM_SRC_EXT),$(OUT_DIR)/%.$(OUT_EXT),$(ASM_FILES))

CXXFLAGS += -I$(HPP_DIR) -I../libc/include

all: $(OUT_SRC_FILES) $(OUT_ASM_FILES)

$(OUT_SRC_FILES): $(SRC_FILES)
	@mkdir -p $(OUT_DIR)
	$(CXX) $(CXXFLAGS) -c $(patsubst $(OUT_DIR)/%.$(OUT_EXT),$(SRC_DIR)/%.$(SRC_EXT),$@) -o $@

$(OUT_ASM_FILES): $(ASM_FILES)
	@mkdir -p $(OUT_DIR)
	$(AS) $(ASFLAGS) $(patsubst $(OUT_DIR)/%.$(OUT_EXT),$(SRC_DIR)/%.$(ASM_SRC_EXT),$@) -o $@

print:
	@echo "CXX = $(CXX)"
	@echo "AS = $(AS)"
	@echo "LD = $(LD)"
	@echo "CXXFLAGS = $(CXXFLAGS)"
	@echo "ASFLAGS = $(ASFLAGS)"
	@echo "SRC_DIR = $(SRC_DIR)"
	@echo "HPP_DIR = $(HPP_DIR)"
	@echo "OUT_DIR = $(OUT_DIR)"
	@echo "ASM_SRC_EXT = $(ASM_SRC_EXT)"
	@echo "SRC_EXT = $(SRC_EXT)"
	@echo "OUT_EXT = $(OUT_EXT)"
	@echo "SRC_FILES = $(SRC_FILES)"
	@echo "OUT_SRC_FILES = $(OUT_SRC_FILES)"
	@echo "ASM_FILES = $(ASM_FILES)"
	@echo "OUT_ASM_FILES = $(OUT_ASM_FILES)"

clean:
	rm $(OUT_SRC_FILES)
	rm $(OUT_ASM_FILES)
	rm -f ./$(OUT_DIR)/*

.PHONY: all clean
