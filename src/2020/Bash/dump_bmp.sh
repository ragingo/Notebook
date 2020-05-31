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
