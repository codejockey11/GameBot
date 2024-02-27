#pragma once

#include "framework.h"

#include "CErrorLog.h"
#include "CVertex.h"

class CServerInfo
{
public:

	enum Team
	{
		E_TEAM_RED = 0,
		E_TEAM_BLUE,
		E_TEAM_GREEN,
		E_TEAM_YELLOW,
	};

	enum ClientState
	{
		E_GAME = 0,
		E_LOADING,
		E_LOBBY
	};

	enum
	{
		E_MAX_CLIENTS = 4,
		E_NAME_SIZE = 32,
		E_CHAT_SIZE = 256
	};

	char m_name[CServerInfo::E_NAME_SIZE];
	char m_chat[CServerInfo::E_CHAT_SIZE];

	CVertex m_direction;
	CVertex m_position;

	BYTE m_action;
	BYTE m_state;
	BYTE m_team;

	bool m_isAvailable;
	bool m_isFalling;
	bool m_isRunning;

	int m_clientNumber;
	int m_countdown;

	float m_acceleration;
	float m_freefallVelocity;
	float m_idleTime;
	float m_velocity;

	SOCKET m_socket;

	CServerInfo();
	CServerInfo(CErrorLog* errorLog);
	~CServerInfo();

	void Constructor(CErrorLog* errorLog);
	void Initialize(CServerInfo* serverInfo);
	void Reset();
	void SetChat(const char* chat);
	void SetDirection(XMFLOAT3* direction);
	void SetName(const char* name);
	void Shutdown();

private:

	CErrorLog* m_errorLog;
};