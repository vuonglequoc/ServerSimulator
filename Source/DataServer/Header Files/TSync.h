/*#############################################################################
# Zodea MU Suite - AIO
#
# RealCoderZ Software Incorpration.	http://www.realcoderz.com
#
# Copyright © 2011 RealCoderZ Software Incorpration.
#
# Author: WolF & M.E.S
#
# [09.08.2011] This Script For Synchronization Mechanisms.
#
#############################################################################*/
#ifndef TSYNC_HEADER
#define TSYNC_HEADER

#include "StdAfx.H"


class TSyncCSection 
{
public:
	TSyncCSection( DWORD dwSpinCount = 0 )
	{
		if(0 != dwSpinCount)
			InitializeCriticalSectionAndSpinCount(&m_CritiSec, dwSpinCount);
		else
			InitializeCriticalSection(&m_CritiSec);
	}
	// -----
	virtual ~TSyncCSection()
	{
		DeleteCriticalSection(&m_CritiSec);
	}
	// -----
	bool CSInit(DWORD dwSpinCount)
	{
		if (InitializeCriticalSectionAndSpinCount(&m_CritiSec, dwSpinCount)) return true; else return false;
	}
	// -----
	void CSLock()
	{
		EnterCriticalSection(&m_CritiSec);
		// -----
		m_Locked = TRUE;
	}
	// -----
	void CSUnlock(void)
	{
		if ( m_Locked )
		{
			LeaveCriticalSection(&m_CritiSec);
			// -----
			m_Locked = FALSE;
		}
	}
	// -----
private:
	CRITICAL_SECTION	m_CritiSec;
	BOOL				m_Locked;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* TSYNC_HEADER */
// -------------------------------------------------------------------------------------------------------------------------------------------------------