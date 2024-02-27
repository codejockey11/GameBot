#include "CTextureManager.h"

/*
*/
CTextureManager::CTextureManager()
{
	memset(this, 0x00, sizeof(CTextureManager));
}

/*
*/
CTextureManager::CTextureManager(CVideoDevice* videoDevice, CErrorLog* errorLog)
{
	memset(this, 0x00, sizeof(CTextureManager));

	m_videoDevice = videoDevice;

	m_errorLog = errorLog;

	m_textures = new CLinkList<CTexture>();
}

/*
*/
CTextureManager::~CTextureManager()
{
	delete m_textures;
}

/*
*/
CTexture* CTextureManager::Create(const char* filename)
{
	CTexture* texture = CTextureManager::Get(filename);

	if (texture)
	{
		return texture;
	}

	texture = new CTexture(m_errorLog, m_videoDevice, filename);

	if (texture->m_isInitialized)
	{
		m_textures->Add(texture, filename);

		return texture;
	}

	delete texture;

	m_errorLog->WriteError("CTextureManager::MakeTexture:%s\n", filename);

	return nullptr;
}

/*
*/
CTexture* CTextureManager::Get(const char* filename)
{
	CLinkListNode<CTexture>* lln = m_textures->Search(filename);

	if (lln)
	{
		return lln->m_object;
	}

	return nullptr;
}

/*
*/
void CTextureManager::Delete(const char* filename)
{
	CLinkListNode<CTexture>* lln = m_textures->Search(filename);

	if (lln)
	{
		m_textures->Delete(lln);
	}
}