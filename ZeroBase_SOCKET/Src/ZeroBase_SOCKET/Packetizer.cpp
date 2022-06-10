#include <Windows.h>
#include "Packetizer.h"
#include "packet.h"
#include "PacketHandlerSuper.h"
#include "PacketHandler.h"
#include "PacketHandler.cpp"

void CPacketizer::OnRecv(REQ_CONNECT* packet)
{
	std::string strUserName = packet->szUserID;
	printf("%s Connected\n", strUserName.c_str());
}

void CPacketizer::OnRecv(RES_ACCEPT* packet)
{
	printf("Connected\n");
}

void CPacketizer::OnRecv(REQ_MESSAGE_INPUT* packet)
{
	std::string strMessage = packet->szMessage;
	printf("Recved : %s\n", strMessage.c_str());
}

void CPacketizer::OnRecv(RES_MESSAGE* packet)
{
	std::string strMessage = packet->szMessage;
	printf("Server : %s\n", strMessage.c_str());
}

void CPacketizer::OnRecv(REQ_DISCONNECT* packet)
{
	printf("Disconnected\n");
}

void CPacketizer::ProcessPacket(void* pData, size_t tDataSize)
{
	PACKET_HEADER& header = *(PACKET_HEADER*)pData;
	std::vector<CPacketHandlerSuper*>::iterator iter;
	for (iter = m_vecHandler.begin(); iter != m_vecHandler.end(); iter++)
	{
		if (header.m_eType == (*iter)->GetType())
			(*iter)->OnRecv(pData, tDataSize);
	}
}

// 1. m_vecHandler 액세스가 모호합니다?
// 2. PacketHandler<>에서 GetType()함수 구현 오류
// 3. packetizer 안에서 recv를 하려는 순간 소켓이 필요함. -> 밖에서 받아서 들어오자니 받을 사이즈가 패킷마다 다름.