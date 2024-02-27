#pragma once

#include "framework.h"

#include "CConstantBuffer.h"

class CFloat4x4Buffer
{
public:

	enum
	{
		E_VIEW_COUNT = 16
	};

	CConstantBuffer* m_constantBuffer;
	
	UINT m_size;
	UINT m_stride;
	
	XMFLOAT4X4 m_values[CFloat4x4Buffer::E_VIEW_COUNT];

	CFloat4x4Buffer();
	CFloat4x4Buffer(CVideoDevice* videoDevice, CErrorLog* errorLog, ComPtr<ID3D12GraphicsCommandList> commandList);
	~CFloat4x4Buffer();

	void UpdateConstantBuffer();
};