#include "stdafx.h"
#include "Hangul.h"

int main()
{
	Hangul Hg;
	std::setlocale(LC_ALL, "ko_KR.UTF-8");
	std::wstring strContext;

	while (true)
	{
		char c = (char)_getch();
		//int key_flag = IsKeyboardChar(c);

		//std::wstring strNewContext;
		std::wstring strNewContext = Hg.AssembleHangul(strContext, c);

		if (strNewContext == strContext)
			continue;

		strContext = strNewContext;

		system("cls");
		wprintf(L"%s\n", strContext.c_str());
	}
	return 0;
}