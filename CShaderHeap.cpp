#include "CShaderHeap.h"

/*
*/
CShaderHeap::CShaderHeap()
{
	memset(this, 0x00, sizeof(CShaderHeap));
}

/*
*/
CShaderHeap::CShaderHeap(CVideoDevice* videoDevice, CErrorLog* errorLog, ComPtr<ID3D12GraphicsCommandList> commandList, UINT descriptorCount)
{
	memset(this, 0x00, sizeof(CShaderHeap));

	// The number of items that are going to be slotted for the shader
	m_desc.NumDescriptors = descriptorCount;

	// If you are providing a sampler other than the static definition in the root signature
	// the type here would need to be D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER to accomodate them
	m_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	m_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	HRESULT hr = videoDevice->m_device->CreateDescriptorHeap(&m_desc, IID_PPV_ARGS(&m_heap));

	if (hr != S_OK)
	{
		errorLog->WriteError("CShaderHeap::CShaderHeap::CreateDescriptorHeap:%s %s\n",
			errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return;
	}

	m_heap->SetName(L"CShaderHeap::ShaderHeap");

	m_float4x4View = new CShaderResourceView(videoDevice);

	m_floatView = new CShaderResourceView(videoDevice);

	m_intView = new CShaderResourceView(videoDevice);

	m_textureView = new CShaderResourceView(videoDevice);
}

/*
*/
CShaderHeap::~CShaderHeap()
{
	delete m_textureView;
	delete m_intView;
	delete m_floatView;
	delete m_float4x4View;

	m_heap.Reset();
}

/*
*/
void CShaderHeap::SlotResource(UINT slot, CFloat4x4Buffer* float4x4Buffer)
{
	m_float4x4View->SlotBuffer(m_heap->GetCPUDescriptorHandleForHeapStart(), slot, float4x4Buffer);
}

/*
*/
void CShaderHeap::SlotResource(UINT slot, CFloatBuffer* floatBuffer)
{
	m_floatView->SlotBuffer(m_heap->GetCPUDescriptorHandleForHeapStart(), slot, floatBuffer);
}

/*
*/
void CShaderHeap::SlotResource(UINT slot, CIntBuffer* intBuffer)
{
	m_intView->SlotBuffer(m_heap->GetCPUDescriptorHandleForHeapStart(), slot, intBuffer);
}

/*
*/
void CShaderHeap::SlotResource(UINT slot, CTexture* texture)
{
	m_textureView->SlotBuffer(m_heap->GetCPUDescriptorHandleForHeapStart(), slot, texture);
}