#pragma once
#include <windows.h>
#define PACKET_HEADER_SIZE 4
#define PACKET_CHAT_MSG 1000
#pragma pack(push,1)
typedef struct {
	WORD len;
	WORD type;
}PACKET_HEADER;

typedef struct {
	PACKET_HEADER p_header;
	char msg[2048];
}UPACKET;

typedef struct {
	char name[20];
	char msg[2048];
}CahtMSG;

#pragma pack(pop)