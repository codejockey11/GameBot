#pragma once

#include "framework.h"

#include "CBmpImage.h"
#include "CErrorLog.h"
#include "CString.h"
#include "CTgaImage.h"
#include "CVideoDevice.h"

class CTexture
{
public:

	CBmpImage* m_bimage;
	CString* m_name;
	CTgaImage* m_image;
	
	ComPtr<ID3D12CommandAllocator> m_commandAllocators[CVideoDevice::E_BACKBUFFER_COUNT];
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	ComPtr<ID3D12Resource> m_texture;
	
	D3D12_RESOURCE_BARRIER m_barrier;
	
	UINT m_height;
	
	UINT64 m_width;
	
	bool m_isInitialized;

	CTexture();
	CTexture(CErrorLog* errorLog, CVideoDevice* videoDevice, const char* filename);
	~CTexture();

	void CreateResource(BYTE* pixels);

private:

	CErrorLog* m_errorLog;
	CVideoDevice* m_videoDevice;
	
	ComPtr<ID3D12Resource> m_textureUploadHeap;
	
	D3D12_HEAP_PROPERTIES m_heapProperties;
	D3D12_HEAP_PROPERTIES m_textureHeapProperties;
	D3D12_RESOURCE_ALLOCATION_INFO m_allocInfo;
	D3D12_RESOURCE_DESC m_heapDesc;
	D3D12_RESOURCE_DESC m_textureDesc;
	D3D12_SUBRESOURCE_DATA m_uploadData;
	
	UINT64 m_uploadBufferSize;
};

class CDynamicTexture : public CTexture
{
public:

	CDynamicTexture(CErrorLog* errorLog, CVideoDevice* videoDevice, ComPtr<ID3D12GraphicsCommandList> commandList, UINT width, UINT height, const char* name);
	~CDynamicTexture();

	void CreateResource();
	void UpdateTextureResource(void* bytes);

private:

	CErrorLog* m_errorLog;
	CVideoDevice* m_videoDevice;
	
	ComPtr<ID3D12Resource> m_textureUploadHeap;
	
	D3D12_HEAP_PROPERTIES m_heapProperties;
	D3D12_HEAP_PROPERTIES m_textureHeapProperties;
	D3D12_RESOURCE_ALLOCATION_INFO m_allocInfo;
	D3D12_RESOURCE_DESC m_heapDesc;
	D3D12_RESOURCE_DESC m_textureDesc;
	D3D12_SUBRESOURCE_DATA m_uploadData;
	
	UINT64 m_uploadBufferSize;
};