#include <string>
#include <conio.h>
#include <cstring>
#include <iostream>

const char* charset_cho[] = { "r","R","s","e","E","f","a","q","Q","t","T","d","w","W","c","z","x","v","g" };
const char* charset_jung[] = { "k","o","i","O","j,","p","u","P","h","hk","ho","hl","y","n","nj","np","nl","b","m","ml","l" };
const char* charset_jong[] = { "", "r","R","rt","s","sw","sg","e","f","fr","fa","fq","ft","fx","fv","fg","a","q","qt","t","T","d","w","c","z","x","v","g" };

int charset_single[40] = { 0x3131, 0x3132, 0x3134, 0x3137, 0x3138, 0x3139, 0x3141, 0x3142, 0x3143, 0x3145, 0x3146, 0x3147, 0x3148, 0x3149, 0x314A, 0x314B, 0x314C, 0x314D, 0x314E, 0x314F, 0x3150, 0x3151, 0x3152, 0x3153, 0x3154, 0x3155, 0x3156, 0x3157, 0x3158, 0x3159, 0x315A, 0x315B, 0x315C, 0x315D, 0x315E, 0x315F, 0x3160, 0x3161, 0x3162, 0x3163 };

struct CompletedEumjeol
{
	int flag = 0;
	int choseong = 99;
	int jungseong = 99;
	int jongseong = 0;
};

int FindUnicode(CompletedEumjeol stsyllable)
{
	return 44032 + ((stsyllable.choseong * 21) + stsyllable.jungseong) * 28 + stsyllable.jongseong;
}

// 배열에서 몇번째 인덱스를 가지는지 리턴
int FindIndex(const char* chararray[], char* c)
{
	char szInput[2] = { *c, 0 };
	int i = 0;
	for (i; i < 28; i++) // sizeof가 잘못되었음
	{
		if (strcmp(szInput, chararray[i]) == 0)
			return i;
	}
	return -1;
}

// 자음인지 모음인지 구분
int SortChar(char *c)
{
	char* tmp;
	char szInput[2] = { *c, 0 };
	for (int i = 0; i < 19; i++)
	{
		tmp = (char*)charset_cho[i];
		if (strcmp(szInput, tmp) == 0) {
			return 0;
			break;
		}
	}
	return 1; // 없으면 모음
}

// 글자가 어디까지 완성되었는지
int SortEumjeol(CompletedEumjeol stsyllabel)
{
	if (stsyllabel.choseong == 99 && stsyllabel.jungseong == 99 && stsyllabel.jongseong == 0)
		return 0;
	else if (stsyllabel.choseong == 99 && stsyllabel.jungseong != 99 && stsyllabel.jongseong == 0)
		return 1;
	else if (stsyllabel.choseong != 99 && stsyllabel.jungseong == 99 && stsyllabel.jongseong == 0)
		return 2;
	else if (stsyllabel.choseong != 99 && stsyllabel.jungseong != 99 && stsyllabel.jongseong == 0)
		return 3;
	else if (stsyllabel.choseong != 99 && stsyllabel.jungseong != 99 && stsyllabel.jongseong != 0)
		return 4;
}

void ResetEumjeol(CompletedEumjeol& stsyllabel)
{
	stsyllabel.flag = 0;
	stsyllabel.choseong = 99;
	stsyllabel.jungseong = 99;
	stsyllabel.jongseong = 0;
}

void InsertChar(CompletedEumjeol& stsyllabel, char* c)
{
	int CharFlag = SortChar(c); // 1이면 모음, 0이면 자음
	stsyllabel.flag = SortEumjeol(stsyllabel); // 글자 완성도 구분
	if (CharFlag == 0)
	{
		switch (stsyllabel.flag)
		{
		case 0: // 000 -> 초성에 삽입
			stsyllabel.choseong = FindIndex(charset_cho, c);
			break;
		case 1: // 010 -> 리셋 후 초성에 삽입W
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
			//char tmp2char[] = { *c, 0 };
			char* tmp2char = c;
			char* tmpchar = (char*)(charset_jong[stsyllabel.jongseong]);
			//tmpchar += tmp2char;
			int Index = FindIndex(charset_jong, tmp2char);
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
		switch (stsyllabel.flag)
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
			std::string tmp3char = tmp2char.append(c);
			char* tmp4char = (char*)tmp3char.c_str();
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
				stsyllabel.jongseong = FindIndex(charset_jong, tmp4char);
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

	std::wstring strRet = strCurrentContext;

	if (' ' == c)
	{
		strRet.push_back(c);
		return strRet;
	}

	//char InsertedChar = c;
	InsertChar(stsyllabel, &c); // 여기서부터 갑자기 &c 문자열에 잘못된 문자가 있다고 뜸.
	stsyllabel.flag = SortEumjeol(stsyllabel);

	
	if (stsyllabel.flag != 0 && stsyllabel.flag != 1 && stsyllabel.flag != 2)
		strRet.push_back(FindUnicode(stsyllabel));
	else if (stsyllabel.flag == 1)
		strRet.push_back(charset_single[stsyllabel.jungseong + 19]);
	else if (stsyllabel.flag == 2)
		strRet.push_back(charset_single[stsyllabel.choseong]);

	return strRet;
}

int main()
{
	std::setlocale(LC_ALL, "ko_KR.UTF-8");
	std::wstring strContext;

	CompletedEumjeol stsyllabel;

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