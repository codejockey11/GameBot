#include "CClient.h"

// Network thread for client receive events
unsigned __stdcall CClient_ReceiveThread(void* obj);

// Event functions
void CClient_Accepted(CClient* client, CNetwork* network, CServerInfo* serverInfo);
void CClient_AccountInfo(CClient* client, CNetwork* network, CServerInfo* serverInfo);
void CClient_Chat(CClient* client, CNetwork* network, CServerInfo* serverInfo);
void CClient_ExitGame(CClient* client, CNetwork* network, CServerInfo* serverInfo);
void CClient_LoadEnvironment(CClient* client, CNetwork* network, CServerInfo* serverInfo);
void CClient_PlayerAttack(CClient* client, CNetwork* network, CServerInfo* serverInfo);
void CClient_PlayerExit(CClient* client, CNetwork* network, CServerInfo* serverInfo);
void CClient_PlayerEnter(CClient* client, CNetwork* network, CServerInfo* serverInfo);
void CClient_PlayerInfoRequest(CClient* client, CNetwork* network, CServerInfo* serverInfo);
void CClient_PlayerUpdate(CClient* client, CNetwork* network, CServerInfo* serverInfo);
void CClient_ServerFull(CClient* client, CNetwork* network, CServerInfo* serverInfo);
void CClient_UpdateCollectable(CClient* client, CNetwork* network, CServerInfo* serverInfo);

/*
*/
CClient::CClient()
{
	memset(this, 0x00, sizeof(CClient));
}

/*
*/
CClient::CClient(CVideoDevice* videoDevice, CErrorLog* errorLog, CTextureManager* textureManager, CShaderManager* shaderManager, CModelManager* modelManager, CWavefrontManager* wavefrontManager)
{
	memset(this, 0x00, sizeof(CClient));

	m_videoDevice = videoDevice;

	m_errorLog = errorLog;

	m_textureManager = textureManager;

	m_shaderManager = shaderManager;

	m_modelManager = modelManager;

	m_wavefrontManager = wavefrontManager;

	m_network = new CNetwork();

	m_serverInfo = new CHeapArray(sizeof(CServerInfo), true, true, 1, CClient::E_MAX_CLIENTS);

#ifdef _DEBUG
	constexpr auto playerInfoSize = sizeof(CPlayerInfo);
	constexpr auto serverInfoSize = sizeof(CServerInfo);

	m_errorLog->WriteError("Sizes:p%u s%u\n", playerInfoSize, serverInfoSize);
#endif

	pFunc[CNetwork::ClientEvent::E_CE_ACCEPTED] = &CClient_Accepted;
	pFunc[CNetwork::ClientEvent::E_CE_ACCOUNT_INFO] = &CClient_AccountInfo;
	pFunc[CNetwork::ClientEvent::E_CE_CHAT] = &CClient_Chat;
	pFunc[CNetwork::ClientEvent::E_CE_EXIT_GAME] = &CClient_ExitGame;
	pFunc[CNetwork::ClientEvent::E_CE_LOAD_ENVIRONMENT] = &CClient_LoadEnvironment;
	pFunc[CNetwork::ClientEvent::E_CE_PLAYER_ATTACK] = &CClient_PlayerAttack;
	pFunc[CNetwork::ClientEvent::E_CE_PLAYER_ENTER] = &CClient_PlayerEnter;
	pFunc[CNetwork::ClientEvent::E_CE_PLAYER_EXIT] = &CClient_PlayerExit;
	pFunc[CNetwork::ClientEvent::E_CE_PLAYER_INFO] = &CClient_PlayerInfoRequest;
	pFunc[CNetwork::ClientEvent::E_CE_PLAYER_UPDATE] = &CClient_PlayerUpdate;
	pFunc[CNetwork::ClientEvent::E_CE_SERVER_FULL] = &CClient_ServerFull;
	pFunc[CNetwork::ClientEvent::E_CE_UPDATE_COLLECTABLE] = &CClient_UpdateCollectable;
}

/*
*/
CClient::~CClient()
{
	CClient::Disconnect();

	CClient::DestroyEnvironment();

	delete m_serverInfo;
	delete m_network;
}

/*
*/
void CClient::Connect(const char* address)
{
	sscanf_s(address, "%s %s", m_ip, 15, m_port, 6);

	CClient::InitializeWinsock();
	CClient::CreateSocket(m_ip, m_port);

	m_hThread = (HANDLE)_beginthreadex(0, sizeof(CClient),
		&CClient_ReceiveThread,
		(void*)this,
		0,
		&m_receiveThreadId);
}

/*
*/
void CClient::Connect(const char* address, const char* port)
{
	CClient::InitializeWinsock();
	CClient::CreateSocket(address, port);

	m_hThread = (HANDLE)_beginthreadex(0, sizeof(CClient),
		&CClient_ReceiveThread,
		(void*)this,
		0,
		&m_receiveThreadId);
}

/*
*/
bool CClient::ConnectSocket(addrinfo* ptr)
{
	size_t err = connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen);

	if (err == SOCKET_ERROR)
	{
		m_errorLog->WriteError("CClient::ConnectSocket::%s %s\n",
			m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
			m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());

		return false;
	}

	return true;
}

/*
*/
void CClient::CreateSocket(const char* address, const char* port)
{
	if (address != nullptr)
	{
		memcpy_s((void*)m_ip, 15, (void*)address, strlen(address));
		memcpy_s((void*)m_port, 6, (void*)port, strlen(port));
	}
	else
	{
		memcpy_s((void*)m_ip, 15, (void*)"127.0.0.1", 9);
		memcpy_s((void*)m_port, 6, (void*)"26105", 5);
	}

	struct addrinfo* result = {};
	struct addrinfo* ptr = {};
	struct addrinfo  hints = {};

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO::IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	size_t err = getaddrinfo(m_ip, m_port, &hints, &result);

	if (err != 0)
	{
		m_errorLog->WriteError("CClient::CreateSocket::getaddrinfo:%i\n", err);

		return;
	}

	ptr = result;

	while (ptr != NULL)
	{
		m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (m_socket == INVALID_SOCKET)
		{
			m_errorLog->WriteError("CClient::CreateSocket::socket:%s %s\n",
				m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
				m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());

			return;
		}

		if (CClient::ConnectSocket(ptr))
		{
			m_connectionState = CClient::ClientState::E_CONNECTING;

			break;
		}

		ptr = ptr->ai_next;
	}

	freeaddrinfo(result);

	gethostname(m_hostname, CClient::E_HOSTNAME_LENGTH);

	m_errorLog->WriteError("CClient::CreateSocket::hostname:%s\n", m_hostname);
}

/*
*/
void CClient::Disconnect()
{
	if (m_connectionState != CClient::ClientState::E_CONNECTED)
	{
		return;
	}

	m_connectionState = CClient::ClientState::E_NOTCONNECTED;

	m_isRunning = false;

	int err = shutdown(m_socket, SD_BOTH);

	if (err == SOCKET_ERROR)
	{
		m_errorLog->WriteError("CClient::Disconnect::shutdown:%s %s\n",
			m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
			m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());
	}

	WaitForSingleObject(m_hThread, 500);

	CloseHandle(m_hThread);

	closesocket(m_socket);

	m_socket = 0;

	WSACleanup();
}

/*
*/
void CClient::DestroyEnvironment()
{
	while (m_isActiveRender)
	{
		// Waiting on the rendering loop to finish
	}

	// Lock out the rendering loop
	m_isActiveThread = true;

	if (m_serverEnvironment)
	{
		delete m_serverEnvironment;

		m_serverEnvironment = nullptr;
	}

	if (m_visibility)
	{
		delete m_visibility;

		m_visibility = nullptr;
	}

	for (int i = 0; i < CClient::E_MAX_CLIENTS; i++)
	{
		CServerInfo* serverInfo = (CServerInfo*)m_serverInfo->GetElement(1, i);

		serverInfo->Deconstruct();
	}

	// Unlock
	m_isActiveThread = false;
}

/*
*/
void CClient::InitializeWinsock()
{
	WORD wVersionRequested = MAKEWORD(2, 2);

	int err = WSAStartup(wVersionRequested, &m_wsaData);

	if (err != 0)
	{
		m_errorLog->WriteError("CClient::InitializeWinsock::WSAStartup:%i\n", err);

		return;
	}

	m_errorLog->WriteError("CClient::InitializeWinsock::WSAStartup:%s\n", m_wsaData.szDescription);
}

/*
*/
void CClient::Process(CNetwork* network)
{
	CPlayerInfo* playerInfo = (CPlayerInfo*)network->m_playerInfo;
	CServerInfo* serverInfo = (CServerInfo*)network->m_serverInfo;

	CServerInfo* si = (CServerInfo*)m_serverInfo->GetElement(1, serverInfo->m_playerNumber);

	if (!si->m_isConstructed)
	{
		si->Construct();
	}

	si->Initialize(serverInfo);
	si->m_playerInfo->Initialize(playerInfo);


	pFunc[network->m_type](this, network, si);
}

/*
*/
void CClient::Receive()
{
	int totalBytes = recv(m_socket, (char*)m_network, sizeof(CNetwork), 0);

	if (totalBytes == 0)
	{
		m_errorLog->WriteError("CClient::Receive::Server Closing Connection\n");

		m_connectionState = CClient::ClientState::E_NOTCONNECTED;

		m_isRunning = false;

		CClient::DestroyEnvironment();

		return;
	}
	else if (totalBytes == SOCKET_ERROR)
	{
		m_errorLog->WriteError("CClient::Receive::recv:%s %s\n",
			m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
			m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());

		m_connectionState = CClient::ClientState::E_NOTCONNECTED;

		m_isRunning = false;

		CClient::DestroyEnvironment();

		return;
	}
}

/*
*/
void CClient::Send(CNetwork* network)
{
	if (network->m_audience == CNetwork::ClientEvent::E_CE_TO_LOCAL)
	{
		pFunc[network->m_type](this, network, nullptr);

		return;
	}

	if (m_connectionState != CClient::ClientState::E_CONNECTED)
	{
		return;
	}

	int totalBytes = send(m_socket, (char*)network, sizeof(CNetwork), 0);

	if (totalBytes == 0)
	{
		m_errorLog->WriteError("CClient::Send::send:Server Closing Connection\n");

		m_connectionState = CClient::ClientState::E_NOTCONNECTED;
	}
	else if (totalBytes == SOCKET_ERROR)
	{
		m_errorLog->WriteError("CClient::Send::send:%s %s\n",
			m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
			m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());

		m_connectionState = CClient::ClientState::E_NOTCONNECTED;
	}
}

/*
*/
void CClient::SetLogin(const char* name)
{
	memset(m_name, 0x00, CPlayerInfo::E_NAME_SIZE);

	memcpy((void*)m_name, (void*)name, strlen(name));
}

/*
*/
void CClient_Accepted(CClient* client, CNetwork* network, CServerInfo* serverInfo)
{
	client->m_localPlayer = serverInfo;

	client->m_connectionState = CClient::ClientState::E_CONNECTED;


	CString* message = new CString("Connection accepted\n");

	// Chat message
	CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_CHAT,
		(void*)message->GetText(), message->GetLength(),
		(void*)serverInfo->m_playerInfo,
		(void*)serverInfo);

	client->Send(n);

	delete n;

	delete message;


	serverInfo->m_playerInfo->SetName(client->m_name);

	// This send will be for the recv in CServer::CreatePlayer where the name is updated
	n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, NULL,
		nullptr, 0,
		(void*)serverInfo->m_playerInfo,
		(void*)serverInfo);

	client->Send(n);

	delete n;
}

/*
*/
void CClient_AccountInfo(CClient* client, CNetwork* network, CServerInfo* serverInfo)
{
	client->m_errorLog->WriteError("%s\n", (char*)network->m_data);
}

/*
*/
void CClient_Chat(CClient* client, CNetwork* network, CServerInfo* serverInfo)
{
	client->m_errorLog->WriteError("%s", (char*)network->m_data);
}

/*
*/
void CClient_ExitGame(CClient* client, CNetwork* network, CServerInfo* serverInfo)
{
	SendMessage(client->m_videoDevice->m_hWnd, WM_COMMAND, IDM_EXIT, 0);
}

/*
*/
void CClient_LoadEnvironment(CClient* client, CNetwork* network, CServerInfo* serverInfo)
{
	client->m_errorLog->WriteError("CClient_LoadEnvironment:%s\n", (char*)network->m_data);

	if (client->m_serverEnvironment)
	{
		delete client->m_serverEnvironment;
	}

	client->m_errorLog->WriteError("CServer_LoadEnvironment::%s\n", (char*)network->m_data);

	client->m_serverEnvironment = new CServerEnvironment((char*)network->m_data);

	CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_PLAYER_READY,
		nullptr, 0,
		(void*)serverInfo->m_playerInfo,
		(void*)serverInfo);

	client->Send(n);

	delete n;
}

/*
*/
void CClient_PlayerAttack(CClient* client, CNetwork* network, CServerInfo* serverInfo)
{
	client->m_errorLog->WriteError("Attack:%s\n", serverInfo->m_playerInfo->m_name);
}

/*
*/
void CClient_PlayerEnter(CClient* client, CNetwork* network, CServerInfo* serverInfo)
{
	client->m_errorLog->WriteError("CClient_PlayerEnter:%s\n", serverInfo->m_playerInfo->m_name);
}

/*
*/
void CClient_PlayerExit(CClient* client, CNetwork* network, CServerInfo* serverInfo)
{

}

/*
*/
void CClient_PlayerInfoRequest(CClient* client, CNetwork* network, CServerInfo* serverInfo)
{
	client->m_errorLog->WriteError("CClient_PlayerInfoRequest:%s\n", serverInfo->m_playerInfo->m_name);
}

/*
*/
void CClient_PlayerUpdate(CClient* client, CNetwork* network, CServerInfo* serverInfo)
{
	//client->m_errorLog->WriteError("CClient_PlayerUpdate:%s\n", serverInfo->m_playerInfo->m_name);
}

/*
*/
void CClient_ServerFull(CClient* client, CNetwork* network, CServerInfo* serverInfo)
{
	client->m_errorLog->WriteError("CClient_ServerFull\n");
}

/*
*/
void CClient_UpdateCollectable(CClient* client, CNetwork* network, CServerInfo* serverInfo)
{
	int px = 0;// (int)(serverInfo->m_playerInfo->m_position.p.x + (client->m_visibility->m_width / 2.0f)) / client->m_visibility->m_gridUnits;
	int pz = 0;// (int)(serverInfo->m_playerInfo->m_position.p.z + (client->m_visibility->m_height / 2.0f)) / client->m_visibility->m_gridUnits;

	char itemName[128] = {};
	sscanf_s((char*)network->m_data, "%s %i %i", itemName, 128, &px, &pz);

	CLinkList<CObject>* collectables = (CLinkList<CObject>*)client->m_visibility->m_collectables->GetElement(2, px, pz);

	if (collectables != nullptr)
	{
		if (collectables->m_count > 0)
		{
			CLinkListNode<CObject>* collectable = collectables->Search(itemName);

			if (collectable != nullptr)
			{
				collectable->m_object->m_limboTimer->Start();
			}
		}
	}
}

/*
*/
unsigned __stdcall CClient_ReceiveThread(void* obj)
{
	CClient* client = (CClient*)obj;

#ifdef _DEBUG
	client->m_errorLog->WriteError("CClient_ReceiveThread:Starting\n");
#endif

	client->m_isRunning = true;

	while (client->m_isRunning)
	{
		client->Receive();

		if (client->m_connectionState == CClient::E_NOTCONNECTED)
		{
			client->m_isRunning = false;
		}
		else
		{
			client->Process(client->m_network);
		}
	}

#ifdef _DEBUG
	client->m_errorLog->WriteError("CClient_ReceiveThread Ending\n");
#endif

	_endthreadex(0);

	return 0;
}