#include "pch.h"
#pragma comment(lib, "Ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 55000
#define USER_NAME L"[ ÀÓÃ¢Çö ]";

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

std::vector<std::wstring> vecChatData;
std::wstring strCurrentText;

void PrintChatData()
{
	system("cls");
	for (int i = 0; i < vecChatData.size(); i++)
	{
		wprintf(L"%s\n", vecChatData[i].c_str());
	}
	wprintf(L"%s\n", strCurrentText.c_str());
}

DWORD WINAPI UpdateChatDataThread(LPVOID pContext)
{
	while (true)
	{
		CClient& client = *(CClient*)pContext;
		std::wstring strMessage = client.Recv();

		vecChatData.push_back(strMessage);
		PrintChatData();
		//HANDLE hPrintThread = ::CreateThread(nullptr, 0, InsertChatDataThread, &strMessage, 0, nullptr);
		//::WaitForSingleObject(hPrintThread, INFINITE);
	}
	return 0;
}

DWORD WINAPI KeyInputThread(LPVOID pContext)
{
	CClient& client = *(CClient*)pContext;
	CKeyInput input;

	// ¼ýÀÚ
	for (int i = 0x30; i < 0x40; i++)
		input.Register(i, i);

	// ¾ËÆÄ¹î
	for (int i = 0x41; i < 0x5B; i++)
		input.Register(i, i);
	{
		input.Register(VK_DELETE, VK_DELETE);
		input.Register(VK_BACK, VK_BACK);
		input.Register(VK_RETURN, VK_RETURN);
		input.Register(VK_ESCAPE, VK_ESCAPE);
		input.Register(VK_LEFT, VK_LEFT);
		input.Register(VK_RIGHT, VK_RIGHT);
		input.Register(VK_UP, VK_UP);
		input.Register(VK_SPACE, ' ');
		input.Register(VK_DOWN, VK_DOWN);
		input.Register(VK_OEM_PLUS, '=');
		input.Register(VK_OEM_COMMA, ',');
		input.Register(VK_OEM_MINUS, '-');
		input.Register(VK_OEM_PERIOD, '.');
		input.Register(VK_OEM_1, ';');
		input.Register(VK_OEM_2, '/');
		input.Register(VK_OEM_3, '~');
		input.Register(VK_OEM_4, '[');
		input.Register(VK_OEM_5, '|');
		input.Register(VK_OEM_6, ']');
		input.Register(VK_OEM_7, '\'');
		input.Register(VK_HANGUL, VK_HANGUL);
		input.Register(VK_CAPITAL, VK_CAPITAL);
	}

	CStringBuilder stringbuilder;

	bool bIsShiftEnabled;
	bool bIsCapsLockEnabled;

	while (true)
	{
		Sleep(10);

		std::list<ST_KEYSTATE> listKeys;
		
		if (input.Query(listKeys) == 1) 
		{
			std::wstring strMessage = stringbuilder.GetContext();
			client.Send(strMessage);
			stringbuilder.Clear();
			strCurrentText.clear();
			PrintChatData();
			continue;
		}
		// ¾ê³× À§¾Æ·¡ ¼ø¼­ ¹Ù²ãºÁ¹Ù;;;
		if (listKeys.empty())
			continue;

		bIsShiftEnabled = input.IsEnabledShift();
		bIsCapsLockEnabled = input.IsEnabledCapsLock();

		stringbuilder.BuildContext(listKeys, bIsShiftEnabled, bIsCapsLockEnabled);

		strCurrentText = stringbuilder.GetContextWithCursor();
		PrintChatData();
	}

	return 0;
}

int main()
{
	std::setlocale(LC_ALL, "ko_KR.UTF-8");

	ST_WSA_INITIALIZER stWsaInit;
	CClient client;

	ST_SERVER_INFO stServerInfo;
	stServerInfo.IP = SERVER_IP;
	stServerInfo.Port = SERVER_PORT;
	
	if (!client.Connect(stServerInfo))
		return 1;

	std::wstring strUserName = USER_NAME;
	client.Send(strUserName);

	vecChatData = client.RecvChatData();
	PrintChatData();

	HANDLE hInputThread = CreateThread(nullptr, 0, KeyInputThread, &client, 0, nullptr);
	HANDLE hUpdateChatDataThread = CreateThread(nullptr, 0, UpdateChatDataThread, &client, 0, nullptr);

	WaitForSingleObject(hInputThread, INFINITE);
	WaitForSingleObject(hUpdateChatDataThread, INFINITE);
	return 0;
}