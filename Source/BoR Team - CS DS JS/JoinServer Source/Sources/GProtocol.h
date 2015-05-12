//-------------------------------------------------------------------------------------------------------------------------------------------
#ifndef GJS_PROTOCOLH
#define GJS_PROTOCOLH
//-------------------------------------------------------------------------------------------------------------------------------------------

#include "..\..\Protocol\JoinProtocol.h"
//-------------------------------------------------------------------------------------------------------------------------------------------

void JSProtocolCore			(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize);
// ----
void JoinServerLogin		(int aIndex, SDHP_SERVERINFO * lpMsg);
void JGPAccountRequest		(int aIndex, SDHP_IDPASS * lpMsg);
void GJPAccountFail			(int aIndex, SDHP_JOINFAIL * lpMsg);
void GJPAccountBlock		(int aIndex, SDHP_COMMAND_BLOCK * lpMsg);
void GJPUserClose			(int aIndex, SDHP_USERCLOSE_ID * lpMsg);
void GJReqMapSvrMove		(int aIndex, PMSG_REQ_MAPSVRMOVE * lpMsg);
void GJReqMapSvrAuth		(int aIndex, PMSG_REQ_MAPSVRAUTH * lpMsg);
void GJNotifyMaxUserCount	(int aIndex, PMSG_NOTIFY_MAXUSER * lpMsg);
void GJAllNoticeSend		(int aIndex, PMSG_SETENTIRENOTICE * lpMsg);
void GJUserNoticeSend		(int aIndex, PMSG_USERNOTICE * lpMsg);

//-------------------------------------------------------------------------------------------------------------------------------------------
#endif /* GJS_PROTOCOLH */
//-------------------------------------------------------------------------------------------------------------------------------------------