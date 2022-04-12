#pragma once
#include "ICharSet.h"
#include "StContext.h"

class ICharSet
{
public:
	virtual void Update(int nVirtualKey, ST_STRING_CONTEXT& context) = 0;
};
