#pragma once

#include "framework.h"

#include "CErrorLog.h"
#include "CString.h"

class CShader
{
public:

	CString* m_entryPoint;
	CString* m_filename;
	CString* m_version;

	ID3DBlob* m_shader;

	CShader();
	CShader(CErrorLog* errorLog, const char* shaderName, const char* entryPoint, const char* version);
	~CShader();

private:

	CErrorLog* m_errorLog;

	ID3DBlob* m_errors;
};