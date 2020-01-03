//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Kanturu Event Structures & Functions					//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef KANTURU_H
#define KANTURU_H

#include "MapClass.h"
#include "KanturuStateInfo.h"
#include "KanturuBattleStanby.h"
#include "KanturuBattleOfMaya.h"
#include "KanturuBattleOfNightmare.h"
#include "KanturuTowerOfRefinement.h"


#define MAX_KANTURU_STATE_INFO	6
#define MAX_KANTURU_MAP_LEVEL	2

#define KANTURU_MAP_CLOSE	0
#define KANTURU_MAP_OPEN	1

#define KANTURU_STATE_NONE					0
#define KANTURU_STATE_BATTLE_STANTBY		1
#define KANTURU_STATE_BATTLE_OF_MAYA		2
#define KANTURU_STATE_BATTLE_OF_NIGHTMARE	3
#define KANTURU_STATE_TOWER_OF_REFINEMENT	4
#define KANTURU_STATE_END					5



class CKanturu
{
public:

	CKanturu(void);
	~CKanturu(void);
	// ----
	int BlockKanturuMapEnter;
	// ----
	void ResetAllData();
	BOOL LoadData(const char * lpszFileName);
	void LoadKanturuMapAttr(const char * lpszFileName, BYTE btLevel);
	void SetKanturuMapAttr(BYTE btLevel);
	void Run();
	void SetState(int iKanturuState);
	void SetNextState(int iCurrentState);
	void ChangeState(int iState, int DetailState);
	void SetState_NONE();
	void SetState_BATTLE_STANDBY();
	void SetState_BATTLE_OF_MAYA();
	void SetState_BATTLE_OF_NIGHTMARE();
	void SetState_TOWER_OF_REFINEMENT();
	void SetState_END();
	void ProcState_NONE();
	void ProcState_BATTLE_STANDBY();
	void ProcState_BATTLE_OF_MAYA();
	void ProcState_BATTLE_OF_NIGHTMARE();
	void ProcState_TOWER_OF_REFINEMENT();
	void ProcState_END();
	void SetKanturuState(int iKanturuState){this->m_iKanturuState = iKanturuState;}
	int GetKanturuState(){return this->m_iKanturuState;}
	int GetKanturuDetailState();
	void CheckStateTime();
	int GetRemainTime();
	void CheckUserOnKanturuBossMap();
	int CheckEnterKanturu(int iUserIndex);
	BOOL CheckCanEnterKanturuBattle();
	void SetEnableCheckMoonStone(BOOL bEnable){this->m_bEnableCheckMoonStone = bEnable;}
	int GetEnableCheckMoonStone(){return this->m_bEnableCheckMoonStone;}
	BOOL CheckEqipmentMoonStone(int iUserIndex);
	void KanturuMonsterDieProc(int iMonIndex, int iKillerIndex);
	void OperateGmCommand(int iUserIndex, int iCommand);
	void UserMonsterCountCheck();
	void SetKanturuTimeAttackEventInfo();
	int GetKanturuBattleCounter(){return this->m_iKanturuBattleCounter;}
	int GetKanturuBattleDate(){return this->m_iKanturuBattleDate;}
	bool KanturuMonsterDropManager(int aIndex, int MonsterIndex);
private:

	int Enabled;
	int m_iKanturuState;
	BOOL m_bIsSucccess;
	BOOL m_bFileDataLoad;
	CKanturuStateInfo m_StateInfo[MAX_KANTURU_STATE_INFO];
	int m_StateInfoCount;
	CKanturuBattleStanby m_BattleStanby;
	CKanturuBattleOfMaya m_BattleOfMaya;
	CKanturuBattleOfNightmare m_BattleOfNightmare;
	CKanturuTowerOfRefinement m_TowerOfRefinement;
	MapClass m_KanturuMap[2];
	BOOL m_bEnableCheckMoonStone;
	int m_iKanturuBattleCounter;
	int m_iKanturuBattleDate;
};
extern CKanturu g_Kanturu;


#endif
