#include "pch.h"
#include "ChatConnection.h"

void CChatConnection::onConnect()
{
	std::wstring strMessage = L"";
	strMessage = Recv();
	strMessage += L"���� �����ϼ̽��ϴ�";

	m_pServer->UpdateChatData(strMessage);
	m_pServer->InsertConnectedSet(this);

	// ���� ä�õ����� ����
	std::vector<std::wstring> vecChatData = m_pServer->GetChatData();
	SendChatData(vecChatData);
}

void CChatConnection::onRecv()
{
	std::wstring strCloseCommand = L"/logout";

	while (true)
	{
		std::wstring strMessage = Recv();
		if (!wcscmp(strMessage.c_str(), strCloseCommand.c_str()))
			break;

		m_pServer->UpdateChatData(strMessage);
		//m_pServer->BroadcastChatData();
	}
}

void CChatConnection::onClose()
{
	std::wstring strMessage = L"exit";
	//m_pServer->PushChatMessage(strMessage);
	m_pServer->BroadcastChatData();
}


