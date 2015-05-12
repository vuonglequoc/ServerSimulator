/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Project:		Zodea MU GameServer - Season 6
 * Developer:	RealCoderZ MU Development © 2011
 * Hint:		Log Paint For Zodea GameServer
 * Coded By:	WolF & mesosa
 */
#include "../Header Files/stdafx.h"
#include "../Header Files/LogProc.H"


TLog	CLog;
TCOLORS TColor;
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::Initialize(HWND hWnd)
{
	this->m_hWnd = hWnd;
	// -----
	COLORREF TextColor	= RGB(0, 230, 230);
	COLORREF BGColor	= RGB(110, 110, 250);
	// -----
	SetFolderPath("LOG");
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::PrepareClass()
{
	GetCurrentDate(& m_SysTime_Start);
	GetCurrentDate(& m_SysTime_Previous);

	// -----
	m_LogFont.Width			= 6;
	m_LogFont.Height		= 15;
	// -----
	m_LogFont.Font		= CreateFontA(m_LogFont.Height, m_LogFont.Width, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET,
									  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Tahoma");
	// -----
	m_iTextLengthKeep			= 0;
	m_iCurrentLine				= 0;
	m_xPos						= 5;
	// -----
	memset(m_LogBuffFile, 0, sizeof(m_LogBuffFile));
	memset(m_Log, 0, sizeof(m_Log));
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::SetFolderPath(const char * szFolderPath)
{
	memset(m_szFolderPath, 0, sizeof(m_szFolderPath));
	memcpy(m_szFolderPath, szFolderPath, strlen(szFolderPath));
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::Run()
{
	GCS_Lock();
	// ----
	if( !(IsWindowVisible(m_hWnd)) || (IsIconic(m_hWnd)) ) return;
	// ----
	this->PaintLog();
	// ----
	GCS_UnLock();
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::PaintLog()
{
	m_hDC			= GetDC(m_hWnd);
	// ----
	GetClientRect(m_hWnd, & m_Rect);
	// ----
	m_Rect.top		= 10;
	// ----
	HBRUSH Color	= CreateSolidBrush(COLORREF(RGB(0,0,0)));
	// ----
	FillRect(m_hDC, & m_Rect, Color);
	// ----
	m_LinesPerSize	= m_Rect.bottom - m_Rect.top;
	// ----
	m_yPos			= m_Rect.bottom - (m_LinesPerSize % m_LogFont.Height) - m_LogFont.Height;
	// ----
	m_LinesPerSize	/= m_LogFont.Height;
	// ----
	i				= m_iCurrentLine - 1;
	// ----
	LOG_INFO * lpInfo;
	// ----
	for(j = 0; j < m_LinesPerSize; j++, i--)
	{
		if(i < 0)
		{
			i		= LOG_LINE_COUNT - 1;
		}
		// ----
		lpInfo		= & m_Log[i];
		// ----
		if(lpInfo->Len <= 0)
		{
			break;
		}
		// ----
		SetBkMode(m_hDC, TRANSPARENT);
		SelectObject(m_hDC, m_LogFont.Font);
		SetTextColor(m_hDC, RGB(GetBValue(lpInfo->Color), GetGValue(lpInfo->Color), GetRValue(lpInfo->Color)));
		// ----
		TextOut(m_hDC, m_xPos, m_yPos, lpInfo->Text, lpInfo->Len);
		// ----
		m_yPos		-= m_LogFont.Height;
	}
	// ----
	ReleaseDC(m_hWnd, m_hDC);
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::LogTextAdd(COLORREF color, char* text, int iLen)
{
    if(m_iCurrentLine == LOG_LINE_COUNT)
	{
		m_iCurrentLine		= 0;
	}
	// ----
	LOG_INFO * lpInfo		= & m_Log[m_iCurrentLine];
	// ----
	lpInfo->Color			= color;
	// ----
	memset(lpInfo->Text, 0, sizeof(lpInfo->Text));
	memcpy(lpInfo->Text, text, iLen);
	// ----
	lpInfo->Len				= iLen;
	// ----
	m_iCurrentLine++;
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::LogAddStr(COLORREF colorText, char * pText)
{
    GetCurrentDate(& m_SysTime);
	// -----
	m_iTextLength = sprintf(m_LogBuff, "[%02d:%02d:%02d]  %s\0", m_SysTime.wHour, m_SysTime.wMinute, m_SysTime.wSecond, pText);
	// -----
	LogTextAdd(colorText, m_LogBuff, m_iTextLength);
	// -----
	if( CheckDate(& m_SysTime, & m_SysTime_Previous) )
	{
		sprintf(m_szFilePath, "LOG_%02d.%02d.%04d[%s].log\0", m_SysTime.wDay, m_SysTime.wMonth, m_SysTime.wYear, szServerName);
		// -----
		GetCurrentDate(& m_SysTime_Previous);
		// -----
		WriteDataInFile(m_LogBuffFile, m_iTextLengthKeep);
		// -----
		memset(m_LogBuffFile, 0, sizeof(m_LogBuffFile));
		m_iTextLengthKeep	= 0;
	}
	else
	{
		if( m_iTextLengthKeep + m_iTextLength >= LOG_TEXT_FILE_LENGTH )
		{
			sprintf(m_szFilePath, "LOG_%02d.%02d.%04d[%s].log\0", m_SysTime.wDay, m_SysTime.wMonth, m_SysTime.wYear, szServerName);
			// -----
			WriteDataInFile(m_LogBuffFile, m_iTextLengthKeep);
			// -----
			memset(m_LogBuffFile, 0, sizeof(m_LogBuffFile));
			m_iTextLengthKeep	= 0;
		}
	}
	// ----
	memcpy(& m_LogBuffFile[m_iTextLengthKeep], m_LogBuff, m_iTextLength);
	// ----
	m_iTextLengthKeep	+= m_iTextLength;
	// ----
	sprintf(& m_LogBuffFile[m_iTextLengthKeep], "\n");
	// ----
	m_iTextLengthKeep	+= 1;
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::VLogFormat(COLORREF colorText, const char *& AddressOfFmt)
{
	PrepareLogBuff();
	// ----	
	va_start(m_pArguments, AddressOfFmt);
	vsprintf(m_LogTempBuff, AddressOfFmt, m_pArguments);
	va_end(m_pArguments);
	// ----
	LogAddStr(colorText, m_LogTempBuff);
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
int TLog::CheckDate(LPSYSTEMTIME CurrTime, LPSYSTEMTIME PrevTime)
{
	return ((CurrTime->wDay != PrevTime->wDay) || (CurrTime->wMonth != PrevTime->wMonth) || (CurrTime->wYear != PrevTime->wYear));
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::WriteDataInFile(const char * szBuf, const int iSize)
{
	if( strlen(m_szFilePath) < 1 )
	{
		return;
	}
	// ----
	CreateDirectory(m_szFolderPath, NULL);
	// ----
	sprintf(m_szFullPath, "%s\\%s\0", m_szFolderPath, m_szFilePath);
	// ----
	m_fHandle	= fopen(m_szFullPath, "a");
	// ----
	if( m_fHandle != NULL )
	{
		fwrite(szBuf, iSize, 1, m_fHandle);
		// ----
		fclose(m_fHandle);
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::Error(const char * Message, ...)
{
	this->VLogFormat(TColor.Red(), Message);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::Debug(const char * Message, ...)
{
	this->VLogFormat(TColor.Peru(), Message);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::Info(const char * Message, ...)
{
	this->VLogFormat(TColor.OliveDrab(), Message);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::Load(const char * Message, ...)
{
	this->VLogFormat(TColor.Gray(), Message);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::Connect(const char * Message, ...)
{
	this->VLogFormat(TColor.PaleGoldenrod(), Message);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::Disconnect(const char * Message, ...)
{
	this->VLogFormat(TColor.Blue(), Message);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::LogIn(const char * Message, ...)
{
	this->VLogFormat(TColor.Purple(), Message);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::LogOut(const char * Message, ...)
{
	this->VLogFormat(TColor.Orange(), Message);
}
// ----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::Event(const char * Message, ...)
{
	this->VLogFormat(TColor.DarkTurquoise(), Message);
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::MsgBox(const char * Message, ...)
{
	char szTemp[1024];
	// -----
	va_list pArguments;
	va_start(pArguments, Message);
	vsprintf(szTemp, Message, pArguments);
	va_end(pArguments);
	// -----
	MessageBoxA(NULL, szTemp, "Critical Error", MB_OK | MB_APPLMODAL);
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::MsgBox(const char* Caption, const char * Message, ...)
{
	char szTemp[1024];
	// -----
	va_list pArguments;
	va_start(pArguments, Message);
	vsprintf(szTemp, Message, pArguments);
	va_end(pArguments);
	// -----
	MessageBoxA(NULL, szTemp, Caption, MB_OK | MB_APPLMODAL);
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
// Temperoray Logs!
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::LogAdd(const char * fmt, ...)
{
	//VLogFormat(TColor.White(), fmt);
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::LogAddC(COLORREF colorText, const char * fmt, ...)
{
	//VLogFormat(TColor.BlanchedAlmond(), fmt);
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------
void TLog::MsgBoxError(LPCCH Message, ...)
{
	va_start		( m_pArguments, Message );
	vsprintf		( &szTemp[0], Message, m_pArguments );
	va_end			( m_pArguments );
	// -----
	MessageBoxA(NULL, szTemp, "Error", MB_OK|MB_ICONERROR);
}