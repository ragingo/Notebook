# CompileTimeValue

`-enable-experimental-feature CompileTimeValue` と  
`-enable-experimental-feature CompileTimeValuePreview` を使って  
Swift 言語のコンパイル時計算がどのくらい使えるのか調査するためのコード。

# 環境構築

## Windows

https://www.swift.org/install/windows/ から release/6.2 を探し、 x86_64 向けインストーラーをダウンロード＆インストール。

```shell
> swift --version
Swift version 6.2-dev (LLVM 5fbc818cf26c90b, Swift 2e1897356956e43)
Target: x86_64-unknown-windows-msvc
Build config: +assertions
```

# 注意点

## コンパイル方法

`swift build` ではまともにコンパイルできないため、`swiftc` を直接使ってコンパイルする。  
原因は調べてない。

```swift
@const func add(_ lhs: Int, _ rhs: Int) -> Int {
    return lhs + rhs
}

// swift build
@const let result = add(1, 2) // コンパイルエラー

// swiftc
@const let result = add(1, 2) // コンパイル成功
```

## コンパイラのクラッシュ

### 列挙体

```swift
// コンパイルエラー
enum Math {
    let pi = 3.14
}

// コンパイラがクラッシュした
enum Math {
    @const let pi = 3.14
}
@const x = Math.pi
```