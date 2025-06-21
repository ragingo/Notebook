PHONY: build
build:
	@echo "Building CompileTimeValuesSampleLib..."
	swiftc \
		-module-name CompileTimeValuesSampleLib \
		-emit-library \
		-enable-experimental-feature CompileTimeValues \
		-enable-experimental-feature CompileTimeValuesPreview \
		-o CompileTimeValuesSampleLib.dylib \
		./Sources/CompileTimeValuesSampleLib.swift
	@echo "Build complete."

