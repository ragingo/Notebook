#!/bin/bash -eu

if [[ ! -e build ]]; then
    mkdir build
fi

pushd build > /dev/null
rm -rf ./*

cmake -G Ninja ../
ninja

popd > /dev/null
