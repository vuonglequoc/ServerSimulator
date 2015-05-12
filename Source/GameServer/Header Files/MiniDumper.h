//----------------------------------------------------------------------------------
// # Project:	Zodea eMU GameServer
// # Company:	RealCoderZ Software Incorpration. © 2012 http://www.realcoderz.com
// # Author:	WolF & M.E.S & Kristians
// # Hint:		This Script For Mini Dump Class.
//----------------------------------------------------------------------------------
#ifndef MINI_DUMPER_H
#define MINI_DUMPER_H

#include "StdAfx.H"
#include <DbgHelp.h>


#define EXCEPTION_INFO		_MINIDUMP_EXCEPTION_INFORMATION
// ------------------------------------------------------------------------------------------------------------------------------------------------------
typedef BOOL (*MINIDUMPWRITEDUMP)
(
	IN HANDLE			hProcess,
	IN DWORD			ProcessId,
	IN HANDLE			hFile,
	IN MINIDUMP_TYPE	DumpType,
	IN CONST			PMINIDUMP_EXCEPTION_INFORMATION	ExceptionParam, OPTIONAL
	IN CONST			PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, OPTIONAL
	IN CONST			PMINIDUMP_CALLBACK_INFORMATION CallbackParam OPTIONAL
);
// ------------------------------------------------------------------------------------------------------------------------------------------------------
class CMiniDump
{
public:
	CMiniDump();
	virtual ~CMiniDump();
	// -----
	void SetExceptionHook();
	// -----
private:
	static LONG TopLevelFilter(struct _EXCEPTION_POINTERS * pExceptionInfo);
};
// ------------------------------------------------------------------------------------------------------------------------------------------------------
extern CMiniDump gMiniDump;
// ------------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* MINI_DUMPER_H */