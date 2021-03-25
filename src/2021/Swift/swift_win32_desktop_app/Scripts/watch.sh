#!/bin/bash

set -eu

readonly BUILD_DIR=$1
readonly BUILD_TEMP_FILE=${BUILD_DIR}/.building

handle_abort_signal() {
    echo "[watch] abort"
    exit 0
}

trap handle_abort_signal SIGINT
trap handle_abort_signal SIGQUIT
trap handle_abort_signal SIGTERM

echo "[watch] start"

# ゴミ掃除
if [[ -e "${BUILD_TEMP_FILE}" ]]; then
    rm "${BUILD_TEMP_FILE}"
fi

while true; do
    if [[ ! -e "${BUILD_TEMP_FILE}" ]]; then
        echo "[watch] build start"
        touch "${BUILD_TEMP_FILE}"
        make 2>&1 || true
        rm "${BUILD_TEMP_FILE}"
        echo "[watch] build finish"
    fi
    sleep 5
done;
