#pragma once

#include "framework.h"

#include "CString.h"
#include "CTexture.h"

class CMaterial
{
public:

	enum
	{
		E_MATERIAL_COUNT = 8,
		E_MATERIAL_NAME_LENGTH = 32
	};

	CString* m_name;
	CTexture* m_map_Ka;
	CTexture* m_map_Kb;
	CTexture* m_map_Kd;
	CTexture* m_map_Ks;
	CTexture* m_map_Ns;
	CTexture* m_map_d;
	CTexture* m_refl;
	
	UINT m_illum;
	
	XMFLOAT3 m_ambient;
	XMFLOAT3 m_diffuse;
	XMFLOAT3 m_emissive;
	XMFLOAT3 m_specular;
	
	float m_opacity;
	float m_opticalDensity;
	float m_specularExponent;

	CMaterial();
	CMaterial(const char* name);
	~CMaterial();

	void Copy(CMaterial* material);
};