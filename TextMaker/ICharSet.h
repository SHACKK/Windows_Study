#pragma once
#include "CAsciiCharset.h"
#include "CHangulCharset.h"

class ICharSet
{
public:
	virtual void Update(int nVirtualKey, ST_STRING_CONTEXT& context) = 0;
};