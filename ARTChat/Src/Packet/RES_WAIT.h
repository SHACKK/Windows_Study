#pragma once
#include "PACKET_HEADER.h"

#pragma pack(push, 1)
class RES_WAIT : public PACKET_HEADER
{
public:
	unsigned int nWaittingNum;
	RES_WAIT()
	{
		nWaittingNum = 0;
		m_nType = 3;
		m_tSize = (int)sizeof(*this) - (int)sizeof(PACKET_HEADER);
	};
	~RES_WAIT() {};
};
#pragma pack(pop)