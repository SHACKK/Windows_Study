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

int CHangulCharset::AssemUnicode(ST_CONSTRUCT stCurrentConstruct)
{
	int nResult;
	if (stCurrentConstruct.choseong == CONSTRUCT_DEFAULT && stCurrentConstruct.jungseong != CONSTRUCT_DEFAULT)		// 초성은 없는데 중성이 있는 경우
		nResult = charset_single[stCurrentConstruct.jungseong + 19];
	else if (stCurrentConstruct.choseong != CONSTRUCT_DEFAULT && stCurrentConstruct.jungseong == CONSTRUCT_DEFAULT)	// 초성은 있는데 중성이 없는경우
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

void CHangulCharset::DeleteChar(int nVirtualKey, ST_STRING_CONTEXT& context, ST_CONSTRUCT& stUnderConstruct)
{
	std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
	std::string strPosContext = context.strContext.substr(context.nCursorPos);

	switch (state)
	{
	case BLINK:
	{
		strPreContext.pop_back();
		context.nCursorPos--;
		break;
	}
	case ONLY_CHOSEONG:
	{
		strPreContext.pop_back();
		context.nCursorPos--;
		stUnderConstruct.choseong = CONSTRUCT_DEFAULT;
		context.strUnderConstruct.clear();

		state = BLINK;
		break;
	}
	case ONLY_JUNGSEONG:
	{
		strPreContext.pop_back();
		context.nCursorPos--;
		stUnderConstruct.jungseong = CONSTRUCT_DEFAULT;
		context.strUnderConstruct.clear();

		state = BLINK;
		break;
	}
	case NO_JONGSEONG_COMB_ABLE_H:
	{
		strPreContext.pop_back();
		stUnderConstruct.jungseong = CONSTRUCT_DEFAULT;
		
	}
	case NO_JONGSEONG_COMB_ABLE_N:
	{}
	case NO_JONGSEONG_COMB_ABLE_M:
	{}
	case ONE_JONGSEONG_COMB_ABLE_R:
	{}
	case ONE_JONGSEONG_COMB_ABLE_S:
	{}
	case ONE_JONGSEONG_COMB_ABLE_F:
	{}
	case ONE_JONGSEONG_COMB_ABLE_Q:
	{}
	case DOUBLE_JONGSEONG:
	{}

	context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

	}
}

void CHangulCharset::Update(int nVirtualKey, ST_STRING_CONTEXT& context)
{
	if (nVirtualKey == 0x0C)
	{
		DeleteChar(nVirtualKey, context, stUnderConstruct);
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
		if (style == CONSONANT) //자음
		{
			stUnderConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
			context.strUnderConstruct.push_back(AssemUnicode(stUnderConstruct));

			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
			state = ONLY_CHOSEONG;
			break;
		}
		else if (style == VOWEL) // 모음
		{
			stUnderConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			context.strUnderConstruct.push_back(AssemUnicode(stUnderConstruct));

			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
			state = ONLY_JUNGSEONG;
			break;
		}
	}
	case ONLY_CHOSEONG: // ex) "ㄱ"
	{
		if (style == CONSONANT) // 자음
		{
			context.strUnderConstruct.clear();
			stUnderConstruct.clear();

			stUnderConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
			context.strUnderConstruct = AssemUnicode(stUnderConstruct);

			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
			state = ONLY_CHOSEONG;
			context.nCursorPos++;
			break;
		}
		else if (style == VOWEL) // 모음일 경우
		{
			context.strUnderConstruct.clear();

			stUnderConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			context.strUnderConstruct = AssemUnicode(stUnderConstruct);
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
	case ONLY_JUNGSEONG: // ex) "ㅏ"
	{
		if (style == CONSONANT) // 자음
		{
			context.strUnderConstruct.clear();
			stUnderConstruct.clear();

			stUnderConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
			context.strUnderConstruct.push_back(AssemUnicode(stUnderConstruct));
			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

			state = ONLY_CHOSEONG;
			context.nCursorPos++;
			break;
		}
		else if (style == VOWEL) // 모음
		{
			context.strUnderConstruct.clear();
			stUnderConstruct.clear();

			stUnderConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			context.strUnderConstruct = AssemUnicode(stUnderConstruct);
			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

			state = ONLY_CHOSEONG;
			context.nCursorPos++;
			break;
		}
	}
	case NO_JONGSEONG_COMB_ABLE_H: // ex) "고"
	{
		if (style == CONSONANT) // 자음
		{
			context.strUnderConstruct.clear();

			if (c == "r")
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_R;
				break;
			}
			else if (c == "s")
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_S;
				break;
			}
			else if (c == "f")
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_F;
				break;
			}
			else if (c == "q")
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_Q;
				break;
			}
			else if (c == "Q" || c == "W" || c == "E")
			{
				stUnderConstruct.clear();
				stUnderConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_CHOSEONG;
				context.nCursorPos++;
				break;
			}
			else
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_UNABLE;
				break;
			}
		}
		else if (style == VOWEL) // 모음
		{
			context.strUnderConstruct.clear();

			if (c == "k" || c == "o" || c == "l")
			{
				std::string strDoubleVowel = charset_jung[stUnderConstruct.jungseong] + c;
				stUnderConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, strDoubleVowel.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = NO_JONGSEONG_COMB_UNABLE;
				break;
			}
			else
			{
				stUnderConstruct.clear();
				stUnderConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_JUNGSEONG;
				context.nCursorPos++;
				break;
			}

		}
	}
	case NO_JONGSEONG_COMB_ABLE_N: // ex) "구"
	{
		if (style == CONSONANT) // 자음
		{
			context.strUnderConstruct.clear();

			if (c == "r")
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct); 
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_R;
				break;
			}
			else if (c == "s")
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_S;
				break;
			}
			else if (c == "f")
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_F;
				break;
			}
			else if (c == "q")
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_Q;
				break;
			}
			else if (c == "Q" || c == "W" || c == "E")
			{
				stUnderConstruct.clear();
				stUnderConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_CHOSEONG;
				context.nCursorPos++;
				break;
			}
			else
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_UNABLE;
				break;
			} 
		}
		else if (style == VOWEL) // 모음
		{
			context.strUnderConstruct.clear();

			if (c == "j" || c == "p" || c == "l")
			{
				std::string strDoubleVowel = charset_jung[stUnderConstruct.jungseong] + c;
				stUnderConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, strDoubleVowel.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = NO_JONGSEONG_COMB_UNABLE;
				break;
			}
			else
			{
				stUnderConstruct.clear();
				stUnderConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_JUNGSEONG;
				context.nCursorPos++;
				break;
			}
		}
	}
	case NO_JONGSEONG_COMB_ABLE_M: // ex) "그"
	{
		if (style == CONSONANT) // 자음
		{
			context.strUnderConstruct.clear();

			if (c == "r")
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_R;
				break;
			}
			if (c == "s")
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_S;
				break;
			}
			if (c == "f")
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_F;
				break;
			}
			if (c == "q")
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_Q;
				break;
			}
			if (c == "Q" || c == "W" || c == "E")
			{
				stUnderConstruct.clear();
				stUnderConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_CHOSEONG;
				context.nCursorPos++;
				break;
			}
			else
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_UNABLE;
				break;
			}
		}
		else if (style == VOWEL) // 모음
		{
			context.strUnderConstruct.clear();

			if (c == "l")
			{
				std::string strDoubleVowel = charset_jung[stUnderConstruct.jungseong] + c;
				stUnderConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, strDoubleVowel.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = NO_JONGSEONG_COMB_UNABLE;
				break;
			}
			else
			{
				stUnderConstruct.clear();
				stUnderConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_JUNGSEONG;
				context.nCursorPos++;
				break;
			}
		}
	}
	case NO_JONGSEONG_COMB_UNABLE: // ex) "규" or "과"
	{
		if (style == CONSONANT)
		{
			context.strUnderConstruct.clear();

			if (c == "r")
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_R;
				break;
			}
			else if (c == "s")
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_S;
				break;
			}
			else if (c == "f")
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_F;
				break;
			}
			else if (c == "q")
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_ABLE_Q;
				break;
			}
			else if (c == "Q" || c == "W" || c == "E")
			{
				stUnderConstruct.clear();
				stUnderConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_CHOSEONG;
				context.nCursorPos++;
				break;
			}
			else
			{
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONE_JONGSEONG_COMB_UNABLE;
				break;
			}
		}
		else if (style == VOWEL) // 모음
		{
			context.strUnderConstruct.clear();
			stUnderConstruct.clear();
			stUnderConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			context.strUnderConstruct = AssemUnicode(stUnderConstruct);
			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

			state = ONLY_JUNGSEONG;
			context.nCursorPos++;
			break;
		}
	}
	case ONE_JONGSEONG_COMB_ABLE_R: // ex) "각"
	{
		if (style == CONSONANT) // 자음
		{
			context.strUnderConstruct.clear();
			if (c == "t")
			{
				std::string strDoubleConsonant = charset_jong[stUnderConstruct.jongseong] + c; //Warning : 숫자로 더해져서 이상한 값이 들어갈 수도 있음
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, strDoubleConsonant.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = DOUBLE_JONGSEONG;
				break;
			}
			else
			{
				stUnderConstruct.clear();
				stUnderConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_CHOSEONG;
				context.nCursorPos++;
				break;
			}
		}
		else if (style == VOWEL) // 모음
		{
			context.strUnderConstruct.clear();

			ST_CONSTRUCT stPreConstruct = DisassemUnicode(context.strUnderConstruct);
			stUnderConstruct.clear();
			stUnderConstruct.choseong = stPreConstruct.jongseong;
			stUnderConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			stPreConstruct.jongseong = 0;

			context.strUnderConstruct = AssemUnicode(stPreConstruct);
			context.strUnderConstruct.push_back(AssemUnicode(stUnderConstruct));

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
	case ONE_JONGSEONG_COMB_ABLE_S: // ex) "간"
	{
		if (style == CONSONANT)
		{
			context.strUnderConstruct.clear();
			if (c == "w" || c =="g")
			{
				std::string strDoubleConsonant = charset_jong[stUnderConstruct.jongseong] + c; //Warning : 숫자로 더해져서 이상한 값이 들어갈 수도 있음
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, strDoubleConsonant.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = DOUBLE_JONGSEONG;
			}
			else
			{
				stUnderConstruct.clear();
				stUnderConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
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
			stUnderConstruct.clear();
			stUnderConstruct.choseong = stPreConstruct.jongseong;
			stUnderConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			stPreConstruct.jongseong = 0;

			context.strUnderConstruct = AssemUnicode(stPreConstruct);
			context.strUnderConstruct.push_back(AssemUnicode(stUnderConstruct));

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
	case ONE_JONGSEONG_COMB_ABLE_F: // ex) "갈"
	{
		if (style == CONSONANT) //자음
		{
			context.strUnderConstruct.clear();
			if (c == "r" || c == "a" || c == "q" || c == "t" || c == "x" || c == "v" || c == "g")
			{
				std::string strDoubleConsonant = charset_jong[stUnderConstruct.jongseong] + c; //Warning : 숫자로 더해져서 이상한 값이 들어갈 수도 있음
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, strDoubleConsonant.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = DOUBLE_JONGSEONG;
			}
			else
			{
				stUnderConstruct.clear();
				stUnderConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = ONLY_CHOSEONG;
				context.nCursorPos++;
			}
			break;
		}
		else if (style == VOWEL) // 모음
		{
			context.strUnderConstruct.clear();

			ST_CONSTRUCT stPreConstruct = DisassemUnicode(context.strUnderConstruct);
			stUnderConstruct.clear();
			stUnderConstruct.choseong = stPreConstruct.jongseong;
			stUnderConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			stPreConstruct.jongseong = 0;

			context.strUnderConstruct = AssemUnicode(stPreConstruct);
			context.strUnderConstruct.push_back(AssemUnicode(stUnderConstruct));

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
	case ONE_JONGSEONG_COMB_ABLE_Q: // ex) "갑"
	{
		if (style == CONSONANT)
		{
			context.strUnderConstruct.clear();
			if (c == "t")
			{
				std::string strDoubleConsonant = charset_jong[stUnderConstruct.jongseong] + c; //Warning : 숫자로 더해져서 이상한 값이 들어갈 수도 있음
				stUnderConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, strDoubleConsonant.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
				context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

				state = DOUBLE_JONGSEONG;
			}
			else
			{
				stUnderConstruct.clear();
				stUnderConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
				context.strUnderConstruct = AssemUnicode(stUnderConstruct);
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
			stUnderConstruct.clear();
			stUnderConstruct.choseong = stPreConstruct.jongseong;
			stUnderConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			stPreConstruct.jongseong = 0;

			context.strUnderConstruct = AssemUnicode(stPreConstruct);
			context.strUnderConstruct.push_back(AssemUnicode(stUnderConstruct));

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
	case ONE_JONGSEONG_COMB_UNABLE: // ex) "갚"
	{
		if (style == CONSONANT) // 자음
		{
			context.strUnderConstruct.clear();
			stUnderConstruct.clear();

			stUnderConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
			context.strUnderConstruct = AssemUnicode(stUnderConstruct);
			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

			state = ONLY_CHOSEONG;
			context.nCursorPos++;
			break;
		}
		else if (style == VOWEL) // 모음
		{
			context.strUnderConstruct.clear();

			ST_CONSTRUCT stPreConstruct = DisassemUnicode(context.strUnderConstruct);
			stUnderConstruct.clear();
			stUnderConstruct.choseong = stPreConstruct.jongseong;
			stUnderConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			stPreConstruct.jongseong = 0;

			context.strUnderConstruct = AssemUnicode(stPreConstruct);
			context.strUnderConstruct.push_back(AssemUnicode(stUnderConstruct));

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
	case DOUBLE_JONGSEONG: // ex) "값"
	{
		if (style == CONSONANT) // 자음
		{
			stUnderConstruct.clear();
			context.strUnderConstruct.clear();

			stUnderConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
			context.strUnderConstruct = AssemUnicode(stUnderConstruct);
			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;

			state = ONLY_CHOSEONG;
			context.nCursorPos++;
			break;
		}
		else if (style == VOWEL) // 모음
		{
			ST_CONSTRUCT stPreConstruct = DisassemUnicode(context.strUnderConstruct);
			stUnderConstruct.clear();

			std::string strOriginConsonant = charset_jong[stPreConstruct.jongseong];
			std::string strNewConsonant = strOriginConsonant.substr(1, 1);
			strOriginConsonant.pop_back();
			stPreConstruct.jongseong = GetIndexNum(charset_jong, NUM_OF_JONGSEONG, strOriginConsonant.c_str());
			stUnderConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, strNewConsonant.c_str());
			stUnderConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());

			context.strUnderConstruct = AssemUnicode(stPreConstruct);
			context.strUnderConstruct.push_back(AssemUnicode(stUnderConstruct));

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