//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Double Goer Event
// # Developed By:	M.E.S & WolF
//---------------------------------------------------------------------------

#ifndef _DOUBLE_GOER_H_
#define _DOUBLE_GOER_H_

#if(!GS_CASTLE)

struct PMSG_DG_MAPATTR
{
	unsigned char c;
	unsigned char size;
	unsigned char headcode;
	unsigned char btType;
	unsigned char btMapAttr;
	unsigned char btMapSetType;
	unsigned char btCount;
};

struct PMSG_DG_SETATTR
{
	unsigned char btX;
	unsigned char btY;
};

struct PMSG_DG_STANDBYTIMER
{
	unsigned char c;
	unsigned char size;
	unsigned char headcode;
	unsigned char type;
};


#include "User.h"

void StandByStart(void * lpParam);
void EventRun(void * lpParam);
void EventCompleted(void * lpParam);

#define SIGN_MARK_COUNT	5

#define SIGN_MARK_RANGE(x) ( ((x) <0)?FALSE:((x) > SIGN_MARK_COUNT-1)?FALSE:TRUE)

class CDoubleGoer
{
public:
	CDoubleGoer(void);
	~CDoubleGoer(void);
	// ----
	void Init();
	void Run();
	void ReadFile();
	void InitTimer();
	void Clear();
	void CheckCanEnter(int aIndex);
	void EventEnd(int aIndex, BYTE State);
	void Teleport(int aIndex);
	void MapCheck();
	void SetMonsters();
	void DeleteMonsters();
	void DeleteChests(char ChestType, char Type);
	void OpenChest(int aIndex,LPOBJ lpNpc,BYTE Chest);
	void AddMonster(unsigned char Map,short MonsterType,unsigned char X,unsigned char Y,unsigned char Type);
	bool ChestCreate(LPOBJ lpObj);
	int CheckSignPos(LPOBJ lpObj);
	char GetEventIndex(unsigned char MapNumber);
	void ChestDrop(int aIndex, unsigned char Type, unsigned char Map, unsigned char X, unsigned char Y);
	void BlockDoubleGoerEntrance(char Map);
	void SendDoubleGoerEntranceBlock(char Map,int aIndex,bool Live);
	void ReleaseDoubleGoerEntrance(char Map);
	void MonsterMoveProccess(LPOBJ lpObj);
	// ----
	char Enabled;
	char Running;
	DWORD OriginalTimer;
	DWORD OriginalStandByTimer;
	DWORD Timer;
	DWORD StandByTimer;
	char StandBy;
	short MonsterCount;
	char EventMap;
	short EventGate;
	unsigned char EventY;
	unsigned char EventX;
	short EventMaxLevel;
	short MinLevel;
	short MaxLevel;
	char UserCount;
	short UserIndex[5];
	unsigned char UserPos[5];
	short MonstersAddIndex[255];
	short MonstersAddCount;
	unsigned char MonsterPos;
	short MonsterPosIndex;
	unsigned char BossMonsterPoss;
	short BossMonsterIndex[3];
	unsigned char BossStartX;
	unsigned char BossStartY;
	short BossMonsterNumber;
	char BossStage;
	short LarvaRate;
	char Success;
	char DistanceY;
	char CalcDistanceY;
	short MobNearY;
	bool PartyActived;
	short UserIndexMaxHit[3];

	// ---- NEW ADDED TEST
	short NormalMonsterPos[4][255];
	short NormalMonsterCount[4];
	short BossMonsterPos[4][3];
	short ChestIndex[3][3];
	short LarvaIndex[3][50];
	short LarvaCount[3];
	short GoldenChestIndex;

	// ---- Reward Info
	unsigned char Type[2][255];
	unsigned short Index[2][255];
	unsigned char Level[2][255];
	unsigned char Dur[2][255];
	unsigned char Skill[2][255];
	unsigned char Luck[2][255];
	unsigned char Opt[2][255];
	unsigned char Exe[2][255];
	unsigned char Anc[2][255];
	unsigned char Socket[5][2][255];
	short ItemCount[2];

	// ---- Monster Level Setting
	unsigned char EventLevel;
	unsigned char MobCfgIndex;
	unsigned short PlayerMinLvl[50];
	unsigned short PlayerMaxLvl[50];
	unsigned short MobID[50][20];
	unsigned char MobLvl[50][20];
	unsigned int MobHP[50][20];
	unsigned int MobMana[50][20];
	unsigned int MobMinDmg[50][20];
	unsigned int MobMaxDmg[50][20];
	unsigned int MobDef[50][20];
};

extern CDoubleGoer gDoubleGoer;

struct PMSG_DGOER_TIMER_INIT
{
	BYTE c;
	BYTE size;
	BYTE headcode;
	BYTE subcode;
	BYTE Timer;
};

struct PMSG_DGOER_MONSTER_COUNT
{
	BYTE c;
	BYTE size;
	BYTE headcode;
	BYTE subcode;
	BYTE Total;
	BYTE Killed;
};

struct PMSG_DGOER_TRIANG_POS
{
	BYTE c;
	BYTE size;
	BYTE headcode;
	BYTE subcode;
	BYTE Pos;
};

//Double Goer Set Red Triangle Position
//0xC1 0x06 0xBF 0x11 0x00 0x12
struct PMSG_DGOER_RTRIANG_POS // 0x11
{
	BYTE c;
	BYTE size;
	BYTE headcode;
	BYTE subcode;
	BYTE Unk;
	BYTE Pos;
};

//Double Goer End, Timer Stop ( 0x00 - Success, 0x01 - Fail)
//0xC1 0x05 0xBF 0x13 0xXX
struct PMSG_DGOER_END
{
	BYTE c;
	BYTE size;
	BYTE headcode;
	BYTE subcode;
	BYTE Status;
};

//Double Goer Update Bar(Triangles)
//0xC1, 0x1C, 0xBF, 0x12, 0x00, 0x00, 0x02, 0x00, 0x64, 0x2F, 0x01, 0x0E, 0x64, 0x2F, 0x02 , 0x08 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00
struct PMSG_DGOER_UPDATE_BAR
{
	BYTE c;
	BYTE size;
	BYTE headcode;
	BYTE subcode;
	
	//Time
	BYTE TimerL;
	BYTE TimerH;

	//Users
	BYTE UserCount;
	BYTE Unk;

	//User 1
	BYTE User1L;
	BYTE User1H;
	BYTE Unk1;
	BYTE Pos1;

	//User 2
	BYTE User2L;
	BYTE User2H;
	BYTE Unk2;
	BYTE Pos2;

	//User 3
	BYTE User3L;
	BYTE User3H;
	BYTE Unk3;
	BYTE Pos3;

	//User 4
	BYTE User4L;
	BYTE User4H;
	BYTE Unk4;
	BYTE Pos4;

	//User 5
	BYTE User5L;
	BYTE User5H;
	BYTE Unk5;
	BYTE Pos5;
};

#endif

#endif