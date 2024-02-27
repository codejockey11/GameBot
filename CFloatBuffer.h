#pragma once

#include "framework.h"

#include "CConstantBuffer.h"

class CFloatBuffer
{
public:

	enum
	{
		E_VIEW_COUNT = 64
	};

	CConstantBuffer* m_constantBuffer;
	
	UINT m_size;
	UINT m_stride;
	
	float m_values[CFloatBuffer::E_VIEW_COUNT];

	CFloatBuffer();
	CFloatBuffer(CVideoDevice* videoDevice, CErrorLog* errorLog, ComPtr<ID3D12GraphicsCommandList> commandList);
	~CFloatBuffer();

	void UpdateConstantBuffer();
};