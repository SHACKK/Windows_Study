#include "stdafx.h"
#include "Hangul.h"

const char* keyboard_char[] = { "q","w","e","r","t","y","u","i","o","p","a","s","d","f","g","h","j","k","l","z","x","c","v","b","n","m" };

const char* charset_cho[] = { "r","R","s","e","E","f","a","q","Q","t","T","d","w","W","c","z","x","v","g" }; // 19개
const char* charset_jung[] = { "k","o","i","O","j","p","u","P","h","hk","ho","hl","y","n","nj","np","nl","b","m","ml","l" }; // 21개
const char* charset_jong[] = { "", "r","R","rt","s","sw","sg","e","f","fr","fa","fq","ft","fx","fv","fg","a","q","qt","t","T","d","w","c","z","x","v","g" }; // 28개
const char* charset_Useless[] = { "F","A","D","C","Z","X","V","G","K","I","J","U","N","B","H","Y","M","L","S" }; // 19개

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
int FindIndex(const char* chararray[], const char* c, int SizeofArray)
{
	int i = 0;
	for (i; i < SizeofArray; i++)
	{
		if (strcmp(c, chararray[i]) == 0)
			return i;
	}
	return -1;
}

// 자음인지 모음인지 구분
int SortChar(const char* c)
{
	char* tmp;
	for (int i = 0; i < 19; i++)
	{
		tmp = (char*)charset_cho[i];
		if (strcmp(c, tmp) == 0) {
			return 0;
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

int InsertChar(CompletedEumjeol& stsyllabel, char c)
{
	std::string ch;
	ch.push_back(c);

	int CharFlag = SortChar(ch.c_str()); // 1이면 모음, 0이면 자음
	stsyllabel.flag = SortEumjeol(stsyllabel); // 글자 완성도 구분

	if (CharFlag == 0) // 자음일 경우
	{
		switch (stsyllabel.flag)
		{
		case 0: // 000 -> 초성에 삽입
			stsyllabel.choseong = FindIndex(charset_cho, ch.c_str(), 19);
			break;
		case 1: // 010 -> 리셋 후 초성에 삽입
			ResetEumjeol(stsyllabel);
			stsyllabel.choseong = FindIndex(charset_cho, ch.c_str(), 19);
			break;
		case 2: // 100 -> 리셋 후 초성에 삽입
			ResetEumjeol(stsyllabel);
			stsyllabel.choseong = FindIndex(charset_cho, ch.c_str(), 19);
			break;
		case 3: // 110 -> 종성에 삽입, 종성이 될 수 없는 경우 리셋 후 삽입
		{
			int Index = FindIndex(charset_jong, ch.c_str(), 28);
			if (Index != -1)
				stsyllabel.jongseong = Index;
			else
			{
				ResetEumjeol(stsyllabel);
				stsyllabel.choseong = FindIndex(charset_cho, ch.c_str(), 19);
			}
			break;
		}

		case 4: // 111 -> 기존의 종성에 있는 문자열과 합쳐서 charset_jong에 있는지 확인하고, 있으면 합쳐서 삽입 / 없으면 리셋 후 초성에 삽입
		{
			std::string tmp = charset_jong[stsyllabel.jongseong];
			tmp.push_back(c);
			int Index = FindIndex(charset_jong, tmp.c_str(), 28);
			if (Index != -1)
				stsyllabel.jongseong = Index;
			else
			{
				ResetEumjeol(stsyllabel);
				stsyllabel.choseong = FindIndex(charset_cho, ch.c_str(), 19);
			}
			break;
		}
		}
	}
	else //모음일 경우
	{
		switch (stsyllabel.flag)
		{
		case 0: // 000 -> 중성에 삽입
			stsyllabel.jungseong = FindIndex(charset_jung, ch.c_str(), 21);
			break;
		case 1: // 010 -> 리셋후 중성에 삽입
			ResetEumjeol(stsyllabel);
			stsyllabel.jungseong = FindIndex(charset_jung, ch.c_str(), 21);
			break;
		case 2: // 100-> 중성에 삽입
			stsyllabel.jungseong = FindIndex(charset_jung, ch.c_str(), 21);
			break;
		case 3:
		{// 110 -> 기존의 중성에 있는 문자열과 합쳐서 charset_jung에 있는지 확인하고, 있으면 합쳐서 삽입 / 없으면 리셋 후 중성에 삽입 
			std::string tmp = charset_jung[stsyllabel.jungseong];
			tmp.push_back(c);

			int Index = FindIndex(charset_jung, tmp.c_str(), 21);
			if (Index == -1)
			{
				ResetEumjeol(stsyllabel);
				stsyllabel.jungseong = FindIndex(charset_jung, ch.c_str(), 21);
			}
			else
			{
				stsyllabel.jungseong = Index;
			}
			break;
		}
		case 4: // 111 -> 종성이 두개인 경우 하나를 빼고 표시 후 다음 글자에 합친다. 종성이 한개인 경우 빼고 표시 후 다음 글자에 합친다. 
		{
			int PrevIndex;
			if (stsyllabel.jongseong == 3 ||
				stsyllabel.jongseong == 5 ||
				stsyllabel.jongseong == 6 ||
				stsyllabel.jongseong == 9 ||
				stsyllabel.jongseong == 10 ||
				stsyllabel.jongseong == 11 ||
				stsyllabel.jongseong == 12 ||
				stsyllabel.jongseong == 13 ||
				stsyllabel.jongseong == 14 ||
				stsyllabel.jongseong == 15 ||
				stsyllabel.jongseong == 18)
			{
				std::string tmp = charset_jong[stsyllabel.jongseong];
				std::string tmp2 = tmp.substr(1, 1);
				//두번째 글자 저장해두고
				PrevIndex = FindIndex(charset_cho, tmp2.c_str(), 19);
				tmp.pop_back();
				stsyllabel.jongseong = FindIndex(charset_jong, tmp.c_str(), 28);
			}
			else
			{
				std::string tmp = charset_jong[stsyllabel.jongseong];
				PrevIndex = FindIndex(charset_cho, tmp.c_str(), 19);
				stsyllabel.jongseong = 0;
			}
			return PrevIndex;
		}
		}
	}
	return -1;
}

// 0 반환 : stsyllabel에 아무것도 없음
// 1 반환 : stsyllabel에 무언가 들어있음
void DeleteEumjeol(CompletedEumjeol& stsyllabel, int status)
{
	switch (status)
	{
	case 0: // 000
		ResetEumjeol(stsyllabel);
	case 1: // 010
	{
		std::string tmp = charset_jung[stsyllabel.jungseong];

		stsyllabel.jungseong = 99;
	}
	case 2: // 100
	{
		stsyllabel.choseong = 99;
	}
	case 3: // 110
	{
		// 9 10 11 14 15 16 19
		if (stsyllabel.jungseong == 9 ||
			stsyllabel.jungseong == 10 ||
			stsyllabel.jungseong == 11 ||
			stsyllabel.jungseong == 14 ||
			stsyllabel.jungseong == 15 ||
			stsyllabel.jungseong == 16 ||
			stsyllabel.jungseong == 19)
		{
			std::string tmp = charset_jung[stsyllabel.jungseong];
			tmp.pop_back();
			int Index = FindIndex(charset_jung, tmp.c_str(), 21);
			stsyllabel.jungseong = Index;
		}
		else
		{
			stsyllabel.jungseong = 99;
		}
	}
	case 4: // 111
	{
		if (stsyllabel.jongseong == 3 ||
			stsyllabel.jongseong == 5 ||
			stsyllabel.jongseong == 6 ||
			stsyllabel.jongseong == 9 ||
			stsyllabel.jongseong == 10 ||
			stsyllabel.jongseong == 11 ||
			stsyllabel.jongseong == 12 ||
			stsyllabel.jongseong == 13 ||
			stsyllabel.jongseong == 14 ||
			stsyllabel.jongseong == 15 ||
			stsyllabel.jongseong == 18)
		{
			std::string tmp = charset_jong[stsyllabel.jongseong];
			tmp.pop_back();
			int Index = FindIndex(charset_jong, tmp.c_str(), 28);
			stsyllabel.jongseong = Index;
		}
		else
		{
			stsyllabel.jongseong = 0;
		}
	}
	}
}

int IsKeyboardChar(char c)
{
	std::string tmp;
	tmp.push_back(c);
	int res = FindIndex(keyboard_char, tmp.c_str(), 26);
	return res;
}
