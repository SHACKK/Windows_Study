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

	switch (state)
	{
	case BLINK:
	{
		if (style == CONSONANT) //자음일 경우
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
			std::string strPosContext = context.strContext.substr(context.nCursorPos);
			//std::string strCursorContext;

			stCurrentConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());
			context.strUnderConstruct.push_back(AssemUnicode(stCurrentConstruct));
			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
			state = ONLY_CHOSEONG;
			break;
		}
		else if (style == VOWEL) // 모음일 경우
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
			std::string strPosContext = context.strContext.substr(context.nCursorPos);

			stCurrentConstruct.jungseong = GetIndexNum(charset_jung, NUM_OF_JUNGSEONG, c.c_str());
			context.strUnderConstruct.push_back(AssemUnicode(stCurrentConstruct));
			context.strContext = strPreContext + context.strUnderConstruct + strPosContext;
			state = ONLY_JUNGSEONG;
			break;
		}
		else
		{
			//에러처리
			break;
		}
	}
	case ONLY_CHOSEONG:
	{
		if (style == CONSONANT) // 자음일 경우
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
			std::string strPosContext = context.strContext.substr(context.nCursorPos);

			context.strUnderConstruct.pop_back();
			stCurrentConstruct.clear();

			stCurrentConstruct.choseong = GetIndexNum(charset_cho, NUM_OF_CHOSEONG, c.c_str());


			break;
		}
		else if (style == VOWEL) // 모음일 경우
		{
			break;
		}
	}
	case ONLY_JUNGSEONG:
	{}
	case NO_JONGSEONG_COMB_ABLE_H:
	{}
	case NO_JONGSEONG_COMB_ABLE_M:
	{}
	case NO_JONGSEONG_COMB_ABLE_N:
	{}
	case NO_JONGSEONG_COMB_UNABLE:
	{}
	case ONE_JONGSEONG_COMB_ABLE_F:
	{}
	case ONE_JONGSEONG_COMB_ABLE_Q:
	{}
	case ONE_JONGSEONG_COMB_ABLE_R:
	{}
	case ONE_JONGSEONG_COMB_ABLE_S:
	{}
	case ONE_JONGSEONG_COMB_UNABLE:
	{}
	case DOUBLE_JONGSEONG:
	{}
}