#pragma once

#include "framework.h"

#include "CLinkListNode.h"
#include "CString.h"

template <class T>

class CLinkList
{
public:

	CLinkListNode<T>* m_list;

	int m_count;

	bool m_isThreading;
	bool m_isTraversing;

	/*
	*/
	CLinkList()
	{
		CLinkList::Constructor();
	}

	/*
	*/
	~CLinkList()
	{
		CLinkListNode<T>* lln = m_list;

		while (lln)
		{
			CLinkListNode<T>* n = lln->m_next;

			delete lln;

			m_count--;

			lln = n;
		}

		m_list = nullptr;
	}

	/*
	*/
	void Constructor()
	{
		memset(this, 0x00, sizeof(CLinkList<T>));

		m_list = new CLinkListNode<T>();
	}
	/*
	*/
	void Add(T* element, const char* key)
	{
		if (m_list->m_object == 0)
		{
			m_list->m_object = element;
			m_list->m_key = new CString(key);
			m_list->m_next = new CLinkListNode<T>();

			m_count++;

			return;
		}

		CLinkListNode<T>* previous = 0;
		CLinkListNode<T>* traverser = m_list;

		while (traverser->m_object)
		{
			previous = traverser;
			traverser = traverser->m_next;
		}

		traverser->m_previous = previous;
		traverser->m_object = element;
		traverser->m_key = new CString(key);
		traverser->m_next = new CLinkListNode<T>();

		m_count++;
	}

	/*
	*/
	void Add(T* element, int key)
	{
		if (m_list->m_object == 0)
		{
			m_list->m_object = element;
			m_list->m_ikey = key;
			m_list->m_next = new CLinkListNode<T>();

			m_count++;

			return;
		}

		CLinkListNode<T>* previous = 0;
		CLinkListNode<T>* traverser = m_list;

		while (traverser->m_object)
		{
			previous = traverser;
			traverser = traverser->m_next;
		}

		traverser->m_previous = previous;
		traverser->m_object = element;
		traverser->m_ikey = key;
		traverser->m_next = new CLinkListNode<T>();

		m_count++;
	}

	/*
	*/
	void Clear()
	{
		CLinkListNode<T>* p = m_list;

		while (p->m_object)
		{
			p = CLinkList::Delete(p);
		}
	}

	/*
	*/
	CLinkListNode<T>* Delete(CLinkListNode<T>* element)
	{
		if (m_list == element)
		{
			m_list->m_next->m_previous = 0;

			CLinkListNode<T>* next = m_list->m_next;

			delete m_list;

			m_list = next;

			m_count--;

			//if (m_count == 0)
			//{
				//m_list->m_next = new CLinkListNode<T>();
			//}

			return m_list;
		}

		CLinkListNode<T>* traverser = m_list;

		while (traverser->m_object)
		{
			if (traverser == element)
			{
				CLinkListNode<T>* next = traverser->m_next;

				traverser->m_previous->m_next = traverser->m_next;
				traverser->m_next->m_previous = traverser->m_previous;

				delete traverser;

				m_count--;

				return next;
			}

			traverser = traverser->m_next;
		}

		return nullptr;
	}

	/*
	*/
	CLinkListNode<T>* Search(const char* key)
	{
		CLinkListNode<T>* traverser = m_list;

		while (traverser->m_object)
		{
			if (strcmp(traverser->m_key->GetText(), key) == 0)
			{
				return traverser;
			}

			traverser = traverser->m_next;
		}

		return nullptr;
	}

	/*
	*/
	CLinkListNode<T>* Search(int key)
	{
		CLinkListNode<T>* traverser = m_list;

		while (traverser->m_object)
		{
			if (traverser->m_ikey == key)
			{
				return traverser;
			}

			traverser = traverser->m_next;
		}

		return nullptr;
	}
};