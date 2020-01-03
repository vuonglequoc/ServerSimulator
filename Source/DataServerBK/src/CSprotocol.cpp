// CSprotocol.cpp: implementation of the CCSprotocol class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CSprotocol.h"

#include "giocp.h"
#include "LogProc.h"
#include "winutil.h"


void CSProtocolCore(int aIndex, DWORD HeadCode, LPBYTE aRecv, int iSize)
{
	if (g_UseCashShopServer == TRUE)
	{
#if (TRACE_PACKET == 1 )
	LogAddHeadHex("CASHSHOP_SERVER", aRecv, iSize);
#endif
	
	g_ServerInfoDisplayer.CheckCSConnection(TRUE);
	/*switch ( HeadCode )
	{
	}*/
	g_ServerInfoDisplayer.CheckCSConnection(FALSE);
	}
}