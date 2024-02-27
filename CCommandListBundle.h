#pragma once

#include "framework.h"

#include "CErrorLog.h"
#include "CVideoDevice.h"

class CCommandListBundle
{
public:

	enum
	{
		E_MAX_BUNDLES = 5,
		E_MAX_COMMAND_LIST_COUNT = 50
	};

	ID3D12CommandList* m_commandListPointers[CCommandListBundle::E_MAX_COMMAND_LIST_COUNT];

	ComPtr<ID3D12GraphicsCommandList> m_commandLists[CCommandListBundle::E_MAX_COMMAND_LIST_COUNT];

	CCommandListBundle();
	CCommandListBundle(CErrorLog* errorLog, CVideoDevice* device);
	~CCommandListBundle();

	void Add(ComPtr<ID3D12GraphicsCommandList> commandList);
	void Clear();
	void Close();
	void Perform();

private:

	CErrorLog* m_errorLog;
	CVideoDevice* m_videoDevice;

	UINT m_count;
};

