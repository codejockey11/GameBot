#include "CIntBuffer.h"

/*
*/
CIntBuffer::CIntBuffer()
{
	memset(this, 0x00, sizeof(CIntBuffer));
}

/*
*/
CIntBuffer::CIntBuffer(CVideoDevice* videoDevice, CErrorLog* errorLog, ComPtr<ID3D12GraphicsCommandList> commandList)
{
	memset(this, 0x00, sizeof(CIntBuffer));

	m_stride = sizeof(int);

	m_size = CIntBuffer::E_VIEW_COUNT * m_stride;

	m_constantBuffer = new CConstantBuffer(videoDevice, errorLog, commandList, m_size);
}

/*
*/
CIntBuffer::~CIntBuffer()
{
	delete m_constantBuffer;
}

/*
*/
void CIntBuffer::UpdateConstantBuffer()
{
	m_constantBuffer->UpdateBuffer((void*)m_values);
}