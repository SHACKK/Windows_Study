#include "pch.h"
#include "Connection.h"
#include "TestCase.h"
#pragma comment(lib, "Ws2_32.lib")

void CheckResult(bool bRes, std::string strTestName)
{
	if (bRes)
		printf("[SUCCESS] %s", strTestName.c_str());
	else
		printf("[FAILED] %s", strTestName.c_str());
}

int main()
{
	std::setlocale(LC_ALL, "ko_KR.UTF-8");
	ST_WSA_INITIALIZER stWsaInit;
	bool bRes;

	bRes = SendRecvTest();
	CheckResult(bRes, "SendTest");

	return 0;
}