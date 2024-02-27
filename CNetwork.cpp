#include "CNetwork.h"

/*
*/
CNetwork::CNetwork()
{
	memset(this, 0x00, sizeof(CNetwork));
}

/*
*/
CNetwork::CNetwork(BYTE audience, BYTE type, void* data, int length, void* serverInfo)
{
	memset(this, 0x00, sizeof(CNetwork));

	m_audience = audience;
	m_type = type;

	CNetwork::SetData(data, length);
	CNetwork::SetServerInfo(serverInfo);
}

/*
*/
CNetwork::~CNetwork()
{
}

/*
*/
void CNetwork::SetData(void* data, int length)
{
	if (data == nullptr)
	{
		return;
	}

	if (length > CNetwork::E_DATA_SIZE)
	{
		length = CNetwork::E_DATA_SIZE;
	}

	memcpy_s((void*)m_data, CNetwork::E_DATA_SIZE, data, length);
}

/*
*/
void CNetwork::SetServerInfo(void* serverInfo)
{
	if (serverInfo == nullptr)
	{
		return;
	}

	memcpy_s((void*)m_serverInfo, CNetwork::E_SERVER_INFO_SIZE, serverInfo, CNetwork::E_SERVER_INFO_SIZE);
}