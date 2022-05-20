#include "pch.h"
#include "ChatConnection.h"

void CChatConnection::onConnect()
{
	// REQ_ACCEPT
	CPacketHeader header;
	header.tSize = 0;
	Send((LPBYTE)&header, sizeof(header));

	printf("Connection Established..!");

	// RES_CHAT_DATA
	CPacketHeader header2;
	std::vector<std::string> body = this->m_pServer->GetChatData();

	header.tSize = sizeof(body);
	Send((LPBYTE)&header, sizeof(header));
	Send((LPBYTE)&body, header.tSize);
}

void CChatConnection::onRecv()
{
	CPacketHeader header;
	Recv((LPBYTE)&header, sizeof(header));
	CPacketHandler body;

	m_pServer->PushChatMessage();
}

void CChatConnection::onClose()
{
}
