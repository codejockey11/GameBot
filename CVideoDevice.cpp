#include "CVideoDevice.h"

/*
*/
CVideoDevice::CVideoDevice()
{
	memset(this, 0x00, sizeof(CVideoDevice));
}

/*
*/
CVideoDevice::CVideoDevice(CErrorLog* errorLog, HWND hWnd, int width, int height, bool windowed, CColor* color)
{
	memset(this, 0x00, sizeof(CVideoDevice));

	m_errorLog = errorLog;

	m_hWnd = hWnd;

	m_width = width;
	m_height = height;

	m_windowed = windowed;

	m_color = color;

	m_posX = (GetSystemMetrics(SM_CXSCREEN) - m_width) / 2;
	m_posY = ((GetSystemMetrics(SM_CYSCREEN) - m_height) / 2) - 20;


	GetWindowRect(GetDesktopWindow(), &m_desktop);

	m_middle.x = m_posX + (m_width / 2);
	m_middle.y = m_posY + (m_height / 2);

	m_vsyncEnabled = false;

	m_swapChainViewport = new CViewport(0.0f, 0.0f, (float)m_width, (float)m_height);


#if defined(_DEBUG)
	HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(&m_debugController));

	m_debugController->EnableDebugLayer();

	m_factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif	

	CVideoDevice::DXGIFactory();
	CVideoDevice::GetHardwareAdapter();
	CVideoDevice::CreateDevice();

	// following section from github.com/jpvanoosten/LearningDirectX12/blob/master/Tutorial2/src/Application.cpp
	#ifdef _DEBUG

	m_device->QueryInterface(IID_PPV_ARGS(&m_infoQueue));


	// Suppress whole categories of messages
	//D3D12_MESSAGE_CATEGORY Categories[] = {};

	D3D12_MESSAGE_SEVERITY Severities[] =
	{
		D3D12_MESSAGE_SEVERITY::D3D12_MESSAGE_SEVERITY_INFO
	};

	// Suppress individual messages by their ID
	D3D12_MESSAGE_ID DenyIds[] =
	{
		D3D12_MESSAGE_ID::D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE   // I'm really not sure how to avoid this message.
	};

	D3D12_INFO_QUEUE_FILTER NewFilter = {};

	//NewFilter.DenyList.NumCategories = _countof(Categories);
	//NewFilter.DenyList.pCategoryList = Categories;

	NewFilter.DenyList.NumSeverities = _countof(Severities);
	NewFilter.DenyList.pSeverityList = Severities;

	NewFilter.DenyList.NumIDs = _countof(DenyIds);
	NewFilter.DenyList.pIDList = DenyIds;

	m_infoQueue->PushStorageFilter(&NewFilter);
	#endif

	CVideoDevice::CreateCommandQueue();
	CVideoDevice::CreateSwapChain();
	CVideoDevice::Create11on12Device();
	CVideoDevice::CreateRenderTargets();
	CVideoDevice::CreateDepthBuffers();


	for (UINT i = 0; i < CVideoDevice::E_BACKBUFFER_COUNT; i++)
	{
		m_commandAllocators[i] = CVideoDevice::CreateCommandAllocator();
	}

	m_commandList = CVideoDevice::CreateCommandList(m_commandAllocators[0]);

	m_commandList->SetName(L"CVideoDevice::CommandList");

	m_commandList->Close();


	// Create at least one fence for the physical video card
	CVideoDevice::CreateFence();
}

/*
*/
CVideoDevice::~CVideoDevice()
{
	CVideoDevice::WaitForGPU();

	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, nullptr);
	}

	CloseHandle(m_fenceEvent);

	m_fenceEvent = 0;

	m_fence.Reset();


	m_commandList.Reset();

	for (int i = 0; i < CVideoDevice::E_BACKBUFFER_COUNT; i++)
	{
		m_commandAllocators[i].Reset();
	}

	for (UINT i = 0; i < CVideoDevice::E_BACKBUFFER_COUNT; i++)
	{
		delete m_swapChainDepthBuffers[i];
	}

	for (UINT i = 0; i < CVideoDevice::E_BACKBUFFER_COUNT; i++)
	{
		delete m_swapChainRenderTargets[i];
	}

	delete m_d3d11On12;


	m_swapChain.Reset();

	m_commandQueue.Reset();

	m_infoQueue.Reset();

	m_device.Reset();

	m_adapter.Reset();

	m_factory.Reset();

	delete m_swapChainViewport;

	#ifdef _DEBUG
	m_debugController.Reset();

	IDXGIDebug1* pDebug = nullptr;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&pDebug))))
	{
		pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS::DXGI_DEBUG_RLO_DETAIL);
		pDebug->Release();
	}
	#endif
}

/*
*/
void CVideoDevice::Create11on12Device()
{
	m_d3d11On12 = new CD3D11On12(m_errorLog, m_device, m_commandQueue);
}

/*
*/
ComPtr<ID3D12CommandAllocator> CVideoDevice::CreateCommandAllocator()
{
	ComPtr<ID3D12CommandAllocator> commandAllocator;

	HRESULT hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CVideoDevice::CommandProcessor::CreateCommandAllocator:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return nullptr;
	}

	commandAllocator->SetName(L"CVideoDevice::CommandAllocator");

	return commandAllocator;
}

/*
*/
ComPtr<ID3D12GraphicsCommandList> CVideoDevice::CreateCommandList(ComPtr<ID3D12CommandAllocator> ca)
{
	ComPtr<ID3D12GraphicsCommandList> commandList;

	HRESULT hr = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, ca.Get(), nullptr, IID_PPV_ARGS(&commandList));

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CVideoDevice::CreateCommandList::CreateCommandList:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return nullptr;
	}

	return commandList;
}

/*
*/
void CVideoDevice::CreateCommandQueue()
{
	m_commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
	m_commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	m_commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	m_commandQueueDesc.NodeMask = 0;

	HRESULT hr = m_device->CreateCommandQueue(&m_commandQueueDesc, __uuidof(ID3D12CommandQueue), (void**)&m_commandQueue);

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CVideoDevice::CommandQueue::CreateCommandQueue:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());
	}
}

/*
*/
void CVideoDevice::CreateDepthBuffers()
{
	for (UINT i = 0; i < CVideoDevice::E_BACKBUFFER_COUNT; i++)
	{
		m_swapChainDepthBuffers[i] = new CDepthBuffer(m_errorLog, m_device, m_width, m_height);
	}
}

/*
*/
void CVideoDevice::CreateDevice()
{
	HRESULT hr = D3D12CreateDevice(m_adapter.Get(), D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1, _uuidof(ID3D12Device), (void**)&m_device);

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CVideoDevice::CreateDevice::D3D12CreateDevice:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return;
	}

	hr = m_device->CheckFeatureSupport(D3D12_FEATURE::D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, (void*)&m_sampleQualityLevels, sizeof(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS));
	if (hr == S_OK)
	{
		m_errorLog->WriteError("CVideoDevice::CreateDevice::D3D12_FEATURE::D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS:%i\n", m_sampleQualityLevels.NumQualityLevels);
	}
	else
	{
		m_errorLog->WriteError("CVideoDevice::CreateDevice::D3D12_FEATURE::D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS:None\n");
	}
}

/*
*/
void CVideoDevice::CreateFence()
{
	HRESULT hr = m_device->CreateFence(m_fenceValues[m_backbufferIndex], D3D12_FENCE_FLAGS::D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CVideoDevice::CreateFence::CreateFence:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return;
	}

	m_fence->SetName(L"CVideoDevice::Fence");

	m_fenceValues[m_backbufferIndex]++;

	m_fenceEvent = CreateEvent(nullptr, false, false, nullptr);
}

/*
*/
void CVideoDevice::CreateRenderTargets()
{
	float dpi = (float)GetDpiForWindow(m_hWnd);

	for (UINT i = 0; i < CVideoDevice::E_BACKBUFFER_COUNT; i++)
	{
		m_swapChainRenderTargets[i] = new CRenderTarget(m_errorLog, m_device);

		m_swapChainRenderTargets[i]->SwapChainTarget(m_swapChain, i);

		m_swapChainRenderTargets[i]->WrapResource(dpi, m_d3d11On12->m_d3d11On12Device, m_d3d11On12->m_d2dDeviceContext);
	}
}

/*
*/
void CVideoDevice::CreateSwapChain()
{
	m_swapChainDesc.BufferCount = CVideoDevice::E_BACKBUFFER_COUNT;

	m_swapChainDesc.BufferDesc.Height = m_height;
	m_swapChainDesc.BufferDesc.Width = m_width;

	m_swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;

	m_swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	m_swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;

	m_swapChainDesc.OutputWindow = m_hWnd;

	m_swapChainDesc.Windowed = m_windowed;

	if (m_vsyncEnabled)
	{
		m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 59;// numerator;
		m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;// denominator;
	}
	else
	{
		m_swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		m_swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	m_swapChainDesc.SampleDesc.Count = 1;
	m_swapChainDesc.SampleDesc.Quality = 0;

	m_swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	m_swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;

	m_swapChainDesc.Flags = 0;


	HRESULT hr = m_factory->CreateSwapChain(m_commandQueue.Get(), &m_swapChainDesc, (IDXGISwapChain**)m_swapChain.GetAddressOf());

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CVideoDevice::CreateSwapChain::CreateSwapChain:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());
	}


	if (m_tearingSupport)
	{
		m_factory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);
	}

	m_backbufferIndex = m_swapChain->GetCurrentBackBufferIndex();
}

/*
*/
void CVideoDevice::DXGIFactory()
{
	HRESULT hr = CreateDXGIFactory2(m_factoryFlags, IID_IDXGIFactory4, (void**)&m_factory);

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CVideoDevice::DXGIFactory::CreateDXGIFactory2:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());
	}
}

/*
*/
void CVideoDevice::DeviceInformation()
{
	HRESULT hr = m_adapter->GetDesc(&m_adapterDesc);

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CVideoDevice::DeviceInformation::GetDesc:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());
	}

	m_videoCardMemory = (int)(m_adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	CString* description = new CString(m_adapterDesc.Description);

	m_errorLog->WriteError("CVideoDevice::DeviceInformation::Name:%s\n", description->GetText());
	m_errorLog->WriteError("CVideoDevice::DeviceInformation::Memory:%dM\n", m_videoCardMemory);

	delete description;


	ComPtr<IDXGIOutput> adapterOutput;

	hr = m_adapter->EnumOutputs(0, &adapterOutput);

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CVideoDevice::DeviceInformation::EnumOutputs:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());
	}

	// get the number of modes
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &m_numModes, nullptr);

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CVideoDevice::DeviceInformation::GetDisplayModeList for numModes:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());
	}

	// load the list with the number of modes
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &m_numModes, m_displayModeList);

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CVideoDevice::DeviceInformation::GetDisplayModeList:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());
	}

	m_errorLog->WriteError("CVideoDevice::DeviceInformation::DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM\n");

	UINT prevWidth = 0;
	UINT prevHeight = 0;

	for (UINT i = 0; i < m_numModes; i++)
	{
		if ((m_displayModeList[i].Width != prevWidth) && (m_displayModeList[i].Height != prevHeight))
		{
			m_errorLog->WriteError("CVideoDevice::DeviceInformation::w:%i h:%i h:%i\n",
				m_displayModeList[i].Width, m_displayModeList[i].Height,
				m_displayModeList[i].RefreshRate.Numerator / m_displayModeList[i].RefreshRate.Denominator);

			prevWidth = m_displayModeList[i].Width;
			prevHeight = m_displayModeList[i].Height;
		}
	}

	adapterOutput.Reset();



	// The display supports tearing, a requirement of VARIABLE refresh rate displays
	hr = m_factory->CheckFeatureSupport(DXGI_FEATURE::DXGI_FEATURE_PRESENT_ALLOW_TEARING, &m_tearingSupport, (UINT)sizeof(UINT));

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CVideoDevice::DXGIFactory::CheckFeatureSupport:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());
	}

	if (m_tearingSupport)
	{
		m_errorLog->WriteError("CVideoDevice::DXGIFactory::DXGI_FEATURE::DXGI_FEATURE_PRESENT_ALLOW_TEARING\n");
	}
	else
	{
		m_errorLog->WriteError("CVideoDevice::DXGIFactory::DXGI_FEATURE::DXGI_FEATURE_PRESENT_ALLOW_TEARING False\n");
	}
}

/*
*/
void CVideoDevice::End2DDraw()
{
	m_d3d11On12->m_d2dDeviceContext->EndDraw();

	m_d3d11On12->m_d3d11On12Device->ReleaseWrappedResources(m_swapChainRenderTargets[m_backbufferIndex]->m_wrappedBackBuffer.GetAddressOf(), 1);

	m_d3d11On12->m_d3d11DeviceContext->Flush();
}

/*
*/
void CVideoDevice::GetHardwareAdapter()
{
	for (UINT ai = 0; ; ai++)
	{
		// Enumerating all attached devices
		if (DXGI_ERROR_NOT_FOUND == m_factory->EnumAdapters1(ai, &m_adapter))
		{
			break;
		}

		HRESULT hr = D3D12CreateDevice(m_adapter.Get(), D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1, _uuidof(ID3D12Device), nullptr);

		// S_FALSE results when the ID3D12Device pointer is passed as a nullptr.
		if (SUCCEEDED(hr))
		{
			m_errorLog->WriteError("CVideoDevice::GetHardwareAdapter:D3D12CreateDevice:D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1 supported\n");

			CVideoDevice::DeviceInformation();

			return;
		}
	}

	if (m_adapter == 0)
	{
		m_errorLog->WriteError("CVideoDevice::GetHardwareAdapter::D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_12_1 not supported\n");
	}
}

/*
*/
void CVideoDevice::Init2DDraw()
{
	m_d3d11On12->m_d3d11On12Device->AcquireWrappedResources(m_swapChainRenderTargets[m_backbufferIndex]->m_wrappedBackBuffer.GetAddressOf(), 1);

	m_d3d11On12->m_d2dDeviceContext->SetTarget(m_swapChainRenderTargets[m_backbufferIndex]->m_d2dRenderTarget.Get());

	m_d3d11On12->m_d2dDeviceContext->BeginDraw();

	m_d3d11On12->m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
}

/*
*/
void CVideoDevice::MoveToNextFrame()
{
	// Present the current back buffer
	if (m_vsyncEnabled)
	{
		m_swapChain->Present(1, 0);
	}
	else
	{
		m_swapChain->Present(0, 0);
	}

	// Fence the current back buffer
	m_currentFenceValue = m_fenceValues[m_backbufferIndex];

	HRESULT hr = m_commandQueue->Signal(m_fence.Get(), m_currentFenceValue);

	if (hr != S_OK)
	{
		m_errorLog->WriteError("MoveToNextFrame::commandQueue->Signal:%s %s\n",
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());
	}

	// Get the next back buffer
	m_backbufferIndex = m_swapChain->GetCurrentBackBufferIndex();

	// Wait for the current GPU fence(s) to finish
	if (m_fence->GetCompletedValue() < m_fenceValues[m_backbufferIndex])
	{
		hr = m_fence->SetEventOnCompletion(m_fenceValues[m_backbufferIndex], m_fenceEvent);

		if (hr != S_OK)
		{
			m_errorLog->WriteError("MoveToNextFrame::fence->SetEventOnCompletion:%s %s\n",
				m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
				m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());
		}

		WaitForSingleObject(m_fenceEvent, 1000);
	}

	// Increment the fence value for the next fence
	m_fenceValues[m_backbufferIndex] = m_currentFenceValue + 1;
}

/*
*/
void CVideoDevice::ResetAndClearTargets()
{
	m_commandAllocators[m_backbufferIndex]->Reset();

	m_commandList->Reset(m_commandAllocators[m_backbufferIndex].Get(), nullptr);
	
	m_swapChainRenderTargets[m_backbufferIndex]->SetPreDrawBarrier();
	m_swapChainDepthBuffers[m_backbufferIndex]->SetPreDrawBarrier();
	
	m_commandList->ResourceBarrier(1, &m_swapChainRenderTargets[m_backbufferIndex]->m_barrier);

	m_commandList->ClearRenderTargetView(m_swapChainRenderTargets[m_backbufferIndex]->m_handle, m_color->CornflowerBlue->GetFloatArray(), 0, nullptr);
	m_commandList->ClearDepthStencilView(m_swapChainDepthBuffers[m_backbufferIndex]->m_handle, D3D12_CLEAR_FLAGS::D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

/*
*/
void CVideoDevice::ToggleFullScreenWindow()
{
	if (m_windowed)
	{
		GetWindowRect(m_hWnd, &m_windowRect);

		SetWindowLong(m_hWnd, GWL_STYLE, WS_POPUP);

		if (m_swapChain)
		{
			IDXGIOutput* pOutput;

			m_swapChain->GetContainingOutput(&pOutput);

			DXGI_OUTPUT_DESC Desc;

			pOutput->GetDesc(&Desc);

			m_fullScreenWindowRect = Desc.DesktopCoordinates;

			pOutput->Release();
			pOutput = 0;
		}

		SetWindowPos(m_hWnd, HWND_TOPMOST,
			m_fullScreenWindowRect.left, m_fullScreenWindowRect.top, m_fullScreenWindowRect.right, m_fullScreenWindowRect.bottom,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);

		ShowWindow(m_hWnd, SW_MAXIMIZE);
	}
	else
	{
		SetWindowLong(m_hWnd, GWL_STYLE, WS_POPUP);

		SetWindowPos(m_hWnd, HWND_NOTOPMOST,
			m_windowRect.left, m_windowRect.top, m_windowRect.right - m_windowRect.left, m_windowRect.bottom - m_windowRect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);

		ShowWindow(m_hWnd, SW_NORMAL);
		SetFocus(m_hWnd);
	}

	GetWindowRect(GetDesktopWindow(), &m_desktopRect);

	m_windowed = !m_windowed;
}

/*
*/
void CVideoDevice::WaitForGPU()
{
	if (!m_fence)
	{
		return;
	}

	// Signal and wait for the current GPU fence to finish
	m_commandQueue->Signal(m_fence.Get(), m_fenceValues[m_backbufferIndex]);

	m_fence->SetEventOnCompletion(m_fenceValues[m_backbufferIndex], m_fenceEvent);

	WaitForSingleObject(m_fenceEvent, INFINITE);

	m_fenceValues[m_backbufferIndex]++;
}