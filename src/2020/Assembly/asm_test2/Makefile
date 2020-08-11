EXECUTABLE=asm_test2.exe

AS=nasm
ASFLAGS=-fwin64
LD=clang
LDFLAGS=
SOURCES=$(wildcard ./*.asm)
OBJECTS=$(SOURCES:.asm=.obj)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(LD) $(LDFLAGS) ./obj/$(OBJECTS) -o "./bin/$@"

$(OBJECTS): $(SOURCES)
	$(AS) $(ASFLAGS) "$(SOURCES)" -o "./obj/$@"

init:
	mkdir ./obj ; mkdir ./bin
clean:
	rm -rf ./obj/*.obj ./bin/$(EXECUTABLE)
