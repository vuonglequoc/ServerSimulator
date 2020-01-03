// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#pragma warning ( disable : 4786 )

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define TRACE_PACKET 1	
#define CURRENT_DB_VERSION 3
#define MAX_TYPE_PLAYER 5
#define MAX_NUMBER_MAP 40
#define CHECK_LIMIT(value, limit) (((value)< 0)?FALSE:((value)> limit-1)?FALSE:TRUE)

// Windows Header Files:
#include <windows.h>
#include <winsock2.h>
#include <commctrl.h>
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>

#pragma comment ( lib , "comctl32.lib" )
#pragma comment ( lib , "WS2_32.lib" )

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <map>


// Local Header Files
#include "prodef.h"
#include "ServerEngine.h"
#include "eDataServer.h"
#include "Query.h"
#include "TServerInfoDisplayer.h"
#include "MapServerManager.h"

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
