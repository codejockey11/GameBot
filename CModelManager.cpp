#include "CModelManager.h"

/*
*/
CModelManager::CModelManager()
{
	memset(this, 0x00, sizeof(CModelManager));

	m_models = new CLinkList<CModel>();
}

/*
*/
CModelManager::CModelManager(CErrorLog* errorLog)
{
	memset(this, 0x00, sizeof(CModelManager));

	m_errorLog = errorLog;

	m_models = new CLinkList<CModel>();
}

/*
*/
CModelManager::~CModelManager()
{
	delete m_models;
}

/*
*/
CModel* CModelManager::Create(const char* filename)
{
	CModel* model = CModelManager::Get(filename);

	if (model)
	{
		return model;
	}

	model = new CModel(filename);

	if (model->m_isInitialized)
	{
		m_models->Add(model, filename);

		return model;
	}

	delete model;

	m_errorLog->WriteError(true, "CModelManager::Create:%s\n", filename);

	return nullptr;
}

/*
*/
void CModelManager::Delete(const char* filename)
{
	CLinkListNode<CModel>* lln = m_models->Search(filename);

	if (lln)
	{
		m_models->Delete(lln);
	}
}

/*
*/
CModel* CModelManager::Get(const char* filename)
{
	CLinkListNode<CModel>* lln = m_models->Search(filename);

	if (lln)
	{
		return lln->m_object;
	}

	return nullptr;
}