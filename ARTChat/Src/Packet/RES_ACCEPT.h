#pragma once
#include "PACKET_HEADER.h"

#define MAX_STRACCEPT_SIZE 20

#pragma pack(push, 1)
class RES_ACCEPT : public PACKET_HEADER
{
public:
	std::wstring strAccept;

	RES_ACCEPT()
	{
		m_nType = 2;
		strAccept.resize(MAX_STRACCEPT_SIZE);
		m_tSize = (int)strAccept.size() * (int)sizeof(wchar_t);
	};
	~RES_ACCEPT() {};
};
#pragma pack(pop)