// eDataServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "eDataServer.h"
#include "ServerEngine.h"
#include "DSprotocol.h"
#include "EDSprotocol.h"
#include "Sprotocol.h"
#include "RSprotocol.h"
#include "Eprotocol.h"
#include "LogProc.h"
#include "Giocp.h"
#include "TServerInfoDisplayer.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
HWND ghWnd;
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text
int g_dwMaxServerGroups=GetPrivateProfileInt("SETTINGS", "MAX_SERVER", 10, ".\\eDataServer.ini") * MAX_SERVER_TYPE;	

WORD g_FSGateServerListPort = GetPrivateProfileInt("SETTINGS", "FSGatePort", 55909, ".\\eDataServer.ini");
WORD g_JoinServerListPort = GetPrivateProfileInt("SETTINGS", "JoinServerPort", 55970, ".\\eDataServer.ini");
WORD g_ConnectServerListPort = GetPrivateProfileInt("SETTINGS", "ConnectServerPort", 44405, ".\\eDataServer.ini");
WORD g_DataServer1ListPort = GetPrivateProfileInt("SETTINGS", "DataServer1Port", 55960, ".\\eDataServer.ini");
WORD g_DataServer2ListPort = GetPrivateProfileInt("SETTINGS", "DataServer2Port", 55962, ".\\eDataServer.ini");
WORD g_ExDataServerListPort = GetPrivateProfileInt("SETTINGS", "ExDataServerPort", 55906, ".\\eDataServer.ini");
WORD g_ChatServerListPort = GetPrivateProfileInt("SETTINGS", "ChatServerPort", 55980, ".\\eDataServer.ini");
WORD g_EventServerListPort = GetPrivateProfileInt("SETTINGS", "EventServerPort", 44456, ".\\eDataServer.ini");
WORD g_RankingServerListPort = GetPrivateProfileInt("SETTINGS", "RankingServerPort", 44455, ".\\eDataServer.ini");
WORD g_CashShopServerListPort = GetPrivateProfileInt("SETTINGS", "CashShopServerPort", 55821, ".\\eDataServer.ini");
BOOL g_UseMD5 = GetPrivateProfileInt("SQL", "UseMD5Encrypt", 1, ".\\eDataServer.ini");

// Them chuc nang bat tat che do ket noi
BOOL g_UseJoinServer = GetPrivateProfileInt("SETTINGS", "UseJoinServer", 1, ".\\eDataServer.ini");
BOOL g_UseDataServer1 = GetPrivateProfileInt("SETTINGS", "UseDataServer1", 1, ".\\eDataServer.ini");
BOOL g_UseDataServer2 = GetPrivateProfileInt("SETTINGS", "UseDataServer2", 1, ".\\eDataServer.ini");
BOOL g_UseCashShopServer = GetPrivateProfileInt("SETTINGS", "UseCashShopServer", 1, ".\\eDataServer.ini");

BOOL g_UseExDataServer = GetPrivateProfileInt("SETTINGS", "UseExDataServer", 1, ".\\eDataServer.ini");
BOOL g_UseEventServer = GetPrivateProfileInt("SETTINGS", "UseEventServer", 1, ".\\eDataServer.ini");
BOOL g_UseRankingServer = GetPrivateProfileInt("SETTINGS", "UseRankingServer", 1, ".\\eDataServer.ini");
//

TCHAR g_MuOnlineDNS[64];
TCHAR g_MeMuOnlineDNS[64];
TCHAR g_EventServerDNS[64];
TCHAR g_RankingServerDNS[64];
TCHAR g_UserID[64];
TCHAR g_Password[64];


// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	JSDisconnect(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_EDATASERVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	InitCommonControls();
	CreateDirectory("LOG", NULL);
	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_EDATASERVER);
	LogInit(TRUE);
	LogAddC(2, "Initializing...");
	LogDateChange();
	SendMessage(ghWnd, WM_TIMER, WM_LOG_PAINT, NULL);
	gObjServerInit();
	
	IniteDataServer();
	
	//
	InitExDataServer();
	//
	
	IocpServerStart();

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}


bool IniteDataServer()
{
	SetTimer(ghWnd, WM_LOG_PAINT, 1000, NULL);
	SetTimer(ghWnd, WM_LOG_DATE_CHANGE, 300, NULL);

	GetPrivateProfileString("SQL", "MuOnlineDNS", "", g_MuOnlineDNS, sizeof(g_MuOnlineDNS), ".\\eDataServer.ini");
	GetPrivateProfileString("SQL", "MeMuOnlineDNS", "", g_MeMuOnlineDNS, sizeof(g_MeMuOnlineDNS), ".\\eDataServer.ini");
	GetPrivateProfileString("SQL", "EventDNS", "", g_EventServerDNS, sizeof(g_EventServerDNS), ".\\eDataServer.ini");
	GetPrivateProfileString("SQL", "RankingDNS", "", g_RankingServerDNS, sizeof(g_RankingServerDNS), ".\\eDataServer.ini");
	GetPrivateProfileString("SQL", "User", "", g_UserID, sizeof(g_UserID), ".\\eDataServer.ini");
	GetPrivateProfileString("SQL", "Pass", "", g_Password, sizeof(g_Password), ".\\eDataServer.ini");

	//InitMuOnlineDB();
	//InitMeMuOnlineDB();
	
	InitMuOnlineExDB();
	
	if(g_UseRankingServer == TRUE)
		InitRankingDB();
	
	if (g_UseEventServer == TRUE)
		InitEventDB();

	SetWindowText(ghWnd, "[v1.0.0.1] ..::[- MuBK eDS -]::..");


	return true;
}
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_EDATASERVER);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_EDATASERVER;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ghWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_EXIT:
				   //DestroyWindow(hWnd);
					 // VuongLQ
					if ( MessageBox(NULL, "Are you sure to Close?", "Confirmation", MB_YESNO|MB_ICONQUESTION) == IDYES )
						DestroyWindow(hWnd);
				   break;
				   
				case ID_OPTIONS_CASHSHOPRELOAD:
					break;
				case ID_OPTIONS_JSDISCONNECT:
					DialogBox(hInst, (LPCTSTR)IDD_JSDISCONNECT, hWnd, (DLGPROC)JSDisconnect);
					break;
					
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   if (IDC_BUTTONJSD) 
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_TIMER:
			switch ( wParam )
			{
				case WM_LOG_PAINT:
					{
						RECT rect;
						HDC hdc = GetDC(hWnd);
						GetClientRect(hWnd, &rect);
						FillRect(hdc, &rect, (HBRUSH)GetStockObject(0));
						ReleaseDC(hWnd, hdc);

						if ( LogTextPaint != NULL )
							LogTextPaint(hWnd);

						g_ServerInfoDisplayer.Run(hWnd);
					}
					break;
				case WM_LOG_DATE_CHANGE:
					LogDateChange();
					break;
			}
			break;
		case WM_DESTROY:
			DestroyGIocp();
			PostQuitMessage(0);
			break;
		case WM_CLOSE:
			if ( MessageBox(NULL, "Are you sure to Close?", "Confirmation", MB_YESNO|MB_ICONQUESTION) == IDYES )
				DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

// Mesage handler for JSDisconnect box.
LRESULT CALLBACK JSDisconnect(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDC_BUTTONJSD) 
			{
				CQuery g_MuOnlineDB;
				//EndDialog(hDlg, LOWORD(wParam));
				//IDC_IDJSDISCONNECT.GetWindowText(s);
				char szAccountID[20];

				LPTSTR szText = new TCHAR[20];
				GetDlgItemText(hDlg, IDC_IDJSDISCONNECT, szText, 20);
				if (sizeof(szText) != 0)
				{
					memcpy(szAccountID, szText, sizeof(szText));
				}

				if ( g_MuOnlineDB.Connect(g_MuOnlineDNS, g_UserID, g_Password) == TRUE )
				{
					//tagUSER_DATA * lpUser = & m_UserObj[nIndex];
					// -----
					//m_ServObj[lpUser->m_ServerIndex].m_ConnectedCount--;
					// -----
					
					g_MuOnlineDB.ExecQuery("EXEC WZ_DISCONNECT_MEMB '%s'", szAccountID);
					g_MuOnlineDB.Fetch();
					g_MuOnlineDB.Close();
					LogAddC(2, "[JSDisconnect] :: ID (%s) is Disconnected...", szAccountID);
					g_MuOnlineDB.ExecQuery("UPDATE MEMB_STAT SET ConnectStat='0' WHERE memb___id='%s'", szAccountID);
					g_MuOnlineDB.Fetch();
					g_MuOnlineDB.Close();
					
					// -----
					//JS_USER_DISCONNECT pMsg	=	{0};
					// -----
					//pMsg.h.Set(0x09, sizeof(pMsg));
					// -----
					//memcpy(szAccountID, szAccountID, sizeof(szAccountID));
					// -----
					//pMsg.aIndex	= lpUser->nNumber;
					// -----
					//DataSendAll((LPBYTE)&pMsg, pMsg.h.Size);
					// -----
					//lpUser->Clear();
					// -----
					//m_UserConnectedCount--;
				}
				else
				{
					LogAddC(2, "[MuOnlineDB] Error On Connection!");
				}
				

				return TRUE;
			}
			if (LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

