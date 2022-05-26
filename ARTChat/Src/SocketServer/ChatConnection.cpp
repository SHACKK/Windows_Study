#include "pch.h"
#include "ChatConnection.h"

void CChatConnection::onConnect()
{
	std::wstring strConnectedMessage = L"Accept";
	Send(strConnectedMessage);

	strUserName = Recv();

	m_pServer->UpdateChatData(L"---------------" + strUserName + L" 님이 입장하셨습니다" + L"---------------");
	m_pServer->InsertConnectedSet(this);
	wprintf(L"%s", strUserName.c_str());

	// 최초 채팅데이터 전송
	std::vector<std::wstring> vecChatData = m_pServer->GetChatData();
	SendChatData(vecChatData);
}

void CChatConnection::onRecv()
{
	while (true)
	{
		std::wstring strMessage = Recv();
		if (!wcscmp(strMessage.c_str(), CONNECTION_CLOSE_BY_CLIENT) || strMessage.empty()) // 임시방편으로 이렇게 막기는 했는데... 왜 메모리가 튀는지???.
			break;

		m_pServer->UpdateChatData(strUserName + L" : " + strMessage);
		//m_pServer->BroadcastChatData();
	}
}

void CChatConnection::onClose()
{
	Send(CONNECTION_CLOSE_BY_SERVER);
	m_pServer->DisConnect(this);
	m_pServer->UpdateChatData(L"---------------" + strUserName + L" 님이 퇴장하셨습니다" + L"---------------");
}


