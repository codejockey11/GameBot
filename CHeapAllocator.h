#pragma once

#include "framework.h"

class CHeapAllocator
{
public:

	int m_count;
	int m_entrySize;

	CHeapAllocator();
	~CHeapAllocator();
};