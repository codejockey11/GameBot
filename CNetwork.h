#pragma once

#include "framework.h"

class CNetwork
{
public:

	enum ClientEvent
	{
		E_CE_ACCEPTED = 0,
		E_CE_ACCOUNT_INFO,
		E_CE_ATTACK,
		E_CE_CAMERA_MOVE,
		E_CE_CHAT,
		E_CE_CHATBOX,
		E_CE_ENTER,
		E_CE_EXIT,
		E_CE_EXIT_GAME,
		E_CE_INFO,
		E_CE_LOAD_ENVIRONMENT,
		E_CE_SEND_ACTIVITY,
		E_CE_SERVER_FULL,
		E_CE_TO_LOCAL,
		E_CE_TO_SERVER,
		E_CE_UPDATE,
		E_CE_UPDATE_COLLECTABLE,
		E_CE_WINDOW_MODE,

		E_CE_MAX_EVENTS
	};

	enum ServerEvent
	{
		E_SE_ACCOUNT_INFO = 0,
		E_SE_ACTIVITY,
		E_SE_CHAT,
		E_SE_DISCONNECT,
		E_SE_IDLE,
		E_SE_INFO,
		E_SE_JOIN,
		E_SE_LOAD_ENVIRONMENT,
		E_SE_READY,
		E_SE_TO_CLIENT,

		E_SE_MAX_EVENTS
	};

	enum ClientActivity
	{
		E_CA_ATTACK = 1,
		E_CA_BACKWARD,
		E_CA_FORWARD,
		E_CA_STEP_LEFT,
		E_CA_STEP_RIGHT,
		E_CA_STOP,
	};

	enum
	{
		E_DATA_SIZE = 1024,
		E_SERVER_INFO_SIZE = 512	// 360 = sizeof(CServerinfo)
	};

	BYTE m_audience;
	BYTE m_data[CNetwork::E_DATA_SIZE];
	BYTE m_serverInfo[CNetwork::E_SERVER_INFO_SIZE];
	BYTE m_type;

	int m_length;

	CNetwork();
	CNetwork(BYTE audience, BYTE type, void* data, int length, void* serverInfo);
	~CNetwork();

	void SetData(void* data, int length);
	void SetServerInfo(void* serverInfo);
};