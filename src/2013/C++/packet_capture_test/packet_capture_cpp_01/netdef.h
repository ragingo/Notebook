#pragma once
#include <stdint.h>

#pragma pack(1)
struct IPv4Header
{
	uint8_t  HeaderLength : 4;
	uint8_t  Version : 4;
	uint8_t  TypeOfService;
	uint16_t TotalLength;
	uint16_t ID;
	uint16_t FlagsAndFragmentOffset;
	uint8_t  TTL;
	uint8_t  Protocol;
	uint16_t Checksum;
	uint32_t SrcAddr;
	uint32_t DestAddr;
	uint32_t Option;
	uint32_t Data;
};
#pragma pack()
