/*#############################################################################
# Zodea MU Suite - AIO
#
# RealCoderZ Software Incorpration.	http://www.realcoderz.com
#
# Copyright © 2011 RealCoderZ Software Incorpration.
#
# Author: WolF & M.E.S
#
# [19.08.2011] This Script For JoinServer Protocol Class.
#
#############################################################################*/
#ifndef JOIN_PROTO_HEADER
#define JOIN_PROTO_HEADER

#include "JoinProtocol.H"


class JSProtocol
{
public:
	void JSProtocolCore			( int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize );
	// -----
	void JoinServerLogin		( int aIndex, JOIN_SERVER_INFO * lpMsg );
	void JGPAccountRequest		( int aIndex, JOIN_SERVER_ACC_REQUEST * lpMsg );
	void GJPAccountFail			( int aIndex, JOIN_SERVER_JOIN_FAIL * lpMsg );
	void GJPUserClose			( int aIndex, JOIN_SERVER_USER_CLOSE * lpMsg );
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
extern JSProtocol JSProto;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
#endif
// -------------------------------------------------------------------------------------------------------------------------------------------------------