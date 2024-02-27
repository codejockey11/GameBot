#include "CD3D11On12.h"

/*
*/
CD3D11On12::CD3D11On12()
{
	memset(this, 0x00, sizeof(CD3D11On12));
}

/*
*/
CD3D11On12::CD3D11On12(CErrorLog* errorLog, ComPtr<ID3D12Device> device, ComPtr<ID3D12CommandQueue> commandQueue)
{
	memset(this, 0x00, sizeof(CD3D11On12));

	m_errorLog = errorLog;

	m_videoDevice = device;

	m_commandQueue = commandQueue;

	UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	#ifdef _DEBUG
	d3d11DeviceFlags |= D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
	#endif

	D3D11On12CreateDevice(m_videoDevice.Get(), d3d11DeviceFlags,
		nullptr,
		0,
		(IUnknown**)m_commandQueue.GetAddressOf(),
		1,
		0,
		m_d3d11Device.GetAddressOf(),
		m_d3d11DeviceContext.GetAddressOf(),
		nullptr
	);

	m_d3d11Device->QueryInterface(IID_PPV_ARGS(&m_d3d11On12Device));

	D2D1_DEVICE_CONTEXT_OPTIONS deviceOptions = D2D1_DEVICE_CONTEXT_OPTIONS::D2D1_DEVICE_CONTEXT_OPTIONS_NONE;

	D2D1_FACTORY_OPTIONS d2dFactoryOptions = {};

	#ifdef _DEBUG
	d2dFactoryOptions.debugLevel = D2D1_DEBUG_LEVEL::D2D1_DEBUG_LEVEL_INFORMATION;
	#endif

	D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED,
		__uuidof(ID2D1Factory3), &d2dFactoryOptions, (void**)&m_d2dFactory);

	m_d3d11On12Device->QueryInterface(IID_PPV_ARGS(&m_dxgiDevice));

	if (m_dxgiDevice)
	{
		m_d2dFactory->CreateDevice(m_dxgiDevice.Get(), m_d2dDevice.GetAddressOf());

		m_d2dDevice->CreateDeviceContext(deviceOptions, m_d2dDeviceContext.GetAddressOf());

		DWriteCreateFactory(DWRITE_FACTORY_TYPE::DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory), (IUnknown**)m_dWriteFactory.GetAddressOf());
	}
}

/*
*/
CD3D11On12::~CD3D11On12()
{
	m_dWriteFactory.Reset();

	m_d2dDeviceContext.Reset();

	m_d2dDevice.Reset();

	m_dxgiDevice.Reset();

	m_d2dFactory.Reset();

	m_d3d11On12Device.Reset();

	m_d3d11DeviceContext.Reset();

	// This debug will report 3 live objects when compiled with _DEBUG option as the m_d3d11Device is still live
	// D3D11 WARNING: Live ID3D11Device at 0x000001F23E453860, Refcount: 3 [ STATE_CREATION WARNING #441: LIVE_DEVICE]
	#ifdef _DEBUG
	ComPtr<ID3D11Debug> d3d11Debug;

	HRESULT hr = m_d3d11Device->QueryInterface(IID_PPV_ARGS(&d3d11Debug));

	d3d11Debug->ReportLiveDeviceObjects(D3D11_RLDO_FLAGS::D3D11_RLDO_IGNORE_INTERNAL);

	d3d11Debug.Reset();
	#endif

	m_d3d11Device.Reset();
}