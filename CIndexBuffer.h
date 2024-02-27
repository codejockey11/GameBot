#pragma once

#include "framework.h"

#include "CErrorLog.h"
#include "CVideoDevice.h"

class CIndexBuffer
{
public:

	ComPtr<ID3D12Resource> m_indexBuffer;
	ComPtr<ID3D12GraphicsCommandList> m_commandList;

	D3D12_INDEX_BUFFER_VIEW	m_view;

	UINT m_count;

	CIndexBuffer();
	CIndexBuffer(CVideoDevice* videoDevice, CErrorLog* errorLog, ComPtr<ID3D12GraphicsCommandList> commandList, UINT count, void* indicies);
	~CIndexBuffer();

	BYTE CreateResource(CVideoDevice* videoDevice, CErrorLog* errorLog);
	void Draw();
	void LoadBuffer(void* indices);

private:

	D3D12_HEAP_PROPERTIES m_heapProperties;

	D3D12_RESOURCE_DESC m_resourceDesc;
};