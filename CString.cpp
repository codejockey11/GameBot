#include "CString.h"

/*
*/
CString::CString()
{
	memset(this, 0x00, sizeof(CString));
}

/*
*/
CString::CString(UINT length)
{
	memset(this, 0x00, sizeof(CString));


	m_length = length;

	m_text = new char[(size_t)m_length + 1];

	memset((void*)m_text, 0x00, (size_t)m_length + 1);
}

/*
*/
CString::CString(const char* c)
{
	memset(this, 0x00, sizeof(CString));

	if (c == nullptr)
	{
		return;
	}

	
	m_length = (UINT)strlen(c);

	m_text = new char[(size_t)m_length + 1];

	memset((void*)m_text, 0x00, (size_t)m_length + 1);

	memcpy((void*)m_text, (void*)c, m_length);
}

/*
*/
CString::CString(wchar_t* wc)
{
	memset(this, 0x00, sizeof(CString));

	if (wc == nullptr)
	{
		return;
	}
	
	m_length = (UINT)wcslen(wc);

	m_text = new char[m_length + 1];

	memset((void*)m_text, 0x00, (size_t)m_length + 1);

	wcstombs_s(&gfsize, m_text, (size_t)m_length + 1, wc, (size_t)m_length);
}

/*
*/
CString::~CString()
{
	delete[] m_wtext;
	delete[] m_tempText;
	delete[] m_text;
}

/*
*/
void CString::Clear()
{
	memset((void*)m_text, 0x00, m_length);
}

/*
*/
int CString::Compare(const char* c)
{
	return strcmp(m_text, c);
}

/*
*/
void CString::Concat(const char* c)
{
	UINT tempLength = m_length + (UINT)strlen(c);

	char* tempText = new char[tempLength + 1];

	memset((void*)tempText, 0x00, (size_t)tempLength + 1);

	memcpy_s((void*)tempText, tempLength, (void*)m_text, (rsize_t)m_length);

	memcpy_s((void*)&tempText[m_length], tempLength, (void*)c, (size_t)strlen(c));

	delete[] m_text;

	m_text = tempText;

	m_length = tempLength;
}

/*
*/
void CString::Format(const char* format, ...)
{
	va_list	argptr;

	va_start(argptr, format);

	Clear();

	vsprintf_s(m_text, m_length, format, argptr);

	va_end(argptr);
}

/*
*/
UINT CString::GetLength()
{
	return m_length;
}

/*
*/
char* CString::GetText()
{
	return m_text;
}

/*
*/
wchar_t* CString::GetWText()
{
	delete[] m_wtext;

	m_wlength = (m_length + 1) * (UINT)sizeof(WORD);

	m_wtext = new wchar_t[(size_t)m_wlength + 1];

	memset((void*)m_wtext, 0x00, (size_t)m_wlength + 1);

	mbstowcs_s(&gfsize, m_wtext, m_wlength, m_text, strlen(m_text));

	return m_wtext;
}

/*
*/
char* CString::MBToWide(const wchar_t* wc)
{
	delete[] m_tempText;

	size_t length = (size_t)wcslen(wc);

	m_tempText = new char[length + 1];

	memset((void*)m_tempText, 0x00, length + 1);

	wcstombs_s(&gfsize, m_tempText, length, wc, wcslen(wc));

	return m_tempText;
}

/*
*/
bool CString::Search(const char* c)
{
	size_t cl = strlen(c);

	int count = 0;

	for (UINT i = 0; i < m_length; i++)
	{
		count = 0;

		for (size_t ii = 0; ii < cl; ii++)
		{
			if ((i + ii) >= m_length)
			{
				break;
			}

			if (m_text[i + ii] == c[ii])
			{
				count++;
			}
			else
			{
				break;
			}
		}

		if (count == cl)
		{
			return true;
		}
	}

	return false;
}

/*
*/
UINT CString::StrLen()
{
	return (UINT)strlen(m_text);
}