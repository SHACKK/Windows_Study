#pragma once

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

// <ȸ�ǽ�>
//6�ν� 2Ʋ �ð��� 15,000�������� �Ϸ������̸� ���� �ΰ� ���ֽŴٰ� �Ͻ�.

//������ <���͵�������>
//031-207-5254