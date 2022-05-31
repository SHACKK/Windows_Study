#pragma once

#pragma pack(push, 1)
class PACKET_HEADER
{
protected:
	const char m_szMagic[4] = { 'S', 'H', 'A', 'K' };
	int m_nType;
	int m_tSize;
public:
	const char* GetMagic();
	PACKET_HEADER() {};
	/*CPacketHeader(int nType, int tSize) 
	{
		m_nType = nType;
		m_tSize = tSize;
	};*/
	~PACKET_HEADER() {};
};
#pragma pack(pop)