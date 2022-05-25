#include "pch.h"
#include "ConnectionSuper.h"
#include "ChatConnection.h"

DWORD WINAPI ConnectionThreadCaller(void* pContext)
{
	CConnectionSuper* connection = (CConnectionSuper*)pContext;
	return connection->ConnectionThread();
}

DWORD CConnectionSuper::ConnectionThread()
{
	onConnect();

	onRecv();

	onClose();

	return 0;
}

int CConnectionSuper::Establish(SOCKET acceptedSocket, CServer* pServer)
{
	m_ConnectionSocket = acceptedSocket;
	m_pServer = pServer;

	::CreateThread(nullptr, 0, ConnectionThreadCaller, this, 0, nullptr);
	return 0;
}

int CConnectionSuper::SendChatData(std::vector<std::wstring> vecChatData)
{
	size_t nVecSize = vecChatData.size();
	::send(m_ConnectionSocket, (const char*)&nVecSize, sizeof(size_t), 0);

	for (size_t i = 0; i < nVecSize; i++)
	{
		int nMsgLength = vecChatData[i].size() * sizeof(wchar_t);
		::send(m_ConnectionSocket, (const char*)&nMsgLength, sizeof(nMsgLength), 0);
		::send(m_ConnectionSocket, (const char*)vecChatData[i].c_str(), nMsgLength, 0);
	}
	return 0;
}

int CConnectionSuper::Send(std::wstring strMessage)
{
	int nLength = strMessage.length() * (int)sizeof(wchar_t);
	::send(m_ConnectionSocket, (const char*)&nLength, sizeof(nLength), 0);
	::send(m_ConnectionSocket, (const char*)strMessage.c_str(), nLength, 0);

	return 0;
}

std::wstring CConnectionSuper::Recv()
{
	int nLength = 0;
	::recv(m_ConnectionSocket, (char*)&nLength, (int)sizeof(nLength), 0);

	std::wstring strMsg;
	strMsg.resize(nLength / sizeof(wchar_t));
	::recv(m_ConnectionSocket, (char*)strMsg.c_str(), nLength, 0);

	return strMsg;
}

std::wstring CConnectionSuper::Peek()
{
	int nLength = 0;
	::recv(m_ConnectionSocket, (char*)&nLength, sizeof(nLength), MSG_PEEK);

	std::wstring strRet;
	strRet.resize(nLength);
	::recv(m_ConnectionSocket, (char*)strRet.c_str(), nLength, MSG_PEEK);

	return strRet;
}