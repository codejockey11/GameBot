#pragma once

#include "framework.h"

class CFrametime
{
public:

	UINT64 m_counter;
	
	ULONGLONG m_currTime;
	ULONGLONG m_totalTime;
	
	float m_frametime;
	float m_rframetime;
	
	int m_rframecount;

	CFrametime();
	~CFrametime();

	void Frame();

private:

	ULONGLONG m_countTime;
	ULONGLONG m_startTime;

	int m_framecount;
};