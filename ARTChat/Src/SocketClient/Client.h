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
		std::wstring strCloseCommand = L"/CloseByClient";
		Send(strCloseCommand);
		this->Close();
	};

	int Connect(ST_SERVER_INFO stServerInfo);
	int Close();
	int Send(std::wstring strMessage);
	int Recv(LPBYTE pBuffer);
	std::wstring Recv();
	std::vector<std::wstring> RecvChatData();
	int Peek(LPBYTE pBuffer, size_t tBufferSize);
};

