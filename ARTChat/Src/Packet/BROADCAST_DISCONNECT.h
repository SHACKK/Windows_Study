#pragma once
#include "PACKET_HEADER.h"

#define MAX_STRDISCON_SIZE 20

#pragma pack(push, 1)
class BROADCAST_DISCONNECT : public PACKET_HEADER
{
public:
	std::wstring strDiscon;
	BROADCAST_DISCONNECT()
	{
		m_nType = 8;
		strDiscon.resize(MAX_STRDISCON_SIZE);
		m_tSize = (int)strDiscon.size() * (int)sizeof(wchar_t);
	};
	~BROADCAST_DISCONNECT() {};
};
#pragma pack(pop)