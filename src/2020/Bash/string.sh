#!/bin/bash

bin_to_dec_str() {
    local file=$1

    < "$file" od -t u1 --endian=big -An | \
        tr '\n' ' ' | \
        tr -s ' '
}

export -f bin_to_dec_str

u8x4_to_u32() {
    local arr=("$@")
    local sum=0
    for ((i=0; i < 4; i++)) do
        local e=$((i * 8))
        local v=${arr[$i]}
        sum=$(( sum + v * (2 ** e) ))
    done
    echo "$sum"
}

export -f u8x4_to_u32

substr() {
    local str=$1
    local start=$2
    local len=$3
    local end=0

    end=$((start + len))
    start=$((start + 1))
    echo "$str" | cut -d' ' -f "$start-$end"
}

export -f substr

trim_spaces() {
    local stdin=""
    read -r stdin
    echo "$stdin"
}

export -f trim_spaces
