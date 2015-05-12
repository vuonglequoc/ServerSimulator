#include "GInclude.h"
#include "resource.h"
#include "Main.h"
#include "TDbManager.h"
#include "TServerManager.h"
#include "TTimerManger.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
// # Made by Leo123[BoR]
// EnteryPoint
//-------------------------------------------------------------------------------------------------------------------------------------------

xmlReader xmlConfig("config.xml");
//-------------------------------------------------------------------------------------------------------------------------------------------

MAIN_WORKER_THREAD;
//-------------------------------------------------------------------------------------------------------------------------------------------

void InitLog()
{
	g_Log.SetHandle(gHwnd);
	g_Log.SetFolderPath("Log_JS");
	g_Log.SetWriteLogStatus(true);
	g_Log.Run();
	g_Log.SetStatusText(g_Colors.Gold(), g_Colors.Azure()  , "%s STATE_NONE", MAIN_PROJECT_NAME);
	// ----
	g_Log.LogAdd(g_Colors.DarkGoldenrod(), "%s version %s, Build at %s", MAIN_PROJECT_NAME, MAIN_PROJECT_VERSION, MAIN_PROJECT_DATE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

bool InitDatabase()
{
	bool bReturn = false;
	// ----
	g_Log.LogAdd(g_Colors.Black(), "[GEnteryPoint][InitDatabase] :: Initializing Database connection.");
	// ----
	if(xmlConfig.EnterNode("DataBase") == true)
	{
		if(g_DbManager.Connect(xmlConfig.GetStr("Host"), xmlConfig.GetStr("DB"),
			xmlConfig.GetStr("User"), xmlConfig.GetStr("Pass")) == true)
		{
			bReturn = true;
			// ----
			g_Log.LogAdd(g_Colors.Black(), "[GEnteryPoint][InitDatabase] :: Initialized Database connection.");
			// ----
			g_DbManager.Exec("UPDATE MEMB_STAT SET ConnectStat = 0");
		}
		else
		{
			g_DbManager.DiagnosticConn();
		}
	}
	else
	{
		g_Log.LogAdd(g_Colors.Red(), "[GEnteryPoint][InitDatabase] :: (xmlReader) (error) EnterNode('DataBase')");
	}
	// ----
	xmlConfig.LeaveNode();
	// ----
	return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

bool InitIOCP()
{
	bool bReturn = false;
	// ----
	g_Log.LogAdd(g_Colors.Black(), "[GEnteryPoint][InitIOCP] :: Trying to Initialize IOCP System");
	// ----
	if(xmlConfig.EnterNode("JoinServer") == true)
	{
		g_UseMD5	= xmlConfig.GetBool("MD5");
		// ----
		g_ServerManager.setPort(xmlConfig.GetInt("Port"));
		if(g_ServerManager.Initialize(2) == true)
		{
			g_Log.LogAdd(g_Colors.Black(), "[GEnteryPoint][InitIOCP] :: Initialized IOCP System.");
			// ----
			bReturn = true;
		}
	}
	else
	{
		g_Log.LogAdd(g_Colors.Red(), "[GEnteryPoint][InitIOCP] :: (error) EnterNode('JoinServer')");
	}
	// ----
	xmlConfig.LeaveNode();
	// ----
	return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

APP_MAIN(APP_ARGS)
{
	DWORD	dwTickCount  = GetTickCount();
	bool	bTemp		 = false;
	char	szTemp[1024] = {0};
	// ----
	memset(szTemp, 0, sizeof(szTemp));
	// ----
	g_Main.SetIcon(IDI_ICON1);
	g_Main.Initialize(APP_ARGS_PUSH);
	// ---
	InitLog();
	// ----
	MAIN_SETUP_WORKER;
	// ----
	if(xmlConfig.Load() == true)
	{
		if(InitDatabase() == true)
		{
			if(InitIOCP() == true)
			{
				g_Log.SetStatusText(g_Colors.White(), g_Colors.SkyBlue() , "%s STATE_WAIT", MAIN_PROJECT_NAME);
				// ----
				g_Log.LogAdd(g_Colors.LightBlue(), "%s Loaded in %dms", MAIN_PROJECT_NAME, (GetTickCount() - dwTickCount));
				// ----
				g_TimerManager.Init();
				g_TimerManager.OnMinute();
			}
		}
	}
	// ----
	xmlConfig.UnLoad();
	// ----
	while(g_Main.m_bWindowStop == false)
	{
		g_Main.MsgWorker();
	}
	// ----
	return SUCCESS;
}
//-------------------------------------------------------------------------------------------------------------------------------------------