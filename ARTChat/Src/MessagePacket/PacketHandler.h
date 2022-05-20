#pragma once
#include "PacketHandlerSuper.h"

template <typename T>
class CPacketHandler : public CPacketHandlerSuper
{
public:
	DWORD GetType();
	void OnRecv(void* pData, size_t tDataSize);
	virtual void OnRecv(T* packek) = 0;
};
