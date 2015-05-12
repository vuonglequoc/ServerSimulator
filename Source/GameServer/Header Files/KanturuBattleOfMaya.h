//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Kanturu Battle Of Maya Structure & Functions			//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef KANTURU_BATTLE_OF_MAYA_H
#define KANTURU_BATTLE_OF_MAYA_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KanturuStateInfo.h"
#include "KanturuMaya.h"


#define KANTURU_MAYA_AI_CHANGE_TIME	4
#define KANTURU_MAYA_TIME_INFO		3
#define KANTURU_MAYA_STATE_INFO		19
#define KANTURU_MAYA_GROUP_NUMBER	3

//-- States
#define KANTURU_MAYA_NONE			0
#define KANTURU_MAYA_STANBY1		1
#define KANTURU_MAYA_NOTIFY1		2
#define KANTURU_MAYA_START1			3
#define KANTURU_MAYA1				4
#define KANTURU_MAYA_END1			5
#define KANTURU_MAYA_ENDCYCLE1		6
#define KANTURU_MAYA_STANBY2		7
#define KANTURU_MAYA_START2			8
#define KANTURU_MAYA2				9
#define KANTURU_MAYA_END2			10
#define KANTURU_MAYA_ENDCYCLE2		11
#define KANTURU_MAYA_STANBY3		12
#define KANTURU_MAYA_START3			13
#define KANTURU_MAYA3				14
#define KANTURU_MAYA_END3			15
#define KANTURU_MAYA_ENDCYCLE3		16
#define KANTURU_MAYA_END			17
#define KANTURU_MAYA_ENDCYCLE		18

class CKanturuBattleOfMaya
{
public:
	CKanturuBattleOfMaya();
	virtual ~CKanturuBattleOfMaya();
	void ResetAllData();
	BOOL LoadData(const char * lpszFileName);
	void Run();
	void SetState(int iBattleOfMayaState);
	void SetNextState(int iCurrentState);
	void SetState_NONE();
	void SetState_STANBY1();
	void SetState_NOTIFY1();
	void SetState_START1();
	void SetState_MAYA1();
	void SetState_END_MAYA1();
	void SetState_ENDCYCLE_MAYA1();
	void SetState_STANBY2();
	void SetState_START2();
	void SetState_MAYA2();
	void SetState_END_MAYA2();
	void SetState_ENDCYCLE_MAYA2();
	void SetState_STANBY3();
	void SetState_START3();
	void SetState_MAYA3();
	void SetState_END_MAYA3();
	void SetState_ENDCYCLE_MAYA3();
	void SetState_END();
	void SetState_ENDCYCLE();
	void ProcState_NONE();
	void ProcState_STANBY1();
	void ProcState_NOTIFY1();
	void ProcState_START1();
	void ProcState_MAYA1();
	void ProcState_END_MAYA1();
	void ProcState_ENDCYCLE_MAYA1();
	void ProcState_STANBY2();
	void ProcState_START2();
	void ProcState_MAYA2();
	void ProcState_END_MAYA2();
	void ProcState_ENDCYCLE_MAYA2();
	void ProcState_STANBY3();
	void ProcState_START3();
	void ProcState_MAYA3();
	void ProcState_END_MAYA3();
	void ProcState_ENDCYCLE_MAYA3();
	void ProcState_END();
	void ProcState_ENDCYCLE();
	void SetBattleOfMayaState(int iState){this->m_iBattleOfMayaState = iState;}
	int GetBattleOfMayaState(){return this->m_iBattleOfMayaState;}
	
	void SetBattleSceneState(int iScene)
	{
		this->m_iSceneState = iScene;
		this->m_bIsSceneSuccess = FALSE;
	}

	int GetBattleSceneState(){return this->m_iSceneState;}
	void SetSuccessValue(BOOL bIsSuccess){this->m_bIsSucccess = bIsSuccess;}
	BOOL GetSuccessValue(){return this->m_bIsSucccess;}
	void SetSceneSuccessValue(BOOL bIsSuccess){this->m_bIsSceneSuccess = bIsSuccess;}
	BOOL GetSceneSuccessValue(){return this->m_bIsSceneSuccess;}
	void SetEntrancePermit(BOOL bEntrancePermit){this->m_bEntrancePermit = bEntrancePermit;}
	BOOL GetEntrancePermit(){return this->m_bEntrancePermit;}
	int CheckStateTime();
	int GetRemainTime();
	void CheckMayaBattleUser();
	void SetAIMonsterGroup(int iGroupNumber);
	void ResetAIMonster();
	void ChangeAI(int iGroupNumber);
private:

	int m_bFileDataLoad;
	int m_iBattleOfMayaState;
	int m_iSceneState;
	CKanturuStateInfo m_BattleTimeInfo[KANTURU_MAYA_TIME_INFO];
	int m_BattleTimeInfoCount;
	CKanturuStateInfo m_StateInfo[KANTURU_MAYA_STATE_INFO];
	int m_StateInfoCount;
	BOOL m_bEntrancePermit;
	BOOL m_bIsSucccess;
	BOOL m_bIsSceneSuccess;
	CKanturuMaya m_KanturuMaya;
	int m_iMayaHandDieTimeCounter;
	int m_iMayaLeftHandObjIndex;
	int m_iMayaRightHandObjIndex;
	int m_iMayaHandAIChangeTime[KANTURU_MAYA_AI_CHANGE_TIME];
	int m_iMayaHandAIChangeTimeCount;
	int m_iMayaHandAIAppliedTime;
	int m_iMayaHandCurrentAI;
	int m_iMayaHandGroupNumber[KANTURU_MAYA_GROUP_NUMBER];
	int m_iMayaHAndGroupNumberCount;
	int m_iMayaIceStormUsingRate;
};


#endif