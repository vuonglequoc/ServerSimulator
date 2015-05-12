//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	News Class with Time Separated
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------
#include "../Header Files/stdafx.h"
#include "../Header Files/News.h"
#include "../Header Files/Defines.h"
#include "../Header Files/Notice.H"
#include "../Header Files/LogProc.H"

GCNews NewsC;
// ----------------------------------------------------------------------------------------------------------------------
GCNews::GCNews()
{
	// -----
}
// ----------------------------------------------------------------------------------------------------------------------
GCNews::~GCNews()
{
	// -----
}
// ----------------------------------------------------------------------------------------------------------------------
void GCNews::Init()
{
	if(Enabled = GetPrivateProfileIntA(COMMON_HDR,"Enabled",1,NEWS_FILE_PATH) == 1)
	{
		_beginthread( NewsSystem, 0, NULL  );
	}

	this->LoadNewsFile(NEWS_FILE_PATH);
}
// ----------------------------------------------------------------------------------------------------------------------
void GCNews::LoadNewsFile(char *szFileName)
{
	char szTemp[1024] = {0};
		
		this->m_totalSection				= GetPrivateProfileInt(COMMON_HDR,"TotalSections",1,szFileName);
		this->m_uSleepTimeBfrStartNewsSys	= GetPrivateProfileInt(COMMON_HDR,"IdleTimeForStartNews",1,szFileName) * 60000 ;
		this->m_uSleepTimeSections			= GetPrivateProfileInt(COMMON_HDR,"IdleTimeSections",1,szFileName) * 60000 ;
		this->m_uSleepTimeEachNews			= GetPrivateProfileInt(COMMON_HDR,"IdleTimeEachNews",1,szFileName) * 1000 ;
		this->NewsDebug						= GetPrivateProfileInt(COMMON_HDR,"NewsDebug",0,szFileName);
		// -----
		if ( this->m_totalSection > 33 || this->m_totalSection == 0 || this->m_totalSection < 0 )
		{
			MessageBox(NULL,"Check Total Sections. MIN/MAX Sections are 0/33.","Error in News System", MB_OK);
			ExitProcess(0);
		}
		// -----
		if ( this->m_uSleepTimeSections < 1 || this->m_uSleepTimeEachNews < 1 )
		{
			MessageBox(NULL,"Check Idle Times","Error in News System", MB_OK);
			ExitProcess(0);
		}
		// -----
	for(int i = 0 ; i != this->m_totalSection ; i++)
		{
		sprintf(szTemp,"News%d",i);
		GetPrivateProfileString(szTemp,"NEWS1","NULL",this->m_section[i].NEWS0,MAX_NEWS_LEN,szFileName);
		GetPrivateProfileString(szTemp,"NEWS2","NULL",this->m_section[i].NEWS1,MAX_NEWS_LEN,szFileName);
		GetPrivateProfileString(szTemp,"NEWS3","NULL",this->m_section[i].NEWS2,MAX_NEWS_LEN,szFileName);
		}
}
// ----------------------------------------------------------------------------------------------------------------------
void GCNews::PrintNews()
{
	Sleep(this->m_uSleepTimeBfrStartNewsSys);

	CLog.LogAddC(TColor.Blue(),"[NewsSystem] is Started!");

	for(int i = 0 ; i != this->m_totalSection ; i++)
	{
		if(NewsDebug == 1)
		{
			CLog.LogAddC(TColor.Blue(),this->m_section[i].NEWS0);
		}
		SendMsg.MessageOutAll(0x00, this->m_section[i].NEWS0);
		//---
		Sleep(this->m_uSleepTimeEachNews);
		//--------------------------------
		if(NewsDebug == 1)
		{
			CLog.LogAddC(TColor.Blue(),this->m_section[i].NEWS1);
		}
		SendMsg.MessageOutAll(0x00, this->m_section[i].NEWS1);
		//---
		Sleep(this->m_uSleepTimeEachNews);
		//--------------------------------
		if(NewsDebug == 1)
		{
			CLog.LogAddC(TColor.Blue(),this->m_section[i].NEWS2);
		}
		SendMsg.MessageOutAll(0x00, this->m_section[i].NEWS2);
		//---
		Sleep(this->m_uSleepTimeSections);
		//--------------------------------
	}
}
// ----------------------------------------------------------------------------------------------------------------------
void NewsSystem(void * lpParam)
{
	while(true)
	{
		NewsC.PrintNews();
		Sleep(1);
	}
}
// ----------------------------------------------------------------------------------------------------------------------