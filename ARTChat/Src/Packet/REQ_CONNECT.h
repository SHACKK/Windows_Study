#pragma once
#include "PACKET_HEADER.h"

#define MAX_USERNAME_SIZE 10

#pragma pack(push, 1)
class REQ_CONNECT : public PACKET_HEADER
{
public:
	REQ_CONNECT(void)
	{
		m_nType = 1;
		strUserName.resize(MAX_USERNAME_SIZE);
		m_tSize = (int)strUserName.size() * (int)sizeof(wchar_t);
	};
	~REQ_CONNECT() {};

	std::wstring strUserName;
};
#pragma pack(pop)