#pragma once
#include<windows.h>
#include <iostream>
#define PACKET_HEADER_SIZE 10
#define PACKET_TLoginReq_Size	 40
#define PACKET_TLoginAck_Size	 4

//바이트 패킹, 네트워크를 통한 구조체 전송시 필수!
//구조체가 메모리에 정의되는 형태는 OS와 컴파일러에 따라 달라짐
//동일한 구조체를 서로 다르게 메모리에 정의하고 있는 시스템끼리 
//메모리에 있는 구조체 내용을 그대로 주고 받는다면 구조체의 각 멤버는 서로 다른값을 가지게 됨
#pragma pack(push, 1)
typedef struct
{
	//WORD = 2바이트
	WORD iotype;
	WORD len;
	WORD type;
	DWORD time;
}PACKET_HEADER;

typedef struct
{
	PACKET_HEADER ph;
	char msg[4096];
}UPACKET, *P_UPACKET;

struct ChatMsg
{
	int index;
	char name[20];
	//int damage;
	char message[256];
};
struct LoginReq
{
	char szID[20];
	char szPS[20];
};
struct LoginAck
{
	int iResult;// 1=승인, -1=ID없다. -2=PS없다.
};
#pragma pack(pop)

struct TSTATUS_VARS
{
	uint16_t	Point;				// 미분배 포인트.
	uint16_t	STR;				// 힘.
	uint16_t	INT;				// 지능.
	uint16_t	DEX;				// 민첩.
	uint16_t	CHA;				// 카리스마.
	uint16_t	Life;				// 현재 생명력. (힘 종속)
	uint16_t	Stamina;			// 현재 지구력. (힘 종속)
	int8_t		Status;				// 현재 상태 값.
	uint16_t	Skins[6];			// 현재 모습 상태.(0:머리, 1:얼굴)
	int8_t		Parts[4];			// 부위별 피해 상황. 
	int8_t		Level;				// 레벨.
	uint32_t	Exp;				// 경험치.
	uint32_t	Skills[16];			// 스킬 숙련도. (0->10,000)

};

enum TPACKET_TYPE
{
	PACKET_VERIFICATION_REQ = 1,
	PACKET_VERIFICATION_ACK,

	PACKET_CHAT_NAME_REQ = 1000, // 대화명 입력요청	
	PACKET_CHAT_NAME_ACK, // 대화명 입력 응답
	PACKET_CHAT_MSG, // 보통 채팅 메세지.
	PACKET_LOGIN_REQ, // 로그인요청
	PACKET_LOGOUT_REQ, // 로그아웃요청
	PACKET_LOGIN_ACK, // 로그인응답
	PACKET_LOGOUT_ACK, // 로그아웃응답
	PACKET_LOGOUT_PLAYER, // 로그아웃 플레이어

	PACKET_CREATE_ACCOUNT_REQ = 2000, // 계정생성요청
	PACKET_DELETE_ACCOUNT_REQ, // 계정삭제요청
	PACKET_CREATE_ACCOUNT_ACK, // 계정생성응답
	PACKET_DELETE_ACCOUNT_ACK, // 계정삭제응답

	PACKET_USER_POSITION = 3000, // 케릭터 위치 이동 시작
	PACKET_STOP_POSITION,// 케릭터 정지
	PACKET_LOBBY_USER_INFO, // 대기방 유저리스트 정보


	PACKET_CREATE_CHARACTER_REQ = 4000, // 케릭터 생성 요청
	PACKET_DELETE_CHARACTER_REQ, // 케릭터 삭제 요청
	PACKET_CREATE_CHARACTER_ACK, // 케릭터 생성 응답
	PACKET_DELETE_CHARACTER_ACK, // 케릭터 삭제 응답

	PACKET_HIT_CHARACTER = 5000, // 케릭터 피격 받음
	PACKET_HIT_NPC, // NPC 피격 받음
	PACKET_ATTACK_CHARACTER, // 케릭터 공격 시작
	PACKET_ATTACK_NPC, // NPC 공격 시작
	PACKET_DEAD_CHARACTER, // 케릭터 죽음
	PACKET_DEAD_NPC, // NPC 죽음
	PACKET_DAMAGE_CHARACTER, // 케릭터 데미지 받음
	PACKET_DAMAGE_NPC, // NPC 데미지 받음
	PACKET_SPAWN_CHARACTER, // 케릭터 스폰
	PACKET_SPAWN_NPC, // NPC 스폰
	PACKET_SPAWN_NPC_LIST, // 
	PACKET_SYNC_CHARACTER, // 케릭터 동기화
	PACKET_SYNC_NPC_LIST, // NPC 동기화	
	PACKET_NPC_TARGETMOVE,  // NPC 타켓이동	


	PACKET_ZONE_ENTRY_REQ = 6000,//게임존 입장 요청
	PACKET_ZONE_ENTRY_ACK, // 게임 존 입장 응답
	PACKET_ZONE_NEW_PLAYER, // 게임 존 입장 케릭터 정보 전송
	PACKET_ZONE_PLAYERS_INFO, // 게임 존에 입장한 유저들의 플레이 케릭터 정보 전송 
	PACKET_ZONE_USER_INFO,// 게임존 유저리스트 정보(플레이어+기타(옵저버 등))
	PACKET_ZONE_TARGET_REQ, // 존 타켓 위치 요청  
	PACKET_ZONE_TARGET_ACK, // 존 타켓 위치 응답
};