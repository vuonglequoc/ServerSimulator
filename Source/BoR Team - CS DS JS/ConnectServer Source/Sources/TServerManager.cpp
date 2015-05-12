#include "GInclude.h"
#include "TServerManager.h"
#include "Main.h"
#include "GProtocol.h"

//-------------------------------------------------------------------------------------------------------------------------------------------
// # Made by Leo123[BoR]

//-------------------------------------------------------------------------------------------------------------------------------------------

CServerManager	g_ServerManager;
//-------------------------------------------------------------------------------------------------------------------------------------------

CServerManager::CServerManager(void)
{
	m_Timer.Init();
}
//-------------------------------------------------------------------------------------------------------------------------------------------

CServerManager::~CServerManager(void)
{
	// ----
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CServerManager::OnError(const char * szMessage, ...)
{
	g_Log.VLogAddFormat(g_Colors.Red(), szMessage);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CServerManager::OnMessage(const char * szMessage, ...)
{
	g_Log.VLogAddFormat(g_Colors.Green(), szMessage);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CServerManager::OnRecv(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize)
{
	CSProtocolCore(aIndex, HeadCode , aRecv , iSize);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CServerManager::OnSearch(int aIndex)
{
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CServerManager::OnConnect(int aIndex)
{
	g_Log.LogAdd(g_Colors.Blue(), "[CServerManager][OnConnect] :: index(%d)", aIndex);
	// ----
	m_ActiveTime[aIndex] = m_nIdleTime;
	// ----
	m_ConnectedCount++;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CServerManager::OnDisconnect(int aIndex)
{
	g_Log.LogAdd(g_Colors.Blue(), "[CServerManager][OnDisconnect] :: index(%d)", aIndex);
	// ----
	m_ConnectedCount--;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void CServerManager::OnConnected(int aIndex)
{
	g_Log.LogAdd(g_Colors.Blue(), "[CServerManager][OnConnected] :: index(%d)", aIndex);
	// ----
	CSConnectResultSend(aIndex);
}
//-------------------------------------------------------------------------------------------------------------------------------------------