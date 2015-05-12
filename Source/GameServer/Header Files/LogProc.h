/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Project:		Zodea MU GameServer - Season 6
 * Developer:	RealCoderZ MU Development © 2011
 * Hint:		Log Paint For Zodea GameServer
 * Coded By:	WolF & mesosa
 */
#ifndef LOG_PROC_HEADER
#define LOG_PROC_HEADER

#include "StdAfx.H"
#include "TColors.H"
#include "GameMain.H"


#define LOG_LINE_LENGTH			190
#define LOG_LINE_COUNT			50
#define LOG_TEXT_FILE_LENGTH	512
//-------------------------------------------------------------------------------------------------------------------------------------------
struct CTLDATE
{
	int				Day;
	int				Month;
    int				Year;
};
//-------------------------------------------------------------------------------------------------------------------------------------------
struct FONT_INFO
{
	HFONT			Font;
	int				Height;
	int				Width;
};
//-------------------------------------------------------------------------------------------------------------------------------------------
struct STUATS_COLORS_INFO
{
	HBRUSH			ActiveBG;
	COLORREF		StandByBG;
	COLORREF		ActiveText;
	COLORREF		StandByText;
};
//-------------------------------------------------------------------------------------------------------------------------------------------
struct LOG_INFO
{
	int				Len;
	COLORREF		Color;
	char			Text[LOG_LINE_LENGTH];
};
//-------------------------------------------------------------------------------------------------------------------------------------------
class TLog  
{
public:
	TLog()
	{
		GCS_Create();
		// -----
		GCS_Init();
		// -----
		PrepareClass();
	}
	// -----
	~TLog()
	{
		DeleteObject(m_LogFont.Font);
		// -----
		GCS_Delete();
	}
	// -----
	void				Initialize(HWND hWnd);
	void				SetFolderPath(const char * szFolderPath);
	void				Run();
	// -------
	void				LogAddStr(COLORREF colorText, char * pText);
	void				VLogFormat(COLORREF colorText, const char *& AddressOfFmt);
	int					CheckDate(LPSYSTEMTIME CurrTime, LPSYSTEMTIME PrevTime);
	// -------
	void				Error(const char * Message, ...);
	void				Debug(const char * Message, ...);
	void				Info(const char * Message, ...);
	void				Load(const char * Message, ...);
	void				Connect(const char * Message, ...);
	void				Disconnect(const char * Message, ...);
	void				LogIn(const char * Message, ...);
	void				LogOut(const char * Message, ...);
	void				Event(const char * Message, ...);
	void				MsgBox(const char * Message, ...);
	void				MsgBox(const char * Caption, const char * Message, ...);
	void				MsgBoxError(LPCCH Message, ...);
	// Temperoray Logs
	void				LogAdd(const char * fmt, ...);
	void				LogAddC(COLORREF colorText, const char * fmt, ...);
	// -------
private:
	HWND				m_hWnd;
	HDC					m_hDC;
	LOG_INFO			m_Log[LOG_LINE_COUNT];
	va_list				m_pArguments;
	SYSTEMTIME			m_SysTime;
	SYSTEMTIME			m_SysTime_Start;
	SYSTEMTIME			m_SysTime_Previous;
	RECT				m_Rect;
	FONT_INFO			m_LogFont;
	FILE*				m_fHandle;
	CRITICAL_SECTION	m_LogCriticalSection;
	// -------
	char				szTemp[1024];
	char				szCaption[256];
	char				m_DisplayerText[1024];
	char				m_LogBuff[1024];
	char				m_LogTempBuff[1024];
	char				m_LogBuffFile[LOG_TEXT_FILE_LENGTH];
	char				m_szFilePath[256];
	char				m_szFolderPath[256];
	char				m_szFullPath[1024];
	char				m_szFunctionName[255];
	// -------
	int					m_iCurrentLine;
	int					m_iTextLength;
	int					m_iTextLengthKeep;
	int					m_LinesPerSize;
	int					m_xPos;
	int					m_yPos;
	int					i;
	int					j;
	int					TextWidth;
	int					OldBkMode;
	// -------
	DWORD				m_dwMaxLogBufferSize;
	// -------
	bool GCS_Init()
	{ 
		if (InitializeCriticalSectionAndSpinCount(& m_LogCriticalSection, 4000))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	// -------
	void GCS_Create()
	{
		InitializeCriticalSection(& m_LogCriticalSection);
	}
	void GCS_Lock()
	{
		EnterCriticalSection(& m_LogCriticalSection);
	}
	void GCS_UnLock()
	{
		LeaveCriticalSection(& m_LogCriticalSection);
	}
	void GCS_Delete()
	{
		DeleteCriticalSection(& m_LogCriticalSection);
	}
	// -------
	void PrepareClass();
	void PrepareStatusBuff()	{ memset(m_DisplayerText,	0, sizeof(m_DisplayerText)); };
	void PrepareLogBuff()		{ memset(m_LogTempBuff,		0, sizeof(m_LogTempBuff)); };
	void GetCurrentDate(LPSYSTEMTIME lpSystemTime)		{ GetLocalTime(lpSystemTime); };
	void PaintLog();
	void LogTextAdd(COLORREF color, char * text, int iLen);
	void WriteDataInFile(const char* szBuf, const int iSize);
};
//-------------------------------------------------------------------------------------------------------------------------------------------
extern TLog CLog;
//-------------------------------------------------------------------------------------------------------------------------------------------
#endif
//-------------------------------------------------------------------------------------------------------------------------------------------