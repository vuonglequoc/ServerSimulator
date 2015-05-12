#include "TTimer.h"

//-------------------------------------------------------------------------------------------------------------------------------------------
//# Made by [RCZ]ShadowKing

//-------------------------------------------------------------------------------------------------------------------------------------------

CTimer::CTimer(void)
{
	// ----
}
//-------------------------------------------------------------------------------------------------------------------------------------------

CTimer::~CTimer(void)
{
	// ----
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CTimer::Init()
{
	m_Tick			= GetTickCount();
	// ----
	m_bWorker			= true;
	// ----
	m_nMilliseconds	= 0;
	m_nSeconds		= 0;
	m_nMinutes		= 0;
	m_nHours		= 0;
	m_nDays			= 0;
	// ----
	m_hThread = CreateThread(NULL, 0, (LPTHREAD)TimerWorker, this, 0, NULL);
	// ----
	if(m_hThread == NULL)
	{
		MessageBox(0, "Cannot create the TimerWorker,\nplease restart the application!", "[CTimer][Init] :: CreateThread", MB_OK);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CTimer::TimerWorker(LPVOID lpParam)
{
	CTimer  * lpThis = (CTimer *)lpParam;
	// ----
	while(lpThis->m_bWorker)
	{
		if(lpThis->m_nMilliseconds == 1000)
		{
			lpThis->m_nMilliseconds = 0;
			lpThis->m_nSeconds++;
			// ----
			lpThis->OnSecond();
		}
		// ----
		if(lpThis->m_nSeconds == 60)
		{
			lpThis->m_nSeconds = 0;
			lpThis->m_nMinutes++;
			// ----
			lpThis->OnMinute();
		}
		// ----
		if(lpThis->m_nMinutes == 60)
		{
			lpThis->m_nMinutes = 0;
			lpThis->m_nHours++;
			// ----
			lpThis->OnHour();
		}
		// ----
		if(lpThis->m_nHours == 24)
		{
			// ----
			lpThis->m_nHours = 0;
			lpThis->m_nDays++;
			// ----
			
		}
		// ----
		lpThis->m_nMilliseconds += 100;
		// ----
		Sleep(100);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------