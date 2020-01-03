//----------------------------------------------------------------------------------
// # Project:	Zodea eMU GameServer
// # Company:	RealCoderZ Software Incorpration. © 2012 http://www.realcoderz.com
// # Author:	WolF & M.E.S & Kristians
// # Hint:		This Script For Mini Dump Class.
//----------------------------------------------------------------------------------
#include "stdafx.h"
#include "MiniDumper.H"
#include "GameMain.h"


CMiniDump gMiniDump;
// ------------------------------------------------------------------------------------------------------------------------------------------------------
CMiniDump::CMiniDump()
{
	return;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
CMiniDump::~CMiniDump()
{
	return;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
void CMiniDump::SetExceptionHook()
{
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)TopLevelFilter);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------------
LONG CMiniDump::TopLevelFilter(_EXCEPTION_POINTERS * pExceptionInfo)
{
	LONG				bReturn						= EXCEPTION_CONTINUE_SEARCH;
	HANDLE				hFile						= INVALID_HANDLE_VALUE;
	HMODULE				hDll						= NULL;
	MINIDUMPWRITEDUMP	pDump						= NULL;
	LPCTSTR				szResult					= NULL;
	EXCEPTION_INFO		ExInfo						= {0};
	char *				pSlash						= NULL;
	char				szDbgHelpPath[_MAX_PATH]	= {0};
	char				szDumpPath[_MAX_PATH]		= {0};
	char				szScratch [_MAX_PATH]		= {0};
	SYSTEMTIME			SysTime						= {0};
	// -----
	if(GetModuleFileName(NULL, szDbgHelpPath, _MAX_PATH) != 0)
	{
		pSlash = strrchr(szDbgHelpPath, '\\');
		// -----
		if(pSlash != NULL)
		{
			strcpy(pSlash+1, "DBGHELP.DLL");
			// -----
			hDll = LoadLibrary(szDbgHelpPath);
		}
	}
	else
	{
		MessageBox(NULL, "[MINI-DUMPER] [TopLevelFilter] :: GetModuleFileName Failed!", "Critical Error", MB_OK | MB_ICONERROR);
	}
	// -----
	if(hDll == NULL)
	{
		hDll = LoadLibrary("DBGHELP.DLL");
	}
	// -----
	if(hDll != NULL)
	{
		pDump = (MINIDUMPWRITEDUMP)GetProcAddress(hDll, "MiniDumpWriteDump");
		// -----
		if(pDump != NULL)
		{
			GetLocalTime(&SysTime);
			// -----
			sprintf(szDumpPath, "./[%s][%d_%d][%d-%d-%d].dmp", szServerName, SysTime.wHour, SysTime.wMinute, SysTime.wDay, SysTime.wMonth, SysTime.wYear);
			// -----
			hFile = CreateFile(szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
			// -----
			if(hFile != INVALID_HANDLE_VALUE)
			{
				ExInfo.ThreadId				= GetCurrentThreadId();
				ExInfo.ExceptionPointers	= pExceptionInfo;
				ExInfo.ClientPointers		= NULL;
				// -----
				BOOL bOK = pDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL);
				// -----
				if(bOK == FALSE)
				{
					sprintf(szScratch, "[MINI-DUMPER] Failed to save dump file to [%s] with error code [%d]", szDumpPath, GetLastError());
					// -----
					szResult = szScratch;
				}
				// -----
				CloseHandle(hFile);
			}
			else
			{
				sprintf(szScratch, "[MINI-DUMPER] Failed to create dump file [%s] with error code [%d]", szDumpPath, GetLastError());
				// -----
				szResult = szScratch;
			}
		}
		else
		{
			szResult = "DBGHELP.DLL too Old!";
		}
	}
	else
	{
		szResult = "DBGHELP.DLL not found";
	}
	// -----
	if(szResult != NULL)
	{
		MessageBox(NULL, szResult, "[MINI-DUMPER] [TopLevelFilter] Failed!", MB_OK | MB_ICONERROR);
	}
	// -----
	return bReturn;
}