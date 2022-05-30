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
private:
	SOCKET m_hClientSocket;
	std::wstring m_strUserName;
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

	void setUserName(std::wstring strName);
	std::wstring getUserName();
};