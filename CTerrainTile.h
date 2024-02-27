#pragma once

#include "framework.h"

#include "CVertex.h"

class CTerrainTile
{
public:

	CVertexNT m_a;
	CVertexNT m_b;
	CVertexNT m_c;
	CVertexNT m_d;
	CVertexNT m_e;
	CVertexNT m_f;

	CTerrainTile();
	~CTerrainTile();
};