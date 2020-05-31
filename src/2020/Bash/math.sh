#!/bin/bash

# ビットシフトを書くと、VSCode拡張のエラーでエディタが真っ赤になってしまう・・・
# ということで自分で用意した
# が、 ** 演算子があったから要らなくなった
pow() {
    local b=$1
    local e=$2
    local r=1
    for ((i=0; i<e; i++)) do
        r=$((r * b))
    done
    echo "$r"
}

export -f pow
