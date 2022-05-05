#include "pch.h"
#include "Connection.h"

#pragma comment(lib, "ws2_32.lib")

int CSocketConnection::Create()
{
	printf("Server Connection Established...!\n");
	return 0;
}

void CSocketConnection::Send(std::wstring strMsg)
{
	int nLength = strMsg.length();
	::send(hConnectionSocket, (const char*)&nLength, sizeof(nLength), 0);
	::send(hConnectionSocket, (const char*)strMsg.c_str(), nLength, 0);
}

void CSocketConnection::Send(std::vector<std::wstring> v_ChatData)
{
	size_t nVecSize = v_ChatData.size();
	::send(hConnectionSocket, (const char*)&nVecSize, sizeof(size_t), 0);

	for (size_t i = 0; i < nVecSize; i++)
	{
		int nMsgLength = v_ChatData[i].size() * sizeof(wchar_t);
		::send(hConnectionSocket, (const char*)&nMsgLength, sizeof(nMsgLength), 0);
		::send(hConnectionSocket, (const char*)v_ChatData[i].c_str(), nMsgLength, 0);
	}
}

std::wstring CSocketConnection::Recv()
{
	int nLength = 0;
	::recv(hConnectionSocket, (char*)&nLength, (int)sizeof(nLength), 0);

	std::wstring strMsg;
	strMsg.resize(nLength / sizeof(wchar_t));
	::recv(hConnectionSocket, (char*)strMsg.c_str(), nLength, 0); 

	return strMsg;
}

void CSocketConnection::Close()
{
	if (hConnectionSocket != INVALID_SOCKET)
		closesocket(hConnectionSocket);
}
