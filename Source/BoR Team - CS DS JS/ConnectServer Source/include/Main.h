//-------------------------------------------------------------------------------------------------------------------------------------------
#ifndef MAIN_H
#define MAIN_H
//-------------------------------------------------------------------------------------------------------------------------------------------

#include "GInclude.h"
#include "TTimerManger.h"
//-------------------------------------------------------------------------------------------------------------------------------------------

#define MAIN_WORKER_THREAD			void Task(void *dummy){g_Main.Worker();}
#define MAIN_SETUP_WORKER			_beginthread(Task, 0 , 0)
#define MAIN_PROJECT_NAME			"[- MUBK eCS -]"
#define MAIN_PROJECT_WINDOW_WIDTH	700
#define MAIN_PROJECT_WINDOW_HEIGHT	500
//-------------------------------------------------------------------------------------------------------------------------------------------

enum CMD_MENU_COMMANDS
{
	CMD_EXIT,
	CMD_RELOADCONFIG,
	CMD_UPTIME,
	CMD_ABOUT,
	CMD_RELOADIDLE,
	CMD_DISCONNECTID,
};
//-------------------------------------------------------------------------------------------------------------------------------------------
class Main : cApp
{
private:
	friend APP_LPRESULTCB	sWndProc(WNDPROC_ARGS);
	// ----
	CMiniDumper*			m_Dumper;
	// ----
	int						m_iWorkerDelay;
	char					szTemp[1024];
public:
	Main(void);
	~Main(void);
	// ----
	bool	m_bWindowStop;
	bool	m_bWorkStop;
	// ----
	void	Initialize(APP_ARGS);
	void	SetupMenu();
	void	MsgBox(char * szFormat , ... );
	void 	MsgBoxDisconnect();
	void	Worker();
	void	MsgWorker();
	void	Exit();
	// ----
	void	OnCreate();
	void	OnPaint();
	void	OnClose();
	void	OnSize();
	void	OnCmd(WPARAM Param);
	void	OnApp(WPARAM wParam, LPARAM lParam);
	// ----
	void inline SetIcon(int icon){m_Icon = icon;};
};
//-------------------------------------------------------------------------------------------------------------------------------------------

extern Main			g_Main;
extern xmlReader	g_xmlConfig;
//-------------------------------------------------------------------------------------------------------------------------------------------
#endif /* MAIN_H */
//-------------------------------------------------------------------------------------------------------------------------------------------


