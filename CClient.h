#pragma once

#include "framework.h"

#include "CCamera.h"
#include "CErrorLog.h"
#include "CHeapArray.h"
#include "CModelManager.h"
#include "CNetwork.h"
#include "CServerEnvironment.h"
#include "CServerInfo.h"
#include "CShaderManager.h"
#include "CTerrain.h"
#include "CTextureManager.h"
#include "CVisibility.h"
#include "CVideoDevice.h"
#include "CWavefrontManager.h"

class CClient
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
		E_HOSTNAME_LENGTH = 64,
		E_MAX_CLIENTS = 50
	};
	
	CErrorLog* m_errorLog;
	CHeapArray* m_serverInfo;
	CModelManager* m_modelManager;
	CNetwork* m_network;
	CServerInfo* m_localPlayer;
	CShaderManager* m_shaderManager;
	CTerrain* m_terrain;
	CTextureManager* m_textureManager;
	CVideoDevice* m_videoDevice;
	CVisibility* m_visibility;
	CWavefrontManager* m_wavefrontManager;
	CServerEnvironment* m_serverEnvironment;

	bool m_isActiveRender;
	bool m_isActiveThread;

	BYTE m_connectionState;

	char m_name[CPlayerInfo::E_NAME_SIZE];

	HANDLE m_hThread;
	SOCKET m_socket;
	UINT m_receiveThreadId;
	
	bool m_isRunning;
	
	char m_hostname[CClient::E_HOSTNAME_LENGTH];

	void (*pFunc[CNetwork::ClientEvent::E_CE_MAX_EVENTS]) (CClient* client, CNetwork* network, CServerInfo* serverInfo);

	CClient();
	CClient(CVideoDevice* videoDevice, CErrorLog* errorLog, CTextureManager* textureManager, CShaderManager* shaderManager, CModelManager* modelManager, CWavefrontManager* wavefrontManager);
	~CClient();

	void Connect(const char* address);
	void Connect(const char* address, const char* port);
	bool ConnectSocket(addrinfo* ptr);
	void CreateSocket(const char* address, const char* port);
	void DestroyEnvironment();
	void InitializeWinsock();
	void Disconnect();
	void Process(CNetwork* network);
	void Receive();
	void Send(CNetwork* network);
	void SetLogin(const char* name);

private:

	char m_ip[15];
	char m_port[6];

	WSADATA	m_wsaData;
};