#pragma once
#include "PacketHandler.h"

class CPacketizer : public CPacketHandler<REQ_CONNECT>,
					public CPacketHandler<RES_ACCEPT>,
					public CPacketHandler<REQ_MESSAGE_INPUT>,
					public CPacketHandler<RES_MESSAGE>,
					public CPacketHandler<REQ_DISCONNECT>
{
private:
	void OnRecv(REQ_CONNECT* packet);
	void OnRecv(RES_ACCEPT* packet);
	void OnRecv(REQ_MESSAGE_INPUT* packet);
	void OnRecv(RES_MESSAGE* packet);
	void OnRecv(REQ_DISCONNECT* packet);
public:
	void ProcessPacket(void* pData, size_t tDataSize);
};

