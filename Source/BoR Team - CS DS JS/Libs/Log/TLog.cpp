#include "TLog.h"

//-------------------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------------------
// # Developed by: S@nek[BoR] && Leo123[BoR];
// # Optimized by: S@nek[BoR];
// # Operating System: Windows;
// # IDE: Microsoft Visual Studio;
// # Last change: 14.04.2010 02-46;
//-------------------------------------------------------------------------------------------------------------------------------------------

TLog g_Log;
//-------------------------------------------------------------------------------------------------------------------------------------------

TLog::TLog()
{
	PrepareClass();
}
//-------------------------------------------------------------------------------------------------------------------------------------------

TLog::~TLog()
{
	DeleteObject(m_StatusFont.font);
	DeleteObject(m_LogFont.font);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void TLog::PrepareClass()
{
	GetCurrentDate(& m_SysTime_Start);
	GetCurrentDate(& m_SysTime_Previous);
	// ----
	sprintf(m_szFolderPath, "Logs\0");
	sprintf(m_szFilePath, "Logs_%02d.%02d.%04d.log\0",
				m_SysTime_Start.wDay, m_SysTime_Start.wMonth, m_SysTime_Start.wYear);
	// ----
	m_StatusFont.Width			= 20;
	m_StatusFont.Height			= 90;
	m_StatusFont.font			= CreateFontA(
		m_StatusFont.Height, m_StatusFont.Width,
		0, 0, FW_DONTCARE, 0, 0, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FIXED_PITCH, "Tahoma");
	// ----
	m_LogFont.Width				= 6;
	m_LogFont.Height			= 15;
	m_LogFont.font				= CreateFontA(
		m_LogFont.Height, m_LogFont.Width,
		0, 0, FW_DONTCARE, 0, 0, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, "Tahoma");
	// ----
	m_StatusColors.StandByBG	= RGB(255, 0, 0);
	m_StatusColors.ActiveBG		= CreateSolidBrush(RGB(110, 240, 120));
	m_StatusColors.StandByText	= RGB(255, 255, 255);
	m_StatusColors.ActiveText	= RGB(250, 250, 250);
	// ----
	m_iTextLengthKeep			= 0;
	m_iLineCount				= 0;
	m_xPos						= 5;
	// ----
	// # Prepare File part;
	m_bNeedWriteLog				= true;
	memset(m_LogBuffFile, 0, sizeof(m_LogBuffFile));
	// ----
	memset(m_Log, 0, sizeof(m_Log));
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void TLog::SetHandle(HWND hWnd)
{
	m_hWnd				= hWnd;
};
//-------------------------------------------------------------------------------------------------------------------------------------------

void TLog::SetWriteLogStatus(bool bStatus)
{
	m_bNeedWriteLog		= bStatus;
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void TLog::SetFolderPath(const char * szFolderPath)
{
	memset(m_szFolderPath, 0, sizeof(m_szFolderPath));
	memcpy(m_szFolderPath, szFolderPath, strlen(szFolderPath));
}
//-------------------------------------------------------------------------------------------------------------------------------------------
int test = 0;
void TLog::Run()
{
	if( (IsWindowVisible(m_hWnd) == false) || (IsIconic(m_hWnd) == true) )
	{
		return;
	}
	// ----
	m_runSync.Lock();
	// ----
	if(test == 0)
		this->PaintStatus();
	else test++;
	Sleep(0);
	this->PaintLog();
	// ----
	m_runSync.Unlock();
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void TLog::PaintStatus()
{
	m_hDC					= GetDC(m_hWnd);
	// ----
	GetClientRect(m_hWnd, & m_Rect);
	// ----
	SelectObject(m_hDC, m_StatusFont.font);
	// ----
	TextWidth				= m_StatusTextLength * m_StatusFont.Width;
	xCenterPos				= (m_Rect.right - TextWidth) / 2;
	// ----
	m_Rect.bottom			= m_StatusFont.Height;
	// ----
	FillRect(m_hDC, & m_Rect, CreateSolidBrush(m_StatusColors.StandByBG));
	// ----
	OldBkMode				= SetBkMode(m_hDC, TRANSPARENT);
	// ----
	SetTextColor(m_hDC, m_StatusColors.StandByText);
	// ----
	TextOutA(m_hDC, xCenterPos-60, 0, m_DisplayerText, m_StatusTextLength);
	// ----
	SetBkMode(m_hDC, OldBkMode);
	ReleaseDC(m_hWnd, m_hDC);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void TLog::PaintLog()
{
	m_hDC			= GetDC(m_hWnd);
	// ----
	GetClientRect(m_hWnd, & m_Rect);
	// ----
	m_Rect.top		= m_StatusFont.Height;
	// ----
	FillRect(m_hDC, & m_Rect, (HBRUSH)GetStockObject(0));
	// ----
	m_LinesPerSize	= m_Rect.bottom - m_Rect.top;
	// ----
	m_yPos			= m_Rect.bottom - (m_LinesPerSize % m_LogFont.Height) - m_LogFont.Height;
	// ----
	m_LinesPerSize	/= m_LogFont.Height;
	// ----
	i				= m_iLineCount - 1;
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
		SelectObject(m_hDC, m_LogFont.font);
		SetTextColor(m_hDC, RGB(GetBValue(lpInfo->Color), GetGValue(lpInfo->Color), GetRValue(lpInfo->Color)));
		// ----
		TextOut(m_hDC, m_xPos, m_yPos, lpInfo->Text, lpInfo->Len);
		// ----
		m_yPos		-= m_LogFont.Height;
	}
	// ----
	ReleaseDC(m_hWnd, m_hDC);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void TLog::LogTextAdd(COLORREF color, char * text, int len)
{
	m_Sync.Lock();
	// ----
	if(m_iLineCount == LOG_LINE_COUNT)
	{
		m_iLineCount		= 0;
	}
	// ----
	LOG_INFO * lpInfo		= & m_Log[m_iLineCount];
	// ----
	lpInfo->Color			= color;
	// ----
	memset(lpInfo->Text, 0, sizeof(lpInfo->Text));
	memcpy(lpInfo->Text, text, len);
	// ----
	lpInfo->Len				= len;
	// ----
	m_iLineCount++;
	// ----
	m_Sync.Unlock();
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void TLog::LogAddStr(COLORREF colorText, const char * pText)
{
	GetCurrentDate(& m_SysTime);
	// ----
	m_iTextLength	= sprintf(m_LogBuff, "[%02d:%02d:%02d]  %s\0",
		m_SysTime.wHour, m_SysTime.wMinute, m_SysTime.wSecond, pText);
	// ----
	LogTextAdd(colorText, m_LogBuff, m_iTextLength);
	// ----
	if( m_bNeedWriteLog == true )
	{
		if( IsDateDifferent(& m_SysTime, & m_SysTime_Previous) == true )
		{
			sprintf(m_szFilePath, "Logs_%02d.%02d.%04d_next_%02d.%02d.%04d.log\0",
				m_SysTime.wDay, m_SysTime.wMonth, m_SysTime.wYear,
				m_SysTime_Start.wDay, m_SysTime_Start.wMonth, m_SysTime_Start.wYear);
			// ----
			GetCurrentDate(& m_SysTime_Previous);
			// ----
			// Write Data in File;
			WriteDataInFile(m_LogBuffFile, m_iTextLengthKeep);
			// ----
			// Prepare array to next data;
			memset(m_LogBuffFile, 0, sizeof(m_LogBuffFile));
			m_iTextLengthKeep	= 0;
		}
		else
		{
			if( m_iTextLengthKeep + m_iTextLength >= LOG_TEXT_FILE_LENGTH )
			{
				// Write Data in File;
				WriteDataInFile(m_LogBuffFile, m_iTextLengthKeep);
				// ----
				// Prepare array to next data;
				memset(m_LogBuffFile, 0, sizeof(m_LogBuffFile));
				m_iTextLengthKeep	= 0;
			}
		}
		// ----
		// Keep data in buff;
		memcpy(& m_LogBuffFile[m_iTextLengthKeep],
			m_LogBuff, m_iTextLength);
		// ----
		m_iTextLengthKeep	+= m_iTextLength;
		// ----
		sprintf(& m_LogBuffFile[m_iTextLengthKeep], "\n");
		// ----
		m_iTextLengthKeep	+= 1;
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void TLog::VLogAddFormat(COLORREF colorText, const char *& AddressOfFmt)
{
	m_Sync.Lock();
	// ----
	PrepareLogBuff();
	// ----	
	va_start(m_pArguments, AddressOfFmt);		// the real address of fmt. m_pArguments = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v)
	vsprintf(m_LogTempBuff, AddressOfFmt, m_pArguments);
	va_end(m_pArguments);
	// ----
	LogAddStr(colorText, m_LogTempBuff);
	// ----
	m_Sync.Unlock();
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void TLog::LogAdd(COLORREF colorText, const char * fmt, ...)
{
	VLogAddFormat(colorText, fmt);
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void TLog::SetStatusText(COLORREF colorText, char * text, ...)
{
	PrepareStatusBuff();
	// ----
	va_start(m_pArguments, text);	
	m_StatusTextLength	= vsprintf(m_DisplayerText, text, m_pArguments);
	va_end(m_pArguments);
	// ----
	m_StatusColors.StandByText		= RGB(GetBValue(colorText), GetGValue(colorText), GetRValue(colorText));
}
//-------------------------------------------------------------------------------------------------------------------------------------------

void TLog::SetStatusText(COLORREF colorText, COLORREF colorBG, char * text, ...)
{
	PrepareStatusBuff();
	// ----
	va_start(m_pArguments, text);	
	m_StatusTextLength	= vsprintf(m_DisplayerText, text, m_pArguments);
	va_end(m_pArguments);
	// ----
	m_StatusColors.StandByText		= RGB(GetBValue(colorText), GetGValue(colorText), GetRValue(colorText));
	m_StatusColors.StandByBG		= RGB(GetBValue(colorBG), GetGValue(colorBG), GetRValue(colorBG));
}
//-------------------------------------------------------------------------------------------------------------------------------------------

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
		fwrite(szBuf, iSize, 1, m_fHandle); // Binary;
		// ----
		fclose(m_fHandle);
	}
}
//-------------------------------------------------------------------------------------------------------------------------------------------

bool TLog::IsDateDifferent(LPSYSTEMTIME lpst1, LPSYSTEMTIME lpst2)
{
	return ((lpst1->wDay != lpst2->wDay) ||
		(lpst1->wMonth != lpst2->wMonth) ||
		(lpst1->wYear != lpst2->wYear) );
}
//-------------------------------------------------------------------------------------------------------------------------------------------