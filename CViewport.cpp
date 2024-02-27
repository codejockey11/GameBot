#include "CViewport.h"

/*
*/
CViewport::CViewport()
{
	memset(this, 0x00, sizeof(CViewport));
}

/*
*/
CViewport::CViewport(float x, float y, float width, float height)
{
	memset(this, 0x00, sizeof(CViewport));

	m_viewport.TopLeftX = x;
	m_viewport.TopLeftY = y;
	m_viewport.Width = width;
	m_viewport.Height = height;
	m_viewport.MinDepth = D3D12_MIN_DEPTH;
	m_viewport.MaxDepth = D3D12_MAX_DEPTH;

	m_scissorRect.left = 0;
	m_scissorRect.top = 0;
	m_scissorRect.right = (long)width;
	m_scissorRect.bottom = (long)height;
}

/*
*/
CViewport::~CViewport()
{
}