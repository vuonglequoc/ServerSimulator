//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Accept IP Structures & Functions						//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef ACCEPTIP_H
#define ACCEPTIP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MAX_USE_IP	100

class CAcceptIp
{
public:
	CAcceptIp();
	virtual ~CAcceptIp();
	void Init();
	BOOL IsIp(LPSTR ip);
	void Load(const char* filename);

private:
	bool UseIp[MAX_USE_IP];
	char szIp[MAX_USE_IP][16];
};
extern CAcceptIp acceptIP;


#endif