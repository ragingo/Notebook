SWIFT_OPTIONS = -sdk %SDKROOT% -I %SDKROOT%/usr/lib/swift -L %SDKROOT%/usr/lib/swift/windows -luser32
SWIFT_SOURCES = $(wildcard ./Sources/*.swift)
OUTPUT_DIR = ./build
EXE_NAME = swift_test1
EXE_PATH = $(OUTPUT_DIR)/$(EXE_NAME).exe

all: build_cpp
	@make build_swift

run:
	$(EXE_PATH)

clean:
	$(RM) $(OUTPUT_DIR)/*

build_cpp:
	clang -c ./Sources/lib.cpp -o $(OUTPUT_DIR)/lib.o

build_swift:
	cmd /c swiftc $(SWIFT_OPTIONS) $(SWIFT_SOURCES) -import-objc-header ./Sources/lib.h $(OUTPUT_DIR)/lib.o -o $(EXE_PATH)
