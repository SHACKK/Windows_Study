#pragma once

class ICharSet
{
public:
	virtual void Update(int nVirtualKey, ST_STRING_CONTEXT& context) = 0;
};