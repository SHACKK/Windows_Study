#pragma once

struct PACKET_HEADER
{
	const char m_szMagic[4];
	const int m_nType;
	const int m_nLength;
	PACKET_HEADER(void);
	PACKET_HEADER(int nType, int nPacketSize)
		: m_szMagic{ 'S', 'H', 'A', 'K' }
		, m_nType(nType)
		, m_nLength(nPacketSize)
	{}
};

struct REQ_CONNECT : public PACKET_HEADER
{
	char m_szPass[20];
	REQ_CONNECT(char szPass[20])
		: PACKET_HEADER(1, sizeof(*this) - sizeof(PACKET_HEADER))
		, m_szPass{ szPass[20] }
	{}
};

struct REQ_FILE : public PACKET_HEADER
{
	char m_szFileName[100];
	REQ_FILE(char szFileName[100])
		: PACKET_HEADER(2, sizeof(*this) - sizeof(PACKET_HEADER))
		, m_szFileName{ szFileName[100], 0 }
	{}
};