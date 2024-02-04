AS=nasm
ASFLAGS=-f elf64
LD=ld
LDFLAGS=
OUT_DIR=build
OUTPUT=$(OUT_DIR)/test1

SRC_FILES := $(shell find . -name '*.asm')
OBJECTS=$(patsubst %.asm,$(OUT_DIR)/%.o,$(filter %.asm,$(SRC_FILES)))

all: setup $(OUTPUT)

setup:
	mkdir -p build/lib/src
	mkdir -p build/sample

$(OUTPUT): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(OUTPUT)

$(OUT_DIR)/%.o: %.asm
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	-rm $(OBJECTS) $(OUTPUT)
