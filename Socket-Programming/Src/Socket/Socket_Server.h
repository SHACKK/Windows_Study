#pragma once

class CSocketServer
{
	CSocketServer() {};
	~CSocketServer() {};
private:
	std::vector<SOCKET> vGeneratedSocket;
public:
	SOCKET WINAPI Listen();
	void Close();
};