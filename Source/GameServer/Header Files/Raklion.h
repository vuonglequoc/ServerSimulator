#ifndef _RAKLION_H_
#define _RAKLION_H_

#include "User.h"

void Raklion_EventStart(void * lpParam);
void Raklion_EndEvent(void * lpParam);

struct PMSG_RAKLIONTIMER2
{
	unsigned char c;
	unsigned char size;
	unsigned char headcode;
	unsigned char btPlayState;
	unsigned short wRemainSec;
	unsigned short wMaxKillMonster;
	unsigned short wCurKillMonster;
	unsigned short wUserHaveWeapon;
	unsigned char btWeaponNum;
};

struct PMSG_RAKLIONTIMER
{
	BYTE c;
	BYTE size;
	BYTE headcode;
	BYTE subcode;
	BYTE Status;
	BYTE Unk1;
	BYTE Unk2;
	BYTE Unk3;
	BYTE Unk4;
	BYTE TimerL;
	BYTE TimerH;
	BYTE Unk5;
	BYTE Mob;
};

class CRaklion
{
public:
	CRaklion(void);
	~CRaklion(void);
	// ----
	void Init();
	void ReadFile();
	void SelupanDrop(LPOBJ lpObj,unsigned char X,unsigned char Y);
	void Clear();
	void Run();
	void Start();
	void MoveToGate(int aIndex);
	void SetMonsters(int Stage);
	void MonsterKillManager(LPOBJ lpObj,int MaxHitUser);
	bool DropManager(LPOBJ lpObj);
	// ----
	int Enabled;
	// ----
	int SelupanMaxItem;
	int ItemCount;
	int ItemType[255];
	int ItemDur[255];
	int ItemLevel[255];
	int ItemIndex[255];
	int ItemLuck[255];
	int ItemSkill[255];
	int ItemOpt[255];
	int ItemAncient[255];
	int ItemExe[255];
	int ItemSocket[5][255];

	int MaxUsers;
	int Started;
	int MaxItemDrop;
	int StandByTime;
	int AuxStandByTime;
	int KillSpidersTime;
	int AuxKillSpidersTime;
	int KillEggsTime;
	int AuxKillEggsTime;
	int KillSelupanTime;
	int AuxKillSelupanTime;
	int Running;
	int StandBy;
	int UserIndex[50];
	int UserCount;
	int MonsterCount;
	int MonsterKilled;
	int SpidersPos[255];
	int SpidersCount;
	int EggsPos[255];
	int EggsType[255];
	int EggsCount;
	int SelupanPos;
	unsigned char Stage;
};

extern CRaklion gRaklion;

#endif