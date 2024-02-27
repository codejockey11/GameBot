#pragma once

#include "framework.h"

#include "CCamera.h"
#include "CColor.h"
#include "CDepthBuffer.h"
#include "CErrorLog.h"
#include "CFloatBuffer.h"
#include "CIndexBuffer.h"
#include "CPipelineState.h"
#include "CRange.h"
#include "CRenderTarget.h"
#include "CRootSignature.h"
#include "CShaderHeap.h"
#include "CShaderManager.h"
#include "CTexture.h"
#include "CVertex.h"
#include "CVertexBuffer.h"
#include "CViewport.h"
#include "CVideoDevice.h"

class COverhead
{
public:

	CCamera* m_camera;
	CDepthBuffer* m_depthBuffers[CVideoDevice::E_BACKBUFFER_COUNT];
	CDynamicTexture* m_texture;
	CFloat4x4Buffer* m_wvp;
	CFloatBuffer* m_floats;
	CIntBuffer* m_ints;
	CPipelineState* m_pipelineState;
	CRange* m_range;
	CRenderTarget* m_renderTargets[CVideoDevice::E_BACKBUFFER_COUNT];
	CRootSignature* m_rootSignature;
	CShader* m_pixelShader;
	CShader* m_vertexShader;
	CShaderHeap* m_shaderHeap;
	CVertex2 m_position;
	CVertex2 m_size;
	CViewport* m_viewport;
	
	ComPtr<ID3D12CommandAllocator> m_commandAllocators[CVideoDevice::E_BACKBUFFER_COUNT];
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	ComPtr<ID3D12Resource> m_resources[CVideoDevice::E_BACKBUFFER_COUNT];

	COverhead();
	COverhead(CVideoDevice* videoDevice, CErrorLog* errorLog, CColor* color, CShaderManager* shaderManager, CVertex2 size, CVertex2 position);
	~COverhead();

	void ClearTargets();
	void Record();
	void SetCameraPosition(XMFLOAT3 position);
	void UpdatePosition();

private:

	CColor* m_color;
	CErrorLog* m_errorLog;
	CIndexBuffer* m_indexBuffer;
	CVertexBuffer* m_vertexBuffer;
	CVertexT m_vertices[4];
	CVideoDevice* m_videoDevice;
};