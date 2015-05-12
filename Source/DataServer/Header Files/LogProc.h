//----------------------------------------------------------------------------------
// # Project:	Zodea eMU Suite
// # Company:	RealCoderZ Software Incorpration. © 2012 http://www.realcoderz.com
// # Author:	WolF & M.E.S
// # Hint:		[Aug 09 2011] This Script For Log & Paint Class Of the Application.
// # More Info: This Script has been Modified By WolF in [Nov 27 2011]
//----------------------------------------------------------------------------------
#ifndef LOG_PROC_H
#define LOG_PROC_H

#include "StdAfx.H"


class LogClass  
{
public:
	LogClass();
	virtual ~LogClass();
	// -----
	void		Initialize(HWND szhWnd);
	void		RunWindow();
	void		LogAddStr(COLORREF colorText, LPCCH pText);
	void		VLogAddFormat(COLORREF colorText, LPCCH& AddressOfFmt);
	void		Error(LPCCH Message, ...);
	void		Info(LPCCH Message, ...);
	void		Load(LPCCH Message, ...);
	void		Connect(LPCCH Message, ...);
	void		Disconnect(LPCCH Message, ...);
	void		LogIn(LPCCH Message, ...);
	void		LogOut(LPCCH Message, ...);
	void		Query(LPCCH Message, ...);
	void		Debug(LPCCH Message, ...);
	void		MsgBox(LPCCH Caption, LPCCH Message, ...);
	void		MsgBoxError(LPCCH Message, ...);
	// -----
	bool		CheckDate(LPSYSTEMTIME CurrTime, LPSYSTEMTIME PrevTime);
	// -----
	HWND		ghWnd;
private:
	HDC			m_hDC;
	LOG_INFO	m_Log[LOG_LINE_COUNT];
	va_list		m_pArguments;
	RECT		m_Rect;
	FONT_INFO	m_LogFont;
	FILE*		m_fHandle;
	SYSTEMTIME	m_SysTime;
	SYSTEMTIME	m_Start_SysTime;
	SYSTEMTIME	m_Prev_SysTime;
	// -----
	char		m_DisplayerText[MAX_LINE_LEN];
	char		m_LogBuff[MAX_LINE_LEN];
	char		m_LogTempBuff[MAX_LINE_LEN];
	char		szTemp[MAX_LINE_LEN];
	char		m_LogBuffFile[LOG_TEXT_FILE_LENGTH];
	char		m_szFilePath[256];
	char		m_szFolderPath[256];
	char		m_szFullPath[MAX_LINE_LEN];
	// -----
	int			m_iCurrentLine;
	int			m_iTextLength;
	int			m_iTextLengthKeep;
	int			m_LinesPerSize;
	int			m_xPos;
	int			m_yPos;
	int			i;
	int			j;
	int			TextWidth;
	int			xCenterPos;
	int			OldBkMode;
	// -----
	void		PrepareClass();
	void		PrepareStatusBuff()							{ memset(m_DisplayerText,	0, sizeof(m_DisplayerText)); };
	void		PrepareLogBuff()							{ memset(m_LogTempBuff,		0, sizeof(m_LogTempBuff)); };
	void		GetCurrentDate(LPSYSTEMTIME lpSystemTime)	{ GetLocalTime(lpSystemTime); };
	void inline	SetFolderPath(LPCCH szFP)			{ memset(m_szFolderPath, 0, sizeof(m_szFolderPath)); memcpy(m_szFolderPath, szFP, strlen(szFP)); }
	void		PaintLog();
	void		LogTextAdd(COLORREF color, char * text, int iLen);
	void		WriteDataInFile(const char* szBuf, const int iSize);
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
extern LogClass CLog;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* LOG_PROC_H */
// -------------------------------------------------------------------------------------------------------------------------------------------------------