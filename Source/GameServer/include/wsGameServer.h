//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU wsGameServer Structures								//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef WSGAMESERVER_H
#define WSGAMESERVER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyWinsockBase.h"
#include "IpCache.h"


class CwsGameServer : public MyWinsockBase
{
public:
	BOOL SetSocketIndex(SOCKET socket, int aIndex);
	BOOL SetSocketBuffer(int index, SOCKET socket, LPSTR ip);
	BOOL AcceptSocket(SOCKET& clientSocket, in_addr& cInAddr);
	BOOL DataRecv(SOCKET socket);
	BOOL FDWRITE_MsgDataSend(SOCKET socket);
	BOOL DataSendW(int uindex, PCHAR buf, int len);
	BOOL DataSocketSend(SOCKET socket, PCHAR buf, int len);
	BOOL CreateServer(LPSTR ip_addr, WORD port, DWORD WinServerMsg, DWORD WinClientMsg);
	CwsGameServer();
	virtual ~CwsGameServer();

public:
	int m_SendSec;
	int m_SendSecTmp;
	int m_RecvSec;
	int m_RecvSecTmp;
	DWORD m_WinClientMsg;
};


#endif