#include "pch.h"
#include "ChatConnection.h"

void CChatConnection::onConnect()
{
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
	std::wstring strCloseCommand = L"/CloseByClient";

	while (true)
	{
		std::wstring strMessage = Recv();
		if (!wcscmp(strMessage.c_str(), strCloseCommand.c_str()) || strMessage.empty()) // 임시방편으로 이렇게 막기는 했는데... 왜 메모리가 튀는지???.
			break;

		m_pServer->UpdateChatData(strUserName + L" : " + strMessage);
		//m_pServer->BroadcastChatData();
	}
}

void CChatConnection::onClose()
{
	std::wstring strCloseCommand = L"/CloseByServer";
	Send(strCloseCommand);
	m_pServer->DisConnect(this);
	m_pServer->UpdateChatData(L"---------------" + strUserName + L" 님이 퇴장하셨습니다" + L"---------------");
}


