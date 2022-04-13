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
	return UNICODE_BASE + (((stCurrentConstruct.choseong * 21) + stCurrentConstruct.jungseong) * 28) + stCurrentConstruct.jongseong;
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
int CHangulCharset::CheckChar(std::string c)
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


void CHangulCharset::Update(int nVirtualKey, ST_STRING_CONTEXT& context)
{
	std::string c = StrFromVirtualKey(nVirtualKey);
	int style = CheckChar(c);

	if (style == CONSONANT) // 자음일 경우
	{
		switch (state)
		{
		case BLINK:
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
			std::string strPosContext = context.strContext.substr(context.nCursorPos);
			
			context.strUnderConstruct = charset_cho[GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str())]; // To Check for Logic 
			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
			state = ONLY_CHOSEONG;
			break;
		}
		case ONLY_CHOSEONG:
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
			std::string strPosContext = context.strContext.substr(context.nCursorPos);


			break;
		}
		case ONLY_JUNGSEONG:
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
			std::string strPosContext = context.strContext.substr(context.nCursorPos);


			break;
		}
		case NO_JONGSEONG_COMB_ABLE_H:
		{
			break;
		}
		case NO_JONGSEONG_COMB_ABLE_N:
		{
			break;
		}
		case NO_JONGSEONG_COMB_ABLE_M:
		{
			break;
		}
		case NO_JONGSEONG_COMB_UNABLE:
		{
			break;
		}
		case ONE_JONGSEONG_COMB_ABLE_R:
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
			std::string strPosContext = context.strContext.substr(context.nCursorPos);


			break;
		}
		case ONE_JONGSEONG_COMB_ABLE_S:
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
			std::string strPosContext = context.strContext.substr(context.nCursorPos);


			break;
		}
		case ONE_JONGSEONG_COMB_ABLE_F:
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
			std::string strPosContext = context.strContext.substr(context.nCursorPos);


			break;
		}
		case ONE_JONGSEONG_COMB_ABLE_Q:
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
			std::string strPosContext = context.strContext.substr(context.nCursorPos);


			break;
		}
		case ONE_JONGSEONG_COMB_UNABLE:
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
			std::string strPosContext = context.strContext.substr(context.nCursorPos);


			break;
		}
		case DOUBLE_JONGSEONG:
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
			std::string strPosContext = context.strContext.substr(context.nCursorPos);

			break;
		}
		}
	}
	else if (style == VOWEL) // 모음일 경우
	{
		switch (state)
		{
		case BLINK:
		{

			state = ONLY_JUNGSEONG;
			break;
		}
		case ONLY_CHOSEONG:
		{
			break;
		}
		case ONLY_JUNGSEONG:
		{
			break;
		}
		case NO_JONGSEONG_COMB_ABLE_H:
		{
			break;
		}
		case NO_JONGSEONG_COMB_ABLE_M:
		{
			break;
		}
		case NO_JONGSEONG_COMB_ABLE_N:
		{
			break;
		}
		case NO_JONGSEONG_COMB_UNABLE:
		{
			break;
		}
		case ONE_JONGSEONG_COMB_ABLE_R:
		{
			break;
		}
		case ONE_JONGSEONG_COMB_ABLE_S:
		{
			break;
		}
		case ONE_JONGSEONG_COMB_ABLE_F:
		{
			break;
		}
		case ONE_JONGSEONG_COMB_ABLE_Q:
		{
			break;
		}
		case ONE_JONGSEONG_COMB_UNABLE:
		{
			break;
		}
		case DOUBLE_JONGSEONG:
		{
			break;
		}
		}
}