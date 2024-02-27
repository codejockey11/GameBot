#include "CTeamStart.h"

/*
*/
CTeamStart::CTeamStart()
{
	memset(this, 0x00, sizeof(CTeamStart));
}

/*
*/
CTeamStart::CTeamStart(bool isAvailable, CVertex position, CVertex direction)
{
	memset(this, 0x00, sizeof(CTeamStart));

	m_isAvailable = isAvailable;

	m_position = position;

	m_direction = direction;
}

/*
*/
CTeamStart::~CTeamStart()
{
}