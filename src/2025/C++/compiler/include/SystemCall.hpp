#pragma once
#include <cstdint>

enum class SystemCall: uint16_t {
    READ      = 0,
    WRITE     = 1,
    OPEN      = 2,
    NEWFSTAT  = 5,
    LSEEK     = 8,
    NANOSLEEP = 35,
    EXIT      = 60,
    CREAT     = 85
};
