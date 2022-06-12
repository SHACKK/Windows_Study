#include "packet.h"
#include "PacketHandlerSuper.h"

std::vector<CPacketHandlerSuper*> CPacketHandlerSuper::m_vecHandler;
void CPacketHandlerSuper::AddVector(CPacketHandlerSuper*)
{
	m_vecHandler.push_back(this);
}
