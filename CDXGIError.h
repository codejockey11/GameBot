#pragma once

#include "framework.h"

#include "CDXGIErrorItem.h"
#include "CLinkList.h"

class CDXGIError
{
public:

	CLinkList<CDXGIErrorItem>* m_errors;

	CDXGIErrorItem* m_defaultError;

	CDXGIError();
	~CDXGIError();

	void AddItem(CDXGIErrorItem* item);
	CDXGIErrorItem* GetError(int error);
};