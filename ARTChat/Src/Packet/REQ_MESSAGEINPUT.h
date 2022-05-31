#pragma once
#include "PACKET_HEADER.h"

#define MAX_MESSAGE_SIZE 20

#pragma pack(push, 1)
class REQ_MESSAGEINPUT : public PACKET_HEADER
{
private:
	std::wstring strMessage;
	REQ_MESSAGEINPUT()
	{
		m_nType = 4;
		strMessage.resize(20);
		m_tSize = (int)strMessage.size() * (int)sizeof(wchar_t);
	};
	~REQ_MESSAGEINPUT() {};
};
#pragma pack(pop)
