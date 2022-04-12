#include "stdafx.h"
#include "Hangul.h"

int Hangul::FindUnicode(CompletedEumjeol stsyllabel)
{
	return 44032 + ((stsyllabel.choseong * 21) + stsyllabel.jungseong) * 28 + stsyllabel.jongseong;
}

int Hangul::FindIndex(const char* chararray[], const char* c, int SizeofArray)
{
	int i = 0;
	for (i; i < SizeofArray; i++)
	{
		if (strcmp(c, chararray[i]) == 0)
			return i;
	}
	return -1;
}

int Hangul::SortChar(const char* c)
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

int Hangul::SortEumjeol(CompletedEumjeol stsyllabel)
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

void Hangul::ResetEumjeol(CompletedEumjeol& stsyllabel)
{
	stsyllabel.flag = 0;
	stsyllabel.choseong = 99;
	stsyllabel.jungseong = 99;
	stsyllabel.jongseong = 0;
}

int Hangul::InsertChar(CompletedEumjeol& stsyllabel, char c)
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

void Hangul::DeleteEumjeol(CompletedEumjeol& stsyllabel, int status)
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

std::wstring Hangul::AssembleHangul(std::wstring strCurrentContext, char c)
{
	std::wstring strRet = strCurrentContext;

	if ('\r' == c)
	{
		//ResetEumjeol(stsyllabel);
		return strCurrentContext;
	}

	if ('\b' == c)
	{
		int status = SortEumjeol(stsyllabel);

		if (status == 0) // �̹� �ƹ��͵� ������
			return strCurrentContext = strCurrentContext.substr(0, strCurrentContext.length() - 1);

		DeleteEumjeol(stsyllabel, status);
		status = SortEumjeol(stsyllabel);

		strCurrentContext = strCurrentContext.substr(0, strCurrentContext.length() - 1);
		strRet = strCurrentContext;

		switch (status)
		{
		case 0:
		{
			return strRet;
		}
		case 1:
		{
			strRet.push_back(charset_single[stsyllabel.jungseong + 19]);
			return strRet;
		}
		case 2:
		{
			strRet.push_back(charset_single[stsyllabel.choseong]);
			return strRet;
		}
		case 3:
		{
			strRet.push_back(FindUnicode(stsyllabel));
			return strRet;
		}
		case 4:
		{
			strRet.push_back(FindUnicode(stsyllabel));
			return strRet;
		}
		}

	}

	if (' ' == c)
	{
		ResetEumjeol(stsyllabel);
		strRet.push_back(c);
		return strRet;
	}

	// ������ �ʴ� ���� ó��
	std::string tmp;
	tmp.push_back(c);
	if (FindIndex(charset_Useless, tmp.c_str(), 19) != -1) // charset_Useless[] �� �ִٸ�
	{
		c ^= 32;
	}

	int res = InsertChar(stsyllabel, c);

	stsyllabel.flag = SortEumjeol(stsyllabel);

	if (res == -1)
	{
		if (stsyllabel.flag != 0 && stsyllabel.flag != 1 && stsyllabel.flag != 2)
		{
			strRet.pop_back();
			strRet.push_back(FindUnicode(stsyllabel));
		}
		else if (stsyllabel.flag == 1)
			strRet.push_back(charset_single[stsyllabel.jungseong + 19]);
		else if (stsyllabel.flag == 2)
			strRet.push_back(charset_single[stsyllabel.choseong]);
	}
	else
	{
		strRet.pop_back();
		strRet.push_back(FindUnicode(stsyllabel));
		ResetEumjeol(stsyllabel);
		stsyllabel.choseong = res;
		std::string ch;
		ch.push_back(c);
		stsyllabel.jungseong = FindIndex(charset_jung, ch.c_str(), 21);
		strRet.push_back(FindUnicode(stsyllabel));
	}

	return strRet;
}
