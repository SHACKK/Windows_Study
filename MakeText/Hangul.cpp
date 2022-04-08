#include "stdafx.h"
#include "Hangul.h"

const char* keyboard_char[] = { "q","w","e","r","t","y","u","i","o","p","a","s","d","f","g","h","j","k","l","z","x","c","v","b","n","m" };

const char* charset_cho[] = { "r","R","s","e","E","f","a","q","Q","t","T","d","w","W","c","z","x","v","g" }; // 19��
const char* charset_jung[] = { "k","o","i","O","j","p","u","P","h","hk","ho","hl","y","n","nj","np","nl","b","m","ml","l" }; // 21��
const char* charset_jong[] = { "", "r","R","rt","s","sw","sg","e","f","fr","fa","fq","ft","fx","fv","fg","a","q","qt","t","T","d","w","c","z","x","v","g" }; // 28��
const char* charset_Useless[] = { "F","A","D","C","Z","X","V","G","K","I","J","U","N","B","H","Y","M","L","S" }; // 19��

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

// �迭���� ���° �ε����� �������� ����
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

// �������� �������� ����
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
	return 1; // ������ ����
}

// ���ڰ� ������ �ϼ��Ǿ�����
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

	int CharFlag = SortChar(ch.c_str()); // 1�̸� ����, 0�̸� ����
	stsyllabel.flag = SortEumjeol(stsyllabel); // ���� �ϼ��� ����

	if (CharFlag == 0) // ������ ���
	{
		switch (stsyllabel.flag)
		{
		case 0: // 000 -> �ʼ��� ����
			stsyllabel.choseong = FindIndex(charset_cho, ch.c_str(), 19);
			break;
		case 1: // 010 -> ���� �� �ʼ��� ����
			ResetEumjeol(stsyllabel);
			stsyllabel.choseong = FindIndex(charset_cho, ch.c_str(), 19);
			break;
		case 2: // 100 -> ���� �� �ʼ��� ����
			ResetEumjeol(stsyllabel);
			stsyllabel.choseong = FindIndex(charset_cho, ch.c_str(), 19);
			break;
		case 3: // 110 -> ������ ����, ������ �� �� ���� ��� ���� �� ����
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

		case 4: // 111 -> ������ ������ �ִ� ���ڿ��� ���ļ� charset_jong�� �ִ��� Ȯ���ϰ�, ������ ���ļ� ���� / ������ ���� �� �ʼ��� ����
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
	else //������ ���
	{
		switch (stsyllabel.flag)
		{
		case 0: // 000 -> �߼��� ����
			stsyllabel.jungseong = FindIndex(charset_jung, ch.c_str(), 21);
			break;
		case 1: // 010 -> ������ �߼��� ����
			ResetEumjeol(stsyllabel);
			stsyllabel.jungseong = FindIndex(charset_jung, ch.c_str(), 21);
			break;
		case 2: // 100-> �߼��� ����
			stsyllabel.jungseong = FindIndex(charset_jung, ch.c_str(), 21);
			break;
		case 3:
		{// 110 -> ������ �߼��� �ִ� ���ڿ��� ���ļ� charset_jung�� �ִ��� Ȯ���ϰ�, ������ ���ļ� ���� / ������ ���� �� �߼��� ���� 
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
		case 4: // 111 -> ������ �ΰ��� ��� �ϳ��� ���� ǥ�� �� ���� ���ڿ� ��ģ��. ������ �Ѱ��� ��� ���� ǥ�� �� ���� ���ڿ� ��ģ��. 
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
				//�ι�° ���� �����صΰ�
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

// 0 ��ȯ : stsyllabel�� �ƹ��͵� ����
// 1 ��ȯ : stsyllabel�� ���� �������
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
