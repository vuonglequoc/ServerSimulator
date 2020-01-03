//----------------------------------------------------------------------------------
// # Project:	Zodea eMU GameServer
// # Company:	RealCoderZ Software Incorpration. © 2012 http://www.realcoderz.com
// # Author:	WolF & M.E.S & EmaLeto
// # Hint:		This Script For Join Server Protocol Class.
//----------------------------------------------------------------------------------
#ifndef JS_PROTOCOL_H
#define JS_PROTOCOL_H

#include "StdAfx.H"
#include "User.H"


class GJSProtocol
{
public:
	GJSProtocol();
	virtual ~GJSProtocol();
	// ------
	void	JServerLoginInfo();
	void	JoinServerLogin		( JOIN_SERVER_RESULT * lpMsg );
	void	AccountRequest		( JOIN_SERVER_ACC_REQ_RESULT * lpMsg );
	void	UserClose			( LPOBJ lpObj );
	void	AccountFail			( short aIndex, char * szAccountID, int DBNumber, int UserNumber );
	// ------
	friend void JSProtocolCore	( BYTE HeadCode, LPBYTE aRecv, int iSize );
};
extern GJSProtocol JSProto;
// ---------------------------------------------------------------------------------------------------------------------------------------------------
#endif
// ---------------------------------------------------------------------------------------------------------------------------------------------------