#include "TMiniDumper.h"
//-----------------------------------------------------------------------------------------------------------

LPCSTR CMiniDumper::m_szAppName;
//-----------------------------------------------------------------------------------------------------------

CMiniDumper::CMiniDumper(LPCSTR szAppName)
{
	assert(m_szAppName == NULL);
	// ----
	if(strlen(szAppName) > 0)
	{
		m_szAppName = strdup(szAppName);
	}
	// ----
	SetUnhandledExceptionFilter(TopLevelFilter);
}
//-----------------------------------------------------------------------------------------------------------

LONG CMiniDumper::TopLevelFilter(struct _EXCEPTION_POINTERS *pExceptionInfo)
{
	LONG					lReturn						= EXCEPTION_CONTINUE_SEARCH;
	HANDLE					hFile						= INVALID_HANDLE_VALUE;
	BOOL					bOK							= FALSE;
	// ----
	HWND					hParent						= NULL;
	HMODULE					hDll						= NULL;
	LPCTSTR					szResult					= NULL;
	MINIDUMPWRITEDUMP		pDump						= NULL;
	char *					pSlash						= NULL;
	// ----
	char					szDbgHelpPath[_MAX_PATH]	= {0};
	char					ErrMsg[]					= {0};
	EXCEPTION_INFORMATION	ExInfo						= {0};
	SYSTEMTIME				lpTime						= {0};
	char					szDumpPath[_MAX_PATH]		= {0};
	char					szScratch [_MAX_PATH]		= {0};
	// ----
	if(GetModuleFileNameA(NULL, szDbgHelpPath, _MAX_PATH) != 0)
	{
		pSlash = strrchr(szDbgHelpPath, '\\');
		// ----
		if(pSlash != NULL)
		{
			strcpy(pSlash+1, "DBGHELP.DLL");
			// ----
			hDll = LoadLibrary(szDbgHelpPath);
		}
	}
	else
	{
		sprintf(ErrMsg, "GetModuleFileNameA() :: Failed with error code [%d]", GetLastError());
		// ----
		MessageBox(0, ErrMsg, "[CMiniDumper][TopLevelFilter] :: GetModuleFileNameA Failed!", MB_OK | MB_ICONERROR);
	}
	// ----
	if(hDll == NULL)
	{
		hDll = LoadLibrary("DBGHELP.DLL");
	}
	// ----
	if(hDll != NULL)
	{
		pDump = (MINIDUMPWRITEDUMP)GetProcAddress(hDll, "MiniDumpWriteDump");
		// ----
		if(pDump != NULL)
		{
			GetLocalTime(& lpTime);
			// ----
			sprintf(szDumpPath, "./[%d-%d-%d][%d_%d][%s].dmp", lpTime.wDay, lpTime.wMonth,
				lpTime.wYear, lpTime.wHour, lpTime.wMinute, m_szAppName);
			// ----
			hFile = CreateFile(szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
			// ----
			if(hFile != INVALID_HANDLE_VALUE)
			{
				ExInfo.ThreadId				= GetCurrentThreadId();
				ExInfo.ExceptionPointers	= pExceptionInfo;
				ExInfo.ClientPointers		= NULL;
				// ----
				// # write the dump
				// ----
				bOK = pDump(GetCurrentProcess(), GetCurrentProcessId(), hFile,
					MiniDumpNormal, &ExInfo, NULL, NULL);
				// ----
				if(bOK == FALSE)
				{
					sprintf(szScratch, "pDump() :: Failed to save dump file to [%s] with error code [%d]", szDumpPath, GetLastError());
					// ----
					szResult = szScratch;
				}
				// ----
				CloseHandle(hFile);
			}
			else
			{
				sprintf(szScratch, "CreateFile() :: Failed to create dump file [%s] with error code [%d]", szDumpPath, GetLastError());
				// ----
				szResult = szScratch;
			}
		}
		else
		{
			szResult = "DBGHELP.DLL too old";
		}
	}
	else
	{
		szResult = "DBGHELP.DLL not found";
	}
	// ----
	if(szResult != NULL)
	{
		MessageBox(NULL, szResult, "[CMiniDumper][TopLevelFilter] Failed!", MB_OK | MB_ICONERROR);
	}
	// ----
	return lReturn;
}
//-----------------------------------------------------------------------------------------------------------