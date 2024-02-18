#!/bin/bash -e

make > /dev/null
gdb ./build/test1
