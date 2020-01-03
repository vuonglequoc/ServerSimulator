#include "StdAfx.H"
#include "Main.H"
#include "Database.H"
#include "LogProc.H"
#include "Configs.H"
#include "ReadFileManager.H"
#include "Resource.H"

// Global Variables:
MainClass gMainApp;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
SOFT_WORK_THREAD;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
APP_MAIN WinMain( APP_ARGS )
{
	gMainApp.SetDefault();

	if (!gMainApp.Initialize(APP_ARGS_PUSH)) 
		return FALSE;

	RFile.SetInfo();
	BeginThread(MainTask, NULL, NULL);
	MCfg.Initialize();

	if(gDataBase.Initialize() == true)
	{
		for ( int i = 1; i < MAX_SRV_TYPES; i++ )
		{
			gMainApp.StartServer(i);
		}
		CLog.Info("Network Connection Successfully Initialized.");
	}
	return gMainApp.MsgLoop();
}

void MainClass::SetDefault()
{
	this->m_PosX			= 180;
	this->m_PosY			= 30;
	//this->m_Width			= 1000;
	this->m_Width			= 700;
	//this->m_Height			= 650;
	this->m_Height			= 500;
	this->m_bWorkerDelay	= 500;
	this->m_ShowWindow		= SW_NORMAL;
	this->m_bSoftWork		= false;

	sprintf(m_Title, "DataServer Emulator... Loading!");
}
void MainClass::MyRegisterClass( WNDCLASSEX * wcex )
{
	wcex->cbSize			= sizeof(WNDCLASSEX);
	wcex->style				= CS_HREDRAW | CS_VREDRAW;
	wcex->hCursor			= LoadCursor(NULL, IDC_ARROW);
	wcex->hbrBackground		= (HBRUSH)(COLOR_WINDOW+1);
	wcex->lpfnWndProc		= WndProc;
	wcex->hInstance			= this->hInst;
	wcex->hIcon				= LoadIcon(this->hInst, MAKEINTRESOURCE(m_Icon));
	wcex->hIconSm			= LoadIcon(this->hInst, MAKEINTRESOURCE(m_Icon));
	wcex->lpszClassName		= this->m_WinClass;	
	wcex->lpszMenuName		= (LPCSTR)IDR_DATASERVER;
	
	RegisterClassEx(wcex);
}
bool MainClass::Initialize( HINSTANCE hInstance, int nCmdShow )
{
	DWORD StartTime = GetTickCount();
	bool bReturn	= true;
	hInst			= hInstance;
	nCmdShow		= m_ShowWindow;

	LoadString(LOAD_APP_WIN);
	SetIcon(ID_SOFT_ICON);
	MyRegisterClass(&m_wcex);
	InitCommonControls();
	m_hWnd = CreateWindowEx(0, m_WinClass, TEXT (m_Title), WS_OVERLAPPEDWINDOW, m_PosX, m_PosY, m_Width, m_Height, NULL, NULL, hInstance, NULL);

	if (!m_hWnd){
		CLog.MsgBoxError("Window Initialize() :: hWnd Error");
		bReturn = false;
	}

	CLog.Initialize(m_hWnd);
	UpdateWindow(m_hWnd);
	ShowWindow(m_hWnd, nCmdShow);
	SendMessage(m_hWnd, WM_TIMER, WM_LOG_PAINT, NULL);

	CLog.Info("DataServer v%s Successfully Initialized.", SOFT_VER);

	return bReturn;
}
APP_LPRESULTCB WndProc( WNDPROC_ARGS )
{
	switch (uMsg)
	{
		case WM_CLOSE:
			gMainApp.ExitWindow(hWnd);
		break;

		case WM_DESTROY:
			gMainApp.DestroySocket();
			ExitProcess(0);
		break;

		default: 
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}
	return FALSE;
}
int MainClass::MsgLoop()
{
	while(GetMessage(&m_MSG, NULL, 0, 0))
	{
		if (!TranslateAccelerator(m_MSG.hwnd, 0, &m_MSG))
		{
			TranslateMessage(&m_MSG);
			DispatchMessage(&m_MSG);
		}
	}
	return (int) gMainApp.m_MSG.wParam;
}
bool MainClass::ExitWindow(HWND ghWnd)
{
	if ( MessageBox(m_hWnd, "Are You Sure Close DataServer?", "Confirmation", MB_YESNO|MB_APPLMODAL) == IDYES )
	{
		if ( DestroyWindow(ghWnd) == FALSE )
		{
			ExitProcess(0);
		}
	}
	return true;
}
void MainClass::Worker()
{
	while (true)
	{
		sprintf(m_Title, "DataServer Emulator v%s", SOFT_VER);
		
		CLog.RunWindow();
		SetWindowText(m_hWnd, m_Title);

		Sleep(m_bWorkerDelay);
	}
}
