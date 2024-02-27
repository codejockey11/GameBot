#include "CTimer.h"

/*
*/
CTimer::CTimer()
{
	memset(this, 0x00, sizeof(CTimer));
}

/*
*/
CTimer::CTimer(ULONGLONG duration)
{
	memset(this, 0x00, sizeof(CTimer));

	m_duration = duration;
}

/*
*/
CTimer::~CTimer()
{
}

/*
*/
void CTimer::Frame()
{
	m_currentTime = GetTickCount64();

	if ((m_currentTime - m_previousTime) > m_duration)
	{
		m_previousTime = m_currentTime;

		m_isReloading = false;
	}
}

/*
*/
void CTimer::Start()
{
	if (m_isReloading == true)
	{
		return;
	}

	m_isReloading = true;

	m_currentTime = GetTickCount64();

	m_previousTime = m_currentTime;
}