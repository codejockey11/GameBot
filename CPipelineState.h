#pragma once

#include "framework.h"

#include "CErrorLog.h"
#include "CShader.h"
#include "CVertex.h"
#include "CVideoDevice.h"

class CPipelineState
{
public:

	CErrorLog* m_errorLog;
	CVideoDevice* m_videoDevice;

	ComPtr<ID3D12PipelineState> m_pipelineState;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_desc;

	CPipelineState();
	CPipelineState(CVideoDevice* videoDevice, CErrorLog* errorLog, BYTE type);
	~CPipelineState();

	void Create(bool alphaToCoverageEnable, bool blendEnable, bool depthEnable, D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveType,
		CShader* vertexShader, CShader* pixelShader, CShader* domainShader, CShader* hullShader, CShader* geometryShader,
		ComPtr<ID3D12RootSignature> rootSignature);

private:

	D3D12_INPUT_ELEMENT_DESC* m_vertexDesc;

	D3D12_RENDER_TARGET_BLEND_DESC m_blendDesc;

	UINT m_vertexDescCount;
};