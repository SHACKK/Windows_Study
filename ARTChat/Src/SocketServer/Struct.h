#pragma once

// �������ݿ� ���Ǵ� ����ü
// Ȥ�� ��Ŷ ����
// �������� ������������ ���Ǵ� ����ü

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