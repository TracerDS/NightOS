AS := nasm
AS_FLAGS := 

BOOT_X86 := boot_x86
BOOT_X86_64 := boot_x86_64

SRC_DIR := src
OBJ_DIR := obj

build_x86: $(SRC_DIR)/$(BOOT_X86).asm
	@mkdir -p obj
	$(AS) -f bin $< -o $(OBJ_DIR)/$(BOOT_X86).bin
	$(AS) -f elf $(SRC_DIR)/kernel_entry.asm -o $(OBJ_DIR)/kernel_entry.o

build_x86_64: $(SRC_DIR)/$(BOOT_X86_64).asm
	$(AS) $(AS_FLAGS) $< -o $(OBJ_DIR)/$(BOOT_X86_64).bin

clean:
	rm -fr *.bin *.iso

.PHONY: build_x86 build_x86_64 clean obj