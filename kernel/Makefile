CXX := ~/opt/cross/bin/i686-elf-g++
CXXFLAGS := -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -g

SRC_DIR := src
HPP_DIR := include
OUT_DIR := out

SRC_EXT := cpp
OUT_EXT := o

CXXFLAGS += -I$(HPP_DIR) -I../libc/include

all: $(OUT_DIR)/kernel.$(OUT_EXT)

$(OUT_DIR)/kernel.$(OUT_EXT): $(SRC_DIR)/kernel.$(SRC_EXT)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OUT_DIR)/*

.PHONY: all clean
