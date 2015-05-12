//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	News Class with Time Separated
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------
#ifndef NEWS_H
#define NEWS_H

#include "StdAfx.H"
//-------------------------------------------------------------------------------------------------------------------------------------------
#pragma once
//-------------------------------------------------------------------------------------------------------------------------------------------
#define MAX_NEWS_LEN				1024
#define MAX_NEWS_SEC				33
//-------------------------------------------------------------------------------------------------------------------------------------------
struct SECTION
{
	char NEWS0[MAX_NEWS_LEN];
	char NEWS1[MAX_NEWS_LEN];
	char NEWS2[MAX_NEWS_LEN];
};
//-------------------------------------------------------------------------------------------------------------------------------------------
class GCNews
{
public:
	GCNews();
	virtual ~GCNews();

	void			Init();
	void			LoadNewsFile(char *szFileName);
	void			PrintNews();
	void			CheckErrors();
private:
	char *			m_szFileName;
	int				Enabled;
	int				m_totalSection;
	int				m_uSleepTimeBfrStartNewsSys;
	int				m_uSleepTimeEachNews;
	int				m_uSleepTimeSections;
	int				NewsDebug;
	SECTION			m_section[MAX_NEWS_SEC];
};
//-------------------------------------------------------------------------------------------------------------------------------------------
extern GCNews NewsC;
//-------------------------------------------------------------------------------------------------------------------------------------------
void NewsSystem(void * lpParam);
//-------------------------------------------------------------------------------------------------------------------------------------------
#endif