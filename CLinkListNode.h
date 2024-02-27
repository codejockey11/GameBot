#pragma once

#include "framework.h"

#include "CString.h"

template<class T>

class CLinkListNode
{
public:

	CLinkListNode* m_next;
	CLinkListNode* m_previous;
	
	CString* m_key;
	
	T* m_object;
	
	bool m_deleteMe;
	
	int m_ikey;


	CLinkListNode()
	{
		memset(this, 0x00, sizeof(CLinkListNode));
	}

	~CLinkListNode()
	{
		delete m_object;
	}
};