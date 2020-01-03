/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Project:		Zodea MU GameServer - Season 6
 * Developer:	RealCoderZ MU Development © 2011
 * Hint:		Entry Point For Zodea GameServer
 * Coded By:	WolF & mesosa
 */
#ifndef MAIN_HEADER
#define MAIN_HEADER

#include "Defines.H"

// VuongLQ
#define MAX_MENU_ITEM	10
#define MAIN_PROJECT_CREDITS		"[MUBK] VuongLQ"
enum CMD_MENU_COMMANDS
{
	CMD_EXIT,
	CMD_ABOUT,
};

enum CAPP_MYNEW_VAR
{
	UNK = 0x00,
	NO	= 0x01,
	YES = 0x02,
};
//

class MainClass
{
public:
	MainClass() { }
	virtual ~MainClass() { }
	// -----
	friend APP_MAIN			WinMain(APP_ARGS);
	friend APP_LPRESULTCB	WndProc(WNDPROC_ARGS);
	friend APP_INTPTRCB		About(ABOUT_ARGS);
	// -----
	bool					Initialize(HINSTANCE, int);
	void					MyRegisterClass(HINSTANCE hInstance);
	bool					ExitWindow(HWND hWnd);
	bool					PaintWindow(HWND hWnd);
	bool					GameServerStart();
	bool					StartNetwork();
	// -----
	int						bServerReady;
	int						bCloseMsg;
	int						bCloseMsgTime;
	int						bCurPaintPlayer;
	int						bCurPaintType;
	int						bCurPaintMapNumber;

// VuongLQ
	void					SetupMenu();
	void					MyCreateMenu();
	BYTE					CreateMenuItem(char * szName);
	void					CreateSubMenuItem(char * szName, BYTE MenuID, BYTE RegisterCMD);
	void					OnCmd(WPARAM Param);
	void					MsgBox(char * szFormat, ...);
	// -----
public:
	MSG						Msg;
	HINSTANCE				hInst;
	HWND					m_hWnd;
	char					szTitle[MAX_STRING_LEN];
	char					szWindowClass[MAX_STRING_LEN];

// VuongLQ
private:
	HMENU							m_hMenu; // ?
	HMENU							m_hSubMenu[MAX_MENU_ITEM]; // ?
	CAPP_MYNEW_VAR					m_IsMenuCreated;
	BYTE							m_MenuID;
	char							szTemp[1024];
};

// -----------------------------------------------------------------------------------------------------------------------------------------------------
extern MainClass gMainApp;
// -----------------------------------------------------------------------------------------------------------------------------------------------------
//  :: Windows messages for the event handler ::
#define WM_GM_SERVER_MSG_PROC				0x401
#define WM_GM_CLIENT_MSG_PROC				0x402
#define WM_GM_JOIN_CLIENT_MSG_PROC			0x403
#define WM_START_SERVER						0x405
#define WM_GM_DATA1_CLIENT_MSG_PROC			0x40A
#define WM_GM_DATA2_CLIENT_MSG_PROC			0x40B
#define WM_GM_RANKING_CLIENT_MSG_PROC		0x40C
#define WM_GM_EVENTCHIP_CLIENT_MSG_PROC		0x40D
#define WM_GM_EXDATA_CLIENT_MSG_PROC		0x40E
#define WM_GM_CONNECT_SERVER_MSG			0x40F
// -----------------------------------------------------------------------------------------------------------------------------------------------------
//			:: Timer messages ::
#define	WM_LOG_PAINT						0x064
#define WM_FIRST_MSG_PROCESS				0x065
#define WM_AI_MONSTER_PROC					0x1F4
#define WM_SECOND_MSG_PROCESS				0x3E8
#define WM_SET_DATE							0x3E9
#define WM_GS_CLOSE							0x3EA
#define WM_LOG_DATE_CHANGE					0x3EB
#define WM_EVENT_RUN_PROC					0x3EC
#define WM_MOVE_MONSTER_PROC				0x3ED
#define WM_MONSTER_AND_MSG_PROC				0x3EE
#define WM_CONNECT_DATASERVER				0x3EF
#define WM_GUILD_LIST_REQUEST				0x3F0
#define WM_AI_MONSTER_MOVE_PROC				0x3F2
// -----------------------------------------------------------------------------------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------------------------------------------------------------------------------