SRC_DIR := src
LIB_DIR := include
OBJ_DIR := obj

CXX_FLAGS := -ffreestanding -fno-pie -nostdlib -static -I$(LIB_DIR)/

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

obj/%.o: src/%.cpp
	$(CXX) $(CXX_FLAGS) -m32 -c $< -o $@

build_x86: $(OBJ_FILES)
	@mkdir -p obj

clean:
	rm -fr obj/* *.o

.PHONY: build_x86 build_x86_64 clean