#pragma once
class CSocketConnection
{
	CSocketConnection() {};
	~CSocketConnection() {};
public:
	SOCKET hConnectionSock;
	void Create(SOCKET hsocket);
	void Send();
	void Recv();
	void Close();
};

