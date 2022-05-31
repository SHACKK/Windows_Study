#include "pch.h"
#include "ChatConnection.h"

void CChatConnection::onConnect()
{
	std::wstring strConnectedMessage = L"Accept";
	Send(strConnectedMessage);

	strUserName = Recv();
	if (strUserName.empty())
	{
		strUserName = L"[대기 손님]";
		Send(CONNECTION_CLOSE_BY_CLIENT);
		return;
	}

	m_pServer->UpdateChatData(L"---------------" + strUserName + L" 님이 입장하셨습니다" + L"---------------");
	m_pServer->InsertConnectedSet(this);
	//wprintf(L"%s", strUserName.c_str());

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

		m_pServer->UpdateChatData(GetCurrentTimeString() + strUserName + L" : " + strMessage);
	}
}

void CChatConnection::onClose()
{
	Send(CONNECTION_CLOSE_BY_SERVER);
	m_pServer->DisConnect(this);

	if (!strUserName.empty())
		m_pServer->UpdateChatData(L"---------------" + strUserName + L" 님이 퇴장하셨습니다" + L"---------------");

}

std::wstring CChatConnection::Recv()
{
	try
	{
		size_t nLength = 0;
		int nRecved = ::recv(m_ConnectionSocket, (char*)&nLength, (int)sizeof(nLength), 0);

		std::wstring strMsg;
		strMsg.resize(nLength / sizeof(wchar_t));
		::recv(m_ConnectionSocket, (char*)strMsg.c_str(), (int)nLength, 0);

		return strMsg;
	}
	catch (...)
	{
		printf("[Recv Error] : %d\n", WSAGetLastError());
	}
}

void CChatConnection::Send(std::wstring strMessage)
{
	try
	{
		size_t nLength = strMessage.length() * sizeof(wchar_t);
		::send(m_ConnectionSocket, (const char*)&nLength, sizeof(nLength), 0);
		::send(m_ConnectionSocket, (const char*)strMessage.c_str(), (int)nLength, 0);
	}
	catch (...)
	{
		printf("[Send Error] : %d\n", WSAGetLastError());
	}
}

int CChatConnection::SendChatData(std::vector<std::wstring> vecChatData)
{
	size_t nVecSize = vecChatData.size();
	::send(m_ConnectionSocket, (const char*)&nVecSize, sizeof(size_t), 0);

	for (size_t i = 0; i < nVecSize; i++)
	{
		int nMsgLength = (int)vecChatData[i].size() * (int)sizeof(wchar_t);
		::send(m_ConnectionSocket, (const char*)&nMsgLength, sizeof(nMsgLength), 0);
		::send(m_ConnectionSocket, (const char*)vecChatData[i].c_str(), nMsgLength, 0);
	}
	return 0;
}

std::wstring CChatConnection::GetCurrentTimeString()
{
	time_t curTime = time(NULL);
	tm pTime;
	localtime_s(&pTime, &curTime);
	std::wstring strRet = L"[";
	strRet += std::to_wstring(pTime.tm_hour) + L":";
	strRet += std::to_wstring(pTime.tm_min) + L"]";
	//strRet += std::to_wstring(pTime.tm_sec) + L"초]";
	
	return strRet;
}
