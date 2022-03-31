#include <string>
#include <conio.h>
#include <cstring>

const char* choseong[] = { "r","R","s","e","E","f","a","q","Q","t","T","d","w","W","c","z","x","v","g" };
const char* jungseong[] = { "k","o","i","O","j,","p","u","P","h","hk","ho","hl","y","n","nj","np","nl","b","m","ml","l" };
const char* jongseong[] = { "r","R","rt","s","sw","sg","e","f","fr","fa","fq","ft","fx","fv","fg","a","q","qt","t","T","d","w","c","z","x","v","g" };

struct Completed_Eumjeol
{
	int choseong;
	int jungseong;
	int jongseong;
};

int FindUnicode(Completed_Eumjeol stsyllable)
{
	return 44032 + ((stsyllable.choseong * 21) + stsyllable.jungseong) * 28 + stsyllable.jongseong;
}

int FindIndex(const char* chararray[], char* c)
{
	// 인덱스 번호 반환
	
	for (int i = 0; i < sizeof(choseong); i++)
	{
		if (c == choseong[i])
			return i;
	}
	return 0;
}

bool SortChar(char* c)
{
	for (int i = 0; i < sizeof(choseong); i++)
	{
		if (c == choseong[i])
			return false;
	}
	return true;
}

void InsertChar(Completed_Eumjeol &stsyllabel, char* c)
{
	bool flag = SortChar(c); // 1이면 모음, 0이면 자음

	//초성 삽입
	if (stsyllabel.choseong == NULL)
		stsyllabel.choseong = FindIndex(choseong, c);
	// 중성 삽입
	else if ((stsyllabel.choseong != NULL && stsyllabel.jungseong == NULL))
		stsyllabel.jungseong = FindIndex(jungseong, c);
	// 중성이 두개인 경우 재설정 후 삽입
	else if (stsyllabel.choseong != NULL && stsyllabel.jungseong != NULL && flag == true)
	{
		char* tmp_char = (char*)jungseong[stsyllabel.jungseong]; // 액세스 오류
		strncat_s(tmp_char, 1, c, 1);
		stsyllabel.jungseong = FindIndex(jungseong, tmp_char);
	}	
	// 종성 삽입
	else if (stsyllabel.choseong !=NULL && stsyllabel.jungseong != NULL && stsyllabel.jongseong == NULL && flag == false)
		stsyllabel.jongseong = FindIndex(jongseong, c);
	// 종성이 두개인 경우 재설정 후 삽입
}

std::wstring AssembleHangul(std::wstring strCurrentContext, Completed_Eumjeol &stsyllabel, char* c)
{
	if ("\n" == c)
		return strCurrentContext;

	if ("\b" == c)
		return strCurrentContext = strCurrentContext.substr(0, strCurrentContext.length() - 1);

	InsertChar(stsyllabel, c);

	std::wstring strRet = strCurrentContext;
	strRet.push_back(FindUnicode(stsyllabel));
	return strRet;
}

int main()
{
	std::wstring strContext;

	Completed_Eumjeol stsyllable;
	while (true)
	{
		char c = (char)_getch();
		std::wstring strNewContext = AssembleHangul(strContext, stsyllable, &c);

		if (strNewContext == strContext)
			continue;

		strContext = strNewContext;

		wprintf(L"%s\n", strContext.c_str());
	}
	return 0;
}