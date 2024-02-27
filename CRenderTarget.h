#pragma once

#include "framework.h"

#include "CErrorLog.h"

class CRenderTarget
{
public:

	ComPtr<ID2D1Bitmap1> m_d2dRenderTarget;
	ComPtr<ID3D11Resource> m_wrappedBackBuffer;
	ComPtr<ID3D12DescriptorHeap> m_heap;
	ComPtr<ID3D12Resource> m_resource;
	
	D2D1_SIZE_F m_rtSize;
	
	D3D12_CLEAR_VALUE m_clearValue;
	D3D12_CPU_DESCRIPTOR_HANDLE	m_handle;
	D3D12_DESCRIPTOR_HEAP_DESC m_heapDesc;
	D3D12_RESOURCE_BARRIER m_barrier;

	CRenderTarget();
	CRenderTarget(CErrorLog* errorLog, ComPtr<ID3D12Device> device);
	~CRenderTarget();

	void AdditionalTarget(ComPtr<ID3D12Resource> resource, UINT64 width, UINT height);
	void SetPreDrawBarrier();
	void SwapChainTarget(ComPtr<IDXGISwapChain3> swapChain, UINT bufferNbr);
	void WrapResource(float dpi, ComPtr<ID3D11On12Device> d3d11On12Device, ComPtr<ID2D1DeviceContext1> d2dDeviceContext);

private:

	CErrorLog* m_errorLog;
	
	ComPtr<ID2D1DeviceContext1> m_d2dDeviceContext;
	ComPtr<ID3D11On12Device> m_d3d11On12Device;
	ComPtr<ID3D12Device> m_device;
	ComPtr<IDXGISurface> m_surface;
	
	D2D1_BITMAP_PROPERTIES1 m_bitmapProperties;
};