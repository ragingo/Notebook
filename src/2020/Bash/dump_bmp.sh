#!/bin/bash

set -eu

cd "$(dirname "$0")"
pwd

# shellcheck disable=SC1091
. ./array.sh
# shellcheck disable=SC1091
. ./math.sh
# shellcheck disable=SC1091
. ./string.sh

readonly SRC_FILE_PATH=./catman.bmp
readonly SRC_DATA=$(bin_to_dec_str "$SRC_FILE_PATH" | trim_spaces)

calc_offsets() {
    local -n _sizes=$1
    local -n _offsets=$2
    local item_count=${#_sizes[@]}

    array_fill offsets "$item_count" 0

    for ((i=0; i < "$item_count"; i++)) do
        if [ $i -gt 0 ]; then
            local prev_offset=${_offsets[$((i - 1))]}
            local prev_size=${_sizes[$((i - 1))]}
            local offset=$((prev_offset + prev_size))
            offsets[$i]=$offset
        else
            offsets[$i]=0
        fi
    done
}

dump_bitmap_file_header() {
    local data=$1
    local sizes=(2 4 2 2 4)
    local offsets=()
    local item_count=${#sizes[@]}

    calc_offsets sizes offsets

    local ret=()
    for ((i=0; i < "$item_count"; i++)) do
        local val
        val=$(substr "$data" "${offsets[$i]}" "${sizes[$i]}")

        ret+=("$val")
        # ↓ これでも配列に追加できる(標準出力を追加したい場合はこれじゃないとだめそう)
        # mapfile -t -O "${#ret[@]}" ret < <(echo "$val")

        case "$i" in
            0 )
                echo "$val" | awk '{ printf("type: %c%c\n", $1, $2) }'
                ;;
            1 )
                printf "size: %'d B\n" "$(u8x4_string_to_u32 "$val")"
                ;;
            2 | 3)
                echo "reserved"
                ;;
            4 )
                printf "offbits: %'d B\n" "$(u8x4_string_to_u32 "$val")"
                ;;
        esac
    done

    echo "${ret[@]}"
}

# debug dump
# echo "$SRC_DATA"

dump_bitmap_file_header "$SRC_DATA"
