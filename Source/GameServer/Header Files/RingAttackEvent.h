#ifndef RINGATTACKEVENT_H
#define RINGATTACKEVENT_H

#include "User.h"

#define MAX_RINGMONSTER_MAP	3

void RingAttack_EventStart(void * lpParam);

class CRingAttackEvent
{
public:
	CRingAttackEvent();
	~CRingAttackEvent();
	// ----
	void Init();
	void Start();
	void End();
	void AddMonsters();
	void DeleteMonsters();
	bool DropManager(LPOBJ lpObj);
	bool CanMove(LPOBJ lpObj, BYTE x, BYTE y);
	// ----
	int Enabled;
	int DurationTime;
	int AuxDurationTime;
	int Started;
	int BossMapNumber[3];
	int BossMapX[3];
	int BossMapY[3];
	int MonsterIndex[200];
	int MonsterCount;
};
extern CRingAttackEvent gRingAttackEvent;

#endif