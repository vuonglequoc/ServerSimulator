//-------------------------------------------------------------------------------------------------------------------------------------------
#ifndef CAPP_H
#define CAPP_H
//-------------------------------------------------------------------------------------------------------------------------------------------

#include <windows.h>
//-------------------------------------------------------------------------------------------------------------------------------------------

#define MAX_MENU_ITEM	10
//-------------------------------------------------------------------------------------------------------------------------------------------
// # Fixme : whats need todo with this kinda of defines?
//------------------------------------------------------------------------------------------------------------------------------------------- 

#define APP_MAIN		int APIENTRY WinMain
#define APP_ARGS		HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd
#define APP_ARGS_PUSH	hInstance,hPrevInstance,lpCmdLine,nShowCmd
#define WNDPROC_ARGS	HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam
#define APP_LPRESULTCB	LRESULT CALLBACK
#define APP_LPRESULT	LRESULT
#define THREADD			void (__cdecl *)(void *)
#define _SET_PARENT_	gParent	= this;
//-------------------------------------------------------------------------------------------------------------------------------------------

enum MAIN_RETURN_TYPE
{
	FAILL		= 0x00,
	SUCCESS		= 0x01,
};
//-------------------------------------------------------------------------------------------------------------------------------------------

enum CAPP_SHOWWINDOW_COMMANDS
{
	ESW_HIDE = 0x00,
	ESW_SHOWNORMAL,
	ESW_NORMAL,
	ESW_SHOWMINIMIZED,
	ESW_SHOWMAXIMIZED,
	ESW_MAXIMIZE,
	ESW_SHOWNOACTIVATE,
	ESW_SHOW,
	ESW_MINIMIZE,
	ESW_SHOWMINNOACTIVE,
	ESW_SHOWNA,
	ESW_RESTORE,
	ESW_SHOWDEFAULT,
	ESW_FORCEMINIMIZE,
	ESW_MAX,
};
//-------------------------------------------------------------------------------------------------------------------------------------------

enum CAPP_MYNEW_VAR
{
	UNK = 0x00,
	NO	= 0x01,
	YES = 0x02,
};
//-------------------------------------------------------------------------------------------------------------------------------------------

APP_LPRESULTCB					sWndProc(WNDPROC_ARGS);
//-------------------------------------------------------------------------------------------------------------------------------------------

class cApp
{
	friend APP_LPRESULTCB cApp::sWndProc(WNDPROC_ARGS);
	// ----
public:
	cApp(void);
	~cApp(void);
	// ----
private:
	HINSTANCE						m_hIsnt;
	HINSTANCE						m_PrevhIsnt;
	HWND							m_hWnd;
	HMENU							m_hMenu;
	HMENU							m_hSubMenu[MAX_MENU_ITEM];
	LPCSTR							m_CmdLine;
	CAPP_SHOWWINDOW_COMMANDS		m_ShowCmd;
	LPCSTR							m_appName;
	MSG								m_msg;
	// ----
	CAPP_MYNEW_VAR					m_isInit;
	CAPP_MYNEW_VAR					m_isClassReg;
	CAPP_MYNEW_VAR					m_isWndCreated;
	CAPP_MYNEW_VAR					m_IsMenuCreated;
	// ----
	BYTE							m_MenuID;	
public:
	int								m_Icon;
	RECT							m_Rect;
	int								m_ScreenWidth;
	int								m_ScreenHeight;
	int								m_winWidth;
	int								m_winHeight;
	// ----
	void							Init(APP_ARGS);
	void							MyRegisterClass(LPCSTR AppName,WNDCLASSEX * wcex);
	void							MyWindowCreate(int Width,int Height,int xPos,int yPos,DWORD dwStyle);
	void							MyDestroyWindow();
	void							MyCloseWindow();
	void							MoveToCenter();
	void							Size();
	void							MsgLoop();
	void							MyCreateMenu();
	BYTE							CreateMenuItem(char * szName);
	void							CreateSubMenuItem(char * szName, BYTE MenuID, BYTE RegisterCMD);
	APP_LPRESULT					WndProc(WNDPROC_ARGS);	
	// ----
	virtual	void					OnPaint(){};
	virtual void					OnCreate(){};
	virtual void					OnClose(){};
	virtual void					OnSize(){};
	virtual void					OnCmd(WPARAM Param){};
	virtual void					OnApp(WPARAM wParam, LPARAM lParam){};
};
//-------------------------------------------------------------------------------------------------------------------------------------------

extern HWND gHwnd;
//-------------------------------------------------------------------------------------------------------------------------------------------
#endif /* CAPP_H */
//-------------------------------------------------------------------------------------------------------------------------------------------