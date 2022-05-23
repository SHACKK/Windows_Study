#pragma once
#include "PacketHandler.h"

class CPacketizer : public CPacketHandler<CReqConnection>,
					public CPacketHandler<CResAccept>,
					public CPacketHandler<CResWait>, 
					public CPacketHandler<CReqMessageInput>, 
					public CPacketHandler<CResChatData>,
					public CPacketHandler<CReqDisconnection>,
					public CPacketHandler<BroadcastChatData>,
					public CPacketHandler<BroadcastDisconnection>
{
private:
	void OnRecv(CReqConnection packet);
	void OnRecv(CResAccept packet);
	void OnRecv(CResWait packet);
	void OnRecv(CReqMessageInput packet);
	void OnRecv(CResChatData packet);
	void OnRecv(CReqDisconnection packet);
	void OnRecv(BroadcastChatData packet);
	void OnRecv(BroadcastDisconnection packet);
public:
	void ProcessPacket(void* pData, size_t tDataSize);
};