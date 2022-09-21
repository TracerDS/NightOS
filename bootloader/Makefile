AS := nasm
AS_FLAGS := -f bin

BOOT_X86 := boot_x86
BOOT_X86_64 := boot_x86_64

build_x86: $(BOOT_X86).asm
	$(AS) $(AS_FLAGS) $< -o $(BOOT_X86).bin

build_x86_64: $(BOOT_X86_64).asm
	$(AS) $(AS_FLAGS) $< -o $(BOOT_X86_64).bin

clean:
	rm -fr *.bin *.iso

.PHONY: build_x86 build_x86_64 clean