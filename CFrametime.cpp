#include "CFrametime.h"

/*
*/
CFrametime::CFrametime()
{
	memset(this, 0x00, sizeof(CFrametime));

	m_startTime = GetTickCount64();

	m_currTime = GetTickCount64();
}

/*
*/
CFrametime::~CFrametime()
{
}

/*
*/
void CFrametime::Frame()
{
	m_currTime = GetTickCount64();

	if ((m_currTime - m_countTime) > 1000)
	{
		m_countTime = m_currTime;

		m_rframecount = m_framecount;

		m_rframetime = m_frametime;

		m_framecount = 0;
	}

	m_counter++;

	m_framecount++;

	m_frametime = (float)((m_currTime - m_startTime) / 1000.0f);

	m_totalTime += (ULONGLONG)m_frametime;

	m_startTime = m_currTime;
}