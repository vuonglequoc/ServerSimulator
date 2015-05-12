//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Kanturu Battle StandBy Structure & Functions			//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef KANTURU_BATTLE_USERMNG_H
#define KANTURU_BATTLE_USERMNG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KanturuBattleUser.h"

#define MAX_KANTURU_BATTLE_USER	15


class CKanturuBattleUserMng
{
public:
	CKanturuBattleUserMng();
	virtual ~CKanturuBattleUserMng();
	void ResetAllData();
	int AddUserData(int iIndex);
	int DeleteUserData(int iIndex);
	void CheckUserState();
	int MoveAllUser(int iGateNumber);
	int GetUserCount();
	int IsEmpty();
	int IsOverMaxUser();
	void SetMaxUser(int iMaxUser);
	int GetMaxUser();
	void LogBattleWinnerUserInfo(BYTE btFlag, int iElapsedTime);
public:

	CKanturuBattleUser m_BattleUser[MAX_KANTURU_BATTLE_USER];
	int m_iBattleUserCount;
	int m_iBattleMaxUser;
};
extern CKanturuBattleUserMng g_KanturuBattleUserMng;


#endif