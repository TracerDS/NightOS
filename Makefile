# Change this to the path of your cross compiler toolchain.
# Leave empty if its in the PATH
OPT_DIR := /opt/cross/bin/

SRC_DIR := src
HPP_DIR := include
OUT_DIR := out
BIN_DIR := build

SRC_EXT := cpp
ASM_EXT := asm
OUT_EXT := o
LNK_EXT := ld
BIN_EXT := bin

ARCHS_COMPILE := $(shell ls archs)
ARCHS_CLEAN := $(patsubst %,clean-%,$(ARCHS_COMPILE))
ARCHS_LINK := $(patsubst %,link-%,$(ARCHS_COMPILE))

# Take the first architecture from the list for default target
DEFAULT_ARCH := $(word 1,$(ARCHS_COMPILE))


CXX := $(OPT_DIR)$(DEFAULT_ARCH)-elf-g++
AS := nasm
LD := $(OPT_DIR)$(DEFAULT_ARCH)-elf-ld

GCC_VERSION := $(shell $(CXX) -dumpfullversion)

CXXFLAGS :=
ASFLAGS :=
LDFLAGS := -T linker.$(LNK_EXT) -O2 -nostdlib -L$(OPT_DIR)../lib -L$(OPT_DIR)../lib/gcc/$(CROSS_PREFIX)/$(GCC_VERSION)

all: link-$(DEFAULT_ARCH)

compile-all: $(ARCHS_COMPILE)
link-all: $(ARCHS_LINK)
clean-compile: clean all

$(ARCHS_COMPILE):
	$(eval __CURRENT_ARCH__ := $(@))
	@echo "\033[38;5;63mBuilding for architecture: $(__CURRENT_ARCH__)\033[0m"
	$(MAKE) -C archs/$(__CURRENT_ARCH__) compile
	@echo "\033[32mBuilding complete!\033[0m"
	@echo ""

$(ARCHS_CLEAN):
	$(eval __CURRENT_ARCH__ := $(@:clean-%=%))
	@echo "\033[38;5;63mCleaning for architecture: $(__CURRENT_ARCH__)\033[0m"
	$(MAKE) -C archs/$(__CURRENT_ARCH__) clean
	@echo "\033[32mCleaning complete!\033[0m"
	@echo ""

$(ARCHS_LINK): $(ARCHS_COMPILE)
	$(eval __CURRENT_ARCH__ := $(@:link-%=%))
	$(eval __OS_PATH__ := archs/$(__CURRENT_ARCH__)/$(BIN_DIR)/$(__CURRENT_ARCH__).$(BIN_EXT))
	@echo "\033[38;5;63mLinking for architecture: $(__CURRENT_ARCH__)\033[0m"
	$(MAKE) -C archs/$(__CURRENT_ARCH__) link
	@echo "\033[32mLinking complete!\033[0m"
	@echo ""
	@echo "\033[38;5;63mMaking grub image for architecture: $(__CURRENT_ARCH__)\033[0m"

	scripts/check_grub.sh $(__OS_PATH__)
	scripts/make_grub_image.sh $(__CURRENT_ARCH__) $(__OS_PATH__)
	scripts/make_debug_symbols.sh $(__OS_PATH__)

	@echo "\033[38;5;83mBuild finished!\033[0m"

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
	@echo "CXX: $(CXX)"
	@echo "AS: $(AS)"
	@echo "LD: $(LD)"
	@echo ""
	@echo "CXXFLAGS: $(CXXFLAGS)"
	@echo "ASFLAGS: $(ASFLAGS)"
	@echo "LDFLAGS: $(LDFLAGS)"

clean: $(ARCHS_CLEAN)

.PHONY: clean $(ARCHS_COMPILE)
	