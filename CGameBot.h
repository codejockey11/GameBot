#pragma once

#include "framework.h"

#include "CCamera.h"
#include "CErrorLog.h"
#include "CFrametime.h"
#include "CHeapArray.h"
#include "CModelManager.h"
#include "CNetwork.h"
#include "CServerEnvironment.h"
#include "CServerInfo.h"
#include "CShaderManager.h"
#include "CTextureManager.h"
#include "CVisibility.h"
#include "CWavefrontManager.h"

class CGameBot
{
public:

	enum ClientState
	{
		E_CONNECTED = 1,
		E_CONNECTING,
		E_NOTCONNECTED
	};

	enum
	{
		E_HOSTNAME_LENGTH = 64
	};
	
	CCamera* m_camera[CServerInfo::E_MAX_CLIENTS];
	CErrorLog* m_errorLog;
	CFrametime* m_frametime;
	CHeapArray* m_serverInfo;
	CModelManager* m_modelManager;
	CNetwork* m_network;
	CObject* m_model[CServerInfo::E_MAX_CLIENTS];
	CServerInfo* m_localClient;
	CServerEnvironment* m_serverEnvironment;
	CShaderManager* m_shaderManager;
	CTextureManager* m_textureManager;
	CVisibility* m_visibility;
	CWavefrontManager* m_wavefrontManager;

	BYTE m_connectionState;

	float m_spin;

	HANDLE m_hThread;

	HWND m_hWnd;

	SOCKET m_socket;

	UINT m_receiveThreadId;

	bool m_isActiveActivity;
	bool m_isActiveChat;
	bool m_isActiveRender;
	bool m_isActiveUpdate;
	bool m_isRunning;

	char m_hostname[CGameBot::E_HOSTNAME_LENGTH];
	char m_name[CServerInfo::E_NAME_SIZE];
	char m_chat[CServerInfo::E_CHAT_SIZE];

	void (*pFunc[CNetwork::ClientEvent::E_CE_MAX_EVENTS]) (CGameBot* client, CNetwork* network, CServerInfo* serverInfo);

	CGameBot();
	CGameBot(CErrorLog* errorLog, CTextureManager* textureManager, CShaderManager* shaderManager, CModelManager* modelManager, CWavefrontManager* wavefrontManager);
	~CGameBot();

	void Connect(const char* address);
	void Connect(const char* address, const char* port);
	bool ConnectSocket(addrinfo* ptr);
	void CreateSocket(const char* address, const char* port);
	void DestroyEnvironment();
	void InitializeWinsock();
	void Disconnect();
	void ProcessEvent(CNetwork* network);
	void Receive();
	void Send(CNetwork* network);
	void SetLogin(const char* name);

private:

	char m_ip[15];
	char m_port[6];

	WSADATA	m_wsaData;
};