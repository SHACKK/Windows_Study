#include "pch.h"
#pragma comment(lib, "ws2_32.lib")

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

std::vector<std::wstring> v_ChatData;
CSocketClient user;

DWORD WINAPI RecvChatData(LPVOID pContext)
{
	while (true)
	{
		v_ChatData = user.RecvBroadCast();
	}
	return 0;
}

DWORD WINAPI KeyInput(LPVOID pContext)
{
	return 0;
}

int main(void)
{
	std::setlocale(LC_ALL, "ko_KR.UTF-8");
	ST_WSA_INITIALIZER init;
	CKeyInput input;
	{
		// 숫자
		for (int i = 0x30; i < 0x40; i++)
			input.Register(i, i);

		// 알파뱃
		for (int i = 0x41; i < 0x5B; i++)
			input.Register(i, i);

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

	if (!user.Connect())
		return -1;

	std::wstring strUserId = L"jfhg456";
	user.Send(strUserId);

	HANDLE hPrintThread = ::CreateThread(nullptr, 0, RecvChatData, nullptr, 0, nullptr);

	//while (true)
	//{
	//	system("cls");
	//	for (size_t i = 0; i < v_ChatData.size(); i++)
	//		wprintf(L"%s\n", v_ChatData[i].c_str());

	//	std::list<ST_KEYSTATE> ListKey;

	//	input.Query(ListKey);
	//	if (ListKey.empty())
	//		continue;

	//	{	// 키 입력이 있을 경우
	//		bIsCapsLockEnabled = input.IsEnabledCapsLock();
	//		bIsShiftEnabled = input.IsEnabledShift();

	//		stringbuilder.BuildContext(ListKey, bIsShiftEnabled, bIsCapsLockEnabled);
	//		
	//		std::wstring strContext = stringbuilder.GetContextWithCursor();
	//		wprintf(L"%s\n", strContext.c_str());

	//		if (!(GetAsyncKeyState(VK_RETURN) * 0x8000))
	//		{
	//			strContext.clear();
	//			continue;
	//		}

	//		system("cls");
	//		std::wstring strSendMsg = stringbuilder.GetContext();
	//		user.Send(strSendMsg);
	//	}
	//}

	return 0;
}