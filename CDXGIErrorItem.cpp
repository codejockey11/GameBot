#include "CDXGIErrorItem.h"

/*
*/
CDXGIErrorItem::CDXGIErrorItem()
{
	memset(this, 0x00, sizeof(CDXGIErrorItem));
}

/*
*/
CDXGIErrorItem::CDXGIErrorItem(UINT nbr, const char* name, const char* message)
{
	memset(this, 0x00, sizeof(CDXGIErrorItem));

	m_nbr = nbr;

	m_name = new CString(name);
	m_msg = new CString(message);
}

/*
*/
CDXGIErrorItem::~CDXGIErrorItem()
{
	delete m_name;
	delete m_msg;
}