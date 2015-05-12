#include "TApp.h"

//-------------------------------------------------------------------------------------------------------------------------------------------
// # Made By Leo123[BoR]
// # Window control class
//-------------------------------------------------------------------------------------------------------------------------------------------

HWND	gHwnd		  	= NULL;
cApp*	gParent			= NULL;
//-------------------------------------------------------------------------------------------------------------------------------------------

cApp::cApp(void)
{
	m_isInit			= UNK;
	m_isClassReg		= UNK;
	m_isWndCreated		= UNK;
	m_IsMenuCreated		= UNK;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

cApp::~cApp(void)
{
	// -----
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void cApp::Init(APP_ARGS)
{
	_SET_PARENT_
	// ----
	this->m_isInit		= NO;
	//---
	this->m_hIsnt		= hInstance;
	this->m_PrevhIsnt	= hPrevInstance;
	this->m_CmdLine		= lpCmdLine;
	this->m_ShowCmd		= (CAPP_SHOWWINDOW_COMMANDS)nShowCmd;
	//---
	this->m_isInit		= YES;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void cApp::MyRegisterClass(LPCSTR AppName,WNDCLASSEX * wcex)
{
	//_SET_PARENT_
	// ----
	this->m_isClassReg	= NO;
	// --
	wcex->lpfnWndProc	= sWndProc;
	wcex->hInstance		= this->m_hIsnt;
	wcex->hIcon			= LoadIcon(this->m_hIsnt, MAKEINTRESOURCE(m_Icon));
	wcex->hIconSm		= LoadIcon(this->m_hIsnt, MAKEINTRESOURCE(m_Icon));
	wcex->cbClsExtra	= 0;
	wcex->cbWndExtra	= 0;
	wcex->lpszMenuName	= AppName;
	wcex->lpszClassName	= AppName;
	// --
	RegisterClassEx(wcex);
	// --
	this->m_appName		= AppName;
    // --
	this->m_isClassReg	= YES;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void cApp::MyWindowCreate(int Width,int Height,int xPos,int yPos,DWORD dwStyle)
{
	//_SET_PARENT_
	// ----
	this->m_isWndCreated	= NO;
	// ----
	this->m_hWnd = CreateWindow(this->m_appName, this->m_appName, dwStyle, xPos,
		yPos, Width, Height, NULL, NULL, this->m_hIsnt, NULL);
	// ----
	this->m_winWidth	= Width;
	this->m_winHeight	= Height;
	// ----
	if(this->m_hWnd == false)
	{
		MessageBoxA(0, "[cApp](MyWindowCreate) :: hWnd Error", 0, 0);
		return;
	}
	else
	{
		// # save handle for external functions
		gHwnd			= this->m_hWnd;
	}
	// ----
	ShowWindow(this->m_hWnd, this->m_ShowCmd);
	UpdateWindow(this->m_hWnd);
	// ----
	this->m_isWndCreated	= YES;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void cApp::MyDestroyWindow()
{
	//_SET_PARENT_
	// ----
	DestroyWindow(this->m_hWnd);
	// -----
	this->m_isWndCreated	= NO;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void cApp::MyCloseWindow()
{
	//_SET_PARENT_
	// ----
	CloseWindow(this->m_hWnd);
	// -----
	this->m_isWndCreated	= NO;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void cApp::MoveToCenter()
{
	//_SET_PARENT_
	// ----
	this->m_ScreenWidth		= GetSystemMetrics(SM_CXSCREEN);
	this->m_ScreenHeight	= GetSystemMetrics(SM_CYSCREEN);
	// ----
	MoveWindow(m_hWnd, (m_ScreenWidth - m_winWidth) / 2, (m_ScreenHeight - m_winHeight ) / 2,
		m_winWidth, m_winHeight, true);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void cApp::Size()
{
	//_SET_PARENT_
	// ----
	GetClientRect(m_hWnd, & m_Rect);
	// -----
	m_winHeight		=	m_Rect.bottom;
	m_winWidth		=	m_Rect.right;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void cApp::MsgLoop()
{
	if(GetMessage( & m_msg, this->m_hWnd, 0, NULL) != NULL)
	{
		if(TranslateAccelerator(m_msg.hwnd, 0, & m_msg) == false)
		{
			TranslateMessage(& m_msg);
			DispatchMessage(& m_msg);
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void cApp::MyCreateMenu()
{
	//_SET_PARENT_
	// ----
	m_IsMenuCreated		= NO;
	// ----
	m_hMenu				= CreateMenu();
	// -----
	if(m_hMenu)
	{
		m_IsMenuCreated = YES;
		// -----
		SetMenu(m_hWnd, m_hMenu);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------

BYTE cApp::CreateMenuItem(char * szName)
{
	//_SET_PARENT_
	// ----
	BYTE bReturn			= m_MenuID;
	// -----
	m_hSubMenu[m_MenuID]	= CreatePopupMenu();
	// -----
	AppendMenu(m_hMenu, MF_STRING | MF_POPUP, (UINT)m_hSubMenu[m_MenuID], szName);
	// -----
	SetMenu(m_hWnd, m_hMenu);
	// -----
	m_MenuID++;
	// -----
	return bReturn;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void cApp::CreateSubMenuItem(char * szName, BYTE MenuID, BYTE RegisterCMD)
{
	//_SET_PARENT_
	// -----
	AppendMenu(m_hSubMenu[MenuID], MF_STRING, RegisterCMD, szName);
	// -----
	SetMenu(m_hWnd, m_hMenu);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

APP_LPRESULTCB sWndProc(WNDPROC_ARGS)
{
	switch(uMsg)
	{
		case WM_CREATE:
		{
			gParent->OnCreate();				
		}
		break;
		case WM_PAINT:
		{
			//gParent->OnPaint();
		}
		break;
		case WM_CLOSE:
		{
			gParent->OnClose();
			// # Leo123 : OnClose we dont need to back the defualt window function
			// -----
			return 0;
		}
		break;
		case WM_SIZE:
		{
			//gParent->OnSize();
		}
		break;
		case WM_APP:
		{
			gParent->OnApp(wParam, lParam);
		}
		break;
		case WM_COMMAND:
		{
			if(gParent->m_IsMenuCreated == YES)
			{
				gParent->OnCmd(wParam);
			}
		}
		break;
		case WM_SYSCOMMAND:
		{
			switch(wParam)
			{
				case SC_MAXIMIZE:
				{
					//gParent->OnSize();
				}
				break;
			}
		}
		break;
	}
	// -----
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
//-------------------------------------------------------------------------------------------------------------------------------------------