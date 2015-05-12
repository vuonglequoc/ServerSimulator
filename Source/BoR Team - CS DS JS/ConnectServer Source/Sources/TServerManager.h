//-------------------------------------------------------------------------------------------------------------------------------------------
#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H
//-------------------------------------------------------------------------------------------------------------------------------------------

#include "TTimerManger.h"
//-------------------------------------------------------------------------------------------------------------------------------------------

class CServerManager : public IOCP
{
public:
	ULONG			m_ActiveTime[MAX_OBJECT];
	CTimerManager	m_Timer;
	// ----
	ULONG			m_nIdleTime;
	// ----
	CServerManager(void);
	~CServerManager(void);
	// ----
	bool StartServer(int iPort);
	// ----
	void OnError(const char * szMessage, ...);
	void OnMessage(const char * szMessage, ...);
	void OnRecv(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize);
	void OnSearch(int aIndex);
	void OnConnect(int aIndex);
	void OnDisconnect(int aIndex);
	void OnConnected(int aIndex);
	// ----
	int m_ConnectedCount;
};
//-------------------------------------------------------------------------------------------------------------------------------------------

extern CServerManager	g_ServerManager;
//-------------------------------------------------------------------------------------------------------------------------------------------
#endif /* SERVERMANAGER_H */
//-------------------------------------------------------------------------------------------------------------------------------------------
