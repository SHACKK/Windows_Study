#pragma once
#include "PacketHandlerSuper.h"

template <typename T>
class CPacketHandler : public CPacketHandlerSuper
{
	E_PACKET_TYPE GetType();
	void OnRecv(void* pData, size_t tDataSize);
	virtual void OnRecv(T* packet) = 0;
};

template<typename T>
inline E_PACKET_TYPE CPacketHandler<T>::GetType()
{
	T packet;
	return packet.m_eType;
}

template<typename T>
inline void CPacketHandler<T>::OnRecv(void* pData, size_t tDataSize)
{
	T* packet = (T*)pData;
	OnRecv(packet);
}
