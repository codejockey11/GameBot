#pragma once

#include "framework.h"

#include "CLinkList.h"
#include "CMesh.h"
#include "CString.h"
#include "CVertex.h"

class CModel
{
public:

	enum
	{
		E_MATERIAL_NAME_LENGTH = 256
	};

	CLinkList<CMesh>* m_meshs;
	CMesh* m_mesh;
	CString* m_name;
	
	bool m_isInitialized;
	
	short m_materialCount;

	CModel();
	CModel(const char* filename);
	~CModel();
};