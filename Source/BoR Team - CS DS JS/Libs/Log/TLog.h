//-------------------------------------------------------------------------------------------------------------------------------------------
#ifndef TLOG_H
#define TLOG_H
//-------------------------------------------------------------------------------------------------------------------------------------------

#include <windows.h>
#include <time.h>
#include <cstdio>
#include <fstream>
#include "..\..\include\TSynchro.h"
//-------------------------------------------------------------------------------------------------------------------------------------------

#define LOG_LINE_LENGTH			190
#define LOG_LINE_COUNT			50
#define LOG_TEXT_FILE_LENGTH	512 // 2^15=32768; 2^16=65536;
//-------------------------------------------------------------------------------------------------------------------------------------------

struct FONT_INFO
{
	HFONT					font;
	int						Height;
	int						Width;
};
//-------------------------------------------------------------------------------------------------------------------------------------------

struct STUATS_COLORS_INFO
{
	HBRUSH					ActiveBG;
	COLORREF				StandByBG;
	COLORREF				ActiveText;
	COLORREF				StandByText;
};
//-------------------------------------------------------------------------------------------------------------------------------------------

struct LOG_INFO
{
	int						Len;
	COLORREF				Color;
	char					Text[LOG_LINE_LENGTH];
};
//-------------------------------------------------------------------------------------------------------------------------------------------

class TLog  
{
private:
	char					m_DisplayerText	[1024];
	int						m_StatusTextLength;
	// ----
	LOG_INFO				m_Log[LOG_LINE_COUNT];
	// ----
	char					m_LogBuff		[1024];
	char					m_LogTempBuff	[1024];
	// ----
	va_list					m_pArguments;
	SYSTEMTIME				m_SysTime;
	// ----
	FONT_INFO				m_LogFont;
	FONT_INFO				m_StatusFont;
	// ----
	STUATS_COLORS_INFO		m_StatusColors;
	RECT					m_Rect;
	int						m_iLineCount;
	int						m_iTextLength;
	HWND					m_hWnd;
	// ----
	SYSTEMTIME				m_SysTime_Start;
	SYSTEMTIME				m_SysTime_Previous;
	// ----
	TSynchroCriticalSection m_runSync;
	TSynchroCriticalSection	m_Sync;
	// ----
	void PrepareClass();
	// ----
	void PrepareStatusBuff()	{ memset(m_DisplayerText,	0, sizeof(m_DisplayerText)); };
	void PrepareLogBuff()		{ memset(m_LogTempBuff,		0, sizeof(m_LogTempBuff)); };
	void GetCurrentDate(LPSYSTEMTIME lpSystemTime)		{ GetLocalTime(lpSystemTime); };
	bool IsDateDifferent(LPSYSTEMTIME lpst1, LPSYSTEMTIME lpst2);
	// ----
	void PaintStatus();
	void PaintLog();
	void LogTextAdd(COLORREF color, char * text, int len);
	// ----
	HDC						m_hDC;
	int						m_LinesPerSize;
	int						m_xPos;
	int						m_yPos;
	int						i;
	int						j;
	// ----
	int						OldBkMode;
	int						bStatusNum;
	int						TextWidth;
	int						xCenterPos;
	// ----
	// # Part of file;
	// ----
	char					m_LogBuffFile	[LOG_TEXT_FILE_LENGTH];
	char					m_szFilePath	[256];
	char					m_szFolderPath	[256];
	char					m_szFullPath	[1024];
	bool					m_bNeedWriteLog;
	int						m_iTextLengthKeep;
	FILE					*m_fHandle;
	// ----
	void WriteDataInFile(const char * szBuf, const int iSize);
public:
	TLog();
	virtual ~TLog();
	// ----
	void SetHandle(HWND hWnd);
	void SetWriteLogStatus(bool bStatus);
	void SetFolderPath(const char * szFolderPath);
	// ----
	void Run();
	// ----
	void LogAddStr(COLORREF colorText, const char * pText);
	void VLogAddFormat(COLORREF colorText, const char *& AddressOfFmt);
	void LogAdd(COLORREF colorText, const char * fmt, ...);
	void SetStatusText(COLORREF colorText, char * text, ...);
	void SetStatusText(COLORREF colorText, COLORREF colorBG, char * text, ...);
};
//-------------------------------------------------------------------------------------------------------------------------------------------

extern TLog g_Log;
//-------------------------------------------------------------------------------------------------------------------------------------------

#endif /* TLOG_H */
//-------------------------------------------------------------------------------------------------------------------------------------------



