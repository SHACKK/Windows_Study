#include "pch.h"
#include "ChatClient.h"

bool CChatClient::Connect(ST_SERVER_INFO stServerInfo, std::wstring strUserName)
{
	__super::Connect(stServerInfo);

	std::wstring strConnectResult = Recv();
	if (!wcscmp(strConnectResult.c_str(), L"Wait"))
	{
		wprintf(L"Server is too busy...\nPlease Wait");
		strConnectResult = Recv();
	}
	if (!wcscmp(strConnectResult.c_str(), L"Accept"))
	{
		wprintf(L"Connected!\n");
		m_strUserName = strUserName;
		Send(m_strUserName);
		return true;
	}

	return false;
}

int CChatClient::Send(std::wstring strMessage)
{
	int nRet = 0;
	size_t nLength = strMessage.length() * sizeof(wchar_t);
	nRet += ::send(m_hClientSocket, (const char*)&nLength, sizeof(nLength), 0);
	nRet += ::send(m_hClientSocket, (const char*)strMessage.c_str(), (int)nLength, 0);
	return nRet;
}

std::wstring CChatClient::Recv()
{
	size_t nLength = 0;
	::recv(m_hClientSocket, (char*)&nLength, sizeof(nLength), 0);

	std::wstring strRet;
	strRet.resize(nLength / sizeof(wchar_t));
	::recv(m_hClientSocket, (char*)strRet.c_str(), (int)nLength, 0);

	return strRet;
}

std::vector<std::wstring> CChatClient::RecvChatData()
{
	std::vector<std::wstring> vecChatData;
	size_t nVecSize;
	::recv(m_hClientSocket, (char*)&nVecSize, sizeof(size_t), 0);
	vecChatData.resize(nVecSize);

	for (size_t i = 0; i < nVecSize; i++)
	{
		int nMsgLength;
		::recv(m_hClientSocket, (char*)&nMsgLength, sizeof(nMsgLength), 0);
		vecChatData[i].resize(nMsgLength / sizeof(wchar_t));
		::recv(m_hClientSocket, (char*)vecChatData[i].c_str(), nMsgLength, 0);
	}

	return vecChatData;
}

void CChatClient::setUserName(std::wstring strName)
{
	m_strUserName = strName;
}

std::wstring CChatClient::getUserName()
{
	if (m_strUserName.empty())
		return L"NoName";
	else
		return m_strUserName;
}
