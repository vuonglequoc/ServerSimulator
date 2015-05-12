//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Kanturu Battle StandBy Structure & Functions			//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef KANTURU_BATTLE_STANBY_H
#define KANTURU_BATTLE_STANBY_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "KanturuStateInfo.h"


#define KANTURU_STANBY_STATE_INFO	4

// States
#define KANTURU_STANBY_NONE		0
#define KANTURU_STANBY_START	1
#define KANTURU_STANBY_NOTIFY	2
#define KANTURU_STANBY_END		3

class CKanturuBattleStanby
{
public:
	CKanturuBattleStanby();
	virtual ~CKanturuBattleStanby();
	void Init();
	int LoadData(const char * lpszFileName);
	void ResetAllData();
	void Run();
	void SetState(int iBattleStanbyState);
	void SetNextState(int iCurrentState);
	void SetState_NONE();
	void SetState_START();
	void SetState_NOTIFY();
	void SetState_END();
	void ProcState_NONE();
	void ProcState_START();
	void ProcState_NOTIFY();
	void ProcState_END();
	void SetBattleStanbyState(int iBattleStanbyState){this->m_iBattleStanbyState = iBattleStanbyState;}
	int GetBattleStanbyState(){return this->m_iBattleStanbyState;}
	void CheckStateTime();
	int GetRemainTime();
private:

	int m_iBattleStanbyState;
	BOOL m_bIsSucccess;
	BOOL m_bFileDataLoad;
	CKanturuStateInfo m_StateInfo[KANTURU_STANBY_STATE_INFO];
	int m_StateInfoCount;
};


#endif
