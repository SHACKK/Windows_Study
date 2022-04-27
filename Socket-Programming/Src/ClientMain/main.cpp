#include "pch.h"
#pragma comment(lib, "ws2_32.lib")

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

int main(void)
{
	ST_WSA_INITIALIZER init;

	CSocketClient user;
	if (!user.Connect())
		return -1;

	std::wstring strUserId = L"jfhg456";
	user.Send(strUserId);

	while (true)
	{
		std::wstring strSendMsg = L"Hello Server~";
		user.Send(strSendMsg);

		std::wstring strRecvMsg = user.Recv();
		wprintf(L"%s", strRecvMsg.c_str());
	}


	return 0;
}