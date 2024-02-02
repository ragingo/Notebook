AS=nasm
ASFLAGS=-f elf64
LD=ld
LDFLAGS=
OUTPUT=test1
SOURCE=main.asm
OBJECT=$(SOURCE:.asm=.o)

all: $(OUTPUT)

$(OUTPUT): $(OBJECT)
	$(LD) $(LDFLAGS) $(OBJECT) -o $(OUTPUT)

$(OBJECT): $(SOURCE)
	$(AS) $(ASFLAGS) $(SOURCE) -o $(OBJECT)

clean:
	rm -f $(OBJECT) $(OUTPUT)
