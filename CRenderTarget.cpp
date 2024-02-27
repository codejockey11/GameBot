#include "CRenderTarget.h"

/*
*/
CRenderTarget::CRenderTarget()
{
	memset(this, 0x00, sizeof(CRenderTarget));
}

/*
*/
CRenderTarget::CRenderTarget(CErrorLog* errorLog, ComPtr<ID3D12Device> device)
{
	memset(this, 0x00, sizeof(CRenderTarget));

	m_errorLog = errorLog;

	m_device = device;

	m_barrier.Flags = D3D12_RESOURCE_BARRIER_FLAGS::D3D12_RESOURCE_BARRIER_FLAG_NONE;
	m_barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET;
	m_barrier.Transition.StateAfter = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT;
	m_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE::D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;

	m_heapDesc.NumDescriptors = 1;
	m_heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	m_heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HRESULT hr = device->CreateDescriptorHeap(&m_heapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&m_heap);

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CRenderTarget::CRenderTarget::CreateDescriptorHeap:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());
	}

	m_handle = m_heap->GetCPUDescriptorHandleForHeapStart();

	m_heap->SetName(L"CRenderTarget::Heap");
}

/*
*/
CRenderTarget::~CRenderTarget()
{
	m_resource.Reset();

	m_wrappedBackBuffer.Reset();

	m_surface.Reset();

	m_d2dRenderTarget.Reset();

	m_heap.Reset();
}

/*
*/
void CRenderTarget::AdditionalTarget(ComPtr<ID3D12Resource> resource, UINT64 width, UINT height)
{
	m_resource = resource;

	D3D12_RESOURCE_DESC desc = {};

	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Width = width;
	desc.Height = height;
	desc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	desc.DepthOrArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D;

	D3D12_HEAP_PROPERTIES heapProperties = {};

	heapProperties.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 1;
	heapProperties.VisibleNodeMask = 1;


	m_clearValue.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;



	HRESULT hr = m_device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, &m_clearValue, IID_PPV_ARGS(&m_resource));

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CRenderTarget::AdditionalTarget::CreateCommittedResource::resource:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return;
	}

	m_resource->SetName(L"CRenderTarget::AdditionalRenderTarget");

	m_device->CreateRenderTargetView(m_resource.Get(), nullptr, m_handle);
}

/*
*/
void CRenderTarget::SetPreDrawBarrier()
{
	m_barrier.Transition.pResource = m_resource.Get();
	m_barrier.Transition.StateBefore = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT;
	m_barrier.Transition.StateAfter = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET;
}

/*
*/
void CRenderTarget::SwapChainTarget(ComPtr<IDXGISwapChain3> swapChain, UINT bufferNbr)
{
	HRESULT hr = swapChain->GetBuffer(bufferNbr, IID_PPV_ARGS(&m_resource));

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CRenderTarget::SwapChainTarget::GetBuffer:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());
	}

	m_device->CreateRenderTargetView(m_resource.Get(), nullptr, m_handle);

	m_resource->SetName(L"CRenderTarget::SwapChainTarget");
}

/*
*/
void CRenderTarget::WrapResource(float dpi, ComPtr<ID3D11On12Device> d3d11On12Device, ComPtr<ID2D1DeviceContext1> d2dDeviceContext)
{
	m_d3d11On12Device = d3d11On12Device;

	m_d2dDeviceContext = d2dDeviceContext;

	D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET };

	HRESULT hr = m_d3d11On12Device->CreateWrappedResource(m_resource.Get(), &d3d11Flags, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, IID_PPV_ARGS(&m_wrappedBackBuffer));

	// Create a render target for D2D to draw directly to this back buffer.
	hr = m_wrappedBackBuffer->QueryInterface(m_surface.GetAddressOf());

	m_bitmapProperties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS::D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS::D2D1_BITMAP_OPTIONS_CANNOT_DRAW, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpi, dpi);

	if (m_surface)
	{
		hr = m_d2dDeviceContext->CreateBitmapFromDxgiSurface(m_surface.Get(), &m_bitmapProperties, m_d2dRenderTarget.GetAddressOf());

		if (m_d2dRenderTarget)
		{
			m_rtSize = m_d2dRenderTarget->GetSize();
		}
	}
}