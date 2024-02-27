#pragma once

#include "framework.h"

#include "CColor.h"
#include "CD3D11On12.h"
#include "CDepthBuffer.h"
#include "CErrorLog.h"
#include "CRenderTarget.h"
#include "CViewport.h"

class CVideoDevice
{
public:

	enum
	{
		E_BACKBUFFER_COUNT = 2,
		E_DISPLAY_MODE_COUNT = 256
	};

	bool m_windowed;

	CD3D11On12* m_d3d11On12;
	CDepthBuffer* m_swapChainDepthBuffers[CVideoDevice::E_BACKBUFFER_COUNT];
	CRenderTarget* m_swapChainRenderTargets[CVideoDevice::E_BACKBUFFER_COUNT];
	CViewport* m_swapChainViewport;
	
	ComPtr<ID3D12CommandAllocator> m_commandAllocators[CVideoDevice::E_BACKBUFFER_COUNT];
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12Device> m_device;
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	ComPtr<IDXGISwapChain3> m_swapChain;
	
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS  m_sampleQualityLevels;

	HWND m_hWnd;

	int m_height;
	int m_width;

	POINT m_middle;

	RECT m_desktop;
	
	UINT m_backbufferIndex;
	UINT m_posX;
	UINT m_posY;
	UINT m_tearingSupport;

	CVideoDevice();
	CVideoDevice(CErrorLog* errorLog, HWND hWnd, int width, int height, bool windowed, CColor* color);
	~CVideoDevice();

	void Create11on12Device();
	ComPtr<ID3D12CommandAllocator> CreateCommandAllocator();
	ComPtr<ID3D12GraphicsCommandList> CreateCommandList(ComPtr<ID3D12CommandAllocator> ca);
	void CreateCommandQueue();
	void CreateDepthBuffers();
	void CreateDevice();
	void CreateFence();
	void CreateRenderTargets();
	void CreateSwapChain();
	void DXGIFactory();
	void DeviceInformation();
	void End2DDraw();
	void GetHardwareAdapter();
	void Init2DDraw();
	void MoveToNextFrame();
	void ResetAndClearTargets();
	void ToggleFullScreenWindow();
	void WaitForGPU();

private:

	CColor* m_color;
	CErrorLog* m_errorLog;

#ifdef _DEBUG
	ComPtr<ID3D12Debug> m_debugController;
#endif
	ComPtr<ID3D12Fence> m_fence;
	ComPtr<ID3D12InfoQueue> m_infoQueue;
	ComPtr<IDXGIAdapter1> m_adapter;
	ComPtr<IDXGIFactory6> m_factory;

	D3D12_COMMAND_QUEUE_DESC m_commandQueueDesc;

	DXGI_ADAPTER_DESC m_adapterDesc;
	DXGI_MODE_DESC m_displayModeList[CVideoDevice::E_DISPLAY_MODE_COUNT];
	DXGI_SWAP_CHAIN_DESC m_swapChainDesc;

	HANDLE m_fenceEvent;

	RECT m_desktopRect;
	RECT m_fullScreenWindowRect;
	RECT m_windowRect;

	UINT m_factoryFlags;
	UINT m_numModes;

	UINT64 m_currentFenceValue;
	UINT64 m_fenceValues[CVideoDevice::E_BACKBUFFER_COUNT];

	bool m_vsyncEnabled;

	int m_videoCardMemory;
};