#pragma once
#include <vector>
#include <string>

#define MAX_STRING_SIZE 20

enum class E_PACKET_TYPE
{
	REQ_CONNECT,
	RES_ACCEPT,
	RES_CHATDATA,
	REQ_MESSAGEINPUT,
	RES_MESSAGE,
	REQ_DISCONNECT
};

struct PACKET_HEADER
{
	const char m_szMagic[4];
	static const E_PACKET_TYPE m_eType;
	const int m_nSize;

	PACKET_HEADER(void);
	PACKET_HEADER(int nPacketSize)
		: m_szMagic{ 'S', 'H', 'A', 'K' }
		, m_nSize(nPacketSize)
	{}
};
//외부에서 선언되어야 한다?

struct REQ_CONNECT : public PACKET_HEADER
{
	static const E_PACKET_TYPE m_eType = E_PACKET_TYPE::REQ_CONNECT;
	char szUserID[8];
	REQ_CONNECT(E_PACKET_TYPE ePacketType)
		:PACKET_HEADER(sizeof(*this) - sizeof(PACKET_HEADER))
		, szUserID{ 0, }
		, PACKET_HEADER::m_eType = ePacketType
	{}
};

struct RES_ACCEPT : public PACKET_HEADER
{
public:
	char szAccept[8];
	RES_ACCEPT(void)
		: PACKET_HEADER(E_PACKET_TYPE::RES_ACCEPT, sizeof(*this) - sizeof(PACKET_HEADER))
		, szAccept{ '@','A','C','C','E','P','T','@' }
	{}
};

struct REQ_MESSAGE_INPUT : public PACKET_HEADER
{
	char szMessage[20];

	REQ_MESSAGE_INPUT(void)
		: PACKET_HEADER(E_PACKET_TYPE::REQ_MESSAGEINPUT, sizeof(*this) - sizeof(PACKET_HEADER))
		, szMessage{ 0, }
	{}
};

struct RES_MESSAGE : public PACKET_HEADER
{
	char szMessage[20];
	RES_MESSAGE(void)
		:PACKET_HEADER(E_PACKET_TYPE::RES_MESSAGE, sizeof(*this) - sizeof(PACKET_HEADER))
		, szMessage{ 0, }
	{}
};

struct REQ_DISCONNECT : public PACKET_HEADER
{
	char szDiscon[8];
	REQ_DISCONNECT(void)
		:PACKET_HEADER(E_PACKET_TYPE::REQ_DISCONNECT, sizeof(*this) - sizeof(PACKET_HEADER))
		, szDiscon{ '@','D','I','S','C','O','N','@' }
	{}
};