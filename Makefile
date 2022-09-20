build_x86:
	$(MAKE) -s -C bootloader build_x86
	
build_x86_64:
	$(MAKE) -s -C bootloader build_x86_64

clean:
	@$(MAKE) -s -C bootloader clean

.PHONY: build_x86 build_x86_64 clean