#pragma once

struct ST_SERVER_INFO
{
	const char* IP;
	int Port;
};

class CClient
{
private:
	SOCKET m_hClientSocket;
public:
	CClient() {};
	~CClient() 
	{
		this->Close();
	};

	int Connect(ST_SERVER_INFO stServerInfo);
	int Close();
	int Send(LPCBYTE pData, size_t tSize);
	int Recv(LPBYTE pBuffer, size_t tBufferSize);
	int Peek(LPBYTE pBuffer, size_t tBufferSize);
};

