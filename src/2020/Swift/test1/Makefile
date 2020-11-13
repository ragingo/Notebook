EXECUTABLE=swift_test1

OPTIONS=-sdk %SDKROOT% -I %SDKROOT%/usr/lib/swift -L %SDKROOT%/usr/lib/swift/windows
SOURCES=$(wildcard ./*.swift)

all:
	cmd /c swiftc $(OPTIONS) $(SOURCES) -o ./build/$(EXECUTABLE).exe
run:
	./build/$(EXECUTABLE).exe
clean:
	rm -rf ./build/*
