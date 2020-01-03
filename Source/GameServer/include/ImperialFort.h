//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Imperial Fort Event
// # Developed By:	M.E.S & WolF
//---------------------------------------------------------------------------

#ifndef _IMPERIAL_FORT_H_
#define _IMPERIAL_FORT_H_

#include "User.h"

#define SCRAP_PAPER_COUNT	5

#define SCRAP_PAPER_RANGE(x) ( ((x) <0)?FALSE:((x) > SCRAP_PAPER_COUNT-1)?FALSE:TRUE)

void ImperialFort_StartStandBy(void * lpParam);
void ImperialFort_StandBy1(void * lpParam);
void ImperialFort_StandBy2(void * lpParam);
void ImperialFort_StandBy3(void * lpParam);
void ImperialFort_StandBy4(void * lpParam);
void ImperialFort_StandBy5(void * lpParam);
void ImperialFort_StartStage1(void * lpParam);
void ImperialFort_StartStage2(void * lpParam);
void ImperialFort_StartStage3(void * lpParam);
void ImperialFort_StartStage4(void * lpParam);
void ImperialFort_StartStage5(void * lpParam);
void ImperialFort_StartStage6(void * lpParam);
void ImperialFort_StartStage7(void * lpParam);
void ImperialFort_Winner(void * lpParam);

class CImperialFort
{
public:
	CImperialFort(void);
	~CImperialFort(void);
	void Init();
	void Run();
	void ReadFile();
	void DropItem(LPOBJ lpObj,unsigned char X, unsigned char Y, unsigned char Type);
	void ProtocolManager(unsigned char * aRecv,int aIndex);
	void CheckCanEnter(int aIndex);
	void MapTeleport(LPOBJ lpObj);
	bool DropCheck(LPOBJ lpObj);
	void SetMonstersByLevel();
	void InitTimer();
	void Clear();
	void ClearMonsters();
	void GiveUserDamage(LPOBJ lpObj);
	void MonsterKilledManager(LPOBJ lpObj,int MaxHitUser);
	void SetMonsters(int Stage);
	void SetGates();
	void SendFailure();
	bool MoveToGate(int aIndex);
	char GetIndexType(int MapNumber);
	void ReleseGate(char GateNumber,char Map);
	void BlockGate(char GateNumber,char Map);
	void SendBlockInfo(char GateNumber,char Map, int aIndex);
	// ----
	void CanMoveNextStage(LPOBJ lpObj);
	int CheckScrapPaperPos(LPOBJ lpObj);
	// ----
	char Enabled;
	short MinLevel;
	short MaxLevel;
	bool Running;
	char StandByRun;
	char CanUseSkill;
	char CanKillGate;
	int EventMaxLevel;
	char EventMap;
	char EventDay;
	char EventDayOfWeek;
	unsigned char EventX;
	unsigned char EventY;
	char EventState;
	char EventStage;
	char UserCount;
	short UserIndex[5];
	int StageTimer[10];
	short EventGateOpen;
	short EventGateUserOpen[5];
	short MonstersAddIndex[255];
	short	MonstersAddCount;
	short GatesAddIndex[255];
	short GatesAddNum[255];
	short GatesAddCount;
	// ----
	short MonsterLevel[17];
	int MonsterHP[17];
	int MonsterMP[17];
	int MonsterMinDmg[17];
	int MonsterMaxDmg[17];
	int MonsterDefense[17];

	// ---- NEW ADDED TEST
	char NormalBossNum;
	short NormalMonsterPos[4][255];
	short NormalMonsterCount[4];
	short GatesPos[4][20];
	short GatesCount[4];
	short BossMonsterPos[4][2];
	short FinalBossPos;
	// ----
	unsigned char GateStartX[10][4];
	unsigned char GateEndX[10][4];
	unsigned char GateStartY[10][4];
	unsigned char GateEndY[10][4];
	// ----
	unsigned char TrapStartX[10][4];
	unsigned char TrapEndX[10][4];
	unsigned char TrapStartY[10][4];
	unsigned char TrapEndY[10][4];

	// ---- Reward File
	char ItemType[8][40];
	short ItemIndex[8][40];
	char ItemLevel[8][40];
	unsigned char ItemDur[8][40];
	char ItemSkill[8][40];
	char ItemLuck[8][40];
	char ItemOpt[8][40];
	char ItemExe[8][40];
	char ItemAnc[8][40];
	unsigned char ItemSocket[5][8][40];
	int ItemCount[8];
};

extern CImperialFort gImperialFort;

struct PMSG_FORT_TIMER
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

//0xC1, 0x06, 0xBF, 0x09, 0x01, 0x02
//Result : 2 - Open, 3 - close
struct PMSG_FORT_OPENCLOSE_GATE
{
	BYTE c;
	BYTE size;
	BYTE headcode;
	BYTE subcode;
	BYTE Status;
	BYTE Result;
};

struct PMSG_FORT_END_MESSAGE
{
	BYTE c;
	BYTE size;
	BYTE headcode;
	BYTE subcode;
	BYTE Type;
	BYTE Unk1;
	BYTE Unk2;
	BYTE Unk3;
	unsigned long Experience;
	BYTE Unk4;
};

#endif