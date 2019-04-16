
#pragma once
#include <winsock2.h>

#pragma pack(push, 1)
typedef struct _tagIPHeader
{
#ifdef BIGENDIAN
	unsigned char header_length : 4;
	unsigned char version : 4;
	unsigned char tos_reserved : 2;
	unsigned char tos_reliability : 1;
	unsigned char tos_throughput : 1;
	unsigned char tos_delay : 1;
	unsigned char tos_precedence : 3;
	unsigned short total_length;
	unsigned short identification;
	unsigned short offset : 13;
	unsigned short flag_moreflagment : 1;
	unsigned short flag_dontflagment : 1;
	unsigned short flag_reserved : 1;
	unsigned char ttl;
	unsigned char protocol;
	unsigned short checksum;
	unsigned int source_address;
	unsigned int destination_address;
	unsigned char *option;
#else if LITTLEENDIAN
	unsigned char version : 4;
	unsigned char header_length : 4;
	unsigned char tos_precedence : 3;
	unsigned char tos_delay : 1;
	unsigned char tos_throughput : 1;
	unsigned char tos_reliability : 1;
	unsigned char tos_reserved : 2;
	unsigned short total_length;
	unsigned short identification;
	unsigned short flag_reserved : 1;
	unsigned short flag_dontflagment : 1;
	unsigned short flag_moreflagment : 1;
	unsigned short offset : 13;
	unsigned char ttl;
	unsigned char protocol;
	unsigned short checksum;
	unsigned int source_address;
	unsigned int destination_address;
	unsigned char *option;
#endif
} IPHeader, *LPIPHeader;
#pragma pack(pop)