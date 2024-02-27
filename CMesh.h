#pragma once

#include "framework.h"

#include "CMaterial.h"
#include "CString.h"
#include "CTexture.h"

class CMesh
{
public:

	BYTE* m_vertices;

	CMaterial* m_material;
	CString* m_map_Ka;
	CString* m_map_Kb;
	CString* m_map_Kd;
	CString* m_map_Ks;
	CString* m_map_Ns;
	CString* m_map_d;
	CString* m_refl;
	
	UINT m_vertexCount;

	CMesh();
	~CMesh();
};