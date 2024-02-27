#include "CWavefrontManager.h"

/*
*/
CWavefrontManager::CWavefrontManager()
{
	memset(this, 0x00, sizeof(CWavefrontManager));

	m_models = new CLinkList<CWavefront>();
}

/*
*/
CWavefrontManager::CWavefrontManager(CErrorLog* errorLog)
{
	memset(this, 0x00, sizeof(CWavefrontManager));

	m_errorLog = errorLog;

	m_models = new CLinkList<CWavefront>();
}

/*
*/
CWavefrontManager::~CWavefrontManager()
{
	delete m_models;
}

/*
*/
CWavefront* CWavefrontManager::Create(const char* filename, const char* materialname)
{
	CWavefront* model = CWavefrontManager::Get(filename);

	if (model)
	{
		return model;
	}

	model = new CWavefront(filename, materialname);

	if (model->m_isInitialized)
	{
		m_models->Add(model, filename);

		return model;
	}

	delete model;

	m_errorLog->WriteError(true, "CWavefrontManager::MakeTexture:%s\n", filename);

	return nullptr;
}

/*
*/
void CWavefrontManager::Delete(const char* filename)
{
	CLinkListNode<CWavefront>* lln = m_models->Search(filename);

	if (lln)
	{
		m_models->Delete(lln);
	}
}

/*
*/
CWavefront* CWavefrontManager::Get(const char* filename)
{
	CLinkListNode<CWavefront>* lln = m_models->Search(filename);

	if (lln)
	{
		return lln->m_object;
	}

	return nullptr;
}