#pragma once

class CClient
{
public:
	SOCKET hClientSocket;
	int Connect();
	int Close();
	int Send(LPCBYTE pData, size_t Size);
	int Recv(LPBYTE pBuffer, size_t BufferSize);
};

