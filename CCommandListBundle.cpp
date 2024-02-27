#include "CCommandListBundle.h"

CCommandListBundle::CCommandListBundle()
{
	memset(this, 0x00, sizeof(CCommandListBundle));
}

CCommandListBundle::CCommandListBundle(CErrorLog* errorLog, CVideoDevice* videoDevice)
{
	memset(this, 0x00, sizeof(CCommandListBundle));

	m_errorLog = errorLog;

	m_videoDevice = videoDevice;
}

CCommandListBundle::~CCommandListBundle()
{
}

// The order that the command lists are added determines the drawing from back to front if necessary
void CCommandListBundle::Add(ComPtr<ID3D12GraphicsCommandList> commandList)
{
	m_count++;

	if (m_count > CCommandListBundle::E_MAX_COMMAND_LIST_COUNT)
	{
		m_errorLog->WriteError("CCommandListBundle::Add::Command list count exceeds maximum\n");

		return;
	}

	for (UINT i = 0; i < m_count; i++)
	{
		if (m_commandLists[i] == 0)
		{
			m_commandLists[i] = commandList;

			m_commandListPointers[i] = commandList.Get();

			return;
		}
	}
}

void CCommandListBundle::Clear()
{
	for (UINT i = 0; i < m_count; i++)
	{
		m_commandLists[i] = nullptr;

		m_commandListPointers[i] = nullptr;
	}

	m_count = 0;
}

void CCommandListBundle::Close()
{
	for (UINT i = 0; i < m_count; i++)
	{
		if (m_commandLists[i] == 0)
		{
			return;
		}

		m_commandLists[i]->Close();
	}
}

void CCommandListBundle::Perform()
{
	m_videoDevice->m_commandQueue->ExecuteCommandLists(m_count, (ID3D12CommandList* const*)m_commandListPointers);
}