//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU gObj Monsters Structures & Functions					//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef GOBJMONSTER_H
#define GOBJMONSTER_H

#include "LogProc.H"
#include "MapClass.h"
#include "user.h"

#define MAX_QUEST_TELEPORT 4
#define RING_OF_TRANSFORM_BASE_RATE 10000
#define FIRE_CRACKER_BASE_RATE 10000

struct QuestNPCTeleportPos 
{
	int mapnum;
	int x;
	int y;
	int dir;
};

class CQeustNpcTeleport
{
public:
	CQeustNpcTeleport()
	{
		this->LastMapNumber = -1;
		this->RefCount = 0;
		InitializeCriticalSection(&this->CsRef);
		this->TimeCount = 0;
		this->m_QuestNPCTeleportPos[0].mapnum = MAP_INDEX_DEVIAS;
		this->m_QuestNPCTeleportPos[0].x = 198;
		this->m_QuestNPCTeleportPos[0].y = 47;
		this->m_QuestNPCTeleportPos[0].dir = 2;
		this->m_QuestNPCTeleportPos[1].mapnum = MAP_INDEX_LORENCIA;
		this->m_QuestNPCTeleportPos[1].x = 137;
		this->m_QuestNPCTeleportPos[1].y = 87;
		this->m_QuestNPCTeleportPos[1].dir = 1;
		this->m_QuestNPCTeleportPos[2].mapnum = MAP_INDEX_NORIA;
		this->m_QuestNPCTeleportPos[2].x = 169;
		this->m_QuestNPCTeleportPos[2].y = 89;
		this->m_QuestNPCTeleportPos[2].dir = 2;
		this->m_QuestNPCTeleportPos[3].mapnum = MAP_INDEX_ATLANS;
		this->m_QuestNPCTeleportPos[3].x = 17;
		this->m_QuestNPCTeleportPos[3].y = 0x19;
		this->m_QuestNPCTeleportPos[3].dir = 2;
	};

	~CQeustNpcTeleport()
	{
		DeleteCriticalSection(&this->CsRef);
	};

	void TalkRefAdd()
	{
		EnterCriticalSection(&this->CsRef);

		this->RefCount++;

		CLog.LogAdd("QeustNpc RefCount Inc = %d", this->RefCount);

		LeaveCriticalSection(&this->CsRef);
		
	};


	void TalkRefDel()
	{
		EnterCriticalSection(&this->CsRef);

		this->RefCount--;

		if ( this->RefCount < 0 )
		{
			this->RefCount = 0;
		}
		
		CLog.LogAdd("QeustNpc RefCount Dec= %d", this->RefCount);

		LeaveCriticalSection(&this->CsRef);
	};

	void Run(int aIndex);

private:

	int RefCount;
	CRITICAL_SECTION CsRef;
	int TimeCount;
	int LastMapNumber;
	QuestNPCTeleportPos m_QuestNPCTeleportPos[MAX_QUEST_TELEPORT];

};
extern CQeustNpcTeleport gQeustNpcTeleport;


void gObjMonsterMoveRegen(unsigned char x, unsigned char y, LPOBJ lpObj);
bool gObjMonsterRegen(LPOBJ lpObj);
int  gObjMonsterViewportIsCharacter(LPOBJ lpObj);
void gObjMonsterHitDamageInit(LPOBJ lpObj);
int  gObjMonsterHitDamageUserDel(LPOBJ lpObj);
void gObjMonsterSetHitDamage(LPOBJ lpObj, int hit_player, int hit_damage);
int  gObjMonsterTopHitDamageUser(LPOBJ lpMonObj);
int  gObjMonsterLastHitDamageUser(LPOBJ lpMonObj, int & hitindex);
bool gObjMonsterMoveCheck(LPOBJ lpObj, int tx, int ty);
bool gObjMonsterGetTargetPos(LPOBJ lpObj);
bool gObjGetTargetPos(LPOBJ lpObj, int sx, int sy, int & tx , int & ty);
int  gObjCallMonsterSetEnemy(LPOBJ lpObj, int Target);
int  gObjMonsterSearchEnemy(LPOBJ lpObj, BYTE objtype);
int  gObjGuardSearchEnemy(LPOBJ lpObj);
void gObjMonsterStateProc(LPOBJ lpObj, int aMsgCode, int aIndex, int aMsgSubCode);
void gObjMonsterProcess(LPOBJ lpObj);
void gObjMonsterMagicAttack(LPOBJ lpObj, int iMonsterClass);
void gObjUseMonsterSpecialAbillity(LPOBJ lpMonsterObj);
void gObjMonsterBeattackRecv(BYTE * lpRecv, int aIndex);
void gObjMonsterAttack(LPOBJ lpObj, LPOBJ lpTargetObj);
BOOL PathFindMoveMsgSend(LPOBJ lpObj);
void gObjMonsterMoveAction(LPOBJ lpObj);
void gObjMonsterBaseAct(LPOBJ lpObj);
void gObjTrapAttackEnemySearchX(LPOBJ lpObj, int count);
void gObjTrapAttackEnemySearchY(LPOBJ lpObj, int count);
void gObjTrapAttackEnemySearch(LPOBJ lpObj);
void gObjTrapAttackEnemySearchRange(LPOBJ lpObj,int iRange);
void gObjMonsterTrapAct(LPOBJ lpObj);
BYTE NewOptionRand(int level);
bool IsCanNotItemDtopInDevilSquare(int ItemType);
void gObjRefillMonsterHP(LPOBJ lpMonsterObj, int iRefillHPSec);
void gObjMonsterDieGiveItem(LPOBJ lpObj, LPOBJ lpTargetObj);
bool gEventMonsterItemDrop(LPOBJ lpObj, LPOBJ lpTargetObj);


#endif