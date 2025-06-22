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

## Linux

https://www.swift.org/install/linux を参考に Swiftly をインストール。

```shell
# Swift 6.2 をインストール
$ swiftly install 6.2-snapshot

# `6.2-snapshot-YYYY-MM-DD` があるのを確認
$ swiftly list

# `6.2-snapshot-YYYY-MM-DD` を default に変更
$ swiftly use 6.2-snapshot-YYYY-MM-DD

$ swift --version
Swift version 6.2-dev (LLVM 5fbc818cf26c90b, Swift 2e1897356956e43)
Target: x86_64-unknown-linux-gnu
Build config: +assertions
```

# 注意点

## コンパイル方法

`swift build` ではまともにコンパイルできないため、`swiftc` を直接使ってコンパイルする。  

>[!NOTE]
`swift build` では必ず `-emit-module` が使われるみたい。  
`swiftc` に `-emit-module` を指定するとコンパイルエラーになる。

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

## `@const` 関数B内で `@const` 関数Aを呼んだら、関数Bの値を取り出せない

```swift
// コンパイルエラーになる
@const func a() -> Int { 1 }
@const func b() -> Int { a() + 2 }
@const let c = b() // ここでコンパイルエラー

// コンパイルエラーにならない
@const func a() -> Int { 1 }
@const func b() -> Int { 2 } // a() を呼ばない
@const let c = b()
```

再帰呼び出しもできない、`@const var` もできない、ループもできない。  
現時点では `sum` 関数すら作ることはできなそう。
