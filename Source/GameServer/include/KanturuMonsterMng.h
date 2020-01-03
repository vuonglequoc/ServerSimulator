//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Kanturu Monster Manager Structure & Functions		//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef KANTURU_MONSTER_MNG_H
#define KANTURU_MONSTER_MNG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "KanturuObjInfo.h"


#define MAX_KANTURU_MONSTER	200

struct KANTURU_MONSTER_SETBASE_INFO
{
	BYTE btGroup;
	WORD wType;
	BYTE btMapNumber;
	BYTE btDistance;
	BYTE btX;
	BYTE btY;
	BYTE btDir;
};

class CKanturuMonsterMng  
{
public:
	CKanturuMonsterMng();
	virtual ~CKanturuMonsterMng();
	void ResetLoadData();
	void ResetRegenMonsterObjData();
	void MonsterDie(int iIndex);
	BOOL LoadData(const char * lpszFileName);
	void SetMonsterSetBaseInfo(int iIndex, BYTE btGroup, WORD wType, BYTE btMapNumber, BYTE btDistance, BYTE btX, BYTE btY, BYTE btDir);
	int SetKanturuMonster(int iGroupNumber);
	int GetPosition(int TableNum, short MapNumber, short & x, short & y);
	int GetAliveMonsterCount(){return this->m_iAliveMonsterCount;}
	BOOL IsExistAliveMonster();
	int GetMayaObjIndex(){return this->m_iMayaObjIndex;}
private:

	int m_iMaxMonsterCount;
	KANTURU_MONSTER_SETBASE_INFO m_SetBaseInfo[MAX_KANTURU_MONSTER];
	int m_iMayaObjIndex;
	int m_bFileDataLoad;
	CKanturuObjInfo m_KanturuMonster;
	int m_iMaxRegenMonsterCount;
	int m_iAliveMonsterCount;
};
extern CKanturuMonsterMng g_KanturuMonsterMng;


#endif
