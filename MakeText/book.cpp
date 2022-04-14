#include <string>
#include <conio.h>
#include <Windows.h>

size_t ASCII_TO_UTF16(const char* pSrc, size_t tSrcCch, wchar_t* pDest, size_t tDestCch)
{
		size_t tDstLen = 0;
	try
	{
		if (tSrcCch == 0)
			return 0;
		if (NULL == pDest)
		{
			tDstLen = ::MultiByteToWideChar(CP_ACP, 0, pSrc, tSrcCch, NULL, 0);
			if (0 == tDstLen)
				throw std::exception("MultiByteToWideChar failure");
		}
		else
		{
			tDstLen = ::MultiByteToWideChar(CP_ACP, 0, pSrc, tSrcCch, pDest, tDestCch);
			if (0 == tDstLen)
				throw std::exception("MultiByteToWideChar faliure");
		}
	}
	catch (std::exception& e)
	{
		ECODE nRet = ::GetLastError();
		printf("Conv failure(MBS -> WCS) - %s, Error:%d\n", e.what(), nRet());
		return 0;
	}
	return 0;
}