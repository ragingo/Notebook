CXX := g++
CXXFLAGS := -std=c++23 -O2 -Wall -Wextra

INPUT ?=

BUILD_DIR := build
COMPILER := $(BUILD_DIR)/compiler
SRC := compiler.cpp
ASM := $(BUILD_DIR)/program.asm
OBJ := $(BUILD_DIR)/program.o
BIN := $(BUILD_DIR)/program

NASM := nasm
NASM_FMT := elf64
LD := ld

.PHONY: all clean run compile help

all: $(BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(COMPILER): $(SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

$(ASM): $(COMPILER)
	@if [ -z "$(INPUT)" ]; then \
		echo "Error: INPUT variable is required. Use: make INPUT=filename.txt"; \
		exit 1; \
	fi
	@echo "Compiling $(INPUT) to assembly..."
	./$(COMPILER) $(INPUT)
	@echo "Generated assembly file: $(ASM)"

$(OBJ): $(ASM)
	$(NASM) -f $(NASM_FMT) -o $@ $<

$(BIN): $(OBJ)
	$(LD) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)

run: $(BIN)
	./$(BIN)

compile: $(ASM)
