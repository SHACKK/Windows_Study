#include "pch.h"
#include "Server.h"

SOCKET CSocketServer::Listen()
{
	sockaddr RemoteInfo;
	int nRemoteInfoSize = (int)sizeof(RemoteInfo);
	SOCKET hConnectionSock = ::accept(hListenSocket, &RemoteInfo, &nRemoteInfoSize);
	if (INVALID_SOCKET == hConnectionSock)
	{
		int nErrorCode = ::WSAGetLastError();
		return nErrorCode;
	}

	return hConnectionSock;

}

void CSocketServer::Close()
{
	if (INVALID_SOCKET != hListenSocket)
		::shutdown(hListenSocket, 2);
}

