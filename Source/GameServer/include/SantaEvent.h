//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Evil Santa Invasion Event
// # Developed By:	M.E.S & WolF
//---------------------------------------------------------------------------

#ifndef SANTAEVENT_H
#define SANTAEVENT_H

#include "User.h"

void SantaInvasion_EventStart(void * lpParam);

class CSantaEvent
{
public:
	CSantaEvent();
	~CSantaEvent();
	// ----
	void Init();
	void Start();
	void End();
	void AddMonsters();
	void DeleteMonsters();
	bool DropManager(LPOBJ lpObj);
	bool CanMove(LPOBJ lpObj, BYTE x, BYTE y);
	// ----
	// BUFFS
	// ----
	void LittleSantaBuffer(LPOBJ lpObj,int Type);
	void LittleSantaCancelBuffer(LPOBJ lpObj);
	// ----
	char Enabled;
	int DurationTime;
	int AuxDurationTime;
	char Started;
	unsigned char BossMapX;
	unsigned char BossMapY;
	int MonsterIndex[200];
	int MonsterCount;
};
extern CSantaEvent gSantaEvent;

#endif