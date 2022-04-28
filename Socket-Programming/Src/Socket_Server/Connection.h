#pragma once
class CSocketConnection
{
public:
	SOCKET hConnectionSocket;
	int Create();
	void Send(std::wstring strMsg);
	std::wstring Recv();
	void Close();
};