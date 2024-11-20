AS := ~/opt/cross/bin/i686-elf-as
AS_FLAGS := 

SRC_DIR := src
OUT_DIR := out

SRC_EXT := s
OUT_EXT := o

all: $(OUT_DIR)/bootloader.$(OUT_EXT)

$(OUT_DIR)/bootloader.$(OUT_EXT): $(SRC_DIR)/bootloader.$(SRC_EXT)
	@mkdir -p $(OUT_DIR)
	$(AS) $(AS_FLAGS) $< -o $@

clean:
	rm -f $(OUT_DIR)/*

.PHONY: all clean
