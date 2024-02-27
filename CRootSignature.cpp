#include "CRootSignature.h"

/*
*/
CRootSignature::CRootSignature()
{
	memset(this, 0x00, sizeof(CRootSignature));
}

/*
*/
CRootSignature::CRootSignature(CVideoDevice* videoDevice, CErrorLog* errorLog, UINT numDescriptorRanges, CRange* range)
{
	memset(this, 0x00, sizeof(CRootSignature));

	m_featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1_1;

	HRESULT hr = videoDevice->m_device->CheckFeatureSupport(
		D3D12_FEATURE::D3D12_FEATURE_ROOT_SIGNATURE,
		&m_featureData,
		sizeof(D3D12_FEATURE_DATA_ROOT_SIGNATURE));

	if (hr != S_OK)
	{
		m_featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION::D3D_ROOT_SIGNATURE_VERSION_1_1;
	}

	m_rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE::D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	m_rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;

	if (numDescriptorRanges > 0)
	{
		m_rootParameter.DescriptorTable.NumDescriptorRanges = numDescriptorRanges;
		m_rootParameter.DescriptorTable.pDescriptorRanges = range->m_range;
	}

	// Here are the static sampler definitions
	m_sampler[0].Filter = D3D12_FILTER::D3D12_FILTER_ANISOTROPIC;
	m_sampler[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	m_sampler[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	m_sampler[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	m_sampler[0].MipLODBias = 0;
	m_sampler[0].MaxAnisotropy = 4;
	m_sampler[0].ComparisonFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_ALWAYS;
	m_sampler[0].BorderColor = D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	m_sampler[0].MinLOD = 0.0f;
	m_sampler[0].MaxLOD = D3D12_FLOAT32_MAX;
	m_sampler[0].ShaderRegister = 0;
	m_sampler[0].RegisterSpace = 0;
	m_sampler[0].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;

	m_sampler[1].Filter = D3D12_FILTER::D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	m_sampler[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	m_sampler[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	m_sampler[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE::D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	m_sampler[1].MipLODBias = 0;
	m_sampler[1].MaxAnisotropy = 4;
	m_sampler[1].ComparisonFunc = D3D12_COMPARISON_FUNC::D3D12_COMPARISON_FUNC_ALWAYS;
	m_sampler[1].BorderColor = D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	m_sampler[1].MinLOD = 0.0f;
	m_sampler[1].MaxLOD = D3D12_FLOAT32_MAX;
	m_sampler[1].ShaderRegister = 1;
	m_sampler[1].RegisterSpace = 0;
	m_sampler[1].ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;

	m_desc.Version = m_featureData.HighestVersion;

	m_desc.Desc_1_1.NumParameters = 1;
	m_desc.Desc_1_1.pParameters = &m_rootParameter;
	m_desc.Desc_1_1.NumStaticSamplers = CRootSignature::STATIC_SAMPLER_COUNT;
	m_desc.Desc_1_1.pStaticSamplers = m_sampler;
	m_desc.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	hr = D3D12SerializeVersionedRootSignature(&m_desc, &m_tempSignature, &m_errors);

	if (hr != S_OK)
	{
		errorLog->WriteError("CRootSignature::CRootSignature::D3D12SerializeVersionedRootSignature:%s %s\n",
			errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		if (m_errors)
		{
			errorLog->WriteError("%s\n", m_errors->GetBufferPointer());
		}

		return;
	}

	hr = videoDevice->m_device->CreateRootSignature(0,
		m_tempSignature->GetBufferPointer(),
		m_tempSignature->GetBufferSize(),
		IID_PPV_ARGS(&m_signature));

	if (hr != S_OK)
	{
		errorLog->WriteError("CRootSignature::CRootSignature::CreateRootSignature:%s %s\n",
			errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return;
	}

	m_signature->SetName(L"CRootSignature::RootSignature");

	m_tempSignature.Reset();

	m_errors.Reset();
}

/*
*/
CRootSignature::~CRootSignature()
{
	m_signature.Reset();
}