#include "CVisibility.h"

CVisibility::CVisibility()
{
	memset(this, 0x00, sizeof(CVisibility));
}

CVisibility::CVisibility(UINT width, UINT height, UINT units)
{
	memset(this, 0x00, sizeof(CVisibility));

	m_width = width;

	m_height = height;

	m_gridUnits = units;

	m_gridHeight = (m_height / m_gridUnits) + 1;
	m_gridWidth = (m_width / m_gridUnits) + 1;

	// These collectable items would be defined in the environment file from the editor or compiler.
	// Allocating the space for the objects as this does not perform the constructor. 
	m_collectables = new CHeapArray(sizeof(CLinkList<CObject>), true, true, 2, m_gridWidth, m_gridHeight);
}

CVisibility::~CVisibility()
{
	// Must delete each cube object's list
	for (UINT pz = 0; pz < m_gridHeight; pz++)
	{
		for (UINT px = 0; px < m_gridWidth; px++)
		{
			CLinkList<CObject>* collectables = (CLinkList<CObject>*)m_collectables->GetElement(2, px, pz);

			if (collectables->m_list != nullptr)
			{
				CLinkListNode<CObject>* lln = collectables->m_list;

				while (lln->m_object)
				{
					delete lln->m_object;

					lln = lln->m_next;
				}
			}
		}
	}

	delete m_collectables;
}