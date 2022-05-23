#include "pch.h"
#pragma comment(lib, "Ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 55000

struct ST_WSA_INITIALIZER
{
	WSAData wsa;
	ST_WSA_INITIALIZER(void)
	{
		WSAStartup(MAKEWORD(2, 2), &wsa);
	}
	~ST_WSA_INITIALIZER(void)
	{
		WSACleanup();
	}
};

int main()
{
	ST_WSA_INITIALIZER stWsaInit;
	CClient client;

	ST_SERVER_INFO stServerInfo;
	stServerInfo.IP = SERVER_IP;
	stServerInfo.Port = SERVER_PORT;

	client.Connect(stServerInfo);


	return 0;
}