#ifndef MINIDUMPER_H
#define MINIDUMPER_H
//-----------------------------------------------------------------------------------------------------------

#include <Windows.h>
#include <stdio.h>
#include <assert.h>
#include "dbghelp.h"
//-----------------------------------------------------------------------------------------------------------

#define EXCEPTION_INFORMATION		_MINIDUMP_EXCEPTION_INFORMATION
//-----------------------------------------------------------------------------------------------------------

typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
									CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
									CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
									CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);
//-----------------------------------------------------------------------------------------------------------

class CMiniDumper
{
private:
	static LPCSTR m_szAppName;
	static LONG WINAPI TopLevelFilter(struct _EXCEPTION_POINTERS *pExceptionInfo);
public:
	CMiniDumper(LPCSTR szAppName);
};
//-----------------------------------------------------------------------------------------------------------
#endif /* MINIDUMPER_H*/
//-----------------------------------------------------------------------------------------------------------