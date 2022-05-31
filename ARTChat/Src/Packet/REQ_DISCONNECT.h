#pragma once
#include "PACKET_HEADER.h"

#define MAX_STRDISCON_SIZE 20

#pragma pack(push, 1)
class REQ_DISCONNECT : public PACKET_HEADER
{
public:
	std::wstring strDiscon;
	REQ_DISCONNECT()
	{
		m_nType = 6;
		strDiscon.resize(MAX_STRDISCON_SIZE);
		m_tSize = (int)strDiscon.size() * (int)sizeof(wchar_t);
	};
	~REQ_DISCONNECT() {};
};
#pragma pack(pop)
