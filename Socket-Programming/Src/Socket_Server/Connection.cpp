#include "pch.h"
#include "Connection.h"

#pragma comment(lib, "ws2_32.lib")

int CSocketConnection::Create()
{
	printf("Server Connection Established.");
	return 0;
}

void CSocketConnection::Send(std::wstring strMsg)
{
	int nLength = strMsg.length();
	::send(hConnectionSocket, (const char*)&nLength, sizeof(nLength), 0);
	::send(hConnectionSocket, (const char*)strMsg.c_str(), nLength, 0);
}


std::wstring CSocketConnection::Recv()
{
	int nLength = 0;
	::recv(hConnectionSocket, (char*)&nLength, (int)sizeof(nLength), 0);

	std::wstring strMsg;
	strMsg.resize(nLength);
	::recv(hConnectionSocket, (char*)strMsg.c_str(), nLength, 0); 

	return strMsg;
}

void CSocketConnection::Close()
{
	if (hConnectionSocket != INVALID_SOCKET)
		closesocket(hConnectionSocket);
}
