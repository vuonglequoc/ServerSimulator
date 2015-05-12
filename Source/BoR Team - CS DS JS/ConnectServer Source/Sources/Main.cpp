#include "Main.h"
#include "TServerManager.h"
#include "TServersConfig.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
// Made by Leo123[BoR]

Main		g_Main;
xmlReader	g_xmlConfig("Config.xml");
//-------------------------------------------------------------------------------------------------------------------------------------------

Main::Main(void)
{
	m_iWorkerDelay	= 500; 
	m_bWindowStop	= false;
	m_bWorkStop		= false;
	// ----
	CMiniDumper		* m_Dumper = new CMiniDumper(MAIN_PROJECT_NAME);
	// ----
	memset(szTemp, 0, sizeof(szTemp));
}
//-------------------------------------------------------------------------------------------------------------------------------------------

Main::~Main(void)
{
	// ----
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void Main::Initialize(APP_ARGS)
{
	nShowCmd				= SW_NORMAL;
	// ----
	Init(APP_ARGS_PUSH);
	// ----
	WNDCLASSEX wcex;
	wcex.cbSize				= sizeof(WNDCLASSEX);
	wcex.style				= CS_HREDRAW | CS_VREDRAW;
	wcex.cbClsExtra			= 0;
	wcex.cbWndExtra			= 0;
	wcex.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground		= (HBRUSH)(COLOR_3DSHADOW + 1);
	// ----
	MyRegisterClass(MAIN_PROJECT_NAME, &wcex);
	MyWindowCreate(MAIN_PROJECT_WINDOW_WIDTH, MAIN_PROJECT_WINDOW_HEIGHT,
		0, 0, WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME);
	// ----
	MoveToCenter();
	SetupMenu();
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void Main::SetupMenu()
{
	BYTE ID	= 0x00;
	// ----
	MyCreateMenu();
	// ----
	ID = CreateMenuItem("&File");
	CreateSubMenuItem("&Exit", ID , (BYTE)CMD_EXIT);
	// ----
	ID = CreateMenuItem("&Options");
	CreateSubMenuItem("&Reload ServerList", ID ,(BYTE)CMD_RELOADCONFIG);
	CreateSubMenuItem("&Reload IdleTime", ID ,(BYTE)CMD_RELOADIDLE);
	CreateSubMenuItem("&Disconnect ID", ID ,(BYTE)CMD_DISCONNECTID);
	// ----
	ID = CreateMenuItem("&Help");
	CreateSubMenuItem("&About", ID , (BYTE)CMD_ABOUT);
	CreateSubMenuItem("&Uptime", ID ,(BYTE)CMD_UPTIME);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void Main::Worker()
{
	while(m_bWindowStop == false)
	{
		//sprintf(szTemp,"[%s] ..::%s::.. [active(%d)]", MAIN_PROJECT_VERSION, MAIN_PROJECT_NAME,
		sprintf_s(szTemp,"[%s] ..::%s::.. [active(%d)]", MAIN_PROJECT_VERSION, MAIN_PROJECT_NAME, // VuongLQ
			g_ServerManager.m_ConnectedCount);
		// ----
		SetWindowText(gHwnd, szTemp);
		// ----
		g_Log.Run();
		// ----
		Sleep(m_iWorkerDelay);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void Main::MsgWorker()
{
	MsgLoop();
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void Main::MsgBox(char * szFormat, ...)
{
	va_list pArguments;
	va_start(pArguments, szFormat);
	//vsprintf(szTemp, szFormat, pArguments);
	vsprintf_s(szTemp, szFormat, pArguments); // VuongLQ
	va_end(pArguments);
	// ----
	MessageBox(NULL, szTemp, "About", MB_OK | MB_APPLMODAL);
}

void Main::MsgBoxDisconnect()
{
	// ----
	MessageBox(NULL, "haha", "Disconnect ID", MB_OK | MB_APPLMODAL);
	
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void Main::Exit()
{
	ExitProcess(0);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void Main::OnCreate()
{
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void Main::OnClose()
{
	if(MessageBoxA(gHwnd, "Are you sure?", "Close ConnectServer", MB_YESNO) == IDYES)
	{	
		DestroyWindow(gHwnd);
		// ----
		m_bWindowStop	= false;
		m_bWorkStop		= false;
		// ----
		Exit();
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void Main::OnPaint()
{
	//g_Log.Run();
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void Main::OnSize()
{
	//g_Log.Run();
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void Main::OnCmd(WPARAM Param)
{
	switch(LOWORD(Param))
	{
		case (BYTE)CMD_EXIT:
		{
			OnClose();
		}
		break;

		case (BYTE)CMD_RELOADCONFIG:
		{
			g_ServersConfig.Read();
		}
		break;
		
		case (BYTE)CMD_DISCONNECTID:
		{
			MsgBoxDisconnect();
		}
		break;
		
		case (BYTE)CMD_UPTIME:
		{
			g_Log.LogAdd(0x000000, "[Main][OnCmd] :: (CMD_UPTIME) %d second(s) %d minute(s) %d day(s)", g_ServerManager.m_Timer.getSeconds(),
				g_ServerManager.m_Timer.getMinutes(), g_ServerManager.m_Timer.getDays());
		}
		break;

		case (BYTE)CMD_ABOUT:
		{
			MsgBox(MAIN_PROJECT_CREDITS);
		}
		break;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void Main::OnApp(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case (BYTE)MSG_FROM_JS:
		{
			g_Log.LogAdd(0x00, "[TEST] : %d %d ", LOWORD(lParam) ,HIWORD(lParam));
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------