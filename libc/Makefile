CXX := ~/opt/cross/bin/i686-elf-g++
LD := ~/opt/cross/bin/i686-elf-ld

CXXFLAGS := -nostartfiles -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fPIC -fno-stack-protector -Wl,-z,noexecstack -Wl,-z,relro,-z,now
LDFLAGS := 
SHFLAGS := -nostdlib -ffreestanding -g

SRC_DIR := src
HPP_DIR := include
OUT_DIR := out

SRC_EXT := cpp
OUT_EXT := o

SRC_FILES := $(wildcard $(SRC_DIR)/*.$(SRC_EXT))
OUT_FILES := $(patsubst $(SRC_DIR)/%.$(SRC_EXT), $(OUT_DIR)/%.$(OUT_EXT), $(SRC_FILES))

CXXFLAGS += -I$(HPP_DIR)

all: $(OUT_DIR)/nightlibc.a

$(OUT_DIR)/nightlibc.a: $(OUT_FILES)
	ar rcs $@ $^

$(OUT_DIR)/%.$(OUT_EXT): $(SRC_DIR)/%.$(SRC_EXT)
	$(CXX) $(CXXFLAGS) $(SHFLAGS) -c $< -o $@

clean:
	rm -f $(OUT_DIR)/*

.PHONY: all clean