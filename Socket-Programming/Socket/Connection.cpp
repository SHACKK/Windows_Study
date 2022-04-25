#include "pch.h"
#include "Connection.h"

DWORD CConnection::ConnectionThread()
{
	printf("server connection established.\n");

	{
		int nLength = 0;
		// ó���� ���� ������ ���� ����
		::recv(hConnectionSock, (char*)&nLength, sizeof(nLength), 0); 

		// ���� ���ڿ��� ����
		std::string strMsg;
		strMsg.resize(nLength);
		::recv(hConnectionSock, (char*)strMsg.c_str(), nLength, 0); 

		printf("server recved msg `%s`.\n", strMsg.c_str());
	}

	Sleep(1000);
	{
		std::string strMsg = "Hello client??";
		int nLength = strMsg.length();
		::send(hConnectionSock, (const char*)&nLength, sizeof(nLength), 0);
		::send(hConnectionSock, strMsg.c_str(), strMsg.length(), 0);
	}

	closesocket(hConnectionSock);
	printf("connection closed\n");

	return 0;
}