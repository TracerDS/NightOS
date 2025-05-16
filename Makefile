# List of architectures are located in archs.txt
# Each architecture should be on a new line in archs.txt
ARCHS_COMPILE := $(shell cat archs.txt | tr '\n' ' ')
ARCHS_CLEAN := $(patsubst %,clean-%,$(ARCHS_COMPILE))
ARCHS_LINK := $(patsubst %,link-%,$(ARCHS_COMPILE))

# Take the first architecture from the list for default target
DEFAULT_ARCH := $(word 1,$(ARCHS_COMPILE))

BIN_DIR := build
OS_NAME := NightOS
OS_EXT := bin
OS_PATH := $(BIN_DIR)/$(OS_NAME).$(OS_EXT)

OUT_DIR := out
OUT_EXT := o

# Change this to the path of your cross compiler toolchain.
# Leave empty if its in the PATH
OPT_DIR := /opt/cross/bin/

CXX := $(OPT_DIR)$(DEFAULT_ARCH)-elf-g++
AS := $(OPT_DIR)$(DEFAULT_ARCH)-elf-as
LD := $(OPT_DIR)$(DEFAULT_ARCH)-elf-ld
LDFLAGS := -T linker.ld -O2 -nostdlib -L/opt/cross/lib/ -L/opt/cross/lib/gcc/$(DEFAULT_ARCH)-elf/15.1.0

all: link-$(DEFAULT_ARCH)

compile-all: $(ARCHS_COMPILE)
link-all: $(ARCHS_LINK)

$(ARCHS_COMPILE):
	$(eval __CURRENT_ARCH__ := $(@))
	@echo "Building for architecture: $(__CURRENT_ARCH__)"
	$(MAKE) -C bootloader/$(__CURRENT_ARCH__) compile
	$(MAKE) -C kernel/$(__CURRENT_ARCH__) compile

$(ARCHS_CLEAN):
	$(eval __CURRENT_ARCH__ := $(@:clean-%=%))
	@echo "Cleaning for architecture: $(__CURRENT_ARCH__)"
	$(MAKE) -C bootloader/$(__CURRENT_ARCH__) clean
	$(MAKE) -C kernel/$(__CURRENT_ARCH__) clean

$(ARCHS_LINK): $(ARCHS_COMPILE)
	$(eval __CURRENT_ARCH__ := $(@:link-%=%))
	$(eval OS_PATH := $(BIN_DIR)/$(@:link-%=%).$(OS_NAME).$(OS_EXT))
	@echo "Linking for architecture: $(__CURRENT_ARCH__)"
	@mkdir -p $(BIN_DIR)

	$(LD) $(LDFLAGS) -o $(OS_PATH) \
		$(wildcard bootloader/$(__CURRENT_ARCH__)/$(OUT_DIR)/*.$(OUT_EXT)) \
		$(wildcard kernel/$(__CURRENT_ARCH__)/$(OUT_DIR)/*.$(OUT_EXT)) \
		crti.$(OUT_EXT) crtn.$(OUT_EXT) \
	-lgcc
	@echo "Linking complete. Making grub image..."
	rm crti.$(OUT_EXT) crtn.$(OUT_EXT)
	scripts/check_grub.sh $(OS_PATH)
	scripts/make_grub_image.sh $(__CURRENT_ARCH__) $(OS_PATH)

info:
	@echo "Available architectures:"
	@for arch in $(ARCHS_COMPILE); do echo "- $$arch"; done
	@echo ""
	@echo "ARCHS_COMPILE: $(ARCHS_COMPILE)"
	@echo "ARCHS_CLEAN: $(ARCHS_CLEAN)"
	@echo "ARCHS_LINK: $(ARCHS_LINK)"
	@echo "DEFAULT_ARCH: $(DEFAULT_ARCH)"
	@echo ""
	@echo "BIN_DIR: $(BIN_DIR)"
	@echo "OS_NAME: $(OS_NAME)"
	@echo "OS_EXT: $(OS_EXT)"
	@echo "OS_PATH: $(OS_PATH)"
	@echo ""
	@echo "OUT_DIR: $(OUT_DIR)"
	@echo "OUT_EXT: $(OUT_EXT)"
	@echo "OPT_DIR: $(OPT_DIR)"
	@echo ""
	@echo "LD: $(LD)"
	@echo "LDFLAGS: $(LDFLAGS)"

clean: $(ARCHS_CLEAN)
	rm -f ./$(BIN_DIR)/*

.PHONY: clean $(ARCHS_COMPILE)
	