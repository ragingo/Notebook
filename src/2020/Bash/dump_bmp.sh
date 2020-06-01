#!/bin/bash

set -eu

cd "$(dirname "${BASH_SOURCE:-$0}")"
pwd

# shellcheck disable=SC1091
. ./array.sh
# shellcheck disable=SC1091
. ./math.sh
# shellcheck disable=SC1091
. ./string.sh

readonly SRC_FILE_PATH=./catman.bmp
readonly SRC_DATA=$(bin_to_dec_str "$SRC_FILE_PATH" | trim_spaces)

# https://docs.microsoft.com/ja-jp/windows/win32/api/wingdi/ns-wingdi-bitmapfileheader
readonly BITMAPFILEHEADER_SIZE=$((2 + 4 + 2 + 2 + 4))

# https://docs.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader
readonly BITMAPINFOHEADER_SIZE=$((4 + 4 + 4 + 2 + 2 + 4 + 4 + 4 + 4 + 4 + 4))


BITMAPFILEHEADER_DATA=$(substr "$SRC_DATA" 0 "$BITMAPFILEHEADER_SIZE")
readonly BITMAPFILEHEADER_DATA

__offset="$BITMAPFILEHEADER_SIZE"
BITMAPINFOHEADER_DATA=$(substr "$SRC_DATA" "$__offset" "$BITMAPINFOHEADER_SIZE")
readonly BITMAPINFOHEADER_DATA

__offset=$((__offset + BITMAPINFOHEADER_SIZE))
IMAGE_DATA=$(substr "$SRC_DATA" "$__offset" "")
readonly IMAGE_DATA


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
    local sizes=(2 4 2 2 4)
    local offsets=()
    local item_count=${#sizes[@]}

    calc_offsets sizes offsets

    for ((i=0; i < "$item_count"; i++)) do
        local val
        val=$(substr "$BITMAPFILEHEADER_DATA" "${offsets[$i]}" "${sizes[$i]}")

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
}

dump_bitmap_info_header() {
    local sizes=(4 4 4 2 2 4 4 4 4 4 4)
    local offsets=()
    local item_count=${#sizes[@]}

    calc_offsets sizes offsets

    for ((i=0; i < "$item_count"; i++)) do
        local val
        val=$(substr "$BITMAPINFOHEADER_DATA" "${offsets[$i]}" "${sizes[$i]}")

        case "$i" in
            0 )
                printf "size: %'d B\n" "$(u8x4_string_to_u32 "$val")"
                ;;
            1 )
                printf "width: %'d px\n" "$(u8x4_string_to_u32 "$val")"
                ;;
            2 )
                printf "height: %'d px\n" "$(u8x4_string_to_u32 "$val")"
                ;;
            3 )
                printf "planes: %d\n" "$(u8x2_string_to_u32 "$val")"
                ;;
            4 )
                printf "bitcount: %d\n" "$(u8x2_string_to_u32 "$val")"
                ;;
            5 )
                printf "compression: %d\n" "$(u8x4_string_to_u32 "$val")"
                ;;
            6 )
                printf "sizeimage: %'d B\n" "$(u8x4_string_to_u32 "$val")"
                ;;
            7 )
                printf "x px/m: %'d\n" "$(u8x4_string_to_u32 "$val")"
                ;;
            8 )
                printf "y px/m: %'d\n" "$(u8x4_string_to_u32 "$val")"
                ;;
            9 )
                printf "clrused: %'d\n" "$(u8x4_string_to_u32 "$val")"
                ;;
            10 )
                printf "cirimportant: %'d\n" "$(u8x4_string_to_u32 "$val")"
                ;;
        esac
    done
}

draw_image() {
    # 引数で指定できるようにする
    local w=32
    local h=32
    local t=80
    local src_img=()
    local dst_img=()

    split src_img "$IMAGE_DATA"
    array_fill dst_img $((w * h)) "0"

    for ((row = 0; row < h; row++)) do
        for ((col = 0; col < w; col++)) do
            local pos=$((h * row + col))
            local v=${src_img[$pos]}

            if [ "$v" -gt "$t" ]; then
                dst_img[$pos]=255
            fi
        done
    done

    echo "converted."

    # for ((row = 0; row < h; row++)) do
    #     for ((col = 0; col < w; col++)) do
    #         local pos=$((h * row + col))
    #         local v=${dst_img[$pos]}
    #         printf "%s" "$v"
    #     done
    #     echo ""
    # done

    local bfh=()
    split bfh "$BITMAPFILEHEADER_DATA"
    array_map bfh dec_to_bin

    local bif=()
    split bif "$BITMAPINFOHEADER_DATA"
    array_map bif dec_to_bin

    array_map dst_img dec_to_bin

    {
        echo -en "${bfh[@]}" | tr -d ' '
        echo -en "${bif[@]}" | tr -d ' '
        echo -en "${dst_img[@]}" | tr -d ' '
    } > ./a.bmp
}

echo "===== BITMAPFILEHEADER ====="
dump_bitmap_file_header

echo "===== BITMAPINFOHEADER ====="
dump_bitmap_info_header

echo "===== IMAGE ====="
draw_image

test1() {
    echo "$BITMAPFILEHEADER_DATA"

    local hex=4d
    echo "$hex"

    local dec
    dec="$(hex_to_dec "$hex")"
    echo "$dec"

    local hex2
    hex2="$(dec_to_hex "$dec")"
    echo "$hex2"

    local bin
    bin="$(dec_to_bin "$dec")"
    echo -en "$bin" | od -c

    local a=(66 77)
    local b
    b=$(array_map a dec_to_bin)
    echo -en "${b[@]}" | od -c
}

# test1
