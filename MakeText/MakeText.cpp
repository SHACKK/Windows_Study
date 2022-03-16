#include <string>
#include <conio.h>

void AssembleHangul(std::wstring* Dest, wchar_t Src)
{
	*Dest = Src;
}
int main()
{
	while (true)
	{
		wchar_t Text = (wchar_t)_getch();
		std::wstring NewText;
		AssembleHangul(&NewText, Text);

		wprintf(L"%s", NewText.c_str());
	}
	return 0;
}
