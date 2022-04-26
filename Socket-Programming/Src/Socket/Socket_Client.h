#pragma once
class CSocketClient
{
	CSocketClient() {};
	~CSocketClient() {};
public:
	void Send();
	void Recv();
	void Close();
};