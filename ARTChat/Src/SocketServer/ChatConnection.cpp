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
	std::wstring strCloseCommand = L"/CloseByClient";

	while (true)
	{
		std::wstring strMessage = Recv();
		if (!wcscmp(strMessage.c_str(), strCloseCommand.c_str()) || strMessage.empty()) // �ӽù������� �̷��� ����� �ߴµ�... �� �޸𸮰� Ƣ����???.
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
	m_pServer->UpdateChatData(L"---------------" + strUserName + L" ���� �����ϼ̽��ϴ�" + L"---------------");
}


