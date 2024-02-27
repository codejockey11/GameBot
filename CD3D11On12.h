#pragma once

#include "framework.h"

#include "CErrorLog.h"

class CD3D11On12
{
public:

	ComPtr<ID2D1DeviceContext1> m_d2dDeviceContext;
	ComPtr<ID3D11DeviceContext> m_d3d11DeviceContext;
	ComPtr<ID3D11On12Device> m_d3d11On12Device;
	ComPtr<IDWriteFactory> m_dWriteFactory;

	CD3D11On12();
	CD3D11On12(CErrorLog* errorLog, ComPtr<ID3D12Device> device, ComPtr<ID3D12CommandQueue> commandQueue);
	~CD3D11On12();

private:

	CErrorLog* m_errorLog;
	
	ComPtr<ID2D1Device2> m_d2dDevice;
	ComPtr<ID2D1Factory3> m_d2dFactory;
	ComPtr<ID3D11Device> m_d3d11Device;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12Device> m_videoDevice;
	ComPtr<IDXGIDevice> m_dxgiDevice;
};