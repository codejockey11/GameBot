#pragma once

#include "framework.h"

#include "CErrorLog.h"
#include "CLinkList.h"
#include "CShader.h"
#include "CVideoDevice.h"

class CShaderManager
{
public:

	CLinkList<CShader>* m_shaders;

	D3D12_FEATURE_DATA_SHADER_MODEL m_shaderModel;

	CShaderManager();
	CShaderManager(CVideoDevice* videoDevice, CErrorLog* errorLog);
	~CShaderManager();

	CShader* Create(const char* shaderName, const char* entryPoint, const char* version);
	void Delete(const char* shaderName);
	CShader* Get(const char* shaderName);

private:

	CErrorLog* m_errorLog;
	CVideoDevice* m_device;
};