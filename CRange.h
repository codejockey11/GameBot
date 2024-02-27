#pragma once

#include "framework.h"

class CRange
{
public:

	enum
	{
		MAX_RANGE = 2
	};

	D3D12_DESCRIPTOR_RANGE1 m_range[CRange::MAX_RANGE];

	UINT m_index;

	CRange();
	~CRange();

	void SetRange(UINT index, D3D12_DESCRIPTOR_RANGE_TYPE rangeType, UINT count);
};