//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU SendHackLog Functions & Structures					//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef SENDHACKLOG_H
#define SENDHACKLOG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WzUdp.h"
#include "protocol.h"



class CSendHackLog 
{

public:

	CSendHackLog();
	virtual ~CSendHackLog();

	void SendSet(char* ip, int port);
	void Send(int aIndex, BYTE type, LPSTR Context);
	BOOL SendData(LPBYTE SendData, DWORD nSendDataLen);
	

private:

	WzUdp m_UdpSocCHackLog;	// 4
	
};

extern CSendHackLog gSendHackLog;


#endif