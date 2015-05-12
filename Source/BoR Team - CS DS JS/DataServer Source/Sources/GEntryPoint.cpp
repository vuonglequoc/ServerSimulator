#include "GInclude.h"
#include "Resource.h"
#include "Main.h"
#include "TDbManager.h"
#include "TDSManager.h"
#include "TRSManager.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
// # Made by Leo123[BoR]
//-------------------------------------------------------------------------------------------------------------------------------------------

xmlReader xmlConfig("config.xml");
//-------------------------------------------------------------------------------------------------------------------------------------------

MAIN_WORKER_THREAD;
//-------------------------------------------------------------------------------------------------------------------------------------------

void InitLog()
{
	g_Log.SetHandle(gHwnd);
	g_Log.SetFolderPath("Log_DS");
	g_Log.SetWriteLogStatus(true);
	g_Log.Run();
	g_Log.SetStatusText(g_Colors.Gold(), g_Colors.Azure()  , "%s STATE_NONE", MAIN_PROJECT_NAME);
	g_Log.LogAdd(g_Colors.DarkGoldenrod(), "%s version %s", MAIN_PROJECT_NAME, MAIN_PROJECT_VERSION);
	g_Log.LogAdd(g_Colors.DarkGoldenrod(), MAIN_PROJECT_DATE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

bool InitDatabase()
{
	bool bReturn = false;
	// ----
	g_Log.LogAdd(g_Colors.Black(), "[GEntryPoint][InitDatabase] :: Initializing Database connection.");
	// ----
	if(xmlConfig.EnterNode("DataBase") == true)
	{
		if(g_DbManager.Connect(xmlConfig.GetStr("Host"), xmlConfig.GetStr("DB"),
			xmlConfig.GetStr("User"), xmlConfig.GetStr("Pass")) == true)
		{
			bReturn = true;
			// ----
			g_Log.LogAdd(g_Colors.Black(), "[GEntryPoint][InitDatabase] :: Initialized Database connection.");
		}
		else
		{
			g_DbManager.DiagnosticConn();
		}
	}
	else
	{
		g_Log.LogAdd(g_Colors.Red(), "[GEntryPoint][InitDatabase] :: (error) EnterNode('DataBase')");
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
	char szTemp[1024] = {0};
	// ----
	g_Log.LogAdd(g_Colors.Black(), "[GEntryPoint][InitIOCP] :: Initialized IOCP system connection.");
	// ----
	if(xmlConfig.EnterNode("DataServer") == true)
	{
		int PortCount	= xmlConfig.GetInt("PortCount");
		int PortStart	= xmlConfig.GetInt("PortStart");
		// ----
		CDSManager * g_DSManager = new CDSManager[PortCount];
		// ----
		for(int i = 0 ; i != PortCount; i++)
		{
			sprintf(szTemp, "DS_IOCP_%d", i+1);
			// ----
			g_DSManager[i].setPort( (i * 2) + PortStart);			
			g_DSManager[i].setName(szTemp);
			// ----
			bReturn = g_DSManager[i].Initialize(xmlConfig.GetInt("ThreadsCount"));
		}
		// ----
		xmlConfig.LeaveNode();
	}
	if(xmlConfig.EnterNode("RankingServer") == true)
	{
		g_TRSManager.setName("RS_IOCP");
		g_TRSManager.setPort(xmlConfig.GetInt("Port"));
		// ----
		bReturn = g_TRSManager.Initialize(1);
		// ----
		xmlConfig.LeaveNode();
	}
	// ----
	return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

APP_MAIN(APP_ARGS)
{
	DWORD	dwTickCount  = GetTickCount();
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
				g_Log.LogAdd(g_Colors.Black(), "[GEntryPoint][APP_MAIN] :: Initialized IOCP system connection.");
				g_Log.LogAdd(g_Colors.LightBlue(), "%s Loaded in %d msec", MAIN_PROJECT_NAME, (GetTickCount() - dwTickCount));
				g_Log.SetStatusText(g_Colors.Black(), g_Colors.LightCoral() , "%s STATE_WAIT", MAIN_PROJECT_NAME);
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

