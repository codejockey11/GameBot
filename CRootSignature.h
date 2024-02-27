#pragma once

#include "framework.h"

#include "CErrorLog.h"
#include "CRange.h"
#include "CVideoDevice.h"

class CRootSignature
{
public:

	enum
	{
		STATIC_SAMPLER_COUNT = 2
	};

	ComPtr<ID3D12RootSignature> m_signature;

	CRootSignature();
	CRootSignature(CVideoDevice* videoDevice, CErrorLog* errorLog, UINT n, CRange* range);
	~CRootSignature();

private:

	ComPtr<ID3DBlob> m_errors;
	ComPtr<ID3DBlob> m_tempSignature;
	
	D3D12_FEATURE_DATA_ROOT_SIGNATURE m_featureData;
	D3D12_ROOT_PARAMETER1 m_rootParameter;
	D3D12_STATIC_SAMPLER_DESC m_sampler[CRootSignature::STATIC_SAMPLER_COUNT];
	D3D12_VERSIONED_ROOT_SIGNATURE_DESC	m_desc;
};