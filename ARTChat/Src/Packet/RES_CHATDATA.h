#pragma once
#include "PACKET_HEADER.h"

#define MAX_BROADCAST_CHATDATA_SIZE 20

#pragma pack(push, 1)
class RES_CHATDATA : public PACKET_HEADER
{
public:
	std::vector<std::wstring> vecChatData;
	RES_CHATDATA()
	{
		m_nType = 5;
		vecChatData.resize(MAX_BROADCAST_CHATDATA_SIZE);
		m_tSize = 400;
	};
	~RES_CHATDATA() {};
};
#pragma pack(pop)
