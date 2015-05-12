//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: Game Map Server Manager									//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef MAPSERVERMANAGER_H
#define MAPSERVERMANAGER_H

#include "MapClass.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#define MAX_MAP_GROUPS 20
#define MAX_MAP_SUBGROUPS 20
#define MAX_SVR_PER_GROUP 400


struct _MAPSVR_DATA
{
public:
	_MAPSVR_DATA();
	virtual ~_MAPSVR_DATA();
	void Clear(int iInitSetVal);
public:
	BOOL m_bIN_USE;
	BYTE m_btMAPSVR_GROUP;
	short m_sSVR_CODE;
	char m_cIPADDR[16];
	WORD m_wPORT;
	short m_sMAP_MOVE[MAX_NUMBER_MAP];
};

class CMapServerManager
{
public:
	CMapServerManager();
	virtual ~CMapServerManager();
	void Clear();
	BOOL LoadData(const char* lpszFileName);
	BOOL CheckMapCanMove(int iMAP_NUM);
	short CheckMoveMapSvr(int iIndex, int iMAP_NUM, short sSVR_CODE_BEFORE);
	BOOL GetSvrCodeData(WORD wServerCode, char* lpszIpAddress, unsigned short* lpwPort);
	int GetMapSvrGroup()
	{
		if ( this->m_lpThisMapSvrData != NULL )
		{
			return m_lpThisMapSvrData->m_btMAPSVR_GROUP;
		}
		return -1;
	}
private:
	BOOL m_bMapDataLoadOk;
	_MAPSVR_DATA m_MAPSVR_DATA[MAX_MAP_GROUPS][MAX_MAP_SUBGROUPS];
	int m_iMAPSVR_GROUP_COUNT[MAX_MAP_GROUPS];
	std::map<int,_MAPSVR_DATA *> m_mapSVRCODE_MAP;
	CRITICAL_SECTION m_critSVRCODE_MAP;
	_MAPSVR_DATA* m_lpThisMapSvrData;
};
extern CMapServerManager g_MapServerManager;

#endif