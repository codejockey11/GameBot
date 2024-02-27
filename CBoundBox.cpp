#include "CBoundBox.h"

/*
*/
CBoundBox::CBoundBox()
{
	memset(this, 0x00, sizeof(CBoundBox));
}

/*
*/
CBoundBox::CBoundBox(CBoundBox* boundBox)
{
	memset(this, 0x00, sizeof(CBoundBox));

	m_mins.p.x = boundBox->m_mins.p.x;
	m_mins.p.y = boundBox->m_mins.p.y;
	m_mins.p.z = boundBox->m_mins.p.z;

	m_maxs.p.x = boundBox->m_maxs.p.x;
	m_maxs.p.y = boundBox->m_maxs.p.y;
	m_maxs.p.z = boundBox->m_maxs.p.z;
}

/*
*/
CBoundBox::CBoundBox(CVertex* mins, CVertex* maxs)
{
	memset(this, 0x00, sizeof(CBoundBox));

	m_mins.p.x = mins->p.x;
	m_mins.p.y = mins->p.y;
	m_mins.p.z = mins->p.z;

	m_maxs.p.x = maxs->p.x;
	m_maxs.p.y = maxs->p.y;
	m_maxs.p.z = maxs->p.z;
}

/*
*/
CBoundBox::~CBoundBox()
{
}

/*
*/
bool CBoundBox::CheckPointInBox(CVertex* point)
{
	if ((point->p.x >= m_mins.p.x) && (point->p.x <= m_maxs.p.x))
	{
		if ((point->p.y >= m_mins.p.y) && (point->p.y <= m_maxs.p.y))
		{
			if ((point->p.z >= m_mins.p.z) && (point->p.z <= m_maxs.p.z))
			{
				return true;
			}
		}
	}

	return false;
}

/*
*/
bool CBoundBox::CheckPointInBox(CVertex2* point)
{
	if ((point->p.x >= m_mins.p.x) && (point->p.x <= m_maxs.p.x))
	{
		if ((point->p.y >= m_mins.p.y) && (point->p.y <= m_maxs.p.y))
		{
			return true;
		}
	}

	return false;
}

/*
*/
void CBoundBox::Copy(CBoundBox* boundBox)
{
	m_mins.p.x = boundBox->m_mins.p.x;
	m_mins.p.y = boundBox->m_mins.p.y;
	m_mins.p.z = boundBox->m_mins.p.z;

	m_maxs.p.x = boundBox->m_maxs.p.x;
	m_maxs.p.y = boundBox->m_maxs.p.y;
	m_maxs.p.z = boundBox->m_maxs.p.z;
}

/*
*/
void CBoundBox::Update(CVertex* mins, CVertex* maxs)
{
	m_mins.p.x = mins->p.x;
	m_mins.p.y = mins->p.y;
	m_mins.p.z = mins->p.z;

	m_maxs.p.x = maxs->p.x;
	m_maxs.p.y = maxs->p.y;
	m_maxs.p.z = maxs->p.z;
}