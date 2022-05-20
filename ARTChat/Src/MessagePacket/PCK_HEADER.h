#pragma once
class CPacketHeader
{
protected:
	DWORD dwMagic;
	DWORD dwType;
public:
	size_t tSize;
	bool MagicOK();
};

