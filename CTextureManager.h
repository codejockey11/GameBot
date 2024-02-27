#pragma once

#include "framework.h"

#include "CErrorLog.h"
#include "CLinkList.h"
#include "CTexture.h"
#include "CVideoDevice.h"

class CTextureManager
{
public:

	CTextureManager();
	CTextureManager(CVideoDevice* videoDevice, CErrorLog* errorLog);
	~CTextureManager();

	CTexture* Create(const char* filename);
	void Delete(const char* filename);
	CTexture* Get(const char* filename);

private:

	CErrorLog* m_errorLog;
	CLinkList<CTexture>* m_textures;
	CVideoDevice* m_videoDevice;
};