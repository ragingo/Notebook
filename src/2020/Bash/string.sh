#!/bin/bash

split() {
    local -n arr=$1
    local str=$2
    arr=(${str// / })
}

export -f split

i_to_c() {
    local i=$1
    echo "$i" | awk '{ print("%c", $1) }'
}

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

u8x4_string_to_u32() {
    local str=$1
    local x=()
    split x "$str"
    local y=0
    y=$(u8x4_to_u32 "${x[@]}")
    echo "$y"
}

export -f u8x4_string_to_u32
