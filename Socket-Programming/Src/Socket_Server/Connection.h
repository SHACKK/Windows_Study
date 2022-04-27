#pragma once
class CSocketConnection
{
public:
	SOCKET hConnectionSocket;
	int Create();
	void Send();
	std::wstring Recv();
	void Close();
};