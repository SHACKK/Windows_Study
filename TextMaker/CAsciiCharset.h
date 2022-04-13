#pragma once
#include "ICharSet.h"

class CAsciiCharset : public ICharSet
{
	void Update(int nVirtualKey, ST_STRING_CONTEXT& context);
};