#pragma once

#include "framework.h"

#include "CFloat4x4Buffer.h"
#include "CFloatBuffer.h"
#include "CIntBuffer.h"
#include "CLinkList.h"
#include "CShaderResourceView.h"
#include "CTexture.h"

class CShaderHeap
{
public:

	CLinkList<CShaderResourceView>* m_resources;

	ComPtr<ID3D12DescriptorHeap> m_heap;

	CShaderHeap();
	CShaderHeap(CVideoDevice* videoDevice, CErrorLog* errorLog, ComPtr<ID3D12GraphicsCommandList> commandList, UINT descriptorCount);
	~CShaderHeap();

	void SlotResource(UINT slot, CFloat4x4Buffer* float4x4Buffer);
	void SlotResource(UINT slot, CFloatBuffer* floatBuffer);
	void SlotResource(UINT slot, CIntBuffer* intBuffer);
	void SlotResource(UINT slot, CTexture* texture);

private:

	CShaderResourceView* m_float4x4View;
	CShaderResourceView* m_floatView;
	CShaderResourceView* m_intView;
	CShaderResourceView* m_textureView;
	
	D3D12_DESCRIPTOR_HEAP_DESC m_desc;
};