#include "pch.h"
#include "Client.h"

DWORD CClient::ClientThread()
{
	SOCKET hClientSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == hClientSocket)
	{
		return -1;
	}

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientService.sin_port = htons(55000);
	int nRet = ::connect(hClientSocket, (sockaddr*)&clientService, sizeof(clientService));
	if (SOCKET_ERROR == nRet)
	{
		int nErrorCode = WSAGetLastError();
		return -1;
	}
	Sleep(500);

	{
		std::string strMsg = "Hello server??";
		int nLength = strMsg.length();
		::send(hClientSocket, (const char*)&nLength, sizeof(nLength), 0);
		::send(hClientSocket, strMsg.c_str(), strMsg.length(), 0);
	}

	{
		int nLength = 0;
		::recv(hClientSocket, (char*)&nLength, sizeof(nLength), 0);

		std::string strMsg;
		strMsg.resize(nLength);
		::recv(hClientSocket, (char*)strMsg.c_str(), nLength, 0);

		printf("client recved msg `%s`.\n", strMsg.c_str());
	}

	char szBuffer[10];
	int nRecvedSize = ::recv(hClientSocket, szBuffer, 10, 0);
	::closesocket(hClientSocket);

	return 0;
}
