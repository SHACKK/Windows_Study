#pragma once
#include "CAsciiCharset.h"
#include "CHangulCharset.h"

//#define VK_HANGUL 0x0001
//#define VK_ENGLISH 0x0002

enum E_CHARSET_TYPE
{
	CHARSET_ASCII,
	CHARSET_HANGUL
};

class CStringBuilder
{
	std::map<E_CHARSET_TYPE, ICharSet*> m_mapCharset;
	ST_STRING_CONTEXT m_Context;

	ICharSet* m_pCurCharset;

public:
	CStringBuilder(void);
	void Clear();
	std::string Input(int nVirtualKey);
};
