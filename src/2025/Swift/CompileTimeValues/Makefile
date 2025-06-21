.PHONY: build
build:
	@echo "[CompileTimeValuesSampleLib] start build"
	swiftc \
		-module-name CompileTimeValuesSampleLib \
		-emit-library \
		-enable-testing \
		-enable-experimental-feature CompileTimeValues \
		-enable-experimental-feature CompileTimeValuesPreview \
		-o CompileTimeValuesSampleLib.dylib \
		./Sources/CompileTimeValuesSampleLib.swift
	@echo "[CompileTimeValuesSampleLib] end build"

# テストをライブラリから分離してみたが、テストファイルのコンパイル時に
# `error: '@const' value should be initialized with a compile-time value` が出てしまう。
.PHONY: test
test: build
	@echo "[CompileTimeValuesSampleLibTests] start build"
	swiftc \
		-module-name CompileTimeValuesSampleLibTests \
		-enable-experimental-feature CompileTimeValues \
		-enable-experimental-feature CompileTimeValuesPreview \
		-I . \
		-L . \
		-lCompileTimeValuesSampleLib \
		-o CompileTimeValuesSampleLibTests \
		./Tests/CompileTimeValuesSampleLibTests.swift
	@echo "[CompileTimeValuesSampleLibTests] end build"
	./CompileTimeValuesSampleLibTests
