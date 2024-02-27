#pragma once

#include "framework.h"

class CTimer
{
public:

	ULONGLONG m_currentTime;
	ULONGLONG m_duration;
	ULONGLONG m_previousTime;
	
	bool m_isReloading;
	
	CTimer();
	CTimer(ULONGLONG d);
	~CTimer();
	
	void Frame();
	void Start();
};