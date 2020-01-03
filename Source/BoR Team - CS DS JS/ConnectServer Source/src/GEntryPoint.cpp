#include "GInclude.h"
#include "Resource.h"
#include "Main.h"
#include "TServerManager.h"
#include "TServersConfig.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
// # Made by Leo123[BoR]
//-------------------------------------------------------------------------------------------------------------------------------------------

MAIN_WORKER_THREAD;
//-------------------------------------------------------------------------------------------------------------------------------------------

void InitLog()
{
	g_Log.SetHandle(gHwnd);
	g_Log.SetFolderPath("Log_CS");
	g_Log.SetWriteLogStatus(true);
	g_Log.Run();
	g_Log.SetStatusText(g_Colors.Gold(), g_Colors.Azure()  , "%s STATE_NONE", MAIN_PROJECT_NAME);
	// ----
	g_Log.LogAdd(g_Colors.DarkGoldenrod(), "%s version %s , Build at %s", MAIN_PROJECT_NAME, MAIN_PROJECT_VERSION, MAIN_PROJECT_DATE);
}
//-------------------------------------------------------------------------------------------------------------------------------------------


bool InitIOCP()
{
	bool bReturn = false;
	// ----
	g_ServerManager.setIsOneConPerIP(true);
	// ----
	g_Log.LogAdd(g_Colors.Black(),"[GEnteryPoint][InitIOCP] :: Trying to Initialize IOCP System");
	// ----
	if(g_xmlConfig.EnterNode("ConnectServer") == true)
	{
		g_ServerManager.m_nIdleTime = g_xmlConfig.GetInt("IdleTime");
		g_ServerManager.setPort(g_xmlConfig.GetInt("Port"));
		if(g_ServerManager.Initialize(2) == true)
		{
			g_Log.LogAdd(g_Colors.Black(), "[GEntryPoint][InitIOCP] :: Initialized IOCP System.");
			// ----
			bReturn = true;
		}
	}
	else
	{
		g_Log.LogAdd(g_Colors.Red(), "[GEntryPoint][InitIOCP] :: (error) EnterNode('ConnectServer')");
	}
	// ----
	g_xmlConfig.UnLoad();
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
	if(g_xmlConfig.Load() == true)
	{
		if(InitIOCP() == true)
		{
			g_Log.SetStatusText(g_Colors.White(), g_Colors.Gray() , "%s STATE_WAIT", MAIN_PROJECT_NAME);
			// ----
			g_ServersConfig.Read();
			// ----
			g_Log.LogAdd(g_Colors.LightBlue(), "%s Loaded in %d msec", MAIN_PROJECT_NAME, (GetTickCount() - dwTickCount));
		}
	}
	while(g_Main.m_bWindowStop == false)
	{
		g_Main.MsgWorker();
	}
	// ----
	return SUCCESS;
}
//-------------------------------------------------------------------------------------------------------------------------------------------