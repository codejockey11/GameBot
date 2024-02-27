#pragma once

#include "framework.h"

#include "CLinkList.h"
#include "CMesh.h"
#include "CString.h"
#include "CVertex.h"

// Model texture needs to be a top left tga

class CWavefront
{
public:

	enum
	{
		VIEW_COUNT = 65536,
		LINE_LENGTH = 132,
	};

	CString* m_name;

	CString* m_materialname;

	CLinkList<CMesh>* m_meshs;

	bool m_isInitialized;


	CMesh* m_mesh[CMaterial::E_MATERIAL_COUNT];


	CWavefront();

	CWavefront(const char* filename, const char* materialFilename);

	~CWavefront();


	void WriteVertices(FILE* file, CVertex* p);

	void LoadMeshList();

private:

	CVertex* m_vertex;

	CVertex* m_normal;

	CVertex2* m_uv;

	char m_materialName[CMaterial::E_MATERIAL_COUNT][CMaterial::E_MATERIAL_NAME_LENGTH];

	UINT m_vi[8];

	UINT m_ni[8];

	UINT m_uvi[8];

	CVertexNT* m_vnt[CMaterial::E_MATERIAL_COUNT];

	CVertexNT* m_vntl[CMaterial::E_MATERIAL_COUNT];

	int m_vntcount;

	int m_icount[CMaterial::E_MATERIAL_COUNT];

	int m_inbr;
};