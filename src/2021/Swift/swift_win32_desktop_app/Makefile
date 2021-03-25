SHELL := /bin/bash
SWIFT_OPTIONS = \
	-g -D DEBUG \
	-sdk %SDKROOT% \
	-I %SDKROOT%/usr/lib/swift \
	-L %SDKROOT%/usr/lib/swift/windows -luser32 -lcomctl32 \
	-I ./Tools/vcpkg/installed/x64-windows-static/include \
	-L ./Tools/vcpkg/installed/x64-windows-static/lib -ljpeg
SWIFT_SOURCES = $(shell shopt -s globstar && ls ./**/*.swift)
OUTPUT_DIR = ./build
EXE_NAME = app
EXE_PATH = $(OUTPUT_DIR)/$(EXE_NAME).exe

all: clean build_cpp build_swift

run:
	$(EXE_PATH)

clean:
	$(RM) $(OUTPUT_DIR)/*

build_cpp:
	clang -c ./Sources/Native/lib.cpp -o $(OUTPUT_DIR)/lib.o 1>/dev/null

build_swift:
	cmd /c swiftc $(SWIFT_OPTIONS) $(SWIFT_SOURCES) -import-objc-header ./Sources/Native/lib.h $(OUTPUT_DIR)/lib.o -o $(EXE_PATH)

watch:
	./Scripts/watch.sh $(OUTPUT_DIR)
