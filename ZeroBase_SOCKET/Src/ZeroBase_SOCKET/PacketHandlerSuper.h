#pragma once
#include <vector>

class CPacketHandlerSuper
{
public:
	static std::vector<CPacketHandlerSuper*> m_vecHandler;
	virtual E_PACKET_TYPE GetType();
	virtual void OnRecv(void* pData, size_t tDataSize);
	CPacketHandlerSuper()
	{
		m_vecHandler.push_back(this);
	}
};

