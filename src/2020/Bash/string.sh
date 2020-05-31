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
    if [ "$len" = "" ]; then
        end=""
    else
        end=$((start + len))
    fi
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

u8xn_to_u32() {
    local n=$1
    shift
    local arr=("$@")
    local sum=0
    for ((i=0; i < n; i++)) do
        local e=$((i * 8))
        local v=${arr[$i]}
        sum=$(( sum + v * (2 ** e) ))
    done
    echo "$sum"
}

export -f u8xn_to_u32

u8x2_to_u32() {
    u8xn_to_u32 2 "$@"
}

export -f u8x2_to_u32

u8x4_to_u32() {
    u8xn_to_u32 4 "$@"
}

export -f u8x4_to_u32

u8xn_string_to_u32() {
    local n=$1
    local str=$2
    local x=()
    split x "$str"
    local y
    y=$(u8xn_to_u32 "$n" "${x[@]}")
    echo "$y"
}

export -f u8xn_string_to_u32

u8x2_string_to_u32() {
    local str=$1
    local x
    x=$(u8xn_string_to_u32 2 "$str")
    echo "$x"
}

export -f u8x2_string_to_u32

u8x4_string_to_u32() {
    local str=$1
    local x
    x=$(u8xn_string_to_u32 4 "$str")
    echo "$x"
}

export -f u8x4_string_to_u32
