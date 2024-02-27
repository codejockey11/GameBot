#include "CShader.h"

/*
*/
CShader::CShader()
{
	memset(this, 0x00, sizeof(CShader));
}

/*
*/
CShader::CShader(CErrorLog* errorLog, const char* shaderName, const char* entryPoint, const char* version)
{
	memset(this, 0x00, sizeof(CShader));

	m_errorLog = errorLog;

	m_filename = new CString(shaderName);

	m_entryPoint = new CString(entryPoint);

	m_version = new CString(version);

	UINT flags = 0;

#ifdef _DEBUG
	flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = D3DCompileFromFile(m_filename->GetWText(), nullptr, nullptr, entryPoint, version, flags, 0, &m_shader, &m_errors);

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CShader::CShader::D3DCompileFromFile:%s:%s\n", m_filename->GetText(), m_errorLog->GetComErrorMessage(hr));

		m_shader = nullptr;

		if (m_errors)
		{
			m_errorLog->WriteError("Compile Errors\n%s", (char*)m_errors->GetBufferPointer());
		}
	}
}

/*
*/
CShader::~CShader()
{
	delete m_version;
	delete m_entryPoint;
	delete m_filename;

	if (m_shader)
	{
		m_shader->Release();
		m_shader = 0;
	}

	if (m_errors)
	{
		m_errors->Release();
		m_errors = 0;
	}
}