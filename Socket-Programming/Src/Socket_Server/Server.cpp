#include "pch.h"
#include "Server.h"

SOCKET CSocketServer::Listen()
{
	hListenSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == hListenSocket)
		int nErrorCode = ::WSAGetLastError();


	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons(55000);
	nRet = ::bind(hListenSocket, (sockaddr*)&service, sizeof(service));
	if (SOCKET_ERROR == nRet)
		int nErrorCode = ::WSAGetLastError();

	nRet = ::listen(hListenSocket, 200);
	if (SOCKET_ERROR == nRet)
	{
		return -1;
	}

	sockaddr RemoteInfo;
	int nRemoteInfoSize = (int)sizeof(RemoteInfo);
	SOCKET hConnectionSock = ::accept(hListenSocket, &RemoteInfo, &nRemoteInfoSize);
	if (INVALID_SOCKET == hConnectionSock)
	{
		return -1;
	}

	ListSocket.push_back(hConnectionSock);
	return hConnectionSock;

}

void CSocketServer::BroadCast(std::vector<std::wstring> v_ChatData)
{
	std::list<SOCKET>::iterator iter;
	for (iter = ListSocket.begin(); iter != ListSocket.end(); iter++)
	{
		size_t nVecLength = v_ChatData.size();
		::send(*iter, (const char*)&nVecLength, sizeof(nVecLength), 0);
		for (size_t i = 0; i < v_ChatData.size(); i++)
		{
			int nLength = v_ChatData[i].length();
			::send(*iter, (const char*)&nLength, sizeof(nLength), 0);
			::send(*iter, (const char*)&v_ChatData[i], nLength, 0);
		}
	}
}

void CSocketServer::Close()
{
	if (INVALID_SOCKET != hListenSocket)
		::shutdown(hListenSocket, 2);
}

