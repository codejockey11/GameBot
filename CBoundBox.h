#pragma once

#include "framework.h"

#include "CVertex.h"

class CBoundBox
{
public:

	CVertex m_maxs;
	CVertex m_mins;

	CBoundBox();
	CBoundBox(CBoundBox* boundBox);
	CBoundBox(CVertex* mins, CVertex* maxs);
	~CBoundBox();

	bool CheckPointInBox(CVertex* point);
	bool CheckPointInBox(CVertex2* point);
	void Copy(CBoundBox* boundBox);
	void Update(CVertex* mins, CVertex* maxs);
};