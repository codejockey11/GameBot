#pragma once

#include "framework.h"

#include "CErrorLog.h"
#include "CLinkList.h"
#include "CModel.h"

class CModelManager
{
public:

	CModelManager();
	CModelManager(CErrorLog* errorLog);
	~CModelManager();

	CModel* Create(const char* filename);
	void Delete(const char* filename);
	CModel* Get(const char* filename);

private:

	CErrorLog* m_errorLog;
	CLinkList<CModel>* m_models;
};