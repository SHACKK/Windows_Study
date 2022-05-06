#pragma once
class CSocketConnection
{
public:
	SOCKET hConnectionSocket; //클라이언트랑 소통할 소켓

	int Create(std::wstring strUserId);
	void Send(std::wstring strMsg);
	void Send(std::vector<std::wstring> v_ChatData);
	std::wstring Recv();
	void Close();
};