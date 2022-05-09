#pragma once
#include "pch.h"
#include "CHangulCharset.h"

int CHangulCharset::GetIndexNum(const char* chararray[], int SizeofArray, const char* c)
{
	for (int i = 0; i < SizeofArray; i++)
	{
		if (strcmp(c, chararray[i]) == 0)
			return i;
	}
	return -1;
}

std::string CHangulCharset::StrFromVirtualKey(int VirtualKey)
{

	return std::string();
}

int CHangulCharset::AssemUnicode(ST_CONSTRUCT stCurrentConstruct)
{
	int nResult;
	if (stCurrentConstruct.choseong == CONSTRUCT_DEFAULT && stCurrentConstruct.jungseong != CONSTRUCT_DEFAULT)		// �ʼ��� ���µ� �߼��� �ִ� ���
		nResult = charset_single[stCurrentConstruct.jungseong + 19];
	else if (stCurrentConstruct.choseong != CONSTRUCT_DEFAULT && stCurrentConstruct.jungseong == CONSTRUCT_DEFAULT)	// �ʼ��� �ִµ� �߼��� ���°��
		nResult = charset_single[stCurrentConstruct.choseong];
	else
		nResult = UNICODE_BASE + (((stCurrentConstruct.choseong * NUM_OF_JUNGSEONG) + stCurrentConstruct.jungseong) * NUM_OF_JONGSEONG) + stCurrentConstruct.jongseong;

	return nResult;
}

ST_CONSTRUCT CHangulCharset::DisassemUnicode(std::string strUnderConstruct)
{
	int nValueofUnicode = std::stoi(strUnderConstruct);

	ST_CONSTRUCT stConstruct;
	stConstruct.choseong = 
		(nValueofUnicode - UNICODE_BASE) / (NUM_OF_CHOSEONG * NUM_OF_JUNGSEONG);
	stConstruct.jungseong = 
		(nValueofUnicode - UNICODE_BASE) - ((stConstruct.choseong * (NUM_OF_CHOSEONG * NUM_OF_JUNGSEONG)) / NUM_OF_JONGSEONG);
	stConstruct.jongseong = 
		(nValueofUnicode - UNICODE_BASE) - ((stConstruct.choseong * (NUM_OF_CHOSEONG * NUM_OF_JUNGSEONG)) - (stConstruct.jungseong * NUM_OF_JONGSEONG));

	return stConstruct;
}

// �������� �������� üũ
int CHangulCharset::CheckStr(std::string c)
{
	std::string index = charset_cho[10];

	for (int i = 0; i < 19; i++)
	{
		index = charset_cho[i];
		if (strcmp(c.c_str(), index.c_str()) == 0) {
			return CONSONANT; // ������ ����
		}
	}
	return VOWEL; // ������ ����
}

void CHangulCharset::DeleteChar(int nVirtualKey, ST_STRING_CONTEXT& context)
{
	std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
	std::string strPosContext = context.strContext.substr(context.nCursorPos);
	
	//strPreContext.pop_back();
	context.strUnderConstruct.clear();

	switch (state)
	{
	case BLINK:
	{
		context.nCursorPos--;
		break;
	}
	case ONLY_CHOSEONG:
	{
		stCurrentConstruct.choseong = CONSTRUCT_DEFAULT;
		context.strUnderConstruct.clear();

		state = BLINK;
		break;
	}
	case ONLY_JUNGSEONG:
	{
		stCurrentConstruct.jungseong = CONSTRUCT_DEFAULT;
		context.strUnderConstruct.clear();

		state = BLINK;
		break;
	}
	case NO_JONGSEONG_COMB_ABLE_H:
	{
		stCurrentConstruct.jungseong = CONSTRUCT_DEFAULT;
		context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
		
		state = ONLY_CHOSEONG;
		break;
	}
	case NO_JONGSEONG_COMB_ABLE_N:
	{
		stCurrentConstruct.jungseong = CONSTRUCT_DEFAULT;
		context.strUnderConstruct = AssemUnicode(stCurrentConstruct);

		state = ONLY_CHOSEONG;
		break;
	}
	case NO_JONGSEONG_COMB_ABLE_M:
	{
		stCurrentConstruct.jungseong = CONSTRUCT_DEFAULT;
		context.strUnderConstruct = AssemUnicode(stCurrentConstruct);

		state = ONLY_CHOSEONG;
		break;
	}
	case NO_JONGSEONG_COMB_UNABLE:
	{
		// �߼��� �ΰ��� �����ִ� ���
		if (stCurrentConstruct.jungseong == 9 || stCurrentConstruct.jungseong == 10 || stCurrentConstruct.jungseong == 11) // ��
		{
			std::string strSingleVowel = charset_jung[stCurrentConstruct.jungseong];
			strSingleVowel.pop_back();
			stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, strSingleVowel.c_str());
			
			state = NO_JONGSEONG_COMB_ABLE_H;
		}
		else if (stCurrentConstruct.jungseong == 14 || stCurrentConstruct.jungseong == 15 || stCurrentConstruct.jungseong == 16) // ��
		{
			std::string strSingleVowel = charset_jung[stCurrentConstruct.jungseong];
			strSingleVowel.pop_back();
			stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, strSingleVowel.c_str());

			state = NO_JONGSEONG_COMB_ABLE_N;
		}
		else if (stCurrentConstruct.jungseong == 19) // ��
		{
			std::string strSingleVowel = charset_jung[stCurrentConstruct.jungseong];
			strSingleVowel.pop_back();
			stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, strSingleVowel.c_str());

			state = NO_JONGSEONG_COMB_ABLE_M;
		}
		else // ������
		{
			stCurrentConstruct.jungseong = CONSTRUCT_DEFAULT;
		}
		context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
		break;
	}
	case ONE_JONGSEONG_COMB_ABLE_R:
	{
		stCurrentConstruct.jongseong = 0;
		context.strUnderConstruct = AssemUnicode(stCurrentConstruct);

		if (stCurrentConstruct.jungseong == 8) // �߼��� "��"�� ���
			state = NO_JONGSEONG_COMB_ABLE_H;
		else if (stCurrentConstruct.jungseong == 13) // �߼��� "��"�� ���
			state = NO_JONGSEONG_COMB_ABLE_N;
		else if (stCurrentConstruct.jungseong == 18) // �߼��� "��"�� ���
			state = NO_JONGSEONG_COMB_ABLE_M;
		else
			state = NO_JONGSEONG_COMB_UNABLE;
		break;
	}
	case ONE_JONGSEONG_COMB_ABLE_S:
	{
		stCurrentConstruct.jongseong = 0;
		context.strUnderConstruct = AssemUnicode(stCurrentConstruct);

		if (stCurrentConstruct.jungseong == 8) // �߼��� "��"�� ���
			state = NO_JONGSEONG_COMB_ABLE_H;
		else if (stCurrentConstruct.jungseong == 13) // �߼��� "��"�� ���
			state = NO_JONGSEONG_COMB_ABLE_N;
		else if (stCurrentConstruct.jungseong == 18) // �߼��� "��"�� ���
			state = NO_JONGSEONG_COMB_ABLE_M;
		else
			state = NO_JONGSEONG_COMB_UNABLE;
		break; 
	}
	case ONE_JONGSEONG_COMB_ABLE_F:
	{
		stCurrentConstruct.jongseong = 0;
		context.strUnderConstruct = AssemUnicode(stCurrentConstruct);

		if (stCurrentConstruct.jungseong == 8) // �߼��� "��"�� ���
			state = NO_JONGSEONG_COMB_ABLE_H;
		else if (stCurrentConstruct.jungseong == 13) // �߼��� "��"�� ���
			state = NO_JONGSEONG_COMB_ABLE_N;
		else if (stCurrentConstruct.jungseong == 18) // �߼��� "��"�� ���
			state = NO_JONGSEONG_COMB_ABLE_M;
		else
			state = NO_JONGSEONG_COMB_UNABLE;
		break;
	}
	case ONE_JONGSEONG_COMB_ABLE_Q:
	{
		stCurrentConstruct.jongseong = 0;
		context.strUnderConstruct = AssemUnicode(stCurrentConstruct);

		if (stCurrentConstruct.jungseong == 8) // �߼��� "��"�� ���
			state = NO_JONGSEONG_COMB_ABLE_H;
		else if (stCurrentConstruct.jungseong == 13) // �߼��� "��"�� ���
			state = NO_JONGSEONG_COMB_ABLE_N;
		else if (stCurrentConstruct.jungseong == 18) // �߼��� "��"�� ���
			state = NO_JONGSEONG_COMB_ABLE_M;
		else
			state = NO_JONGSEONG_COMB_UNABLE;
		break;
	}
	case ONE_JONGSEONG_COMB_UNABLE:
	{
		stCurrentConstruct.jongseong = 0;
		context.strUnderConstruct = AssemUnicode(stCurrentConstruct);

		if (stCurrentConstruct.jungseong == 8) // �߼��� "��"�� ���
			state = NO_JONGSEONG_COMB_ABLE_H;
		else if (stCurrentConstruct.jungseong == 13) // �߼��� "��"�� ���
			state = NO_JONGSEONG_COMB_ABLE_N;
		else if (stCurrentConstruct.jungseong == 18) // �߼��� "��"�� ���
			state = NO_JONGSEONG_COMB_ABLE_M;
		else
			state = NO_JONGSEONG_COMB_UNABLE;
		break;
	}
	case DOUBLE_JONGSEONG:
	{
		std::string strSingleConsonant = charset_jong[stCurrentConstruct.jongseong];
		strSingleConsonant.pop_back();
		stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, strSingleConsonant.c_str());
		context.strUnderConstruct = AssemUnicode(stCurrentConstruct);

		if (stCurrentConstruct.jongseong == 1)
			state = ONE_JONGSEONG_COMB_ABLE_R;
		else if (stCurrentConstruct.jongseong == 4)
			state = ONE_JONGSEONG_COMB_ABLE_S;
		else if (stCurrentConstruct.jongseong == 8)
			state = ONE_JONGSEONG_COMB_ABLE_F;
		else if (stCurrentConstruct.jongseong == 17)
			state = ONE_JONGSEONG_COMB_ABLE_Q;
		else
			state = ONE_JONGSEONG_COMB_UNABLE;
		break;
	}
	}
	context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
}

void CHangulCharset::Update(int nVirtualKey, ST_STRING_CONTEXT& context)
{
	if (nVirtualKey == 0x0C)
	{
		DeleteChar(nVirtualKey, context);
		return;
	}
	std::string c = StrFromVirtualKey(nVirtualKey);
	int style = CheckStr(c);

	std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
	std::string strPosContext = context.strContext.substr(context.nCursorPos);

	switch (state)
	{
	case BLINK: // ex) ""
	{
		if (style == CONSONANT) //����
		{
			stCurrentConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
			context.strUnderConstruct.push_back(AssemUnicode(stCurrentConstruct));

			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
			state = ONLY_CHOSEONG;
			break;
		}
		else if (style == VOWEL) // ����
		{
			stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			context.strUnderConstruct.push_back(AssemUnicode(stCurrentConstruct));

			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
			state = ONLY_JUNGSEONG;
			break;
		}
	}
	case ONLY_CHOSEONG: // ex) "��"
	{
		if (style == CONSONANT) // ����
		{
			context.strUnderConstruct.clear();
			stCurrentConstruct.clear();

			stCurrentConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
			context.strUnderConstruct = AssemUnicode(stCurrentConstruct);

			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
			state = ONLY_CHOSEONG;
			context.nCursorPos++;
			break;
		}
		else if (style == VOWEL) // ������ ���
		{
			context.strUnderConstruct.clear();

			stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

			if(c == "h")
			{
				state = NO_JONGSEONG_COMB_ABLE_H;
				break;
			}
			else if (c == "n")
			{
				state = NO_JONGSEONG_COMB_ABLE_N;
				break;
			}
			else if (c == "m")
			{
				state = NO_JONGSEONG_COMB_ABLE_M;
				break;
			}
			else
			{
				state = NO_JONGSEONG_COMB_UNABLE;
				break;
			}
			break;
		}
	}
	case ONLY_JUNGSEONG: // ex) "��"
	{
		if (style == CONSONANT) // ����
		{
			context.strUnderConstruct.clear();
			stCurrentConstruct.clear();

			stCurrentConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
			context.strUnderConstruct.push_back(AssemUnicode(stCurrentConstruct));
			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

			state = ONLY_CHOSEONG;
			context.nCursorPos++;
			break;
		}
		else if (style == VOWEL) // ����
		{
			context.strUnderConstruct.clear();
			stCurrentConstruct.clear();

			stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

			state = ONLY_CHOSEONG;
			context.nCursorPos++;
			break;
		}
	}
	case NO_JONGSEONG_COMB_ABLE_H: // ex) "��"
	{
		if (style == CONSONANT) // ����
		{
			context.strUnderConstruct.clear();

			if (c == "r")
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_R;
				break;
			}
			else if (c == "s")
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_S;
				break;
			}
			else if (c == "f")
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_F;
				break;
			}
			else if (c == "q")
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_Q;
				break;
			}
			else if (c == "Q" || c == "W" || c == "E")
			{
				stCurrentConstruct.clear();
				stCurrentConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_CHOSEONG;
				context.nCursorPos++;
				break;
			}
			else
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_UNABLE;
				break;
			}
		}
		else if (style == VOWEL) // ����
		{
			context.strUnderConstruct.clear();

			if (c == "k" || c == "o" || c == "l")
			{
				std::string strDoubleVowel = charset_jung[stCurrentConstruct.jungseong] + c;
				stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, strDoubleVowel.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = NO_JONGSEONG_COMB_UNABLE;
				break;
			}
			else
			{
				stCurrentConstruct.clear();
				stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_JUNGSEONG;
				context.nCursorPos++;
				break;
			}

		}
	}
	case NO_JONGSEONG_COMB_ABLE_N: // ex) "��"
	{
		if (style == CONSONANT) // ����
		{
			context.strUnderConstruct.clear();

			if (c == "r")
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct); 
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_R;
				break;
			}
			else if (c == "s")
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_S;
				break;
			}
			else if (c == "f")
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_F;
				break;
			}
			else if (c == "q")
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_Q;
				break;
			}
			else if (c == "Q" || c == "W" || c == "E")
			{
				stCurrentConstruct.clear();
				stCurrentConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_CHOSEONG;
				context.nCursorPos++;
				break;
			}
			else
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_UNABLE;
				break;
			} 
		}
		else if (style == VOWEL) // ����
		{
			context.strUnderConstruct.clear();

			if (c == "j" || c == "p" || c == "l")
			{
				std::string strDoubleVowel = charset_jung[stCurrentConstruct.jungseong] + c;
				stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, strDoubleVowel.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = NO_JONGSEONG_COMB_UNABLE;
				break;
			}
			else
			{
				stCurrentConstruct.clear();
				stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_JUNGSEONG;
				context.nCursorPos++;
				break;
			}
		}
	}
	case NO_JONGSEONG_COMB_ABLE_M: // ex) "��"
	{
		if (style == CONSONANT) // ����
		{
			context.strUnderConstruct.clear();

			if (c == "r")
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_R;
				break;
			}
			if (c == "s")
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_S;
				break;
			}
			if (c == "f")
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_F;
				break;
			}
			if (c == "q")
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_Q;
				break;
			}
			if (c == "Q" || c == "W" || c == "E")
			{
				stCurrentConstruct.clear();
				stCurrentConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_CHOSEONG;
				context.nCursorPos++;
				break;
			}
			else
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_UNABLE;
				break;
			}
		}
		else if (style == VOWEL) // ����
		{
			context.strUnderConstruct.clear();

			if (c == "l")
			{
				std::string strDoubleVowel = charset_jung[stCurrentConstruct.jungseong] + c;
				stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, strDoubleVowel.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = NO_JONGSEONG_COMB_UNABLE;
				break;
			}
			else
			{
				stCurrentConstruct.clear();
				stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_JUNGSEONG;
				context.nCursorPos++;
				break;
			}
		}
	}
	case NO_JONGSEONG_COMB_UNABLE: // ex) "��" or "��"
	{
		if (style == CONSONANT)
		{
			context.strUnderConstruct.clear();

			if (c == "r")
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_R;
				break;
			}
			else if (c == "s")
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_S;
				break;
			}
			else if (c == "f")
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_F;
				break;
			}
			else if (c == "q")
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_Q;
				break;
			}
			else if (c == "Q" || c == "W" || c == "E")
			{
				stCurrentConstruct.clear();
				stCurrentConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_CHOSEONG;
				context.nCursorPos++;
				break;
			}
			else
			{
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_UNABLE;
				break;
			}
		}
		else if (style == VOWEL) // ����
		{
			context.strUnderConstruct.clear();
			stCurrentConstruct.clear();
			stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

			state = ONLY_JUNGSEONG;
			context.nCursorPos++;
			break;
		}
	}
	case ONE_JONGSEONG_COMB_ABLE_R: // ex) "��"
	{
		if (style == CONSONANT) // ����
		{
			context.strUnderConstruct.clear();
			if (c == "t")
			{
				std::string strDoubleConsonant = charset_jong[stCurrentConstruct.jongseong] + c; //Warning : ���ڷ� �������� �̻��� ���� �� ���� ����
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, strDoubleConsonant.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = DOUBLE_JONGSEONG;
				break;
			}
			else
			{
				stCurrentConstruct.clear();
				stCurrentConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_CHOSEONG;
				context.nCursorPos++;
				break;
			}
		}
		else if (style == VOWEL) // ����
		{
			context.strUnderConstruct.clear();

			ST_CONSTRUCT stPreConstruct = DisassemUnicode(context.strUnderConstruct);
			stCurrentConstruct.clear();
			stCurrentConstruct.choseong = stPreConstruct.jongseong;
			stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			stPreConstruct.jongseong = 0;

			context.strUnderConstruct = AssemUnicode(stPreConstruct);
			context.strUnderConstruct.push_back(AssemUnicode(stCurrentConstruct));

			if (c == "h")
			{
				state = NO_JONGSEONG_COMB_ABLE_H;
			}
			else if (c == "n")
			{
				state = NO_JONGSEONG_COMB_ABLE_N;
			}
			else if (c == "m")
			{
				state = NO_JONGSEONG_COMB_ABLE_M;
			}
			else
			{
				state = NO_JONGSEONG_COMB_UNABLE;
			}

			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
			context.nCursorPos++;
			break;
		}
	}
	case ONE_JONGSEONG_COMB_ABLE_S: // ex) "��"
	{
		if (style == CONSONANT)
		{
			context.strUnderConstruct.clear();
			if (c == "w" || c =="g")
			{
				std::string strDoubleConsonant = charset_jong[stCurrentConstruct.jongseong] + c; //Warning : ���ڷ� �������� �̻��� ���� �� ���� ����
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, strDoubleConsonant.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = DOUBLE_JONGSEONG;
			}
			else
			{
				stCurrentConstruct.clear();
				stCurrentConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_CHOSEONG;
				context.nCursorPos++;
			}
			break;
		}
		else if (style == VOWEL)
		{
			context.strUnderConstruct.clear();

			ST_CONSTRUCT stPreConstruct = DisassemUnicode(context.strUnderConstruct);
			stCurrentConstruct.clear();
			stCurrentConstruct.choseong = stPreConstruct.jongseong;
			stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			stPreConstruct.jongseong = 0;

			context.strUnderConstruct = AssemUnicode(stPreConstruct);
			context.strUnderConstruct.push_back(AssemUnicode(stCurrentConstruct));

			if (c == "h")
			{
				state = NO_JONGSEONG_COMB_ABLE_H;
			}
			else if (c == "n")
			{
				state = NO_JONGSEONG_COMB_ABLE_N;
			}
			else if (c == "m")
			{
				state = NO_JONGSEONG_COMB_ABLE_M;
			}
			else
			{
				state = NO_JONGSEONG_COMB_UNABLE;
			}

			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
			context.nCursorPos++;
			break;
		}
	}
	case ONE_JONGSEONG_COMB_ABLE_F: // ex) "��"
	{
		if (style == CONSONANT) //����
		{
			context.strUnderConstruct.clear();
			if (c == "r" || c == "a" || c == "q" || c == "t" || c == "x" || c == "v" || c == "g")
			{
				std::string strDoubleConsonant = charset_jong[stCurrentConstruct.jongseong] + c; //Warning : ���ڷ� �������� �̻��� ���� �� ���� ����
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, strDoubleConsonant.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = DOUBLE_JONGSEONG;
			}
			else
			{
				stCurrentConstruct.clear();
				stCurrentConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_CHOSEONG;
				context.nCursorPos++;
			}
			break;
		}
		else if (style == VOWEL) // ����
		{
			context.strUnderConstruct.clear();

			ST_CONSTRUCT stPreConstruct = DisassemUnicode(context.strUnderConstruct);
			stCurrentConstruct.clear();
			stCurrentConstruct.choseong = stPreConstruct.jongseong;
			stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			stPreConstruct.jongseong = 0;

			context.strUnderConstruct = AssemUnicode(stPreConstruct);
			context.strUnderConstruct.push_back(AssemUnicode(stCurrentConstruct));

			if (c == "h")
			{
				state = NO_JONGSEONG_COMB_ABLE_H;
			}
			else if (c == "n")
			{
				state = NO_JONGSEONG_COMB_ABLE_N;
			}
			else if (c == "m")
			{
				state = NO_JONGSEONG_COMB_ABLE_M;
			}
			else
			{
				state = NO_JONGSEONG_COMB_UNABLE;
			}

			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
			context.nCursorPos++;
			break;
		}
	}
	case ONE_JONGSEONG_COMB_ABLE_Q: // ex) "��"
	{
		if (style == CONSONANT)
		{
			context.strUnderConstruct.clear();
			if (c == "t")
			{
				std::string strDoubleConsonant = charset_jong[stCurrentConstruct.jongseong] + c; //Warning : ���ڷ� �������� �̻��� ���� �� ���� ����
				stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, strDoubleConsonant.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = DOUBLE_JONGSEONG;
			}
			else
			{
				stCurrentConstruct.clear();
				stCurrentConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_CHOSEONG;
				context.nCursorPos++;
			}
			break;
		}
		else if (style == VOWEL)
		{
			context.strUnderConstruct.clear();

			ST_CONSTRUCT stPreConstruct = DisassemUnicode(context.strUnderConstruct);
			stCurrentConstruct.clear();
			stCurrentConstruct.choseong = stPreConstruct.jongseong;
			stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			stPreConstruct.jongseong = 0;

			context.strUnderConstruct = AssemUnicode(stPreConstruct);
			context.strUnderConstruct.push_back(AssemUnicode(stCurrentConstruct));

			if (c == "h")
			{
				state = NO_JONGSEONG_COMB_ABLE_H;
			}
			else if (c == "n")
			{
				state = NO_JONGSEONG_COMB_ABLE_N;
			}
			else if (c == "m")
			{
				state = NO_JONGSEONG_COMB_ABLE_M;
			}
			else
			{
				state = NO_JONGSEONG_COMB_UNABLE;
			}

			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
			context.nCursorPos++;
			break;
		}
	}
	case ONE_JONGSEONG_COMB_UNABLE: // ex) "��"
	{
		if (style == CONSONANT) // ����
		{
			context.strUnderConstruct.clear();
			stCurrentConstruct.clear();

			stCurrentConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
			context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

			state = ONLY_CHOSEONG;
			context.nCursorPos++;
			break;
		}
		else if (style == VOWEL) // ����
		{
			context.strUnderConstruct.clear();

			ST_CONSTRUCT stPreConstruct = DisassemUnicode(context.strUnderConstruct);
			stCurrentConstruct.clear();
			stCurrentConstruct.choseong = stPreConstruct.jongseong;
			stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			stPreConstruct.jongseong = 0;

			context.strUnderConstruct = AssemUnicode(stPreConstruct);
			context.strUnderConstruct.push_back(AssemUnicode(stCurrentConstruct));

			if (c == "h")
			{
				state = NO_JONGSEONG_COMB_ABLE_H;
			}
			else if (c == "n")
			{
				state = NO_JONGSEONG_COMB_ABLE_N;
			}
			else if (c == "m")
			{
				state = NO_JONGSEONG_COMB_ABLE_M;
			}
			else
			{
				state = NO_JONGSEONG_COMB_UNABLE;
			}

			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
			context.nCursorPos++;
			break;
		}
	}
	case DOUBLE_JONGSEONG: // ex) "��"
	{
		if (style == CONSONANT) // ����
		{
			stCurrentConstruct.clear();
			context.strUnderConstruct.clear();

			stCurrentConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
			context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

			state = ONLY_CHOSEONG;
			context.nCursorPos++;
			break;
		}
		else if (style == VOWEL) // ����
		{
			ST_CONSTRUCT stPreConstruct = DisassemUnicode(context.strUnderConstruct);
			stCurrentConstruct.clear();

			std::string strOriginConsonant = charset_jong[stPreConstruct.jongseong];
			std::string strNewConsonant = strOriginConsonant.substr(1, 1);
			strOriginConsonant.pop_back();
			stPreConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, strOriginConsonant.c_str());
			stCurrentConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, strNewConsonant.c_str());
			stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());

			context.strUnderConstruct = AssemUnicode(stPreConstruct);
			context.strUnderConstruct.push_back(AssemUnicode(stCurrentConstruct));

			if (c == "h")
			{
				state = NO_JONGSEONG_COMB_ABLE_H;
			}
			else if (c == "n")
			{
				state = NO_JONGSEONG_COMB_ABLE_N;
			}
			else if (c == "m")
			{
				state = NO_JONGSEONG_COMB_ABLE_M;
			}
			else
			{
				state = NO_JONGSEONG_COMB_UNABLE;
			}

			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
			context.nCursorPos++;
			break;
		}
	}
}