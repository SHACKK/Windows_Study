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

// <회의실>
//6인실 2틀 시간당 15,000원이지만 하루종일이면 좀더 싸게 해주신다고 하심.

//수원역 <스터디으라차>
//031-207-5254