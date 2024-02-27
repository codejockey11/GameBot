#include "CFloatBuffer.h"

/*
*/
CFloatBuffer::CFloatBuffer()
{
	memset(this, 0x00, sizeof(CFloatBuffer));
}

/*
*/
CFloatBuffer::CFloatBuffer(CVideoDevice* videoDevice, CErrorLog* errorLog, ComPtr<ID3D12GraphicsCommandList> commandList)
{
	memset(this, 0x00, sizeof(CFloatBuffer));

	m_stride = sizeof(float);

	m_size = CFloatBuffer::E_VIEW_COUNT * m_stride;

	m_constantBuffer = new CConstantBuffer(videoDevice, errorLog, commandList, m_size);
}

/*
*/
CFloatBuffer::~CFloatBuffer()
{
	delete m_constantBuffer;
}

/*
*/
void CFloatBuffer::UpdateConstantBuffer()
{
	m_constantBuffer->UpdateBuffer((void*)m_values);
}