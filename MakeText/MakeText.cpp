#include <string>
#include <conio.h>
#include <Windows.h>

const char choseong[] = { 'r','R','s','e','E','f','a','q','Q','t','T','d','w','W','c','z','x','v','g' };
const char jungseong[] = { 'k','o','i','O','j,','p','u','P','h','hk','ho','hl','y','n','nj','np','nl','b','m','ml','l' };
const char jongseong[] = { 'r','R','rt','s','sw','sg','e','f','fr','fa','fq','ft','fx','fv','fg','a','q','qt','t','T','d','w','c','z','x','v','g' };

std::wstring AssembleHangul(std::wstring strCurrentContext, char c)
{
	if ('\n' == c)
		return strCurrentContext;

	if ('\b' == c)
		return strCurrentContext = strCurrentContext.substr(0, strCurrentContext.length() - 1);


	std::wstring strRet = strCurrentContext;
	strRet.push_back(c);
	return strRet;
}

int main()
{
	std::wstring strContext;
	while (true)
	{
		char c = (char)_getch();
		std::wstring strNewContext = AssembleHangul(strContext, c);

		if (strNewContext == strContext)
			continue;

		strContext = strNewContext;

		wprintf(L"%s\n", strContext.c_str());
	}
	return 0;
}