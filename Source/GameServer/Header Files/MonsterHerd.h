//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU MonsterHerd Structures & Functions					//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef MONSTERHERD_H
#define MONSTERHERD_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"


struct _MONSTER_HERD_DATA 
{
	int m_iIndex;
	int m_iType;
	BYTE m_iX;
	BYTE m_iY;
	BOOL m_bRegen;

	_MONSTER_HERD_DATA()
	{
		this->m_iIndex = -1;
		this->m_iType = -1;
		this->m_iX = -1;
		this->m_iY = -1;
		this->m_bRegen = FALSE;
	}
};

class MonsterHerd
{
public:	
	MonsterHerd();
	virtual ~MonsterHerd();
  	virtual BOOL SetTotalInfo(int iMapNumber, int iRadius, int iStartX, int iStartY);
	virtual BOOL AddMonster(int iMonsterType, BOOL bRegen, BOOL bAttackFirst);
	virtual void SetRadius(int iRadius);
	virtual void SetPosition(BYTE iTX, BYTE iTY);
	virtual BOOL Start();
	virtual void Stop();
	virtual BOOL CheckInRadius(int iIndex);
	virtual BOOL GetCurrentLocation(BYTE&cX, BYTE& cY);
	virtual BOOL GetRandomLocation(BYTE& cX, BYTE& cY);
	virtual BOOL CheckLocation(BYTE& cX, BYTE& cY);
	virtual int MoveHerd(BYTE iTX, BYTE iTY);
	virtual int MonsterHerdItemDrop(LPOBJ lpObj);
	virtual _MONSTER_HERD_DATA* GetMonsterData(int iIndex);
	virtual void BeenAttacked(LPOBJ lpObj, LPOBJ lpTargetObj);
	virtual void OrderAttack(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackPercent);
	virtual void MonsterBaseAct(LPOBJ lpObj);
	virtual void MonsterMoveAction(LPOBJ lpObj);
	virtual void MonsterAttackAction(LPOBJ lpObj, LPOBJ lpTargetObj);
	virtual void MonsterDieAction(LPOBJ lpObj);
	virtual int MonsterRegenAction(LPOBJ lpObj);
public:
	int m_iHerdIndex;
	int m_iMapNumber;
	int m_iCUR_X;
	int m_iCUR_Y;
	int m_iRADIUS;
	BOOL m_bHasInfo;
	BOOL m_bHerdActive;
	std::map<int,_MONSTER_HERD_DATA> m_mapMonsterHerd;
	CRITICAL_SECTION m_critMonsterHerd;
};


#endif