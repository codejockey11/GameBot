#pragma once

#include "framework.h"

#include "CHeapArray.h"
#include "CLinkList.h"
#include "CObject.h"

class CVisibility
{
public:

	CHeapArray* m_collectables;

	int m_px;
	int m_pz;

	UINT m_gridHeight;
	UINT m_gridUnits;
	UINT m_gridWidth;
	UINT m_height;
	UINT m_width;

	CVisibility();
	CVisibility(UINT width, UINT height, UINT gridSize);
	~CVisibility();
};

