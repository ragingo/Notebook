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
    local data=()
    local sizes=(2 4 2 2 4)
    local offsets=()
    local item_count=${#sizes[@]}

    split data "$BITMAPFILEHEADER_DATA"
    calc_offsets sizes offsets

    for ((i=0; i < "$item_count"; i++)) do
        local idx="${offsets[$i]}"
        local len="${sizes[$i]}"
        local val="${data[*]:$idx:$len}"

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
    local data=()
    local sizes=(4 4 4 2 2 4 4 4 4 4 4)
    local offsets=()
    local item_count=${#sizes[@]}

    split data "$BITMAPINFOHEADER_DATA"
    calc_offsets sizes offsets

    for ((i=0; i < "$item_count"; i++)) do
        local idx="${offsets[$i]}"
        local len="${sizes[$i]}"
        local val="${data[*]:$idx:$len}"

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

export_image() {
    # 引数で指定できるようにする
    local w=32  # width
    local h=32  # height
    local t=100 # threshold
    local d=3   # depth
    local src_img=()
    local dst_img=()

    split src_img "$IMAGE_DATA"
    array_fill dst_img $((w * h * d)) "\x0"

    for ((i = 0; i < $((w * h * d)); i+=3)) do
        local r=${src_img[$((i + 0))]}
        local g=${src_img[$((i + 1))]}
        local b=${src_img[$((i + 2))]}

        if [ "$r" -lt "$t" ] || \
           [ "$g" -lt "$t" ] || \
           [ "$b" -lt "$t" ]; then
            dst_img[$((i + 0))]="\xff"
            dst_img[$((i + 1))]="\xff"
            dst_img[$((i + 2))]="\xff"
        fi
    done

    local bfh=()
    split bfh "$BITMAPFILEHEADER_DATA"
    array_map bfh dec_to_bin

    local bif=()
    split bif "$BITMAPINFOHEADER_DATA"
    array_map bif dec_to_bin

    local IFS=""
    {
        echo -en "${bfh[*]}"
        echo -en "${bif[*]}"
        echo -en "${dst_img[*]}"
    } > ./a.bmp
}

echo "===== BITMAPFILEHEADER ====="
dump_bitmap_file_header

echo "===== BITMAPINFOHEADER ====="
dump_bitmap_info_header

echo "===== IMAGE ====="
export_image
