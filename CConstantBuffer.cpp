#include "CConstantBuffer.h"

/*
*/
CConstantBuffer::CConstantBuffer()
{
	memset(this, 0x00, sizeof(CConstantBuffer));
}

/*
*/
CConstantBuffer::CConstantBuffer(CVideoDevice* videoDevice, CErrorLog* errorLog, ComPtr<ID3D12GraphicsCommandList> commandList, UINT bufferSize)
{
	memset(this, 0x00, sizeof(CConstantBuffer));

	m_videoDevice = videoDevice;

	m_errorLog = errorLog;

	m_bufferSize = bufferSize;

	m_commandList = commandList;

	m_bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	m_bufferDesc.Alignment = 0;
	m_bufferDesc.Width = bufferSize;
	m_bufferDesc.Height = 1;
	m_bufferDesc.DepthOrArraySize = 1;
	m_bufferDesc.MipLevels = 1;
	m_bufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	m_bufferDesc.SampleDesc.Count = 1;
	m_bufferDesc.SampleDesc.Quality = 0;
	m_bufferDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	m_bufferDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	m_bufferHeapProperties.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	m_bufferHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	m_bufferHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	m_bufferHeapProperties.CreationNodeMask = 1;
	m_bufferHeapProperties.VisibleNodeMask = 1;

	HRESULT hr = m_videoDevice->m_device->CreateCommittedResource(&m_bufferHeapProperties,
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&m_bufferDesc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		nullptr,
		IID_PPV_ARGS(&m_buffer));

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CConstantBuffer::CConstantBuffer::CreateCommittedResource::buffer:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return;
	}

	m_buffer->SetName(L"CConstantBuffer::CConstantBuffer");

	// Create the GPU upload buffer.	
	m_videoDevice->m_device->GetCopyableFootprints(&m_bufferDesc, 0, 1, 0, nullptr, nullptr, nullptr, &m_uploadBufferSize);

	m_allocInfo.SizeInBytes = m_uploadBufferSize;
	m_allocInfo.Alignment = 0;

	m_heapDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	m_heapDesc.Alignment = m_allocInfo.Alignment;
	m_heapDesc.Width = m_uploadBufferSize;
	m_heapDesc.Height = 1;
	m_heapDesc.DepthOrArraySize = 1;
	m_heapDesc.MipLevels = 1;
	m_heapDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	m_heapDesc.SampleDesc.Count = 1;
	m_heapDesc.SampleDesc.Quality = 0;
	m_heapDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	m_heapDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	m_heapProperties.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	m_heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	m_heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	m_heapProperties.CreationNodeMask = 1;
	m_heapProperties.VisibleNodeMask = 1;

	hr = m_videoDevice->m_device->CreateCommittedResource(&m_heapProperties,
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&m_heapDesc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_uploadHeap));

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CConstantBuffer::CConstantBuffer::CreateCommittedResource::uploadHeap:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return;
	}

	m_uploadHeap->SetName(L"CConstantBuffer::CConstantBuffer");
}

/*
*/
CConstantBuffer::~CConstantBuffer()
{
	m_uploadHeap.Reset();

	m_buffer.Reset();
}

/*
*/
void CConstantBuffer::UpdateBuffer(void* values)
{
	m_barrier.Transition.pResource = m_buffer.Get();
	m_barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
	m_barrier.Transition.StateAfter = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST;

	m_commandList->ResourceBarrier(1, &m_barrier);


	void* pDest;

	HRESULT hr = m_uploadHeap->Map(0, nullptr, &pDest);

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CConstantBuffer::UpdateBuffer::uploadHeap->Map:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return;
	}

	memcpy(pDest, values, m_bufferSize);

	m_uploadHeap->Unmap(0, nullptr);


	m_commandList->CopyResource(m_buffer.Get(), m_uploadHeap.Get());


	m_barrier.Transition.pResource = m_buffer.Get();
	m_barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST;
	m_barrier.Transition.StateAfter = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;

	m_commandList->ResourceBarrier(1, &m_barrier);
}