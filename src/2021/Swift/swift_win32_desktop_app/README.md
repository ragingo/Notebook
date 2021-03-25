# ビルド関連

※ vscode 前提

## ビルド

`Ctrl + Shift + B`

## 実行

`F5`

## デバッグ

print 関数

(デバッガ使いたい...)  
(ステップ実行したい...)  
(変数見たい...)  

# 環境構築メモ

- Windows 10
- Visual Studio 2019
  - https://visualstudio.microsoft.com/ja/vs/
- Clang 11.0.0
  - https://releases.llvm.org/download.html
    - `Pre-Built Binaries`
- Swift 5.4
  - https://swift.org/download/#releases
    - `Swift 5.4 Development`
- Make 3.81
  - http://gnuwin32.sourceforge.net/packages/make.htm
- vcpkg
  - https://github.com/Microsoft/vcpkg
- libjpeg-turbo

# vcpkg setup

※ windows git bash を使う

vcpkg 導入

```sh
cd ./Tools
git clone git@github.com:microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
```

ライブラリ導入

```sh
vcpkg install libjpeg-turbo:x64-windows-static
```
