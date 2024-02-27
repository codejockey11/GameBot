#include "CIndexBuffer.h"

/*
*/
CIndexBuffer::CIndexBuffer()
{
	memset(this, 0x00, sizeof(CIndexBuffer));
}

/*
*/
CIndexBuffer::CIndexBuffer(CVideoDevice* videoDevice, CErrorLog* errorLog, ComPtr<ID3D12GraphicsCommandList> commandList, UINT count, void* indicies)
{
	memset(this, 0x00, sizeof(CIndexBuffer));

	m_commandList = commandList;

	m_count = count;

	if (CIndexBuffer::CreateResource(videoDevice, errorLog))
	{
		CIndexBuffer::LoadBuffer(indicies);
	}
}

/*
*/
CIndexBuffer::~CIndexBuffer()
{
	m_indexBuffer.Reset();
}

/*
*/
BYTE CIndexBuffer::CreateResource(CVideoDevice* videoDevice, CErrorLog* errorLog)
{
	m_heapProperties.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	m_heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	m_heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	m_heapProperties.CreationNodeMask = 1;
	m_heapProperties.VisibleNodeMask = 1;

	m_resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	m_resourceDesc.Alignment = 0;
	m_resourceDesc.Width = m_count * sizeof(UINT);
	m_resourceDesc.Height = 1;
	m_resourceDesc.DepthOrArraySize = 1;
	m_resourceDesc.MipLevels = 1;
	m_resourceDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	m_resourceDesc.SampleDesc.Count = 1;
	m_resourceDesc.SampleDesc.Quality = 0;
	m_resourceDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	m_resourceDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	HRESULT hr = videoDevice->m_device->CreateCommittedResource(&m_heapProperties,
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&m_resourceDesc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_indexBuffer));

	if (hr != S_OK)
	{
		errorLog->WriteError("CIndexBuffer::CreateResource::CreateCommittedResource:%s %s\n",
			errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return false;
	}

	m_indexBuffer->SetName(L"CIndexBuffer::Resource");

	m_view.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
	m_view.Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
	m_view.SizeInBytes = m_count * sizeof(UINT);

	return true;
}

/*
*/
void CIndexBuffer::Draw()
{
	m_commandList->IASetIndexBuffer(&m_view);
}

/*
*/
void CIndexBuffer::LoadBuffer(void* indices)
{
	void* pData = 0;

	m_indexBuffer->Map(0, nullptr, &pData);

	memcpy(pData, indices, m_count * sizeof(UINT));

	m_indexBuffer->Unmap(0, nullptr);
}