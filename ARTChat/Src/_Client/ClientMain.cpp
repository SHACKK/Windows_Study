#include "pch.h"
#pragma comment(lib, "Ws2_32.lib")

#define SERVER_IP "172.30.100.217"
#define SERVER_PORT 55000
#define USER_NAME L"[ 임창현 ]"
#define MAX_CHATDATA_SIZE 25

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
	if (MAX_CHATDATA_SIZE < vecChatData.size())
		vecChatData.erase(vecChatData.begin());

	system("cls");
	for (int i = 0; i < vecChatData.size(); i++)
	{
		wprintf(L"%s\n", vecChatData[i].c_str());
	}
	wprintf(L"\n메세지 >> %s", strCurrentText.c_str());
}

DWORD WINAPI UpdateChatDataThread(LPVOID pContext)
{
	while (true)
	{
		CChatClient& client = *(CChatClient*)pContext;
		std::wstring strMessage = client.Recv();
		
		if (!wcscmp(strMessage.c_str(), CONNECTION_CLOSE_BY_SERVER) || strMessage.empty())
		{
			client.Close();
			break;
		}

		vecChatData.push_back(strMessage);
		PrintChatData();
	}
	return 0;
}

DWORD WINAPI KeyInputThread(LPVOID pContext)
{
	CChatClient& client = *(CChatClient*)pContext;
	CKeyInput input;
	std::wstring strPreText = L"|";

	// 숫자
	for (int i = 0x30; i < 0x40; i++)
		input.Register(i, i);

	// 알파뱃
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
		
		input.Query(listKeys);

		if (listKeys.empty())
			continue;

		bIsShiftEnabled = input.IsEnabledShift();
		bIsCapsLockEnabled = input.IsEnabledCapsLock();

		if (input.IsEnterPressed())
		{	
			std::wstring strMessage = stringbuilder.GetContext();
			if(!strMessage.empty())
				client.Send(strMessage);
			strCurrentText.clear();
			strPreText = L"|";

			if (!wcscmp(strMessage.c_str(), CONNECTION_CLOSE_BY_CLIENT))	
				break;
		}

		stringbuilder.BuildContext(listKeys, bIsShiftEnabled, bIsCapsLockEnabled);

		strCurrentText = stringbuilder.GetContextWithCursor();
		if (wcscmp(strPreText.c_str(), strCurrentText.c_str()))
		{
			strPreText = strCurrentText;
			PrintChatData();
		}
	}

	return 0;
}

int main()
{
	std::setlocale(LC_ALL, "ko_KR.UTF-8");

	ST_WSA_INITIALIZER stWsaInit;
	CChatClient client;

	ST_SERVER_INFO stServerInfo;
	stServerInfo.IP = SERVER_IP;
	stServerInfo.Port = SERVER_PORT;

	
	if (!client.Connect(stServerInfo, USER_NAME))
	{
		printf("Connection Failed\n");
		return 1;
	}
	vecChatData = client.RecvChatData();
	PrintChatData();

	HANDLE hUpdateChatDataThread = CreateThread(nullptr, 0, UpdateChatDataThread, &client, 0, nullptr);
	HANDLE hInputThread = CreateThread(nullptr, 0, KeyInputThread, &client, 0, nullptr);

	WaitForSingleObject(hInputThread, INFINITE);
	WaitForSingleObject(hUpdateChatDataThread, INFINITE);
	
	return 0;
}