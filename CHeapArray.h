#pragma once

#include "framework.h"

#include "CHeapAllocator.h"

class CHeapArray
{
public:

	BYTE* m_heap;

	CHeapArray();
	CHeapArray(UINT size, bool allocateHeap, bool limitCheck, UINT count, ...);
	~CHeapArray();

	BYTE* GetElement(UINT count, ...);
	UINT GetOffset(UINT count, ...);

private:

	CHeapAllocator* m_allocator;

	UINT m_size;

	bool m_limitCheck;
};