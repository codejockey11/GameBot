#pragma once

#include "framework.h"

#include "CCamera.h"
#include "CObject.h"
#include "CVertex.h"

class CPlayerInfo
{
public:

	enum
	{
		E_TEAM_RED = 0,
		E_TEAM_BLUE,
		E_TEAM_GREEN,
		E_TEAM_YELLOW,

		E_NAME_SIZE = 32
	};

	char m_name[CPlayerInfo::E_NAME_SIZE];

	BYTE m_state;
	BYTE m_team;
	
	CCamera* m_camera;
	CObject* m_model;
	CVertex m_direction;
	CVertex m_position;
	
	bool m_isRecordable;
	
	float m_acceleration;

	CPlayerInfo();
	~CPlayerInfo();

	void Initialize(CPlayerInfo* playerInfo);
	void SetDirection(XMFLOAT3* direction);
	void SetName(const char* name);
};