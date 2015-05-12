#include "Main.h"
#include "TServerManager.h"
//-------------------------------------------------------------------------------------------------------------------------------------------
// # Made by Leo123[BoR]

//-------------------------------------------------------------------------------------------------------------------------------------------

Main		g_Main;
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
	// ----
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
	// ----
	SetupMenu();
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void Main::Worker()
{
	while(m_bWindowStop == false)
	{
		sprintf(szTemp,"[%s] ..::%s::.. [active(%d)] [%d/%d]", MAIN_PROJECT_VERSION, MAIN_PROJECT_NAME,
			g_ServerManager.m_ServerConnectedCount, g_ServerManager.m_UserConnectedCount, MAX_USER);
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
	vsprintf(szTemp, szFormat, pArguments);
	va_end(pArguments);
	// ----
	MessageBox(NULL, szTemp, "About", MB_OK | MB_APPLMODAL);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void Main::Exit()
{
	g_Log.LogAdd(g_Colors.Firebrick(), "[Main][Exit] :: Program exit(0) TickCount %d", GetTickCount());
	// ----
	ExitProcess(0);
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
	CreateSubMenuItem("&Disconnect all GameServer", ID , (BYTE)CMD_ALLGSCLOSE);
	CreateSubMenuItem("&Disconnect all Users", ID , (BYTE)CMD_ALLUSERCLOSE);
	// ----
	ID = CreateMenuItem("&Help");
	CreateSubMenuItem("&About", ID , (BYTE)CMD_ABOUT);
	// ----
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void Main::OnCreate()
{

}
//-------------------------------------------------------------------------------------------------------------------------------------------

void Main::OnClose()
{
	if(MessageBoxA(gHwnd, "Are you sure?", "Close JoinServer", MB_YESNO) == IDYES)
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

		case (BYTE)CMD_ALLGSCLOSE:
		{
			for(int i = 0 ; i != g_ServerManager.m_ServerConnectedCount ; i++)
			{
				g_ServerManager.CloseClient(i);
			}
		}
		break;	

		case (BYTE)CMD_ALLUSERCLOSE:
		{
			g_Log.LogAdd(g_Colors.Firebrick(), "[CMD_ALLUSERCLOSE] :: function not work");
			// ----
			for(int i = 0; i != MAX_USER; i++)
			{
				if(m_UserObj[i].iUserNumber != SLOT_FREE)
				{
					g_Log.LogAdd(g_Colors.Pink(), "[CMain][OnCmd] :: CMD_ALLUSERCLOSE Account [%s] was disconnected!",
						m_UserObj[i].m_szAccountID);
					// ----
					g_ServerManager.DelUser(m_UserObj[i].m_szAccountID);
				}
			}
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
