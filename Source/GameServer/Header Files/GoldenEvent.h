#ifndef ELEDORADOEVENT_H
#define ELEDORADOEVENT_H

#include "User.h"

void GoldenInvasion_EventStart(void * lpParam);

class CGoldenEvent
{

public:

	CGoldenEvent();
	~CGoldenEvent();

	void Init();
	void Run();
	void Start();
	void End();
	void SendInvasionEffect();
	// ----
	void DeleteMonsters();
	void RegenGoldGoblen();
	void RegenGoldTitan();
	void RegenGoldSoldier();
	void RegenGoldDercon();
	void RegenGoldLizardKing();
	void RegenGoldVepar();
	void RegenGoldTantalo();
	void RegenGoldWheel();
	// ---- New Gold Mobs
	void RegenGoldRabbit();
	void RegenGoldKnight();
	void RegenGoldDevil();
	void RegenGoldStoneGolem();
	void RegenGoldCrust();
	void RegenGoldSatyros();
	void RegenGoldTwinTail();
	void RegenGoldNapin();
	void RegenGoldIronKnight();
	void RegenGoldGreatDragon();
	// ----
	bool CheckGoldenDrop(LPOBJ lpObj);
	
public:
	int Enabled;
	int Started;
	int DurationTime;
	int AuxDurationTime;
	// ---- MAP SETTING
	int m_BossGoldDragonMapNumber;
	int m_BossGoldTitanMapNumber;
	int m_BossGoldTitanMapX;
	int m_BossGoldTitanMapY;
	int m_BossGoldLizardKingMapNumber;
	int m_BossGoldLizardKingMapX;
	int m_BossGoldLizardKingMapY;
	int m_BossGoldTantalloMapNumber;
	int m_BossGoldTantalloMapX;
	int m_BossGoldTantalloMapY;

	int MonsterIndex[200];
	int MonsterCount;

	int m_BossGoldGreatDragonMapNumber[2];
};

extern CGoldenEvent gGoldenEvent;

#endif