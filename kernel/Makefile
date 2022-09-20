CXX_FLAGS := -ffreestanding -fno-pie
LD_FLAGS := -Ttext 0x0 --oformat binary

SRC_DIR := src
LIB_DIR := include
OBJ_DIR := obj
BUILD_DIR := build


SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

KERNEL := kernel.bin

# TODO: Add this
	$(CXX) $(CXX_FLAGS) -m32 -c $< -o $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$<)

compile_x86: $(OBJ_FILES)
	$(CXX) $(CXX_FLAGS) -m32 -c $< -o $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$<)

link_x86: $(OBJ_FILES)
	$(LD) $(LD_FLAGS) -m elf_i386 $< -o $(BUILD_DIR)/$(KERNEL)

build_x86:
	$(MAKE) -s compile_x86
	$(MAKE) -s link_x86


compile_x86_64: $(SRC_FILES)
	$(CXX) $(CXX_FLAGS) -m64 -c $< -o $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$<)

link_x86_64: $(OBJ_FILES)
	$(LD) $(LD_FLAGS) $< -o $(BUILD_DIR)/$(KERNEL)

build_x86_64:
	$(MAKE) -s compile_x86_64
	$(MAKE) -s link_x86_64


clean:
	rm -fr build/* obj/* *.o

.PHONY: compile_x86 link_x86 build_x86 compile_x86_64 link_x86_64 build_x86_64 clean