#pragma once
#include "packet.h"

class CPacketHandlerSuper
{
private:
	void AddVector(CPacketHandlerSuper*);
public:
	static std::vector<CPacketHandlerSuper*> m_vecHandler;
	virtual E_PACKET_TYPE GetType() = 0;
	virtual void OnRecv(void* pData, size_t tDataSize) = 0;
	CPacketHandlerSuper()
	{
		AddVector(this);
	}
};

