#include "CErrorLog.h"

/*
*/
CErrorLog::CErrorLog()
{
	memset(this, 0x00, sizeof(CErrorLog));
}

/*
*/
CErrorLog::CErrorLog(const char* filename)
{
	memset(this, 0x00, sizeof(CErrorLog));

	errno_t err = fopen_s(&m_file, filename, "w");

	if (err != 0)
	{
		return;
	}

	m_comError = new CComError();

	m_wsError = new CWinsockError();

	m_dxgiError = new CDXGIError();
}

/*
*/
CErrorLog::~CErrorLog()
{
	delete m_comError;
	delete m_wsError;
	delete m_dxgiError;

	if (m_file)
	{
		fclose(m_file);
	}
}

/*
*/
void CErrorLog::WriteBytes(bool time, const char* bytes)
{
	for (int i = 0; i < strlen(bytes); i++)
	{
		memset((void*)m_text, 0x00, CErrorLog::E_LINE_LENGTH);

		sprintf_s(m_text, 2, "%c", bytes[i]);

		fwrite(m_text, 1, 1, m_file);
	}

	fwrite("<WriteBytesEndLine>\n", 1, 20, m_file);

	fflush(m_file);
}

/*
*/
void CErrorLog::WriteComErrorMessage(bool time, const char* format, HRESULT hr)
{
	CString* errorLine = new CString(format);

	errorLine->Concat("%s\n");

	char* msg = m_comError->GetComErrorMessage(hr);

	CErrorLog::WriteError(time, errorLine->GetText(), msg);

	delete errorLine;

	errorLine = nullptr;
}

/*
*/
void CErrorLog::WriteDXGIErrorMessage(bool time, const char* format, int error)
{
	CString* errorLine = new CString(format);

	errorLine->Concat("%s %s\n");

	CDXGIErrorItem* dxgiErrorItem = m_dxgiError->GetError(error);

	if (dxgiErrorItem == nullptr)
	{
		CErrorLog::WriteError(time, errorLine->GetText(), "no name", "no msg found");
	}
	else
	{
		CErrorLog::WriteError(time, errorLine->GetText(), dxgiErrorItem->m_name->GetText(), dxgiErrorItem->m_msg->GetText());
	}

	delete errorLine;

	errorLine = nullptr;
}

/*
*/
void CErrorLog::WriteError(bool time, const char* format, ...)
{
	if (strlen(format) == 0)
	{
		return;
	}

	SYSTEMTIME st = {};

	char timeText[CErrorLog::E_TIME_LENGTH] = {};

	GetLocalTime(&st);

	if (time)
	{
		sprintf_s(timeText, CErrorLog::E_TIME_LENGTH, "%02d:%02d:%02d - ", st.wHour, st.wMinute, st.wSecond);

#ifdef _DEBUG
		OutputDebugStringA(timeText);
#endif
	}

	va_list argptr;

	va_start(argptr, format);

	vsprintf_s(m_text, CErrorLog::E_LINE_LENGTH, format, argptr);

	va_end(argptr);

#ifdef _DEBUG
	OutputDebugStringA(m_text);
#endif

	fwrite(timeText, strlen(timeText), 1, m_file);

	fflush(m_file);

	fwrite(m_text, strlen(m_text), 1, m_file);

	fflush(m_file);

	memset((void*)m_text, 0x00, CErrorLog::E_LINE_LENGTH);
}

/*
*/
void CErrorLog::WriteWinsockErrorMessage(bool time, const char* format)
{
	CString* errorLine = new CString(format);

	errorLine->Concat("%s %s\n");

	CWinsockErrorItem* winsockErrorItem = m_wsError->GetError(WSAGetLastError());

	if (winsockErrorItem == nullptr)
	{
		CErrorLog::WriteError(time, errorLine->GetText(), "no name", "no msg found");
	}
	else
	{
		CErrorLog::WriteError(time, errorLine->GetText(), winsockErrorItem->m_name->GetText(), winsockErrorItem->m_msg->GetText());
	}

	delete errorLine;

	errorLine = nullptr;
}