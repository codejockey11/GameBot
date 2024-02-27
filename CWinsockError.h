#pragma once

#include "framework.h"

#include "CLinkList.h"
#include "CWinsockErrorItem.h"

class CWinsockError
{
public:

	CLinkList<CWinsockErrorItem>* m_errors;

	CWinsockError();
	~CWinsockError();

	void AddItem(CWinsockErrorItem* i);
	CWinsockErrorItem* GetError(int e);
};