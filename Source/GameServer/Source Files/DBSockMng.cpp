//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	DB Socket Manager Class
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------
#include "../Header Files/stdafx.h"
#include "../Header Files/MyWinsockBase.h"
#include "../Header Files/wsJoinServerCli.H"
#include "../Header Files/Main.H"
#include "../Header Files/GameMain.H"
#include "../Header Files/DBSockMng.H"
#include "../Header Files/DSProtocol.H"
#include "../Header Files/LogProc.H"


DBSockMng cDBSMng;
// -----------------------------------------------------------------------------------------------------------------------------------------------------
DBSockMng::DBSockMng() 
{
	m_ActiveServer = DS_UNINITIALIZED;
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
DBSockMng::~DBSockMng()
{
	// -----
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void DBSockMng::Connect()
{	
	if(wsCDBS[0].GetSocket() == INVALID_SOCKET)
	{
		int State;
		// ----
		wsCDBS[0].CreateSocket(gMainApp.m_hWnd);
		wsCDBS[0].SetProtocolCore(DataServerProtocolCore);
		State = wsCDBS[0].Connect((LPSTR)IPAdress[1], Port[2], WM_GM_DATA1_CLIENT_MSG_PROC);
		// ----
		if(State == 0)
		{
			CLog.LogAddC(TColor.Red(),"DataServer01 Connecting Failed IP: (%s) Port: (%d)", IPAdress[1], Port[2]);
			wsCDBS[0].Close();
		}
		else
		{
			CLog.LogAddC(TColor.Purple(),"DataServer01 Connected IP: (%s) Port: (%d)", IPAdress[1], Port[2]);	
			m_ActiveServer = DataServer01;
			DataServerLogin(DataServer01);
		}
	}
	// -----
	if (wsCDBS[1].GetSocket() == INVALID_SOCKET)	
	{
		int State;
		wsCDBS[1].CreateSocket(gMainApp.m_hWnd);
		wsCDBS[1].SetProtocolCore(DataServerProtocolCore);
		State = wsCDBS[1].Connect((LPSTR)IPAdress[2], Port[3], WM_GM_DATA2_CLIENT_MSG_PROC);
		// ----
		if(State == 0)
		{
			CLog.LogAddC(TColor.Red(),"DataServer02 Connecting Failed IP: (%s) Port: (%d)", IPAdress[2], Port[3]);
			wsCDBS[1].Close();
		}
		else
		{
			CLog.LogAddC(TColor.Purple(),"DataServer02 Connected IP: (%s) Port: (%d)", IPAdress[2], Port[3]);	
			m_ActiveServer = DataServer02;
			DataServerLogin(DataServer02);
		}
	}
	// -----
	if ( wsExDbCli.GetSocket()  == INVALID_SOCKET)
	{
		CLog.LogAddC(TColor.Orange(), "ExDB Server Checking..");
		wsExDbCli.CreateSocket(gMainApp.m_hWnd);
		ExDataServerConnect((LPSTR)IPAdress[3], WM_GM_EXDATA_CLIENT_MSG_PROC);
	}
	else
	{
		if ( wsExDbCli.GetConnect() == 0 )
		{
			CLog.LogAddC(TColor.Orange(), "Error() ExDB Server Checking..");
			ExDataServerConnect((LPSTR)IPAdress[3], WM_GM_EXDATA_CLIENT_MSG_PROC);
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void DBSockMng::ActiveChange()
{
	if(m_ActiveServer == DataServer01)
	{
		m_ActiveServer = DataServer02;
		// -----
		CLog.LogAddC(TColor.Cyan(), "DataServer02 is Activated.");
	}
	else
	{
		m_ActiveServer = DataServer01;
		// -----
		CLog.LogAddC(TColor.Cyan(), "DataServer01 is Activated.");
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
bool DBSockMng::Send(char* Buff, int aLen)
{
	if (m_ActiveServer < 0 || m_ActiveServer > 1) return false;
	// -----
	if (wsCDBS[m_ActiveServer].DataSend((char*)Buff, aLen) == NULL)
	{
		ActiveChange();
		wsCDBS[m_ActiveServer].DataSend((char*)Buff, aLen);
	}
	// -----
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
bool DBSockMng::Send(int Server, char* Buff, int aLen)
{
	if (Server < 0 || Server > 1) return false;
	// -----
	if (wsCDBS[Server].DataSend(Buff, aLen) == 0)
	{
		ActiveChange();
	}
	// -----
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
bool DBSockMng::MsgProc(int line, WPARAM wParam, LPARAM lParam)
{
	switch (lParam & 0xFFFF & 0xFFFF)
	{
		case 1:
			wsCDBS[line].DataRecv();
			break;
		// -----
		case 2:
			wsCDBS[line].FDWRITE_MsgDataSend();
			break;
		// -----
		case 32:
			wsCDBS[line].Close();
			break;
		// -----
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------