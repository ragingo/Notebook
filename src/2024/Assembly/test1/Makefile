AS=nasm
ASFLAGS=-f elf64
LD=ld
LDFLAGS=
OUTPUT=test1

SRC_FILES := $(shell find . -name '*.asm')
OBJECTS=$(patsubst %.asm,%.o,$(filter %.asm,$(SRC_FILES)))

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(OUTPUT)

%.o: %.asm
	$(AS) $(ASFLAGS) -o $@ $<

clean:
	-rm $(OBJECTS) $(OUTPUT)
