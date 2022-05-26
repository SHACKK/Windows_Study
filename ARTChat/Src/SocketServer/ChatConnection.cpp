#include "pch.h"
#include "ChatConnection.h"

void CChatConnection::onConnect()
{
	std::wstring strConnectedMessage = L"Accept";
	Send(strConnectedMessage);

	strUserName = Recv();

	m_pServer->UpdateChatData(L"---------------" + strUserName + L" ���� �����ϼ̽��ϴ�" + L"---------------");
	m_pServer->InsertConnectedSet(this);
	wprintf(L"%s", strUserName.c_str());

	// ���� ä�õ����� ����
	std::vector<std::wstring> vecChatData = m_pServer->GetChatData();
	SendChatData(vecChatData);
}

void CChatConnection::onRecv()
{
	while (true)
	{
		std::wstring strMessage = Recv();
		if (!wcscmp(strMessage.c_str(), CONNECTION_CLOSE_BY_CLIENT) || strMessage.empty()) // �ӽù������� �̷��� ����� �ߴµ�... �� �޸𸮰� Ƣ����???.
			break;

		m_pServer->UpdateChatData(strUserName + L" : " + strMessage);
		//m_pServer->BroadcastChatData();
	}
}

void CChatConnection::onClose()
{
	Send(CONNECTION_CLOSE_BY_SERVER);
	m_pServer->DisConnect(this);
	m_pServer->UpdateChatData(L"---------------" + strUserName + L" ���� �����ϼ̽��ϴ�" + L"---------------");
}


