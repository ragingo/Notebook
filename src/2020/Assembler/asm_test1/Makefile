EXECUTABLE=asm_test1

# all: asm_test1
# main.o: main.asm
# 	nasm -f macho32 main.asm
# asm_test1: main.o
# 	ld -lSystem -o asm_test1 main.o

AS=nasm
ASFLAGS=-f macho32
LD=ld
LDFLAGS=-macosx_version_min 10.14 -lSystem
SOURCES=$(wildcard ./*.asm)
OBJECTS=$(SOURCES:.asm=.o)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(LD) $(LDFLAGS) ./obj/$(OBJECTS) -o ./bin/$@

$(OBJECTS): $(SOURCES)
	$(AS) $(ASFLAGS) $(SOURCES) -o ./obj/$@

init:
	mkdir ./obj ; mkdir ./bin
clean:
	rm -rf ./obj/*.o ./bin/$(EXECUTABLE)
