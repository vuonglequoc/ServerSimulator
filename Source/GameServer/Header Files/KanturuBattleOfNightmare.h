//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Kanturu Battle Of NightMare Structure & Functions	//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef KANTURU_BATTLE_OF_NIGHTMARE_H
#define KANTURU_BATTLE_OF_NIGHTMARE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KanturuStateInfo.h"

#define KANTURU_NIGHTMARE_STATE_INFO	6

// States
#define KANTURU_NIGHTMARE_NONE		0
#define KANTURU_NIGHTMARE_IDLE		1
#define KANTURU_NIGHTMARE_NOTIFY	2
#define KANTURU_NIGHTMARE_START		3
#define KANTURU_NIGHTMARE_END		4
#define KANTURU_NIGHTMARE_ENDCYCLE	5

class CKanturuBattleOfNightmare
{
public:
	CKanturuBattleOfNightmare();
	virtual ~CKanturuBattleOfNightmare();
	int LoadData(const char * lpszFileName);
	void ResetAllData();
	void Run();
	void SetState(int iBattleOfNightmareState);
	void SetNextState(int iCurrentState);
	void SetState_NONE();
	void SetState_IDLE();
	void SetState_NOTIFY();
	void SetState_START();
	void SetState_END();
	void SetState_ENDCYCLE();
	void ProcState_NONE();
	void ProcState_IDLE();
	void ProcState_NOTIFY();
	void ProcState_START();
	void ProcState_END();
	void ProcState_ENDCYCLE();
	void SetBattleOfNightmareState(int iBattleOfNightmareState){this->m_iBattleOfNightmareState = iBattleOfNightmareState;}
	int GetBattleOfNightmareState(){return this->m_iBattleOfNightmareState;}
	void SetSuccessValue(BOOL bIsSuccess){this->m_bIsSucccess = bIsSuccess;}
	BOOL GetSuccessValue(){return this->m_bIsSucccess;}
	void CheckStateTime();
	int GetRemainTime();
	void CheckNightmareBattleUser();
private:

	int m_iBattleOfNightmareState;
	BOOL m_bIsSucccess;
	BOOL m_bFileDataLoad;
	CKanturuStateInfo m_StateInfo[KANTURU_NIGHTMARE_STATE_INFO];
	int m_StateInfoCount;
	int m_iNightmareAIGroupNumber;
	int m_iNightmareObjIndex;
};


#endif