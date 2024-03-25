# APPS #
CXX := i686-elf-g++
AS := i686-elf-as
LD := i686-elf-ld

# FILES #
SRC_DIR := src
HPP_DIR := include
OUT_DIR := obj

# FLAGS #
SHARED_FLAGS := -ffreestanding
CXXFLAGS := $(SHARED_FLAGS) -Wall -Wextra -fno-exceptions -fno-rtti -I$(HPP_DIR) -I../libc/include -g3
AS_FLAGS := 
LD_FLAGS := 


SRC_EXT := cpp
OUT_EXT := o

SRC_FILES := $(wildcard $(SRC_DIR)/*.$(SRC_EXT))
OUT_FILES := $(patsubst $(SRC_DIR)/%.$(SRC_EXT),$(OUT_DIR)/%.$(OUT_EXT),$(SRC_FILES))

# Compilation targets #

all: $(OUT_FILES)

$(OUT_DIR)/%.$(OUT_EXT): $(SRC_DIR)/%.$(SRC_EXT)
	$(CXX) $(CXXFLAGS) -c $< -o $@

info:
	@echo "CXX          = $(CXX)"
	@echo "AS           = $(AS)"
	@echo "LD           = $(LD)"
	@echo ""
	@echo "SHARED_FLAGS = $(SHARED_FLAGS)"
	@echo "CXXFLAGS     = $(CXXFLAGS)"
	@echo "AS_FLAGS     = $(AS_FLAGS)"
	@echo "LD_FLAGS     = $(LD_FLAGS)"
	@echo ""
	@echo "SRC_DIR      = $(SRC_DIR)"
	@echo "HPP_DIR      = $(HPP_DIR)"
	@echo "OUT_DIR      = $(OUT_DIR)"
	@echo ""
	@echo "SRC_FILES    = $(SRC_FILES)"
	@echo "OUT_FILES    = $(OUT_FILES)"
	@echo ""
	@echo "SRC_EXT    = $(SRC_EXT)"
	@echo "OUT_EXT    = $(OUT_EXT)"

clean:
	rm -f $(OUT_FILES)

.PHONY: all info clean