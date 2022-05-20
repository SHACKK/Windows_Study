#include "pch.h"
#include "ChatConnection.h"

void CChatConnection::onConnect()
{
	// REQ_ACCEPT
	CResAccept resAcceptHeader;
	resAcceptHeader.tSize = 0;
	Send((LPBYTE)&resAcceptHeader, sizeof(resAcceptHeader));

	printf("Connection Established..!");

	// RES_CHAT_DATA
	CResChatData resChatDataheader;
	std::vector<std::string> resChatDataBody = this->m_pServer->GetChatData();

	resChatDataheader.tSize = sizeof(resChatDataBody);
	Send((LPBYTE)&resChatDataheader, sizeof(resChatDataheader));
	Send((LPBYTE)&resChatDataBody, resChatDataheader.tSize);
}

void CChatConnection::onRecv()
{
	//REQ_MESSAGE_INPUT
	CReqMessageInput reqMessageInputHeader;
	Recv((LPBYTE)&reqMessageInputHeader, sizeof(reqMessageInputHeader));
	std::string strMessage;
	strMessage.resize(reqMessageInputHeader.tSize);
	Recv((LPBYTE)&strMessage, reqMessageInputHeader.tSize);
	m_pServer->PushChatMessage(strMessage);
}

void CChatConnection::onClose()
{

}
