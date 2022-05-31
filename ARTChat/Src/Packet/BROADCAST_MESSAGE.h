#pragma once
#include "PACKET_HEADER.h"

#define MAX_STRMESSAGE_SIZE 20

#pragma pack(push, 1)
class BROADCAST_MESSAGE : public PACKET_HEADER
{
public:
	std::wstring strMessage;
	BROADCAST_MESSAGE() 
	{
		m_nType = 7;
		strMessage.resize(MAX_STRMESSAGE_SIZE);
		m_tSize = (int)strMessage.size() * (int)sizeof(wchar_t);
	};
	~BROADCAST_MESSAGE() {};
};
#pragma pack(pop)
