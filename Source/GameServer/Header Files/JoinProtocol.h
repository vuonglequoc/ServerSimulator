//----------------------------------------------------------------------------------
// # Project:	Zodea eMU GameServer
// # Company:	RealCoderZ Software Incorpration. © 2012 http://www.realcoderz.com
// # Author:	WolF & M.E.S & EmaLeto
// # Hint:		This Script For Join Server Packet Structs.
//----------------------------------------------------------------------------------
#ifndef JOIN_PROTOCOL_H
#define JOIN_PROTOCOL_H

#include "StdAfx.H"


enum ACC_REQUEST_RESULT_TYPES
{
	ACC_REQ_WRONG_PASS				= 0x00,		// Password is Wrong. 
	ACC_REQ_SUCCESS					= 0x01,		// Account Login Succeeded.
	ACC_REQ_WRONG_ID				= 0x02,		// Account ID is Wrong.
	ACC_REQ_ALREADY_CONNECT			= 0x03,		// Account is Already Connected.
	ACC_REQ_MAX_USER				= 0x04,		// Max Users Reached in the Server.
	ACC_REQ_ACC_BANNED				= 0x05,		// Account is Banned.
	ACC_REQ_TEMP_BLOCK				= 0x06,		// Account is temporary.
	ACC_REQ_LOGIN_THREE_TIMES		= 0x08		// Login Closed Due to trying of 3 Times.
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
enum JOIN_SERVER_POROTOCOL_CORE
{
	JS_PROTO_CORE_SRV_LOGIN			= 0x00,
	JS_PROTO_CORE_ACC_REQUEST		= 0x01,
	JS_PROTO_CORE_JOIN_FAIL			= 0x02,
	JS_PROTO_CORE_ACC_CLOSE			= 0x03,
	JS_PROTO_CORE_ANTR_JOIN			= 0x04,
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct JOIN_SERVER_INFO
{
	PBMSG_HEAD		h;
	WORD			Port;
	TCHAR			ServerName[64];
	WORD			ServerCode;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct JOIN_SERVER_RESULT
{
	PBMSG_HEAD		h;
	BYTE			Result;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct JOIN_SERVER_ACC_REQ_RESULT
{
	PBMSG_HEAD		h;
	UCHAR			Result;
	short			aIndex;
	char			AccountID[10];
	int				UserNumber;
	int				DBNumber;
	char			JoominNumber[13];
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct JOIN_SERVER_JOIN_FAIL
{
	PBMSG_HEAD		h;
	short			aIndex;
	char			AccountID[11];
	int				UserNumber;
	int				DBNumber;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct JOIN_SERVER_USER_CLOSE
{
	PBMSG_HEAD		h;
	char			AccountID[10];
	char			PlayerName[10];
	USHORT			Level;
	UCHAR			DbClass;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* JOIN_PROTOCOL_H */
// -------------------------------------------------------------------------------------------------------------------------------------------------------