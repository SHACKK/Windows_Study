#include "pch.h"
#include "CStringBuilder.h"

CStringBuilder::CStringBuilder(void)
{
	m_mapCharset.insert(std::make_pair(CHARSET_ASCII, new CAsciiCharSet()));
	m_mapCharset.insert(std::make_pair(CHARSET_HANGUL, new CHangulCharset()));
    m_pCurCharset = m_mapCharset[CHARSET_ASCII];

    Clear();
}

void CStringBuilder::Clear()
{
	m_Context.strContext.clear();
	m_Context.nCursorPos = 0;
    m_Context.strUnderConstruct.clear();
	m_strContext.clear();
	m_strContextWithCursor.clear();
}

void CStringBuilder::BuildContext(std::list<ST_KEYSTATE> listKeys, bool bIsShiftEnabled, bool bIsCapsLockEnabled)
{
	for (ST_KEYSTATE key : listKeys)
	{
		if (!key.bPressed)
			continue;

		switch (key.nVirtKey)
		{
		case VK_HANGUL:
			bHangulFlag = !bHangulFlag;

			if (bHangulFlag == true)
				m_pCurCharset = m_mapCharset[CHARSET_HANGUL];
			else
				m_pCurCharset = m_mapCharset[CHARSET_ASCII];
			break;
		case VK_RETURN:
			Clear();
			m_pCurCharset->Clear();
			break;
		default:
			m_pCurCharset->Update(key.nVirtKey, m_Context, bIsShiftEnabled, bIsCapsLockEnabled);
			break;
		}
	}

	m_strContext = m_Context.strContext;

	std::wstring strLeft = m_Context.strContext.substr(0, m_Context.nCursorPos);
	std::wstring strRight = m_Context.strContext.substr(m_Context.nCursorPos);
	m_strContextWithCursor = strLeft + L"|" + strRight;
}

std::wstring CStringBuilder::GetContext(void)
{
	return m_strContext;
}

std::wstring CStringBuilder::GetContextWithCursor(void)
{
	return m_strContextWithCursor;
}
