AS := nasm

SRC_DIR := src
LIB_DIR := include
OBJ_DIR := obj
BUILD_DIR := build

CXX_FLAGS := -ffreestanding -fno-pie -nostdlib -static
AS_FLAGS :=
LD_FLAGS := -e main -Ttext=0x1000 --oformat binary -I$(LIB_DIR)/

CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
ASM_FILES := $(wildcard $(SRC_DIR)/*.asm)
SRC_FILES := $(CPP_FILES) $(ASM_FILES)

CPP_OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_FILES))
ASM_OBJ := $(patsubst $(SRC_DIR)/%.asm,$(OBJ_DIR)/%.o,$(ASM_FILES))
OBJ_FILES := $(CPP_OBJ) $(ASM_OBJ)

KERNEL := kernel.bin

compile_x86: $(CPP_FILES)
	$(CXX) $(CXX_FLAGS) -m32 -c $< -o $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$<)

assembly_x86: $(ASM_FILES)
	$(AS) $(AS_FLAGS) -f elf32 $< -o $(patsubst $(SRC_DIR)/%.asm,$(OBJ_DIR)/%.o,$<)

link_x86: $(OBJ_FILES)
	$(LD) $(LD_FLAGS) -m elf_i386 -T x86.ld -o $(BUILD_DIR)/$(KERNEL)

build_x86: $(SRC_FILES)
	@$(MAKE) --no-print-directory compile_x86
	@$(MAKE) --no-print-directory assembly_x86
	@$(MAKE) --no-print-directory link_x86


compile_x86_64: $(CPP_FILES)
	$(CXX) $(CXX_FLAGS) -m64 -c $< -o $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$<)

assembly_x86_64: $(ASM_FILES)
	$(AS) $(AS_FLAGS) -f elf64 $< -o $(patsubst $(SRC_DIR)/%.asm,$(OBJ_DIR)/%.o,$<)

link_x86_64: $(OBJ_FILES)
	$(LD) $(LD_FLAGS) -m elf $< -T x86_64.ld -o $(BUILD_DIR)/$(KERNEL)

build_x86_64: $(SRC_FILES)
	@$(MAKE) --no-print-directory compile_x86_64
	@$(MAKE) --no-print-directory assembly_x86_64
	@$(MAKE) --no-print-directory link_x86_64


clean:
	rm -fr build/* obj/* *.o

.PHONY: compile_x86 assembly_x86 link_x86 build_x86 compile_x86_64 assembly_x86_64 link_x86_64 build_x86_64 clean