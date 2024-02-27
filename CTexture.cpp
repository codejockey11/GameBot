#include "CTexture.h"

/*
*/
CTexture::CTexture()
{
	memset(this, 0x00, sizeof(CTexture));
}

/*
*/
CTexture::CTexture(CErrorLog* errorLog, CVideoDevice* videoDevice, const char* filename)
{
	memset(this, 0x00, sizeof(CTexture));

	m_errorLog = errorLog;

	m_videoDevice = videoDevice;

	for (UINT i = 0; i < CVideoDevice::E_BACKBUFFER_COUNT; i++)
	{
		m_commandAllocators[i] = m_videoDevice->CreateCommandAllocator();
	}

	m_commandList = m_videoDevice->CreateCommandList(m_commandAllocators[0]);

	// m_commandList->Close() happens farther down in the contructor when the texture is actually uploaded

	m_name = new CString(filename);

	if (m_name->Search(".bmp"))
	{
		m_bimage = new CBmpImage(filename);

		if (m_bimage->m_isInitialized)
		{
			m_width = m_bimage->m_bmapInfo.bmiHeader.biWidth;
			m_height = m_bimage->m_bmapInfo.bmiHeader.biHeight;

			CTexture::CreateResource(m_bimage->m_pixels32);

			m_isInitialized = true;
		}
		else
		{
			m_errorLog->WriteError("CTexture::CTexture::NotCreated:%s\n", filename);
		}

		delete m_bimage;
	}
	else if (m_name->Search(".tga"))
	{
		m_image = new CTgaImage(filename);

		if (m_image->m_isInitialized)
		{
			m_width = m_image->m_header->m_width;
			m_height = m_image->m_header->m_height;

			CTexture::CreateResource(m_image->m_pixels32);

			m_isInitialized = true;
		}
		else
		{
			m_errorLog->WriteError("CTexture::CTexture::NotCreated:%s\n", filename);
		}

		delete m_image;
	}
}

/*
*/
CTexture::~CTexture()
{
	m_textureUploadHeap.Reset();

	m_texture.Reset();

	m_commandList.Reset();

	for (int i = 0; i < CVideoDevice::E_BACKBUFFER_COUNT; i++)
	{
		m_commandAllocators[i].Reset();
	}

	delete m_name;
}

/*
*/
void CTexture::CreateResource(BYTE* pixels)
{
	m_textureDesc.MipLevels = 1;
	m_textureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	m_textureDesc.Width = m_width;
	m_textureDesc.Height = m_height;
	m_textureDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	m_textureDesc.DepthOrArraySize = 1;
	m_textureDesc.SampleDesc.Count = 1;
	m_textureDesc.SampleDesc.Quality = 0;
	m_textureDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	m_textureHeapProperties.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	m_textureHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	m_textureHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	m_textureHeapProperties.CreationNodeMask = 1;
	m_textureHeapProperties.VisibleNodeMask = 1;

	HRESULT hr = m_videoDevice->m_device->CreateCommittedResource(&m_textureHeapProperties,
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&m_textureDesc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_texture));

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CTexture::CreateResource::CreateCommittedResource::texture:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return;
	}

	m_texture->SetName(m_name->GetWText());


	// Create the GPU upload buffer.

	m_uploadBufferSize = 0;

	m_videoDevice->m_device->GetCopyableFootprints(&m_textureDesc, 0, 1, 0, nullptr, nullptr, nullptr, &m_uploadBufferSize);

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
		IID_PPV_ARGS(&m_textureUploadHeap));

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CTexture::CreateResource::CreateCommittedResource::textureUploadHeap:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return;
	}

	m_textureUploadHeap->SetName(L"CTexture::UploadHeap");


	m_uploadData.pData = pixels; // pointer to start of image 
	m_uploadData.RowPitch = (LONG_PTR)m_width * 4;   // all images stored as 32 bit
	m_uploadData.SlicePitch = m_uploadData.RowPitch * m_height; // one slice

	void* pData = {};

	hr = m_textureUploadHeap->Map(0, nullptr, &pData);  // get GPU address for where the texture bytes are stored

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CTexture::CreateResource::textureUploadHeap->Map:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return;
	}

	memcpy(pData, m_uploadData.pData, m_uploadData.SlicePitch); // store the color bytes

	m_textureUploadHeap->Unmap(0, nullptr); // release the GPU buffer


	UINT64 RequiredSize = 0;

	UINT64 MemToAlloc = sizeof(D3D12_PLACED_SUBRESOURCE_FOOTPRINT) + sizeof(UINT) + sizeof(UINT64);

	if (MemToAlloc > SIZE_MAX)
	{
		m_errorLog->WriteError("CTexture::CreateResource::MemToAlloc:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return;
	}

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT* pMem = (D3D12_PLACED_SUBRESOURCE_FOOTPRINT*)HeapAlloc(GetProcessHeap(), 0, MemToAlloc);

	if (pMem == nullptr)
	{
		m_errorLog->WriteError("CTexture::CreateResource::HeapAlloc:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return;
	}


	m_videoDevice->m_device->GetCopyableFootprints(&m_textureDesc, 0, 1, 0, pMem, &m_height, (UINT64*)&m_uploadData.RowPitch, &RequiredSize);


	D3D12_TEXTURE_COPY_LOCATION Src = {};

	Src.pResource = m_textureUploadHeap.Get();
	Src.Type = D3D12_TEXTURE_COPY_TYPE::D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	Src.PlacedFootprint = *pMem;

	D3D12_TEXTURE_COPY_LOCATION Dst = {};

	Dst.pResource = m_texture.Get();
	Dst.Type = D3D12_TEXTURE_COPY_TYPE::D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	Dst.SubresourceIndex = 0;

	m_commandList->CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);


	if (!HeapFree(GetProcessHeap(), 0, pMem))
	{
		m_errorLog->WriteError("CTexture::CreateResource::HeapFree:false\n");

		return;
	}


	m_barrier.Transition.pResource = m_texture.Get();
	m_barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST;
	m_barrier.Transition.StateAfter = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	m_commandList->ResourceBarrier(1, &m_barrier);

	m_commandList->Close();


	ID3D12CommandList* commandLists[] = { m_commandList.Get() };

	m_videoDevice->m_commandQueue->ExecuteCommandLists(1, commandLists);
}

/*
*/
CDynamicTexture::CDynamicTexture(CErrorLog* errorLog, CVideoDevice* videoDevice, ComPtr<ID3D12GraphicsCommandList> commandList,
	UINT width, UINT height, const char* name)
{
	memset(this, 0x00, sizeof(CTexture));

	m_errorLog = errorLog;

	m_videoDevice = videoDevice;

	m_commandList = commandList;

	m_width = width;

	m_height = height;

	m_name = new CString(name);

	CDynamicTexture::CreateResource();
}

/*
* Resource deconstruct happens in parent
*/
CDynamicTexture::~CDynamicTexture()
{
}

/*
*/
void CDynamicTexture::CreateResource()
{
	m_textureDesc.MipLevels = 1;
	m_textureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	m_textureDesc.Width = m_width;
	m_textureDesc.Height = m_height;
	m_textureDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;
	m_textureDesc.DepthOrArraySize = 1;
	m_textureDesc.SampleDesc.Count = 1;
	m_textureDesc.SampleDesc.Quality = 0;
	m_textureDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	m_textureHeapProperties.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	m_textureHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	m_textureHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	m_textureHeapProperties.CreationNodeMask = 1;
	m_textureHeapProperties.VisibleNodeMask = 1;

	HRESULT hr = m_videoDevice->m_device->CreateCommittedResource(&m_textureHeapProperties,
		D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
		&m_textureDesc,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_texture));

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CDynamicTexture::CreateResource::CreateCommittedResource::texture:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return;
	}

	m_texture->SetName(m_name->GetWText());


	// Create the GPU upload buffer.

	m_uploadBufferSize = 0;

	m_videoDevice->m_device->GetCopyableFootprints(&m_textureDesc, 0, 1, 0, nullptr, nullptr, nullptr, &m_uploadBufferSize);

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
		IID_PPV_ARGS(&m_textureUploadHeap));

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CDynamicTexture::CreateResource::CreateCommittedResource::textureUploadHeap:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return;
	}

	m_textureUploadHeap->SetName(L"CTexture::UploadHeap");
}

/*
*/
void CDynamicTexture::UpdateTextureResource(void* pixels)
{
	m_uploadData.pData = pixels;
	m_uploadData.RowPitch = (LONG_PTR)m_width * 4;   // all images stored as 32 bit
	m_uploadData.SlicePitch = m_uploadData.RowPitch * m_height;

	void* pData = {};

	HRESULT hr = m_textureUploadHeap->Map(0, nullptr, &pData);

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CDynamicTexture::UpdateTextureResource::textureUploadHeap->Map:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return;
	}

	memcpy(pData, m_uploadData.pData, m_uploadData.SlicePitch);

	m_textureUploadHeap->Unmap(0, nullptr);


	UINT64 RequiredSize = 0;

	UINT64 MemToAlloc = sizeof(D3D12_PLACED_SUBRESOURCE_FOOTPRINT) + sizeof(UINT) + sizeof(UINT64);

	if (MemToAlloc > SIZE_MAX)
	{
		m_errorLog->WriteError("CDynamicTexture::UpdateTextureResource::MemToAlloc:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return;
	}

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT* pMem = (D3D12_PLACED_SUBRESOURCE_FOOTPRINT*)HeapAlloc(GetProcessHeap(), 0, MemToAlloc);

	if (pMem == nullptr)
	{
		m_errorLog->WriteError("CDynamicTexture::UpdateTextureResource::HeapAlloc:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return;
	}


	m_videoDevice->m_device->GetCopyableFootprints(&m_textureDesc, 0, 1, 0, pMem, &m_height, (UINT64*)&m_uploadData.RowPitch, &RequiredSize);


	D3D12_TEXTURE_COPY_LOCATION Src = {};

	Src.pResource = m_textureUploadHeap.Get();
	Src.Type = D3D12_TEXTURE_COPY_TYPE::D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	Src.PlacedFootprint = *pMem;

	D3D12_TEXTURE_COPY_LOCATION Dst = {};

	Dst.pResource = m_texture.Get();
	Dst.Type = D3D12_TEXTURE_COPY_TYPE::D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	Dst.SubresourceIndex = 0;

	m_commandList->CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);


	if (!HeapFree(GetProcessHeap(), 0, pMem))
	{
		m_errorLog->WriteError("CDynamicTexture::UpdateTextureResource::HeapFree:false\n");

		return;
	}


	m_barrier.Transition.pResource = m_texture.Get();
	m_barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST;
	m_barrier.Transition.StateAfter = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;


	m_commandList->ResourceBarrier(1, &m_barrier);
}