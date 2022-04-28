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

std::vector<std::wstring> v_ChatData;

int main(void)
{
	ST_WSA_INITIALIZER init;

	CSocketClient user;
	if (!user.Connect())
		return -1;

	std::wstring strUserId = L"jfhg456";
	user.Send(strUserId);

	int nVecSize = std::stoi(user.Recv());
	for (int i = 0; i < nVecSize; i++)
		v_ChatData.push_back(user.Recv().c_str());

	for (size_t j = 0; j < v_ChatData.size(); j++)
		wprintf(L"%s\n", v_ChatData[j].c_str());
	
	while (true)
	{
		std::wstring strSendMsg = L"Hello Server~";
		user.Send(strSendMsg);



		for (size_t j = 0; j < v_ChatData.size(); j++)
			wprintf(L"%s\n", v_ChatData[j].c_str());
	}


	return 0;
}