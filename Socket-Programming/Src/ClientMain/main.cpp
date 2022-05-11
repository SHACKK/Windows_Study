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

CSocketClient user;
std::mutex mtx;

DWORD WINAPI PrintScreen(LPVOID pContext)
{
	mtx.lock();
	system("cls");
	ST_THREAD_PARAM& stThreadArg = *(ST_THREAD_PARAM*)pContext;
	std::vector<std::wstring>& v_CurrentChatData = *(std::vector<std::wstring>*)stThreadArg.v_ChatData;
	std::wstring& strContext = *(std::wstring*)stThreadArg.strContext;

	for (auto iter : v_CurrentChatData)
		wprintf(L"%s\n", iter.c_str());
	wprintf(L"\n\n%s", strContext.c_str());
	mtx.unlock();

	return 0;
}

DWORD WINAPI RecvChatData(LPVOID pContext)
{
	ST_THREAD_PARAM& stThreadArg = *(ST_THREAD_PARAM*)pContext;
	std::vector<std::wstring>& v_PreChatData = *(std::vector<std::wstring>*)stThreadArg.v_ChatData;

	while (true)
	{
		v_PreChatData = user.RecvChatData();
		::CreateThread(nullptr, 0, PrintScreen, &stThreadArg, 0, nullptr);
	}
	return 0;
}

DWORD WINAPI KeyInput(LPVOID pContext)
{
	ST_THREAD_PARAM& stThreadArg = *(ST_THREAD_PARAM*)pContext;
	std::wstring& strCurrentContext = *(std::wstring*)stThreadArg.strContext;
	std::wstring strPreContext = L"|";

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

	while (true)
	{
		std::list<ST_KEYSTATE> ListKey;

		input.Query(ListKey);

		if (ListKey.empty())
			continue;

		if (input.IsEnterPressed())
		{
			std::wstring strSendMsg = stringbuilder.GetContext();
			user.Send(strSendMsg);
			stringbuilder.Clear();
			strPreContext = L"|";
			strCurrentContext = L"|";
			::CreateThread(nullptr, 0, PrintScreen, &stThreadArg, 0, nullptr);
			continue;
		}

		{	// 키 입력이 있을 경우
			bIsCapsLockEnabled = input.IsEnabledCapsLock();
			bIsShiftEnabled = input.IsEnabledShift();

			stringbuilder.BuildContext(ListKey, bIsShiftEnabled, bIsCapsLockEnabled);

			strCurrentContext = stringbuilder.GetContextWithCursor();

			if (strCurrentContext != strPreContext)
			{
				::CreateThread(nullptr, 0, PrintScreen, &stThreadArg, 0, nullptr);
				strPreContext = strCurrentContext;
			}
		}
	}

	return 0;
}

int main(void)
{
	std::setlocale(LC_ALL, "ko_KR.UTF-8");
	ST_WSA_INITIALIZER init;

	if (!user.Connect())
		return -1;

	std::vector<std::wstring> v_ChatData;
	std::wstring strContext = L"|";

	std::wstring strUserId = L"jfhg456";
	user.Send(strUserId);

	ST_THREAD_PARAM stThreadArg = { &v_ChatData, &strContext };

	HANDLE hInputThread = ::CreateThread(nullptr, 0, KeyInput, &stThreadArg, 0, nullptr);
	HANDLE hRecvThread = ::CreateThread(nullptr, 0, RecvChatData, &stThreadArg, 0, nullptr);

	::WaitForSingleObject(hRecvThread, INFINITE);

	return 0;
}