#pragma once

// 프로토콜에 사용되는 구조체
// 혹은 패킷 형식
// 공통으로 여러군데에서 사용되는 구조체

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

enum class E_RESULT
{
	SUCCESS,
	FAILED
};