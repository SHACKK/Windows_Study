#pragma once

enum class E_PACKET_TYPE
{
	CONNECTION,
	MESSAGE
};

class CPacketHeader
{
public:
protected:
	E_PACKET_TYPE eType;
	DWORD dwSeq;
	DWORD dwSize;
	virtual LPBYTE Packetizer() = 0;
	virtual bool UnPacketizer() = 0;
	virtual E_PACKET_TYPE GetPacketType() = 0;
};

