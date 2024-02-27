#pragma once

#include "framework.h"

#include "CVertex.h"

class CTeamStart
{
public:

	CVertex m_direction;
	CVertex m_position;
	
	bool m_isAvailable;

	CTeamStart();
	CTeamStart(bool isAvailable, CVertex position, CVertex direction);
	~CTeamStart();
};

