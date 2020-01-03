
#if !defined(AFX_EDATASERVER_H__A7C14824_5DD7_49C9_BDFB_C8D813D08CB9__INCLUDED_)
#define AFX_EDATASERVER_H__A7C14824_5DD7_49C9_BDFB_C8D813D08CB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"



#define	WM_LOG_PAINT	(0x101)
#define WM_LOG_DATE_CHANGE	(0x102)



extern HWND ghWnd;
extern int g_dwMaxServerGroups;
extern WORD g_FSGateServerListPort;
extern WORD g_JoinServerListPort;
extern WORD g_ConnectServerListPort;
extern WORD g_DataServer1ListPort;
extern WORD g_DataServer2ListPort;
extern WORD g_ExDataServerListPort;
extern WORD g_ChatServerListPort;
extern WORD g_EventServerListPort;
extern WORD g_RankingServerListPort;
extern WORD g_CashShopServerListPort;
extern TCHAR g_MuOnlineDNS[64];
extern TCHAR g_MeMuOnlineDNS[64];
extern TCHAR g_EventServerDNS[64];
extern TCHAR g_RankingServerDNS[64];
extern TCHAR g_UserID[64];
extern TCHAR g_Password[64];
extern BOOL g_UseMD5;

extern BOOL g_UseCashShopServer;
extern BOOL g_UseDataServer1;
extern BOOL g_UseDataServer2;
extern BOOL g_UseJoinServer;

extern BOOL g_UseExDataServer;
extern BOOL g_UseEventServer;
extern BOOL g_UseRankingServer;


bool IniteDataServer();

struct JS_USER_DISCONNECT
{
	PBMSG_HEAD		h;
	char			AccountID[10];
	short			aIndex;
};

#endif // !defined(AFX_EDATASERVER_H__A7C14824_5DD7_49C9_BDFB_C8D813D08CB9__INCLUDED_)
