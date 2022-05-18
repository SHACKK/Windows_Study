#pragma once
#include "PacketHeader.h"

class CMessagePacket : protected CPacketHeader
{
	CMessagePacket(DWORD dwSeq, DWORD dwSize) 
	{
		this->eType = E_PACKET_TYPE::MESSAGE;
		this->dwSeq = dwSeq;
		this->dwSize = dwSize;
	};
	~CMessagePacket() {};

	LPBYTE Packetizer();
	bool UnPacketizer();
	E_PACKET_TYPE GetPacketType();
};

