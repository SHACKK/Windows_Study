#pragma once
class CSocketConnection
{
public:
	SOCKET hConnectionSocket; //Ŭ���̾�Ʈ�� ������ ����

	int Create(std::wstring strUserId);
	void Send(std::wstring strMsg);
	void Send(std::vector<std::wstring> v_ChatData);
	std::wstring Recv();
	void Close();
};