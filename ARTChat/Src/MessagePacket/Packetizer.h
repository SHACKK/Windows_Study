#pragma once
#include "PacketHandler.h"

class CPacketizer : public CPacketHandler
{
private:
	void OnRecv();
public:
	void ProcessPacket(void* pData, size_t tDataSize);
};