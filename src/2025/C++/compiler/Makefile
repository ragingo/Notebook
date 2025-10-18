CXX := g++
CXXFLAGS := -std=c++23 -O2 -Wall -Wextra -I./include -c
LDFLAGS :=

INPUT ?=

BUILD_DIR := build
SRC_DIR := .
COMPILER := $(BUILD_DIR)/yoctocc
ASM := $(BUILD_DIR)/program.asm
OBJ := $(BUILD_DIR)/program.o
BIN := $(BUILD_DIR)/program

# C++ ソースファイル (サブディレクトリも含む)
SRCS := $(shell find $(SRC_DIR) -name "*.cpp" -type f)
# ヘッダーファイル (コンパイル時の依存関係用)
HEADERS := $(wildcard include/*.hpp include/**/*.hpp)
# オブジェクトファイル
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
# 依存関係ファイル
DEPS := $(OBJS:.o=.d)

NASM := nasm
NASM_FMT := elf64
LD := ld

.PHONY: all clean run compile test help

all: $(COMPILER)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# オブジェクトファイルのコンパイル (自動依存関係生成)
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -o $@ $<

# コンパイラ実行ファイルのリンク
$(COMPILER): $(OBJS) | $(BUILD_DIR)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

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

# 依存関係ファイルのインクルード
-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR)

run: $(COMPILER)
	./$(COMPILER)

compile: $(ASM)

test:
	@echo "Running test suite..."
	@bash test/run_tests.sh

rebuild: clean all
	@echo "Rebuild complete"

help:
	@echo "Available targets:"
	@echo "  all       - Build the compiler (default)"
	@echo "  compile   - Compile input file to assembly (INPUT=filename.txt)"
	@echo "  run       - Run the compiler"
	@echo "  test      - Run test suite"
	@echo "  rebuild   - Clean and rebuild"
	@echo "  clean     - Remove build directory"
	@echo "  help      - Show this help message"

