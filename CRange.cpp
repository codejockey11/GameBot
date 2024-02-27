#include "CRange.h"

/*
*/
CRange::CRange()
{
	memset(this, 0x00, sizeof(CRange));
}

/*
*/
CRange::~CRange()
{
}

/*
*/
void CRange::SetRange(UINT index, D3D12_DESCRIPTOR_RANGE_TYPE rangeType, UINT numberOfDescriptors)
{
	if (index >= CRange::MAX_RANGE)
	{
		return;
	}

	m_index = index;

	m_range[m_index].RangeType = rangeType;
	m_range[m_index].NumDescriptors = numberOfDescriptors;
	m_range[m_index].BaseShaderRegister = 0;
	m_range[m_index].RegisterSpace = 0;
	m_range[m_index].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	m_range[m_index].Flags = D3D12_DESCRIPTOR_RANGE_FLAGS::D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
}