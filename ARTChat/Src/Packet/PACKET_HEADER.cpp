#include "pch.h"
#include "PACKET_HEADER.h"

const char* PACKET_HEADER::GetMagic()
{
	return &m_szMagic[0];
}
