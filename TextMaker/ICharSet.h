#pragma once

class ICharSet
{
public:
	virtual void Update(int nVirtualKey, ST_STRING_CONTEXT& context) = 0;
};

class CAsciiCharset : public ICharSet
{
	void Update(int nVirtualKey, ST_STRING_CONTEXT& context);
};

class CHangulCharset : public ICharSet
{
	void Update(int nVirtualKey, ST_STRING_CONTEXT& context);
};

