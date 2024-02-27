#include "CShaderManager.h"

/*
*/
CShaderManager::CShaderManager()
{
	memset(this, 0x00, sizeof(CShaderManager));
}

/*
*/
CShaderManager::CShaderManager(CVideoDevice* videoDevice, CErrorLog* errorLog)
{
	memset(this, 0x00, sizeof(CShaderManager));

	m_errorLog = errorLog;

	m_device = videoDevice;

	m_shaders = new CLinkList<CShader>();

	// D3D_SHADER_MODEL_6_7 results in an invalid call
	//m_shaderModel.HighestShaderModel = D3D_SHADER_MODEL::D3D_SHADER_MODEL_6_7;

	m_shaderModel.HighestShaderModel = D3D_SHADER_MODEL::D3D_SHADER_MODEL_6_6;
	
	HRESULT hr = m_device->m_device->CheckFeatureSupport(D3D12_FEATURE::D3D12_FEATURE_SHADER_MODEL, &m_shaderModel, sizeof(D3D12_FEATURE_DATA_SHADER_MODEL));
	
	if (hr == S_OK)
	{
		switch (m_shaderModel.HighestShaderModel)
		{
		case D3D_SHADER_MODEL_5_1:
		{
			m_errorLog->WriteError("CShaderManager::CShaderManager::D3D_SHADER_MODEL:D3D_SHADER_MODEL_5_1\n");

			break;
		}
		case D3D_SHADER_MODEL_6_0:
		{
			m_errorLog->WriteError("CShaderManager::CShaderManager::D3D_SHADER_MODEL:D3D_SHADER_MODEL_6_0\n");

			break;
		}
		case D3D_SHADER_MODEL_6_1:
		{
			m_errorLog->WriteError("CShaderManager::CShaderManager::D3D_SHADER_MODEL:D3D_SHADER_MODEL_6_1\n");

			break;
		}
		case D3D_SHADER_MODEL_6_2:
		{
			m_errorLog->WriteError("CShaderManager::CShaderManager::D3D_SHADER_MODEL:D3D_SHADER_MODEL_6_2\n");

			break;
		}
		case D3D_SHADER_MODEL_6_3:
		{
			m_errorLog->WriteError("CShaderManager::CShaderManager::D3D_SHADER_MODEL:D3D_SHADER_MODEL_6_3\n");

			break;
		}
		case D3D_SHADER_MODEL_6_4:
		{
			m_errorLog->WriteError("CShaderManager::CShaderManager::D3D_SHADER_MODEL:D3D_SHADER_MODEL_6_4\n");

			break;
		}
		case D3D_SHADER_MODEL_6_5:
		{
			m_errorLog->WriteError("CShaderManager::CShaderManager::D3D_SHADER_MODEL:D3D_SHADER_MODEL_6_5\n");

			break;
		}
		case D3D_SHADER_MODEL_6_6:
		{
			m_errorLog->WriteError("CShaderManager::CShaderManager::D3D_SHADER_MODEL:D3D_SHADER_MODEL_6_6\n");

			break;
		}
		case D3D_SHADER_MODEL_6_7:
		{
			m_errorLog->WriteError("CShaderManager::CShaderManager::D3D_SHADER_MODEL:D3D_SHADER_MODEL_6_7\n");

			break;
		}
		}
	}
}

/*
*/
CShaderManager::~CShaderManager()
{
	delete m_shaders;
}

/*
*/
CShader* CShaderManager::Create(const char* shaderName, const char* entryPoint, const char* version)
{
	CShader* shader = CShaderManager::Get(shaderName);

	if (shader)
	{
		return shader;
	}

	shader = new CShader(m_errorLog, shaderName, entryPoint, version);

	if (shader->m_shader)
	{
		m_shaders->Add(shader, shaderName);

		return shader;
	}

	m_errorLog->WriteError("CShaderManager::MakeShader:%s\n", shaderName);

	delete shader;

	return nullptr;
}

/*
*/
void CShaderManager::Delete(const char* shaderName)
{
	CLinkListNode<CShader>* lln = m_shaders->Search(shaderName);

	if (lln)
	{
		m_shaders->Delete(lln);
	}
}

/*
*/
CShader* CShaderManager::Get(const char* shaderName)
{
	CLinkListNode<CShader>* lln = m_shaders->Search(shaderName);

	if (lln)
	{
		return lln->m_object;
	}

	return nullptr;
}