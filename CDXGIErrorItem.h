#pragma once

#include "framework.h"

#include "CString.h"

class CDXGIErrorItem
{
public:

	CString* m_msg;
	CString* m_name;
	
	UINT m_nbr;

	CDXGIErrorItem();
	CDXGIErrorItem(UINT nbr, const char* name, const char* message);
	~CDXGIErrorItem();
};