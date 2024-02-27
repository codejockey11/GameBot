#include "CGameBot.h"

// Network thread for client receive events
unsigned __stdcall CGameBot_ReceiveThread(void* obj);

// Event functions
void CGameBot_Accepted(CGameBot* client, CNetwork* network, CServerInfo* serverInfo);
void CGameBot_AccountInfo(CGameBot* client, CNetwork* network, CServerInfo* serverInfo);
void CGameBot_Attack(CGameBot* client, CNetwork* network, CServerInfo* serverInfo);
void CGameBot_CameraMove(CGameBot* client, CNetwork* network, CServerInfo* serverInfo);
void CGameBot_Chat(CGameBot* client, CNetwork* network, CServerInfo* serverInfo);
void CGameBot_Enter(CGameBot* client, CNetwork* network, CServerInfo* serverInfo);
void CGameBot_Exit(CGameBot* client, CNetwork* network, CServerInfo* serverInfo);
void CGameBot_ExitGame(CGameBot* client, CNetwork* network, CServerInfo* serverInfo);
void CGameBot_InfoRequest(CGameBot* client, CNetwork* network, CServerInfo* serverInfo);
void CGameBot_LoadEnvironment(CGameBot* client, CNetwork* network, CServerInfo* serverInfo);
void CGameBot_SendActivity(CGameBot* client, CNetwork* network, CServerInfo* serverInfo);
void CGameBot_ServerFull(CGameBot* client, CNetwork* network, CServerInfo* serverInfo);
void CGameBot_Update(CGameBot* client, CNetwork* network, CServerInfo* serverInfo);
void CGameBot_UpdateCollectable(CGameBot* client, CNetwork* network, CServerInfo* serverInfo);

/*
*/
CGameBot::CGameBot()
{
	memset(this, 0x00, sizeof(CGameBot));
}

/*
*/
CGameBot::CGameBot(CErrorLog* errorLog, CTextureManager* textureManager, CShaderManager* shaderManager, CModelManager* modelManager, CWavefrontManager* wavefrontManager)
{
	memset(this, 0x00, sizeof(CGameBot));

	m_errorLog = errorLog;

	m_textureManager = textureManager;

	m_shaderManager = shaderManager;

	m_modelManager = modelManager;

	m_wavefrontManager = wavefrontManager;

	m_frametime = new CFrametime();

	m_network = new CNetwork();

	m_serverInfo = new CHeapArray(sizeof(CServerInfo), true, true, 1, CServerInfo::E_MAX_CLIENTS);


	pFunc[CNetwork::ClientEvent::E_CE_ACCEPTED] = &CGameBot_Accepted;
	pFunc[CNetwork::ClientEvent::E_CE_ACCOUNT_INFO] = &CGameBot_AccountInfo;
	pFunc[CNetwork::ClientEvent::E_CE_ATTACK] = &CGameBot_Attack;
	pFunc[CNetwork::ClientEvent::E_CE_CAMERA_MOVE] = &CGameBot_CameraMove;
	pFunc[CNetwork::ClientEvent::E_CE_CHAT] = &CGameBot_Chat;
	pFunc[CNetwork::ClientEvent::E_CE_ENTER] = &CGameBot_Enter;
	pFunc[CNetwork::ClientEvent::E_CE_EXIT] = &CGameBot_Exit;
	pFunc[CNetwork::ClientEvent::E_CE_EXIT_GAME] = &CGameBot_ExitGame;
	pFunc[CNetwork::ClientEvent::E_CE_INFO] = &CGameBot_InfoRequest;
	pFunc[CNetwork::ClientEvent::E_CE_LOAD_ENVIRONMENT] = &CGameBot_LoadEnvironment;
	pFunc[CNetwork::ClientEvent::E_CE_SEND_ACTIVITY] = &CGameBot_SendActivity;
	pFunc[CNetwork::ClientEvent::E_CE_SERVER_FULL] = &CGameBot_ServerFull;
	pFunc[CNetwork::ClientEvent::E_CE_UPDATE] = &CGameBot_Update;
	pFunc[CNetwork::ClientEvent::E_CE_UPDATE_COLLECTABLE] = &CGameBot_UpdateCollectable;
}

/*
*/
CGameBot::~CGameBot()
{
	CGameBot::Disconnect();

	CGameBot::DestroyEnvironment();

	delete m_serverInfo;
	delete m_network;
	delete m_frametime;
}

/*
*/
void CGameBot::Connect(const char* address)
{
	sscanf_s(address, "%s %s", m_ip, 15, m_port, 6);

	CGameBot::InitializeWinsock();
	CGameBot::CreateSocket(m_ip, m_port);

	m_hThread = (HANDLE)_beginthreadex(0, sizeof(CGameBot),
		&CGameBot_ReceiveThread,
		(void*)this,
		0,
		&m_receiveThreadId);
}

/*
*/
void CGameBot::Connect(const char* address, const char* port)
{
	CGameBot::InitializeWinsock();
	CGameBot::CreateSocket(address, port);

	m_hThread = (HANDLE)_beginthreadex(0, sizeof(CGameBot),
		&CGameBot_ReceiveThread,
		(void*)this,
		0,
		&m_receiveThreadId);
}

/*
*/
bool CGameBot::ConnectSocket(addrinfo* ptr)
{
	size_t err = connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen);

	if (err == SOCKET_ERROR)
	{
		m_errorLog->WriteWinsockErrorMessage(true, "CGameBot::ConnectSocket::");

		return false;
	}

	return true;
}

/*
*/
void CGameBot::CreateSocket(const char* address, const char* port)
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
		m_errorLog->WriteError(true, "CGameBot::CreateSocket::getaddrinfo:%i\n", err);

		return;
	}

	ptr = result;

	while (ptr != NULL)
	{
		m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (m_socket == INVALID_SOCKET)
		{
			m_errorLog->WriteWinsockErrorMessage(true, "CGameBot::CreateSocket::socket:");

			return;
		}

		if (CGameBot::ConnectSocket(ptr))
		{
			m_connectionState = CGameBot::ClientState::E_CONNECTING;

			break;
		}

		ptr = ptr->ai_next;
	}

	freeaddrinfo(result);

	gethostname(m_hostname, CGameBot::E_HOSTNAME_LENGTH);

	m_errorLog->WriteError(true, "CGameBot::CreateSocket::hostname:%s\n", m_hostname);
}

/*
*/
void CGameBot::Disconnect()
{
	if (m_connectionState != CGameBot::ClientState::E_CONNECTED)
	{
		return;
	}

	m_connectionState = CGameBot::ClientState::E_NOTCONNECTED;

	m_isRunning = false;

	int err = shutdown(m_socket, SD_BOTH);

	if (err == SOCKET_ERROR)
	{
		m_errorLog->WriteWinsockErrorMessage(true, "CGameBot::Disconnect::shutdown:");
	}

	closesocket(m_socket);

	m_socket = 0;

	WaitForSingleObject(m_hThread, 500);

	CloseHandle(m_hThread);

	m_hThread = 0;

	WSACleanup();
}

/*
*/
void CGameBot::DestroyEnvironment()
{
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

	for (int i = 0; i < CServerInfo::E_MAX_CLIENTS; i++)
	{
		CServerInfo* serverInfo = (CServerInfo*)m_serverInfo->GetElement(1, i);

		serverInfo->m_isRunning = false;

		delete m_model[i];

		m_model[i] = nullptr;

		delete m_camera[i];

		m_camera[i] = nullptr;
	}
}

/*
*/
void CGameBot::InitializeWinsock()
{
	WORD wVersionRequested = MAKEWORD(2, 2);

	int err = WSAStartup(wVersionRequested, &m_wsaData);

	if (err != 0)
	{
		m_errorLog->WriteWinsockErrorMessage(true, "CGameBot::InitializeWinsock::WSAStartup:");

		return;
	}

	m_errorLog->WriteError(true, "CGameBot::InitializeWinsock::WSAStartup:%s\n", m_wsaData.szDescription);
}

/*
*/
void CGameBot::ProcessEvent(CNetwork* network)
{
	CServerInfo* serverInfo = (CServerInfo*)network->m_serverInfo;

	CServerInfo* si = (CServerInfo*)m_serverInfo->GetElement(1, serverInfo->m_clientNumber);


	si->Constructor(m_errorLog);

	si->Initialize(serverInfo);


	if ((m_localClient != nullptr) && (serverInfo->m_clientNumber == m_localClient->m_clientNumber))
	{
		m_localClient = si;
	}

	if (network->m_type == 255)
	{
		return;
	}

	pFunc[network->m_type](this, network, si);
}

/*
*/
void CGameBot::Receive()
{
	DWORD millis = 500;
	int iResult = setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&millis, (int)sizeof(DWORD));

	int totalBytes = recv(m_socket, (char*)m_network, sizeof(CNetwork), 0);

	if (totalBytes == 0)
	{
		m_errorLog->WriteError(true, "CGameBot::Receive::Server Closing Connection\n");

		m_connectionState = CGameBot::ClientState::E_NOTCONNECTED;

		m_isRunning = false;

		return;
	}
	else if (WSAGetLastError() == WSAETIMEDOUT)
	{
		m_network->m_type = 0xFF;
	}
	else if (totalBytes == SOCKET_ERROR)
	{
		m_errorLog->WriteError(true, "CGameBot::Receive::recv:");

		m_connectionState = CGameBot::ClientState::E_NOTCONNECTED;

		m_isRunning = false;
	}
}

/*
*/
void CGameBot::Send(CNetwork* network)
{
	if (network->m_audience == CNetwork::ClientEvent::E_CE_TO_LOCAL)
	{
		pFunc[network->m_type](this, network, nullptr);

		return;
	}

	if (m_connectionState != CGameBot::ClientState::E_CONNECTED)
	{
		return;
	}

	int totalBytes = send(m_socket, (char*)network, sizeof(CNetwork), 0);

	if (totalBytes == 0)
	{
		m_errorLog->WriteError(true, "CGameBot::Send::send:Server Closing Connection\n");

		m_connectionState = CGameBot::ClientState::E_NOTCONNECTED;
	}
	else if (totalBytes == SOCKET_ERROR)
	{
		m_errorLog->WriteWinsockErrorMessage(true, "CGameBot::Send::send:");

		m_connectionState = CGameBot::ClientState::E_NOTCONNECTED;
	}
}

/*
*/
void CGameBot::SetLogin(const char* name)
{
	memset(m_name, 0x00, CServerInfo::E_NAME_SIZE);

	memcpy((void*)m_name, (void*)name, strlen(name));
}

/*
*/
void CGameBot_Accepted(CGameBot* client, CNetwork* network, CServerInfo* serverInfo)
{
	client->m_localClient = serverInfo;

	client->m_connectionState = CGameBot::ClientState::E_CONNECTED;


	CString* message = new CString("Connection accepted");

	// Chat message
	CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_CHAT,
		(void*)message->GetText(), message->GetLength(),
		(void*)serverInfo);

	client->Send(n);

	delete n;

	delete message;


	serverInfo->SetName(client->m_name);

	// This send will be for the recv in CServer::CreateClient where the name is updated
	n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, NULL,
		client->m_name, (int)strlen(client->m_name),
		(void*)serverInfo);

	client->Send(n);

	delete n;
}

/*
*/
void CGameBot_AccountInfo(CGameBot* client, CNetwork* network, CServerInfo* serverInfo)
{
	CString* message = new CString((char*)network->m_data);

	CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_CHAT,
		(void*)message->GetText(), message->GetLength(),
		(void*)serverInfo);

	client->ProcessEvent(n);

	delete n;

	delete message;
}

/*
*/
void CGameBot_Attack(CGameBot* client, CNetwork* network, CServerInfo* serverInfo)
{
	client->m_errorLog->WriteError(true, "CGameBot_ClientAttack:%s\n", serverInfo->m_name);
}

/*
*/
void CGameBot_CameraMove(CGameBot* client, CNetwork* network, CServerInfo* serverInfo)
{
	SendMessage(client->m_hWnd, WM_COMMAND, IDM_CAMERA, 0);
}

/*
*/
void CGameBot_Chat(CGameBot* client, CNetwork* network, CServerInfo* serverInfo)
{
	client->m_errorLog->WriteError(true, "CGameBot_Chat:%s\n", (char*)network->m_data);
}

/*
*/
void CGameBot_Enter(CGameBot* client, CNetwork* network, CServerInfo* serverInfo)
{
	while (client->m_isActiveRender)
	{

	}

	client->m_isActiveUpdate = true;

	client->m_errorLog->WriteError(true, "CGameBot_Enter:%s\n", serverInfo->m_name);


	client->m_camera[serverInfo->m_clientNumber] = new CCamera(1240, 768,
		&serverInfo->m_position,
		45.0f,
		1.0f, 50000.0f);

	client->m_camera[serverInfo->m_clientNumber]->UpdateRotation(
		serverInfo->m_direction.p.x,
		serverInfo->m_direction.p.y,
		serverInfo->m_direction.p.z);

	client->m_camera[serverInfo->m_clientNumber]->UpdateView();

	serverInfo->SetDirection(&client->m_camera[serverInfo->m_clientNumber]->m_look);


	CWavefront* model = client->m_wavefrontManager->Create("model\\cube.wft", "model\\cube.mtl");

	client->m_model[serverInfo->m_clientNumber] = new CObject(model->m_meshs);


	// At this point the client can become active on the server game loop
	if (serverInfo->m_clientNumber == client->m_localClient->m_clientNumber)
	{
		// Request other clients info currently on the server
		CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_INFO,
			nullptr, 0,
			(void*)serverInfo);

		client->Send(n);


		serverInfo->m_state = CServerInfo::E_GAME;

		CString* message = new CString(client->m_localClient->m_name);

		message->Concat(" entered");

		serverInfo->SetChat(message->GetText());

		delete message;

		// Idle event to update this clients info on the server
		n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_IDLE,
			nullptr, 0,
			(void*)serverInfo);

		client->Send(n);

		delete n;
	}

	client->m_isActiveUpdate = false;
}

/*
*/
void CGameBot_Exit(CGameBot* client, CNetwork* network, CServerInfo* serverInfo)
{
	client->m_errorLog->WriteError(true, "CGameBot_ClientExit:%s\n", serverInfo->m_name);
}

/*
*/
void CGameBot_ExitGame(CGameBot* client, CNetwork* network, CServerInfo* serverInfo)
{
	SendMessage(client->m_hWnd, WM_COMMAND, IDM_EXIT, 0);
}


/*
*/
void CGameBot_InfoRequest(CGameBot* client, CNetwork* network, CServerInfo* serverInfo)
{
	client->m_camera[serverInfo->m_clientNumber] = new CCamera(1240, 768,
		&serverInfo->m_position,
		45.0f,
		1.0f, 50000.0f);

	CWavefront* model = client->m_wavefrontManager->Create("model\\cube.wft", "model\\cube.mtl");

	client->m_model[serverInfo->m_clientNumber] = new CObject(model->m_meshs);
}

/*
*/
void CGameBot_LoadEnvironment(CGameBot* client, CNetwork* network, CServerInfo* serverInfo)
{
	if (client->m_serverEnvironment)
	{
		delete client->m_serverEnvironment;
	}

	client->m_errorLog->WriteError(true, "CGameBot_LoadEnvironment::%s\n", (char*)network->m_data);

	client->m_serverEnvironment = new CServerEnvironment((char*)network->m_data);



	// Environment cubes that contain some kind of visible item
	client->m_visibility = new CVisibility(client->m_serverEnvironment->m_width, client->m_serverEnvironment->m_height, 8);

	CWavefront* model = client->m_wavefrontManager->Create("model\\cube.wft", "model\\cube.mtl");

	CObject* collectable = new CObject(model->m_meshs);

	collectable->SetScale(0.250f, 0.250f, 0.250f);
	collectable->SetPosition(5.0f, 0.50f, 5.0f);
	collectable->UpdateServer();

	// Determine which cube list to add the object.
	// Would need to test for all the vertices of the models making them contained in each block they belong with.

	// Grab the collectable list for the cube where the client is located
	int px = (int)(collectable->m_position.x + (client->m_visibility->m_width / 2.0f)) / client->m_visibility->m_gridUnits;
	int pz = (int)(collectable->m_position.z + (client->m_visibility->m_height / 2.0f)) / client->m_visibility->m_gridUnits;

	CLinkList<CObject>* cube = (CLinkList<CObject>*)client->m_visibility->m_collectables->GetElement(2, px, pz);

	if (cube != nullptr)
	{
		// Need to perform the constructor when this object is the first one to be added.
		if (cube->m_list == nullptr)
		{
			cube->Constructor();
		}

		cube->Add(collectable, "item01");
	}



	CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_READY,
		nullptr, 0,
		(void*)serverInfo);

	client->Send(n);

	delete n;
}

/*
*/
void CGameBot_ServerFull(CGameBot* client, CNetwork* network, CServerInfo* serverInfo)
{
	client->m_errorLog->WriteError(true, "CGameBot_ServerFull\n");
}

/*
*/
void CGameBot_SendActivity(CGameBot* client, CNetwork* network, CServerInfo* serverInfo)
{
	while (client->m_isActiveChat)
	{

	}

	client->m_isActiveActivity = true;


	client->m_frametime->Frame();


	BYTE types[10] = {};
	int typeCount = 0;


	if (client->m_camera[client->m_localClient->m_clientNumber] != nullptr)
	{
		client->m_camera[client->m_localClient->m_clientNumber]->UpdateRotation(0.0f, 50.0f * client->m_frametime->m_frametime, 0.0f);

		client->m_camera[client->m_localClient->m_clientNumber]->UpdateView();

		serverInfo->SetDirection(&client->m_camera[client->m_localClient->m_clientNumber]->m_look);
	}

	types[0] = CNetwork::ClientActivity::E_CA_FORWARD;

	CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_ACTIVITY,
		(void*)types, 10,
		(void*)client->m_localClient);

	client->Send(n);

	delete n;

	client->m_isActiveActivity = false;
}

/*
*/
void CGameBot_Update(CGameBot* client, CNetwork* network, CServerInfo* serverInfo)
{
	while (client->m_isActiveRender)
	{
	}

	client->m_isActiveUpdate = true;


	if (serverInfo->m_isAvailable)
	{
		delete client->m_camera[serverInfo->m_clientNumber];
		client->m_camera[serverInfo->m_clientNumber] = nullptr;

		delete client->m_model[serverInfo->m_clientNumber];
		client->m_model[serverInfo->m_clientNumber] = nullptr;

		serverInfo->Reset();

		client->m_isActiveUpdate = false;

		return;
	}


	if (client->m_model[serverInfo->m_clientNumber] != nullptr)
	{
		client->m_camera[serverInfo->m_clientNumber]->SetPosition(&serverInfo->m_position);
		client->m_camera[serverInfo->m_clientNumber]->UpdateView();


		client->m_model[serverInfo->m_clientNumber]->SetPosition(&serverInfo->m_position);

		client->m_model[serverInfo->m_clientNumber]->m_rotation.y = serverInfo->m_direction.PointToDegree().p.y;

		client->m_model[serverInfo->m_clientNumber]->UpdateServer();
	}

	client->m_isActiveUpdate = false;
}

/*
*/
void CGameBot_UpdateCollectable(CGameBot* client, CNetwork* network, CServerInfo* serverInfo)
{
	int px = 0;// (int)(serverInfo->m_position.p.x + (client->m_visibility->m_width / 2.0f)) / client->m_visibility->m_gridUnits;
	int pz = 0;// (int)(serverInfo->m_position.p.z + (client->m_visibility->m_height / 2.0f)) / client->m_visibility->m_gridUnits;

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
unsigned __stdcall CGameBot_ReceiveThread(void* obj)
{
	CGameBot* client = (CGameBot*)obj;

#ifdef _DEBUG
	client->m_errorLog->WriteError(true, "CGameBot_ReceiveThread:Starting\n");
#endif

	client->m_isRunning = true;

	while (client->m_isRunning)
	{
		client->Receive();

		if (client->m_connectionState == CGameBot::E_NOTCONNECTED)
		{
			SendMessage(client->m_hWnd, WM_COMMAND, IDM_DISCONNECT, 0);

			client->m_isRunning = false;
		}
		else
		{
			client->ProcessEvent(client->m_network);
		}
	}

#ifdef _DEBUG
	client->m_errorLog->WriteError(true, "CGameBot_ReceiveThread Ending\n");
#endif

	_endthreadex(0);

	return 0;
}