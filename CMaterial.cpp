#include "CMaterial.h"

/*
*/
CMaterial::CMaterial()
{
	memset(this, 0x00, sizeof(CMaterial));
}

/*
*/
CMaterial::CMaterial(const char* name)
{
	memset(this, 0x00, sizeof(CMaterial));

	m_name = new CString(name);
}

/*
*/
CMaterial::~CMaterial()
{
	delete m_name;
}

/*
*/
void CMaterial::Copy(CMaterial* material)
{
	if (material == nullptr)
	{
		return;
	}

	m_name = new CString(material->m_name->GetText());

	m_ambient = material->m_ambient;
	m_diffuse = material->m_diffuse;
	m_specular = material->m_specular;
	m_emissive = material->m_emissive;

	m_specularExponent = material->m_specularExponent;

	m_opacity = material->m_opacity;

	m_illum = material->m_illum;

	m_opticalDensity = material->m_opticalDensity;

	m_map_Ka = material->m_map_Ka;
	m_map_Kd = material->m_map_Kd;
	m_map_Ks = material->m_map_Ks;
	m_map_Ns = material->m_map_Ns;

	m_map_d = material->m_map_d;

	m_map_Kb = material->m_map_Kb;

	m_refl = material->m_refl;
}