#pragma once
#include "PacketHeader.h"

class CConnectionPacket : protected CPacketHeader
{
private:

	CConnectionPacket(DWORD dwSeq, DWORD dwSize) 
	{
		this->eType = E_PACKET_TYPE::CONNECTION;
		this->dwSeq = dwSeq;
		this->dwSize = dwSize;
	};
	~CConnectionPacket() {};

	LPBYTE Packetizer();
	bool UnPacketizer();
	E_PACKET_TYPE GetPacketType();
};

