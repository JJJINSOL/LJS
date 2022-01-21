#include "Message.h"
int Message::RecvMsg(char* recvbuffer, int recvbyte)
{
	memcpy(&m_recvBuffer[m_writePos], recvbuffer, recvbyte);
	m_writePos += recvbyte;
	m_readPos += recvbyte;

	if()
	return 1;
}
int Message:: SendMsg(SOCKET sock, UPACKET& packet)
{
	return 1;
}
Message:: Message()
{

}
Message:: ~Message()
{

}