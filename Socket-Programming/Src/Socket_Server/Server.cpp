#include "pch.h"
#include "Server.h"

SOCKET WINAPI CSocketServer::Listen()
{
	nRet = ::listen(hListenSocket, 200);
	if (SOCKET_ERROR == nRet)
	{
		return -1;
	}

	while (true)
	{
		sockaddr RemoteInfo;
		int nRemoteInfoSize = (int)sizeof(RemoteInfo);
		SOCKET hConnectionSock = ::accept(hListenSocket, &RemoteInfo, &nRemoteInfoSize);
		if (INVALID_SOCKET == hConnectionSock)
		{
			return -1;
		}

		vGeneratedSocket.push_back(hConnectionSock);
		return hConnectionSock;
	}
}

void CSocketServer::Close()
{
	if(INVALID_SOCKET != hListenSocket)
		::shutdown(hListenSocket, 2);
}
