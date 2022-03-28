#include <string>
#include <conio.h>
#include <Windows.h>

void AssembleHangul(std::wstring* Dest, char Src)
{
	*Dest = Src;
}
int main()
{
	while (true)
	{
		int Text = (int)_getch();
		std::wstring NewText;
		AssembleHangul(&NewText, Text);

		//wprintf(L"%s", NewText.c_str());
		printf("%x\n", Text);
	}
	return 0;
}
