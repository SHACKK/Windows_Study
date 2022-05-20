#include "pch.h"
#include "PCK_HEADER.h"

bool CPacketHeader::MagicOK()
{
	if (dwMagic == 92288534)
		return true;

	return false;
}
