#pragma once

#define CONNECTION_CLOSE_BY_CLIENT L"/CloseByClient"
#define CONNECTION_CLOSE_BY_SERVER L"/CloseByServer"

struct ST_SERVER_INFO
{
	const char* IP;
	int Port;
};

class CClient
{
protected:
	SOCKET m_hClientSocket;
public:
	CClient() 
	{
		m_hClientSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == m_hClientSocket)
			printf("[ERROR] Can't Create Socket %d\n", WSAGetLastError());
	};
	~CClient() {};

	int Connect(ST_SERVER_INFO stServerInfo);
	int Close();
	int Send(LPBYTE pData, size_t tSize);
	int Recv(LPBYTE pBuffer, size_t tSize);
	int Peek(LPBYTE pBuffer, size_t tBufferSize);
};