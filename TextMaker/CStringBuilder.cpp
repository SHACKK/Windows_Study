#include "stdafx.h"
#include "CStringBuilder.h"

CStringBuilder::CStringBuilder(void)
{
	m_mapCharset.insert(std::make_pair(CHARSET_ASCII, new CAsciiCharset()));
	m_mapCharset.insert(std::make_pair(CHARSET_HANGUL, new CHangulCharset()));
    m_pCurCharset = m_mapCharset[CHARSET_ASCII];

    Clear();
}

void CStringBuilder::Clear()
{
	m_Context.strContext = "";
	m_Context.nCursorPos = 0;
	m_Context.strUnderConstruct = "";
}

std::string CStringBuilder::Input(int nVirtualKey)
{
    {
        switch (nVirtualKey)
        {
        case VK_HANGUL:
            m_pCurCharset = m_mapCharset[CHARSET_HANGUL];
            break;

        case VK_ENGLISH:
            m_pCurCharset = m_mapCharset[CHARSET_ASCII];
            break;

        default:
            m_pCurCharset->Update(nVirtualKey, m_Context);
            break;
        }

        return m_Context.strContext;
    }
}
