#pragma once

#include "framework.h"

#include "CErrorLog.h"
#include "CLinkList.h"
#include "CWavefront.h"

class CWavefrontManager
{
public:

	CWavefrontManager();
	CWavefrontManager(CErrorLog* errorLog);
	~CWavefrontManager();

	CWavefront* Create(const char* filename, const char* materialname);
	void Delete(const char* filename);
	CWavefront* Get(const char* filename);

private:

	CErrorLog* m_errorLog;
	CLinkList<CWavefront>* m_models;
};