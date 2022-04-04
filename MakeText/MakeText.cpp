#include <string>
#include <conio.h>
#include <cstring>
#include <iostream>

const char* charset_cho[] = { "r","R","s","e","E","f","a","q","Q","t","T","d","w","W","c","z","x","v","g" };
const char* charset_jung[] = { "k","o","i","O","j,","p","u","P","h","hk","ho","hl","y","n","nj","np","nl","b","m","ml","l" };
const char* charset_jong[] = { "", "r","R","rt","s","sw","sg","e","f","fr","fa","fq","ft","fx","fv","fg","a","q","qt","t","T","d","w","c","z","x","v","g" };

struct CompletedEumjeol
{
	int choseong = 0;
	int jungseong = 0;
	int jongseong = 0;
};

int FindUnicode(CompletedEumjeol stsyllable)
{
	return 44032 + ((stsyllable.choseong * 21) + stsyllable.jungseong) * 28 + stsyllable.jongseong;
}

int FindIndex(const char* chararray[], char c)
{
	for (int i = 0; i < (int)sizeof(chararray); i++)
	{
		if (c == (char)chararray[i])
			return i;
	}
	return -1;
}

// 자음인지 모음인지 구분
int SortChar(char c)
{
	char tmp;
	for (int i = 0; i < 19; i++)
	{
		tmp = (char)charset_cho[i];
		if (c == tmp) {
			return 0;
			break;
		}
	}
	return 1; // 없으면 모음
}

// 글자가 어디까지 완성되었는지
int SortEumjeol(CompletedEumjeol stsyllabel)
{
	if (stsyllabel.choseong == NULL && stsyllabel.jungseong == NULL && stsyllabel.jongseong == NULL)
		return 0;
	else if (stsyllabel.choseong == NULL && stsyllabel.jungseong != NULL && stsyllabel.jongseong == NULL)
		return 1;
	else if (stsyllabel.choseong != NULL && stsyllabel.jungseong == NULL && stsyllabel.jongseong == NULL)
		return 2;
	else if (stsyllabel.choseong != NULL && stsyllabel.jungseong != NULL && stsyllabel.jongseong == NULL)
		return 3;
	else if (stsyllabel.choseong != NULL && stsyllabel.jungseong != NULL && stsyllabel.jongseong != NULL)
		return 4;
}

void ResetEumjeol(CompletedEumjeol& stsyllabel)
{
	stsyllabel.choseong = 0;
	stsyllabel.jungseong = 0;
	stsyllabel.jongseong = 0;
}

void PrintUnicode(CompletedEumjeol stsyllabel)
{

}

void InsertChar(CompletedEumjeol& stsyllabel, char c)
{
	int CharFlag = SortChar(c); // 1이면 모음, 0이면 자음
	int EumjeolFlag = SortEumjeol(stsyllabel);
	if (CharFlag == 1)
	{
		switch (EumjeolFlag)
		{
		case 0: // 000 -> 초성에 삽입
			stsyllabel.choseong = FindIndex(charset_cho, c);
			break;
		case 1: // 010 -> 리셋 후 초성에 삽입
			ResetEumjeol(stsyllabel);
			stsyllabel.choseong = FindIndex(charset_cho, c);
			break;
		case 2: // 100 -> 리셋 후 초성에 삽입
			ResetEumjeol(stsyllabel);
			stsyllabel.choseong = FindIndex(charset_cho, c);
			break;
		case 3: // 110 -> 종성에 삽입
			stsyllabel.jongseong = FindIndex(charset_jong, c);
			break;
		case 4: // 111 -> 기존의 종성에 있는 문자열과 합쳐서 charset_jong에 있는지 확인하고, 있으면 합쳐서 삽입 / 없으면 리셋 후 초성에 삽입
			char* tmpchar = (char*)charset_jong[stsyllabel.jongseong];
			strncat_s(tmpchar, 1, &c, 1);
			int Index = FindIndex(charset_jong, *tmpchar);
			if (Index != -1)
				stsyllabel.jongseong = Index;
			else
			{
				ResetEumjeol(stsyllabel);
				stsyllabel.choseong = FindIndex(charset_cho, c);
			}
			break;
		}
	}
	else //모음일 경우
	{
		switch (EumjeolFlag)
		{
		case 0: // 000 -> 중성에 삽입
			stsyllabel.jungseong = FindIndex(charset_jung, c);
			break;
		case 1: // 010 -> 리셋후 중성에 삽입
			ResetEumjeol(stsyllabel);
			stsyllabel.jungseong = FindIndex(charset_jung, c);
			break;
		case 2: // 100-> 중성에 삽입
			stsyllabel.jungseong = FindIndex(charset_jung, c);
			break;
		case 3:
		{// 110 -> 기존의 중성에 있는 문자열과 합쳐서 charset_jung에 있는지 확인하고, 있으면 합쳐서 삽입 / 없으면 리셋 후 중성에 삽입 
			const char* tmpchar = charset_jung[stsyllabel.jungseong];
			std::string tmp2char = tmpchar;
			std::string tmp3char = tmp2char.append(&c);
			char tmp4char = (char)tmp3char.c_str();
			int Index = FindIndex(charset_jung, tmp4char);
			if (Index != -1)
			{
				ResetEumjeol(stsyllabel);
				stsyllabel.jungseong = FindIndex(charset_jung, c);
			}
			else
			{
				stsyllabel.jungseong = Index;
			}
			break;
		}
		case 4: // 111 -> 
		{
			if (stsyllabel.jongseong == 2 ||
				stsyllabel.jongseong == 4 ||
				stsyllabel.jongseong == 5 ||
				stsyllabel.jongseong == 8 ||
				stsyllabel.jongseong == 9 ||
				stsyllabel.jongseong == 10 ||
				stsyllabel.jongseong == 11 ||
				stsyllabel.jongseong == 12 ||
				stsyllabel.jongseong == 13 ||
				stsyllabel.jongseong == 14 ||
				stsyllabel.jongseong == 17)
			{
				const char* tmpchar = charset_jong[stsyllabel.jongseong];
				std::string tmp2char = tmpchar;
				std::string tmp3char = tmp2char.substr(0, 1);
				char* tmp4char = (char*)tmp3char.c_str();
				stsyllabel.jongseong = FindIndex(charset_jong, *tmp4char);
				//
			}
			else
			{
				int tmpIndex = stsyllabel.jongseong;
				ResetEumjeol(stsyllabel);
				stsyllabel.choseong = tmpIndex;
				stsyllabel.jungseong = FindIndex(charset_jung, c);
			}
		}
		}
	}
}

std::wstring AssembleHangul(std::wstring strCurrentContext, CompletedEumjeol& stsyllabel, char c)
{
	if ('\n' == c)
		return strCurrentContext;

	if ('b' == c)
		return strCurrentContext = strCurrentContext.substr(0, strCurrentContext.length() - 1);

	InsertChar(stsyllabel, c);

	std::wstring strRet = strCurrentContext;
	strRet.push_back(FindUnicode(stsyllabel));
	return strRet;
}

int main()
{
	std::setlocale(LC_ALL, "ko_KR.UTF-8");
	std::wstring strContext;

	CompletedEumjeol stsyllabel;
	stsyllabel.choseong = 0;
	stsyllabel.jungseong = 0;
	stsyllabel.jongseong = 0;

	while (true)
	{
		char c = (char)_getch();
		std::wstring strNewContext = AssembleHangul(strContext, stsyllabel, c);

		if (strNewContext == strContext)
			continue;

		strContext = strNewContext;

		wprintf(L"%s\n", strContext.c_str());
	}
	return 0;
}