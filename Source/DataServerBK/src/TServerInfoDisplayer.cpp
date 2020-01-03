// TServerInfoDisplayer.cpp: implementation of the TServerInfoDisplayer class.
//	GS-N	1.00.18	JPN	0x004A9500	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TServerInfoDisplayer.h"

TServerInfoDisplayer g_ServerInfoDisplayer;

// VuongLQ modify
static char * ServerTypeText[] =
{
	/*
	"[- Standby -]",
	"[- Active -]"
	*/
	"[- MuBK eDS -] Standby",
	"[- MuBK eDS -] Active"

};

static char * ErrorMessge[] = 
{
	"0) JoinServer - Active",
	"1) CashShopServer - Active",
	"2) DataServer - Active",
	"3) ExDBServer - Active",
	"4) EventServer - Active",
	"5) RankingServer - Active"
};




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TServerInfoDisplayer::TServerInfoDisplayer()
{
	this->m_bValidJSConnection = FALSE;
	this->m_bValidCSConnection = FALSE;
	this->m_bValidDSConnection = FALSE;
	this->m_bValidEXDSConnection = FALSE;
	this->m_bValidEVDSConnection = FALSE;
	this->m_bValidRKDSConnection = FALSE;

	this->InitGDIObject();
}

TServerInfoDisplayer::~TServerInfoDisplayer()
{
	this->DelGDIObject();
}



void TServerInfoDisplayer::InitGDIObject()
{
	this->m_hFont = CreateFont(80, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, 
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE, "Times");

	this->m_hNormalBrush = CreateSolidBrush(RGB(110, 240, 120));	// GREEN
	this->m_hAbnormalBrush = CreateSolidBrush(RGB(105, 105, 105));	// GRAY
}



void TServerInfoDisplayer::DelGDIObject()
{
	DeleteObject(this->m_hFont);
	DeleteObject(this->m_hNormalBrush);
	DeleteObject(this->m_hAbnormalBrush);
}





void TServerInfoDisplayer::Run(HWND hWnd)
{
	this->PaintAllInfo(hWnd, 0, 20);
}



void TServerInfoDisplayer::PaintAllInfo(HWND hWnd, int iTopLeftX, int iTopLeftY)
{
	HDC hDC = GetDC(hWnd);
	RECT rect;
	GetClientRect(hWnd, &rect);
	rect.top = 20;
	rect.bottom = 100;
	int iLineCount = 0;
	int iIndex=0;

	int iOldBkMode = SetBkMode(hDC, TRANSPARENT);

	if ( this->m_bValidJSConnection != FALSE ||
		 this->m_bValidCSConnection != FALSE ||
		 this->m_bValidDSConnection != FALSE ||
		 this->m_bValidEXDSConnection != FALSE ||
		 this->m_bValidEVDSConnection!= FALSE ||
		 this->m_bValidRKDSConnection != FALSE )
	{
		FillRect(hDC, &rect, this->m_hNormalBrush);
		iIndex = 1;
	}
	else
		FillRect(hDC, &rect, this->m_hAbnormalBrush);

	if ( this->m_bValidJSConnection == TRUE )
	{
		SetTextColor(hDC, RGB(0, 0, 255));
		TextOut(hDC, iTopLeftX, iTopLeftY+iLineCount*15, ErrorMessge[0], strlen(ErrorMessge[0]));
		iLineCount++;
	}

	if ( this->m_bValidCSConnection == TRUE )
	{
		SetTextColor(hDC, RGB(0, 0, 255));
		TextOut(hDC, iTopLeftX,  iTopLeftY+iLineCount*15, ErrorMessge[1], strlen(ErrorMessge[1]));
		iLineCount++;
	}

	if ( this->m_bValidDSConnection == TRUE )
	{
		SetTextColor(hDC, RGB(0, 0, 255));
		TextOut(hDC, iTopLeftX, iTopLeftY+iLineCount*15, ErrorMessge[2], strlen(ErrorMessge[2]));
		iLineCount++;
	}

	if ( this->m_bValidEXDSConnection == TRUE )
	{
		SetTextColor(hDC, RGB(0, 0, 255));
		TextOut(hDC, iTopLeftX, iTopLeftY+iLineCount*15, ErrorMessge[3], strlen(ErrorMessge[3]));
		iLineCount++;
	}

	if ( this->m_bValidEVDSConnection == TRUE )
	{
		SetTextColor(hDC, RGB(0, 0, 255));
		TextOut(hDC, iTopLeftX,  iTopLeftY+iLineCount*15, ErrorMessge[4], strlen(ErrorMessge[4]));
		iLineCount++;
	}

	if ( this->m_bValidRKDSConnection == TRUE )
	{
		SetTextColor(hDC, RGB(0, 0, 255));
		TextOut(hDC, iTopLeftX, iTopLeftY+iLineCount*15, ErrorMessge[5], strlen(ErrorMessge[5]));
		iLineCount++;
	}

	HFONT pOldFont = (HFONT)SelectObject(hDC, this->m_hFont);
	SetTextColor(hDC, RGB(250, 250, 250));

	//TextOut(hDC, 250, 20, ServerTypeText[iIndex], strlen(ServerTypeText[iIndex]));
	// VuongLQ
	char *serverType = ServerTypeText[iIndex];
	size_t stringlen = strlen(serverType);
	TextOut(hDC, 150, 20, serverType, stringlen); // Chua center
	//
	SelectObject(hDC, pOldFont);
	SetBkMode(hDC, iOldBkMode);
	ReleaseDC(hWnd, hDC);
}