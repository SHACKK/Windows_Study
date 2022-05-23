#pragma once
#include "PacketHandlerSuper.h"

template <class T>
class CPacketHandler : public CPacketHandlerSuper
{
public:
	DWORD GetType();
	void OnRecv(void* pData, size_t tDataSize);
	virtual void OnRecv(T* packet) = 0;
};
