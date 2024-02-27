#include "CHeapArray.h"

/*
*/
CHeapArray::CHeapArray()
{
	memset(this, 0x00, sizeof(CHeapArray));
}

/*
*/
CHeapArray::CHeapArray(UINT size, bool allocateHeap, bool limitCheck, UINT count, ...)
{
	memset(this, 0x00, sizeof(CHeapArray));

	m_limitCheck = limitCheck;

	m_allocator = new CHeapAllocator[count]();

	m_size = size;

	va_list argptr;

	va_start(argptr, count);

	int c = va_arg(argptr, int);

	m_allocator[0].m_count = c;
	m_allocator[0].m_entrySize = size * c;

	for (UINT i = 1; i < count; i++)
	{
		c = va_arg(argptr, int);

		m_allocator[i].m_count = c;
		m_allocator[i].m_entrySize = m_allocator[i - 1].m_entrySize * c;
	}

	va_end(argptr);

	UINT mem = m_allocator[count - 1].m_entrySize;

	if (allocateHeap)
	{
		m_heap = (BYTE*)calloc(mem, sizeof(BYTE));
	}
}

/*
*/
CHeapArray::~CHeapArray()
{
	delete[] m_allocator;

	if (m_heap)
	{
		free(m_heap);
	}
}

/*
*/
BYTE* CHeapArray::GetElement(UINT count, ...)
{
	BYTE* p = m_heap;

	va_list argptr;

	va_start(argptr, count);

	int c = va_arg(argptr, int);

	if (m_limitCheck)
	{
		if ((c < 0) || (c > m_allocator[0].m_count))
		{
			return nullptr;
		}
	}

	int e = m_size * c;

	for (UINT i = 1; i < count; i++)
	{
		c = va_arg(argptr, int);

		if (m_limitCheck)
		{
			if ((c < 0) || (c > m_allocator[i].m_count))
			{
				return nullptr;
			}
		}

		e += m_allocator[i - 1].m_entrySize * c;
	}

	va_end(argptr);

	return (p + e);
}

/*
*/
UINT  CHeapArray::GetOffset(UINT count, ...)
{
	BYTE* p = m_heap;

	va_list argptr;

	va_start(argptr, count);

	int c = va_arg(argptr, int);

	UINT e = m_size * c;

	for (UINT i = 1; i < count; i++)
	{
		c = va_arg(argptr, int);

		if (m_limitCheck)
		{
			if ((c < 0) || (c > m_allocator[i].m_count))
			{
				return 0;
			}
		}

		e += m_allocator[i - 1].m_entrySize * c;
	}

	va_end(argptr);

	return e;
}