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
		{
			(*iter)->OnRecv(pData, tDataSize);
			break;
		}
	}
}