#pragma once

class CSocketClient
{
private:
	SOCKET hClientSocket;

public:
	bool Connect();
	void Send(std::wstring strMsg);
	std::wstring Recv();
	void Close();
};