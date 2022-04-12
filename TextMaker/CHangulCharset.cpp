#pragma once

#include "stdafx.h"
#include "ICharSet.h"
#include "StContext.h"
#include "CHangulCharset.h"

int CHangulCharset::AssemUnicode()
{
	return 0;
}

int CHangulCharset::DisassemUnicode()
{
	return 0;
}

int CHangulCharset::CheckChar(int nVirtualKey)
{
	std::string input;
	input.push_back(nVirtualKey);
	char* index;
	for (int i = 0; i < 19; i++)
	{
		index = (char*)charset_cho[i];
		if (strcmp(input.c_str(), index) == 0) {
			return CONSONANT;
		}
	}
	return VOWEL; // 없으면 모음
}

void CHangulCharset::Update(int nVirtualKey, ST_STRING_CONTEXT& context)
{
	int style = CheckChar(nVirtualKey);
	if (style == CONSONANT) // 자음일 경우
	{
		switch (state)
		{
		case BLINK:
		{
			std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
			std::string strPosContext = context.strContext.substr(context.nCursorPos);


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
		case ONE_JONGSEONG_COMB_ABLE:
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
	else if (style == VOWEL) // 모음일 경우
	{
		switch (state)
		{
		case BLINK:
		{
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
		case ONE_JONGSEONG_COMB_ABLE:
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