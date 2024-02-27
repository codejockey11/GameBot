#pragma once

#include "framework.h"

#include "CConstantBuffer.h"

class CIntBuffer
{
public:

	enum
	{
		E_VIEW_COUNT = 64
	};

	CConstantBuffer* m_constantBuffer;

	UINT m_size;
	UINT m_stride;

	int m_values[CIntBuffer::E_VIEW_COUNT];

	CIntBuffer();
	CIntBuffer(CVideoDevice* videoDevice, CErrorLog* errorLog, ComPtr<ID3D12GraphicsCommandList> commandList);
	~CIntBuffer();

	void UpdateConstantBuffer();
};