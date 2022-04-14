#pragma once
#include "stdafx.h"
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

int CHangulCharset::AssemUnicode(CONSTRUCT stCurrentConstruct)
{
	int nResult;
	if (stCurrentConstruct.choseong == CONSTRUCT_DEFAULT)
	{
		nResult = charset_single[stCurrentConstruct.jungseong + 19];
	}
	else if (stCurrentConstruct.jungseong == CONSTRUCT_DEFAULT)
	{
		nResult = charset_single[stCurrentConstruct.choseong];
	}

	nResult = UNICODE_BASE + (((stCurrentConstruct.choseong * NUM_OF_JUNGSEONG) + stCurrentConstruct.jungseong) * NUM_OF_JONGSEONG) + stCurrentConstruct.jongseong;
	return nResult;
}

CONSTRUCT CHangulCharset::DisassemUnicode(std::string strUnderConstruct)
{
	int nValueofUnicode = std::stoi(strUnderConstruct);

	CONSTRUCT stConstruct;
	stConstruct.choseong = 
		(nValueofUnicode - UNICODE_BASE) / (NUM_OF_CHOSEONG * NUM_OF_JUNGSEONG);
	stConstruct.jungseong = 
		(nValueofUnicode - UNICODE_BASE) - ((stConstruct.choseong * (NUM_OF_CHOSEONG * NUM_OF_JUNGSEONG)) / NUM_OF_JONGSEONG);
	stConstruct.jongseong = 
		(nValueofUnicode - UNICODE_BASE) - ((stConstruct.choseong * (NUM_OF_CHOSEONG * NUM_OF_JUNGSEONG)) - (stConstruct.jungseong * NUM_OF_JONGSEONG));

	return stConstruct;
}

// 자음인지 모음인지 체크
int CHangulCharset::CheckStr(std::string c)
{
	std::string index = charset_cho[10];

	for (int i = 0; i < 19; i++)
	{
		index = charset_cho[i];
		if (strcmp(c.c_str(), index.c_str()) == 0) {
			return CONSONANT; // 있으면 자음
		}
	}
	return VOWEL; // 없으면 모음
}

E_CONSONANT_TYPE CHangulCharset::CheckConsonantType(const char* consonant)
{
	return E_CONSONANT_TYPE();
}

E_VOWEL_TYPE CHangulCharset::CheckVowelType(const char* vowel)
{
	return E_VOWEL_TYPE();
}


void CHangulCharset::Update(int nVirtualKey, ST_STRING_CONTEXT& context)
{
	std::string c = StrFromVirtualKey(nVirtualKey);
	int style = CheckStr(c);

	switch (state)
	{
	case BLINK: // ex) ""
	{
		if (style == CONSONANT) //자음
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
			std::string strPosContext = context.strContext.substr(context.nCursorPos);

			stCurrentConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
			context.strUnderConstruct.push_back(AssemUnicode(stCurrentConstruct));

			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
			state = ONLY_CHOSEONG;
			break;
		}
		else if (style == VOWEL) // 모음
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
			std::string strPosContext = context.strContext.substr(context.nCursorPos);

			stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			context.strUnderConstruct.push_back(AssemUnicode(stCurrentConstruct));

			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
			state = ONLY_JUNGSEONG;
			break;
		}
	}
	case ONLY_CHOSEONG: // ex) "ㄱ"
	{
		if (style == CONSONANT) // 자음
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos - 1);
			std::string strPosContext = context.strContext.substr(context.nCursorPos);

			context.strUnderConstruct.clear();
			stCurrentConstruct.clear();

			stCurrentConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
			context.strUnderConstruct = AssemUnicode(stCurrentConstruct);

			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
			context.nCursorPos++;
			state = ONLY_CHOSEONG;
			break;
		}
		else if (style == VOWEL) // 모음일 경우
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos - 1);
			std::string strPosContext = context.strContext.substr(context.nCursorPos);

			context.strUnderConstruct.clear();

			stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			context.strUnderConstruct = AssemUnicode(stCurrentConstruct);

			switch (CheckVowelType(c.c_str()))
			{
			case H:
			{
				state = NO_JONGSEONG_COMB_ABLE_H;
				break;
			}
			case N:
			{
				state = NO_JONGSEONG_COMB_ABLE_N;
				break;
			}
			case M:
			{
				state = NO_JONGSEONG_COMB_ABLE_M;
				break;
			}
			case NORMAL:
			{
				state = NO_JONGSEONG_COMB_UNABLE;
				break;
			}
			}
		}
	}
	case ONLY_JUNGSEONG: // ex) "ㅏ"
	{
		if (style == CONSONANT) // 자음
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos - 1);
			std::string strPosConetxt = context.strContext.substr(context.nCursorPos);

			context.strUnderConstruct.clear();
			stCurrentConstruct.clear();

			stCurrentConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
			context.strUnderConstruct.push_back(AssemUnicode(stCurrentConstruct));

			context.strContext = strPreContext + context.strUnderConstruct + strPosConetxt;
			state = ONLY_CHOSEONG;
			context.nCursorPos++;
		}
		else if (style == VOWEL) // 모음
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos - 1);
			std::string strPosContext = context.strContext.substr(context.nCursorPos);

			context.strUnderConstruct.clear();
			stCurrentConstruct.clear();

			stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			context.strUnderConstruct = AssemUnicode(stCurrentConstruct);

			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
			state = ONLY_CHOSEONG;
			context.nCursorPos++;
		}
	}
	case NO_JONGSEONG_COMB_ABLE_H: // ex) "고"
	{
		if (style == CONSONANT) // 자음
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos - 1);
			std::string strPosContext = context.strContext.substr(context.nCursorPos);

			context.strUnderConstruct.clear();

			stCurrentConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
			context.strUnderConstruct = AssemUnicode(stCurrentConstruct);

			switch (CheckConsonantType(c.c_str()))
			{
			case R:
			{
				state = ONE_JONGSEONG_COMB_ABLE_R;
				break;
			}
			case S:
			{
				state = ONE_JONGSEONG_COMB_ABLE_S;
				break;
			}
			case F:
			{
				state = ONE_JONGSEONG_COMB_ABLE_F;
				break;
			}
			case Q:
			{
				state = ONE_JONGSEONG_COMB_ABLE_Q;
				break;
			}
			case NORMAL:
			{
				state = ONE_JONGSEONG_COMB_UNABLE;
				break;
			}
			}
		}
		else if (style == VOWEL) // 모음
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos - 1);
			std::string strPoscontext = context.strContext.substr(context.nCursorPos);

			context.strUnderConstruct.clear();

			if (c == "k" || c == "o" || c == "l")
			{
				std::string strDoubleVowel = charset_jung[stCurrentConstruct.jungseong] + c;
				stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, strDoubleVowel.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPoscontext;
				state = NO_JONGSEONG_COMB_UNABLE;
				break;
			}
			else
			{
				stCurrentConstruct.clear();
				stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stCurrentConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPoscontext;
				state = ONLY_JUNGSEONG;
				break;
			}

		}
	}
	case NO_JONGSEONG_COMB_ABLE_N: // ex) "구"
	{}
	case NO_JONGSEONG_COMB_ABLE_M: // ex) "그"
	{}
	case NO_JONGSEONG_COMB_UNABLE: // ex) "규"
	{}
	case ONE_JONGSEONG_COMB_ABLE_F: // ex) "갈"
	{}
	case ONE_JONGSEONG_COMB_ABLE_Q: // ex) "갑"
	{}
	case ONE_JONGSEONG_COMB_ABLE_R: // ex) "각"
	{}
	case ONE_JONGSEONG_COMB_ABLE_S: // ex) "간"
	{}
	case ONE_JONGSEONG_COMB_UNABLE: // ex) "갚"
	{}
	case DOUBLE_JONGSEONG: // ex) "값"
	{}
}