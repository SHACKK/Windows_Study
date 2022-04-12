#pragma once

#include "stdafx.h"
#include "ICharSet.h"
#include "StContext.h"
#include "CAsciiCharset.h"

void CAsciiCharset::Update(int nVirtualKey, ST_STRING_CONTEXT& context)
{
	if ('0' <= nVirtualKey && nVirtualKey <= '9')
	{
		std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
		std::string strPosContext = context.strContext.substr(context.nCursorPos);

		char szNewContext[2] = { (char)nVirtualKey, 0 };
		context.strContext = strPreContext + szNewContext + strPosContext;
		context.nCursorPos++;
	}
	if ('A' <= nVirtualKey && nVirtualKey <= 'Z')
	{
		std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
		std::string strPosContext = context.strContext.substr(context.nCursorPos);

		char szNewContext[2] = { (char)nVirtualKey, 0 };
		context.strContext = strPreContext + szNewContext + strPosContext;
		context.nCursorPos++;
	}
	if ('a' <= nVirtualKey && nVirtualKey <= 'z')
	{
		std::string strPreContext = context.strContext.substr(0, context.nCursorPos);
		std::string strPosContext = context.strContext.substr(context.nCursorPos);

		char szNewContext[2] = { (char)nVirtualKey, 0 };
		context.strContext = strPreContext + szNewContext + strPosContext;
		context.nCursorPos++;
	}
}
