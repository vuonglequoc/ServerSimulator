#include "../Header Files/StdAfx.H"
#include "../Header Files/LogProc.H"


LogClass	CLog;
TCOLORS		TColor;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
LogClass::LogClass()
{
	PrepareClass();
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
LogClass::~LogClass()
{
	DeleteObject(m_LogFont.Font);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void LogClass::Initialize(HWND szhWnd)
{
	CLog.ghWnd = szhWnd;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void LogClass::PrepareClass()
{
	GetCurrentDate(&m_Start_SysTime);
	GetCurrentDate(&m_Prev_SysTime);
	// -----
	m_LogFont.Width			= 6;
	m_LogFont.Height		= 15;
	// -----
	sprintf(m_szFolderPath, "LOG\0");
	sprintf(m_szFilePath, "LOG_%02d.%02d.%04d[%s].log\0", m_Start_SysTime.wDay, m_Start_SysTime.wMonth, m_Start_SysTime.wYear, SOFT_NAME);
	// -----
	// -----
	m_iTextLengthKeep			= 0;
	m_iCurrentLine				= 0;
	m_xPos						= 5;
	// -----
	memset(m_LogBuffFile, 0, sizeof(m_LogBuffFile));
	memset(m_Log, 0, sizeof(m_Log));
	// -----
	SetFolderPath("LOG");
	// -----
	CreateDirectory(m_szFolderPath, NULL);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void LogClass::RunWindow()
{
	if( !(IsWindowVisible(ghWnd)) || (IsIconic(ghWnd)) ) return;
	// ----
	this->PaintLog();
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void LogClass::PaintLog()
{
	m_hDC			= GetDC(ghWnd);
	// ----
	GetClientRect(ghWnd, & m_Rect);
	// ----
	m_Rect.top		= 1;
	// ----
	HBRUSH hPaintLogBrush = CreateSolidBrush (RGB(0,0,0));
	// ----
	FillRect(m_hDC, &m_Rect, hPaintLogBrush);
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
	ReleaseDC(ghWnd, m_hDC);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void LogClass::LogTextAdd(COLORREF color, char * text, int len)
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
	memcpy(lpInfo->Text, text, len);
	// ----
	lpInfo->Len				= len;
	// ----
	m_iCurrentLine++;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void LogClass::LogAddStr(COLORREF colorText, LPCCH pText)
{
	GetCurrentDate(&m_SysTime);
	// -----
	m_iTextLength = sprintf(m_LogBuff, "[%02d:%02d:%02d]  %s\0", m_SysTime.wHour, m_SysTime.wMinute, m_SysTime.wSecond, pText);
	// -----
	LogTextAdd(colorText, m_LogBuff, m_iTextLength);
	// -----
	if ( CheckDate(&m_SysTime, &m_Prev_SysTime) == false )
	{
		sprintf(m_szFilePath, "LOG_%02d.%02d.%04d[%s].log\0", m_SysTime.wDay, m_SysTime.wMonth, m_SysTime.wYear, SOFT_NAME);
		// -----
		GetCurrentDate(&m_Prev_SysTime);
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
			sprintf(m_szFilePath, "LOG_%02d.%02d.%04d[%s].log\0", m_Prev_SysTime.wDay, m_Prev_SysTime.wMonth, m_Prev_SysTime.wYear, SOFT_NAME);
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
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void LogClass::VLogAddFormat(COLORREF colorText, LPCCH& AddressOfFmt)
{
	PrepareLogBuff();
	// -----
	VA_START		( m_pArguments, AddressOfFmt );
	vsprintf		( &m_LogTempBuff[0], AddressOfFmt, m_pArguments );
	VA_END			( m_pArguments );
	// -----
	LogAddStr(colorText, m_LogTempBuff);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
bool LogClass::CheckDate(LPSYSTEMTIME CurrTime, LPSYSTEMTIME PrevTime)
{
	if ( ( CurrTime->wDay	!= PrevTime->wDay)	 || 
		 ( CurrTime->wMonth != PrevTime->wMonth) || 
		 ( CurrTime->wYear	!= PrevTime->wYear)	 ) 
		 return true;
	// -----
	return false;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void LogClass::WriteDataInFile(LPCCH szBuf, const int iSize)
{
	if( strlen(m_szFilePath) < 1 ) return;
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
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void LogClass::Error(LPCCH Message, ...)
{
	this->VLogAddFormat(TColor.Red(), Message);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void LogClass::Info(LPCCH Message, ...)
{
	this->VLogAddFormat(TColor.Gold(), Message);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void LogClass::Load(LPCCH Message, ...)
{
	this->VLogAddFormat(TColor.Azure(), Message);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void LogClass::Connect(LPCCH Message, ...)
{
	this->VLogAddFormat(TColor.Cyan(), Message);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void LogClass::Disconnect(LPCCH Message, ...)
{
	this->VLogAddFormat(TColor.CadetBlue(), Message);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void LogClass::LogIn(LPCCH Message, ...)
{
	this->VLogAddFormat(TColor.Coral(), Message);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void LogClass::LogOut(LPCCH Message, ...)
{
	this->VLogAddFormat(TColor.Orange(), Message);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void LogClass::Query(LPCCH Message, ...)
{
	this->VLogAddFormat(TColor.Violet(), Message);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void LogClass::Debug(LPCCH Message, ...)
{
	this->VLogAddFormat(TColor.Blue(), Message);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void LogClass::MsgBox(const char* Caption, LPCCH Message, ...)
{
	VA_START		( m_pArguments, Message );
	vsprintf		( &szTemp[0], Message, m_pArguments );
	VA_END			( m_pArguments );
	// -----
	MessageBoxA(NULL, szTemp, Caption, MB_OK | MB_APPLMODAL);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void LogClass::MsgBoxError(LPCCH Message, ...)
{
	VA_START		( m_pArguments, Message );
	vsprintf		( &szTemp[0], Message, m_pArguments );
	VA_END			( m_pArguments );
	// -----
	MessageBoxA(NULL, szTemp, ErrorMSGCaption, MB_OK|MB_ICONERROR);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------