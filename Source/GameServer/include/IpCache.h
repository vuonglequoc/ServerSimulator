//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU IP Cache Structures & Functions						//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef IPCACHE_H
#define IPCACHE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MAX_IP_CACHE	2000


struct IPINFO 
{
	char IpAddress[16];
	BOOL IpUsed;
	DWORD Time;
};

class CIpCache
{
public:
	void DelIp(char* ip);
	BOOL AddIp(char* ip);
	void Init();
	CIpCache();
	virtual ~CIpCache();

private:
	IPINFO m_IpInfo[MAX_IP_CACHE];
};
extern CIpCache IpCache;


#endif