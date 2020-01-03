//----------------------------------------------------------------------------------
// # Project:	Zodea eMU Suite
// # Company:	RealCoderZ Software Incorpration. © 2012 http://www.realcoderz.com
// # Author:	WolF & M.E.S
// # Hint:		[Aug 08 2011] This Script For Entry Point of the Application.
// # More Info: This Script has been Modified By WolF in [Nov 27 2011]
//----------------------------------------------------------------------------------
#ifndef _MAIN_H_
#define _MAIN_H_

#include "StdAfx.H"


class MainClass	: public IOCP
{
public:
	bool			Initialize(HINSTANCE, int);
	bool			ExitWindow(HWND ghWnd);
	// -----
	void			SetDefault();
	void			MyRegisterClass(WNDCLASSEX * wcex);
	void			Worker();
	// -----
	int				MsgLoop();
	// -----
	friend			LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
	friend			INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
	// -----
	MSG				m_MSG;
	HWND			m_hWnd;
	MAIN_STRUCT		MSTR;
	// -----
	bool			m_bSoftWork;
	// -----
private:
	char			m_WinClass[MAX_STRING_LEN];
	char			m_Title[MAX_STRING_LEN];
	int				m_Icon;
	int				m_PosX;
	int				m_PosY;
	int				m_Width;
	int				m_Height;
	int				m_bWorkerDelay;
	int				m_ShowWindow;
	// -----
	WNDCLASSEX		m_wcex;
	HINSTANCE		hInst;
	// -----
	void inline		SetIcon(int szIcon){m_Icon = szIcon;};
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
extern MainClass gMainApp;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* _MAIN_H_ */
// -------------------------------------------------------------------------------------------------------------------------------------------------------