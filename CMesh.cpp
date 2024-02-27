#include "CMesh.h"

/*
*/
CMesh::CMesh()
{
	memset(this, 0x00, sizeof(CMesh));
}

/*
*/
CMesh::~CMesh()
{
	delete m_material;

	delete m_map_Ka;
	delete m_map_Kd;
	delete m_map_Ks;
	delete m_map_Ns;
	delete m_map_d;
	delete m_map_Kb;
	delete m_refl;

	if (m_vertices)
	{
		delete[] m_vertices;
	}
}