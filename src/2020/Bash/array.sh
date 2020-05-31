#!/bin/bash

#######################################
# 渡された配列を指定されたサイズにし、
# 指定された値で埋める
#
# Globals:
#   None
# Arguments:
#   $1: array
#   $2: array size
#   $3: value
# Outputs:
#   $1
#######################################
array_fill() {
    local -n arr=$1
    local size=$2
    local val=$3
    for _ in $(seq 1 "$size"); do
        arr+=("$val")
    done
}

export -f array_fill

#######################################
# 配列の要素数をカウント
#
# Globals:
#   None
# Arguments:
#   $1: array
# Outputs:
#   要素数
#######################################
array_count() {
    local -n arr=$1
    "${#arr[@]]}"
}

export -f array_count
