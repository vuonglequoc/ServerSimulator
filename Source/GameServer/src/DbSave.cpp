//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	DB Save Class
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------
#include "stdafx.h"
#include "DbSave.H"
#include "WZQueue.H"
#include "LogProc.H"
#include "GameMain.H"


DBSave gDbSave;
// -----------------------------------------------------------------------------------------------------------------------------------------------------
static LRESULT WINAPI cSaveThreadProc(DBSave * pThis);
// -----------------------------------------------------------------------------------------------------------------------------------------------------
DBSave::DBSave()
{
	m_lpWzQueue		= NULL;
	m_bIsRunning	= FALSE;
	Initialize();
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
DBSave::~DBSave()
{
	End();
	Feee();
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
bool DBSave::Initialize()
{
	m_lpWzQueue = new WZQueue(1280);
	// -----
	if(m_lpWzQueue == 0) return false;
	// -----
	InitializeCriticalSection(&criti);
	// -----
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
bool DBSave::Feee()
{
	if ( m_lpWzQueue != 0 )
	{
		delete m_lpWzQueue;
		m_lpWzQueue = 0;
	}
	// -----
	DeleteCriticalSection(&criti);
	// -----
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
BOOL DBSave::Add(LPBYTE pObject, int nSize, BYTE headcode,  int index)
{
	EnterCriticalSection(&criti);
	// -----
	BOOL bRet = m_lpWzQueue->AddToQueue(pObject, nSize, headcode, index);
	// -----
	LeaveCriticalSection(&criti);
	// -----
	return bRet;
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
bool DBSave::Begin()
{
	if ( m_ThreadHandle != NULL )
	{
		End();
	}
	// -----
	m_bIsRunning = TRUE;
	// -----
	m_ThreadHandle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)cSaveThreadProc, this, 0, &m_dwThreadID );
	// -----
	if ( m_ThreadHandle == FALSE )
	{
		CLog.MsgBox("Thread create error %s %d", __FILE__, __LINE__);
		return false;
	}
	// -----
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------
static LRESULT WINAPI cSaveThreadProc(DBSave * pThis)
{
	return pThis->ThreadProc();
}
// -----------------------------------------------------------------------------------------------------------------------
void DBSave::End()
{
	if ( m_ThreadHandle != 0 )
	{
		m_bIsRunning = FALSE;
		// -----
		WaitForSingleObject(m_ThreadHandle , INFINITE);
		// -----
		CloseHandle(m_ThreadHandle);
		// -----
		m_ThreadHandle = 0;
	}
}
// -----------------------------------------------------------------------------------------------------------------------
DWORD DBSave::ThreadProc()
{
	int Count;
	int headcode;
	int aIndex;
	BYTE RecvData[5000];
	UINT nSize;

	while ( true )
	{
		EnterCriticalSection(&criti);
		// -----
		Count = m_lpWzQueue->GetCount();
		// -----
		if ( Count != 0 )
		{
			if (m_lpWzQueue->GetFromQueue(RecvData, &nSize, (UCHAR*)&headcode, &aIndex) == 1)
			{
				if (wsDataCli.DataSend((PCHAR)RecvData, nSize) == 0 )
				{
					CLog.LogAddC(TColor.Red(), "(%d)(%d) Failed to Save DB Character Settings", Count, aIndex);
				}
				else
				{
					CLog.LogAddC(TColor.Green(), "(%d)(%d) Succeeded to Save DB Character Settings", Count, aIndex);
				}
			}
		}
		// -----
		if ( m_bIsRunning == FALSE && Count == 0)
		{
			break;
		}
		// -----
		LeaveCriticalSection(&criti);
		// -----
		WaitForSingleObject(m_ThreadHandle, 300);
		// -----
	}
	// -----
	return FALSE;
}
// -----------------------------------------------------------------------------------------------------------------------