build_x86:
	@mkdir -p bin
	$(MAKE) --no-print-directory -C bootloader build_x86
	$(MAKE) --no-print-directory -C drivers build_x86
	$(MAKE) --no-print-directory -C kernel build_x86
	cat bootloader/obj/boot_x86.bin kernel/obj/kernel.bin > bin/os.bin
	
build_x86_64:
	@mkdir -p bin
	@$(MAKE) --no-print-directory -C bootloader build_x86_64
	@$(MAKE) --no-print-directory -C drivers build_x86_64
	@$(MAKE) --no-print-directory -C kernel build_x86_64
	cat bootloader/obj/boot_x86_64.bin kernel/obj/kernel.bin > bin/os.bin

clean:
	@$(MAKE) --no-print-directory -C bootloader clean
	@$(MAKE) --no-print-directory -C drivers clean
	@$(MAKE) --no-print-directory -C kernel clean
	rm -fr bin/*

.PHONY: build_x86 build_x86_64 clean