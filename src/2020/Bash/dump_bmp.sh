#!/bin/bash

set -eu

bin_to_dec_str() {
    local file=$1

    < "$file" od -t u1 --endian=big -An | \
        tr '\n' ' ' | \
        tr -s ' '
}

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

substr() {
    local str=$1
    local start=$2
    local len=$3
    local end=0

    end=$((start + len))
    start=$((start + 1))
    echo "$str" | cut -d' ' -f "$start-$end"
}

trim_spaces() {
    local stdin=""
    read -r stdin
    echo "$stdin"
}

array_fill() {
    local -n arr=$1
    local size=$2
    local val=$3
    for i in $(seq 1 "$size"); do
        arr+=("$val")
    done
}

array_count() {
    local -n arr=$1
    "${#arr[@]]}"
}

readonly SRC_FILE_PATH=./catman.bmp
readonly SRC_DATA=$(bin_to_dec_str "$SRC_FILE_PATH" | trim_spaces)

dump_bitmap_file_header() {
    local data=$1
    local sizes=(2 4 2 2 4)
    local offsets=()
    local item_count=${#sizes[@]}

    array_fill offsets "$item_count" 0

    for ((i=0; i < "$item_count"; i++)) do
        if [ $i -gt 0 ]; then
            local prev_offset=${offsets[$((i - 1))]}
            local prev_size=${sizes[$((i - 1))]}
            local offset=$((prev_offset + prev_size))
            # echo "prev offset: $prev_offset, prev size: $prev_size, offset: $offset"
            offsets[$i]=$offset
        else
            offsets[$i]=0
        fi
    done

    # echo "${offsets[@]}"

    local ret=()
    for ((i=0; i < "$item_count"; i++)) do
        local start=${offsets[$i]}
        local len=${sizes[$i]}
        local val=""
        val=$(substr "$data" "$start" "$len")

        # echo "start: $start, len: $len, val: $val"

        ret+=("$val")
        # ↓ これでも配列に追加できる(標準出力を追加したい場合はこれじゃないとだめそう)
        # mapfile -t -O "${#ret[@]}" ret < <(echo "$val")

        case "$i" in
            0 )
                echo "$val" | awk '{ printf("type: %c%c\n", $1, $2) }'
                ;;
            1 )
                local x=(${val// / })
                local y=0
                y=$(u8x4_to_u32 "${x[@]}")
                printf "size: %'d B\n" "$y"
                ;;
            2 | 3)
                echo "reserved"
                ;;
            4 )
                echo "offbits"
                local x=(${val// / })
                local y=0
                y=$(u8x4_to_u32 "${x[@]}")
                printf "offbits: %'d B\n" "$y"
                ;;
        esac
    done

    echo "${ret[@]}"
}

# debug dump
# echo "$SRC_DATA"

dump_bitmap_file_header "$SRC_DATA"
