#include "CFloat4x4Buffer.h"

/*
*/
CFloat4x4Buffer::CFloat4x4Buffer()
{
	memset(this, 0x00, sizeof(CFloat4x4Buffer));
}

/*
*/
CFloat4x4Buffer::CFloat4x4Buffer(CVideoDevice* videoDevice, CErrorLog* errorLog, ComPtr<ID3D12GraphicsCommandList> commandList)
{
	memset(this, 0x00, sizeof(CFloat4x4Buffer));

	m_stride = sizeof(XMFLOAT4X4);

	m_size = CFloat4x4Buffer::E_VIEW_COUNT * m_stride;

	m_constantBuffer = new CConstantBuffer(videoDevice, errorLog, commandList, m_size);
}

/*
*/
CFloat4x4Buffer::~CFloat4x4Buffer()
{
	delete m_constantBuffer;
}

/*
*/
void CFloat4x4Buffer::UpdateConstantBuffer()
{
	m_constantBuffer->UpdateBuffer((void*)m_values);
}