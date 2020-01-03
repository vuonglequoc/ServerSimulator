//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Users Functions & Structures							//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef __USER_H__
#define __USER_H__

#include "SkillDelay.h"
#include "ComboAttack.h"
#include "../Test/Network.H"
#include "MagicInf.h"
#include "zzzitem.h"
#include "../Test/CharacterInfo.h"
#include "GuildClass.h"
#include "TDurMagicKeyChecker.h"
#include "TMonsterAIAgro.h"
#include "TMonsterSkillElementInfo.h"
#include "JewelOfHarmonySystem.h"
#include "Item380System.h"
#include "ItemSocketSystem.h"


#define KILLCOUNT_RESET		0
#define KILLCOUNT_INCREASE	1
#define KILLCOUNT_DECREASE	2

enum BOX_TYPE
{
	TYPE_INVENTORY = 0,
	TYPE_TRADE = 1,
	TYPE_WAREHOUSE = 2,
	TYPE_CHAOSMACHINE = 3,
	TYPE_PERSONALSHOP = 4,
	TYPE_PETTRAINER = 5,
};

enum eVIEWSKILL
{
	eVS_NONE									= 0,
	eVS_ATTACK_AURA								= 1,
	eVS_DEFENSE_AURA							= 2,
	eVS_NPC_HELP								= 3,
	eVS_SOUL_BARRIER							= 4,
	eVS_CRITICAL_DAMAGE							= 5,
	eVS_INFINITY_ARROW							= 6,
	eVS_AG_RECOVERY_SPEED						= 7,
	eVS_SWELL_LIFE								= 8,
	eVS_SWELL_MANA								= 9,
	eVS_POTION_BLESS							= 10,
	eVS_POTION_SOUL								= 11,
	eVS_13										= 13,
	eVS_14										= 14,
	eVS_15										= 15,
	eVS_16										= 16,
	eVS_17										= 17,
	eVS_TRANSPARENCY							= 18,
	eVS_19										= 19,
	eVS_20										= 20,
	eVS_21										= 21,
	eVS_22										= 22,
	eVS_23										= 23,
	eVS_ALTAR_OF_WOLF_CONTRACT_ATTEMPT			= 24,
	eVS_ADMIN									= 25,
	eVS_ALTAR_OF_WOLF_HERO_CONTRACT_STATUS		= 26,
	eVS_INVISIBLE								= 27,
	eVS_GAMEMASTER								= 28,
	eVS_SEAL_ASCENCION							= 29,
	eVS_SEAL_WEALTH								= 30,
	eVS_SEAL_SUSTENANCE							= 31,
	eVS_32										= 32,
	eVS_33										= 33,
	eVS_ILLUSION_PROTECTION						= 34,
	eVS_HALLOWEEN_ATTACKSPEED					= 35,
	eVS_HALLOWEEN_ATTACKPOWER					= 36,
	eVS_HALLOWEEN_DEFENSEPOWER					= 37,
	eVS_HALLOWEEN_MAXLIFE						= 38,
	eVS_HALLOWEEN_MAXMANA						= 39,
	eVS_SEAL_ASCENCION2							= 40,
	eVS_SEAL_WEALTH2							= 41,
	eVS_SEAL_SUSTENANCE2						= 42,
	eVS_SEAL_MOVEMENT							= 43,
	eVS_SCROLL_QUICK							= 44, // Attack rate + 15
	eVS_SCROLL_DEFENSE							= 45, // Defense + 60
	eVS_SCROLL_DAMAGE							= 46, // Damage + 30
	eVS_SCROLL_SKILLPOWER						= 47, // Skillpower + 30	
	eVS_SCROLL_STAMMINA			= 48, // Stammina + 200
	eVS_SCROLL_MANA				= 49, // Mana + 300
	eVS_ELIXIR_POWER			= 50, // Increate Power
	eVS_ELIXIR_DEFENSE			= 51, // Increase Defence
	eVS_ELIXIR_STAMMINA			= 52, // Increase Strength
	eVS_ELIXIR_ENERGY			= 53, // Increase Energy
	eVS_ELIXIR_COMMAND			= 54, // Increase Command
	eVS_POISON					= 55,
	eVS_ICE						= 56,
	eVS_ICE_ARROW				= 57,
	eVS_DEFENSE_DOWN			= 58,
	eVS_59						= 59,
	eVS_60						= 60,
	eVS_STUN					= 61,
	eVS_62						= 62,
	eVS_63						= 63,
	eVS_64						= 64,
	eVS_ILLUSION_ORDEROFBONDAGE = 65,
	eVS_66						= 66,
	eVS_67						= 67,
	eVS_68						= 68,
	eVS_69						= 69,
	eVS_70						= 70,
	eVS_SKILL_DAMAGEREFLECTION	= 71,
	eVS_SKILL_SLEEP				= 72,
	eVS_SKILL_BLIDNESS			= 73,
	eVS_SKILL_NEIL				= 74,
	eVS_SKILL_SAHAMUT			= 75,
	eVS_SKILL_NICEWEEK			= 76, // Decrease Skill Power
	eVS_SKILL_INNERBEIYSEYON	= 77, // Decrease Defense
	eVS_CHERRY_MANABUFF			= 78, // Mana + 700
	eVS_CHERRY_LIFEBUFF			= 79, // Life + 700
	eVS_CHERRY_DAMAGEBUFF		= 80, // Increase damage skill power by 40
	eVS_SKILL_SWORDPOWER		= 81, // Increase skill power, damage to maximum, defense to maximum
	eVS_SKILL_MAGICCIRCLE		= 82, // Increase minimum skillpower
	eVS_SKILL_SWORDSLASHEFFECT	= 83, // Fogoooo!
	eVS_SKILL_LIGHTNINGSTORMEFFECT	= 84, // Raioss!
	eVS_SKILL_REDSTORMEFFECT	= 85,
	eVS_SKILL_COLDEFFECT		= 86,
	eVS_SEAL_RED				= 87,
	eVS_SEAL_YELLOW				= 88,
	eVS_UNKNOW_SWORD			= 89, // 20% of ???
	eVS_UNKNOW_TRIANGLE			= 90,
	eVS_IMPROVE_DEFENSE_OFENSE	= 91,
	eVS_IMPROVE_MAXLIFE			= 92, // amarelo
	eVS_IMPROVE_MANA			= 93, // azul
	eVS_IMPROVE_DAMAGE			= 94, // red
	eVS_IMPROVE_DEFENSE			= 95, // green
	eVS_IMPROVE_ATTACKSPEED		= 96,
	eVS_IMPROVE_AGRECOVERY		= 97,
	eVS_DUEL_INTERFACE			= 98,
	eVS_99						= 99,
	eVS_100						= 100,
	eVS_101						= 101, 
	eVS_102						= 102,
	eVS_DUEL_MEDAL				= 103,
	eVS_PK_PENALTY				= 106, // Warp Command Window access restricted for five minutes.
	eVS_CRYWOLF_COMMON_NPC,
	eVS_ALTAR_VALID_CONTRACT,
	eVS_ALTAR_ATTEMP_CONTRACT,
	eVS_ALTAR_MAX_CONTRACT,
	eVS_INMUNE_TO_MAGIC,
	eVS_SACRIFICE,
	eVS_ELEMENT_INMUNE,
	eVS_OLDSHUE					= 128, // Item drop increased.
	eVS_IGNORE_DEFENSE			= 129,
	eVS_FITNESS					= 130,
	eVS_DEFENSE_SUCCESS_RATE	= 131
};

enum BUFF_SKILL
{
	SOUL_BARRIER			= 1,
	GREATER_DAMAGE			= 2,
	GREATER_DEFENSE			= 3,
	GREATER_FORTITUDE		= 4,
	CRITICAL_DAMAGE			= 5,
	REFLEX					= 6,
	WEAKNESS				= 7,
	INNOVATION				= 8,
	MAGIC_CIRCLE			= 9,
	RED_ARMOR_IGNORE		= 10,
	FITNESS					= 11,
	DEFENSE_RATE			= 12
};

enum DB_CLASS_CODES 
{
	DB_DARK_WIZARD		= 0,
	DB_SOUL_MASTER		= 1,
	DB_GRAND_MASTER		= 3,

	DB_DARK_KNIGHT		= 16,
	DB_BLADE_KNIGHT		= 17,
	DB_BLADE_MASTER		= 19,

	DB_FAIRY_ELF		= 32,
	DB_MUSE_ELF			= 33,
	DB_HIGH_ELF			= 35,

	DB_MAGIC_GLADIATOR	= 48,
	DB_DUEL_MASTER		= 51,

	DB_DARK_LORD		= 64,
	DB_LORD_EMPEROR		= 67,

	DB_SUMMONER			= 80,
	DB_BLOODY_SUMMONER	= 81,
	DB_DIMENSION_MASTER = 83,

	DB_RAGE_FIGHTER		= 96,
	DB_FIST_MASTER		= 99,
};

#define MAX_VIEWSKILL	200


#if (GS_CASTLE)
#define OBJMAX 4000
#define OBJMIN 3000
#define OBJMAXUSER 1500
#define OBJ_STARTUSERINDEX (OBJMAX - OBJMAXUSER)
#define OBJ_MAXMONSTER 2000
#define OBJ_CALLMONCOUNT (OBJ_STARTUSERINDEX - OBJ_MAXMONSTER)
#else
#define OBJMAX 10400
#define OBJMIN 9400
#define OBJMAXUSER 1000
#define OBJ_STARTUSERINDEX (OBJMAX - OBJMAXUSER)
#define OBJ_MAXMONSTER 9000
#define OBJ_CALLMONCOUNT (OBJ_STARTUSERINDEX - OBJ_MAXMONSTER)
#endif

#define IF_MAP_RANGE(mapnumber) ( ((mapnumber) < MAP_INDEX_IMPERIAL1)?false:((mapnumber) > MAP_INDEX_IMPERIAL4 )?false:true )
#define DG_MAP_RANGE(mapnumber) ( ((mapnumber) < MAP_INDEX_DOUBLEGOER1)?false:((mapnumber) > MAP_INDEX_DOUBLEGOER4 )?false:true )
#define CRYWOLF_MAP_RANGE(mapnumber) ( ((mapnumber) < MAP_INDEX_CRYWOLF_FIRSTZONE)?false:((mapnumber) > MAP_INDEX_CRYWOLF_SECONDZONE )?false:true )
#define DS_MAP_RANGE(map) ( ( ((map)) == MAP_INDEX_DEVILSQUARE  )?true:( ((map)) == MAP_INDEX_DEVILSQUARE2  )?true:false  )
#define IT_MAP_RANGE(mapnumber) (((mapnumber) < MAP_INDEX_ILLUSION1) ? false : ((mapnumber) > MAP_INDEX_ILLUSION6 ) ? false : true )

#define MAX_MONSTER_SEND_MSG			10
#define MAX_MONSTER_SEND_ATTACK_MSG		50
#define MAX_MONSTER_TYPE				580
#define MAX_GUILD_LEN					8
#define MAX_ACCOUNT_LEN					10
#define INVENTORY_SIZE					172
#define WAREHOUSE_SIZE					240	
#define TRADE_BOX_SIZE					32
#define MAGIC_SIZE						60
#define INVETORY_WEAR_SIZE				12
#define INVENTORY_BAG_START				INVETORY_WEAR_SIZE
#define MAIN_INVENTORY_SIZE				140	// Not included PersonalSh
#define INVENTORY_MAP_SIZE				160	// Not included wared items
#define TRADE_BOX_MAP_SIZE				32
#define PSHOP_SIZE						32
#define PSHOP_MAP_SIZE					32
#define MAX_CHAR_MASTER_LEVEL			200
#define MAX_SELF_DEFENSE				5
#define MAX_ST_HIT_DAMAGE				40
#define MAX_ZEN							2000000000
#define MAX_WAREHOUSE_ZEN				100000000
#define ACTIVE_ITEM_SEARCH				0
#define CHAOS_BOX_SIZE					32
#define CHAOS_BOX_MAP_SIZE				32
#define WAREHOUSE_HEIGH					15
#define WAREHOUSE_WIDTH					8
#define EXTRA_INVENTORY1_START			76
#define EXTRA_INVENTORY1_END			107
#define EXTRA_INVENTORY2_START			108
#define EXTRA_INVENTORY2_END			139
#define EXTRA_WAREHOUSE_START			120
#define EXTRA_WAREHOUSE_END				239
#define PERSONAL_SHOP_START				204
#define PERSONAL_SHOP_END				235
#define MAX_GM_IN_ACCOUNT				5


enum EXP_GETTING_EVENT_TYPE 
{
	EVENT_TYPE_NONE			= 0x0,
	EVENT_TYPE_PARTY		= 0x1,
	EVENT_TYPE_DEVILSQUARE	= 0x2,
	EVENT_TYPE_CHAOSCASTLE	= 0x3,
	EVENT_TYPE_BLOODCASTLE	= 0x4,
	EVENT_TYPE_CRYWOLF		= 0x5,
};

#define MAX_RESISTENCE_TYPE 7

#define R_ICE			0
#define R_POISON		1
#define R_LIGHTNING		2
#define R_FIRE			3
#define R_EARTH			4
#define R_WIND			5
#define R_WATER			6

#define OBJ_EMPTY -1
#define OBJ_MONSTER 2
#define OBJ_USER 1
#define OBJ_NPC	3
#define NO_SPC_MAP -1

#define MAX_PARTY_MEMBER 5
#define MAX_PARTY_LEVEL_DIFFERENCE	130
#define MAX_MAGIC 60
#define MAX_VIEWPORT 75

#if(GS_CASTLE)
#define MAX_VIEWPORT_MONSTER 75
#else
#define MAX_VIEWPORT_MONSTER 20
#endif

#define MAX_ROAD_PATH_TABLE 16
#define MAX_ARRAY_FRUSTRUM 4

#define PLAYER_EMPTY  0
#define PLAYER_CONNECTED 1
#define PLAYER_LOGGED 2
#define PLAYER_PLAYING 3

#define MAX_CHECKSUM_KEY 1024
#define OBJMAX_RANGE(aIndex) ( ((aIndex) < 0 )?false:( (aIndex) > OBJMAX-1 )?false:true )
#define FRIEND_SERVER_STATE_LOGIN_FAIL	0
#define FRIEND_SERVER_STATE_OFFLINE		1
#define FRIEND_SERVER_STATE_ONLINE		2

#define MAIN_INVENTORY_RANGE(x) (((x)<0)?FALSE:((x)>MAIN_INVENTORY_SIZE-1)?FALSE:TRUE )
#define INVENTORY_RANGE(x) (((x)<0)?FALSE:((x)>INVENTORY_SIZE-1)?FALSE:TRUE )
#define WAREHOUSE_RANGE(x) (((x)<0)?FALSE:((x)>WAREHOUSE_SIZE-1)?FALSE:TRUE )
#define TRADE_BOX_RANGE(x) (((x)<0)?FALSE:((x)>TRADE_BOX_SIZE-1)?FALSE:TRUE )
#define OBJMON_RANGE(x) ( ((x)<0)?FALSE:((x)>OBJ_MAXMONSTER-1)?FALSE:TRUE)
#define PSHOP_RANGE(x) ( (((x)-MAIN_INVENTORY_SIZE)<0)?FALSE:(((x)-MAIN_INVENTORY_SIZE)>(INVENTORY_SIZE-MAIN_INVENTORY_SIZE-1))?FALSE:TRUE )
#define ATTRIBUTE_RANGE(x) (((x)<51)?FALSE:((x)>58)?FALSE:TRUE )
#define MAP_RANGE(Map) (((Map) < 0)?FALSE:((Map) >= MAX_NUMBER_MAP)?FALSE:TRUE)


#define CURRENT_DB_VERSION	3

#define CHAR_SET_SIZE		18

#define CS_CLASS		0
#define CS_WEAPON1_TYPE	1
#define CS_WEAPON2_TYPE	2
#define CS_WEAPON1_DATA	12
#define CS_WEAPON2_DATA	13
#define CS_HELMET1		13
#define CS_HELMET2		9
#define CS_HELMET3		3
#define CS_ARMOR1		14
#define CS_ARMOR2		9
#define CS_ARMOR3		3
#define CS_PANTS1		14
#define CS_PANTS2		9
#define CS_PANTS3		4
#define CS_GLOVES1		15
#define CS_GLOVES2		9
#define CS_GLOVES3		4
#define CS_BOOTS1		15
#define CS_BOOTS2		9
#define CS_BOOTS3		5


#define CHECK_LIMIT(value, limit) ( ((value)<0)?false:((value)>((limit)-1))?false:true  )
#define CHECK_LIMIT2(value, base, limit) ( ((value)<base)?false:((value)>((limit)-1))?false:true  )
#define CHECK_CLASS(value, type) ( ((value)!=(type))?FALSE:TRUE )

#define CS_GET_CLASS(x) ( (((x)>>4)<<5)&(0xE0) )
#define CS_GET_CHANGEUP(x) ( ((x)&0x07) )
#define CS_GET_MASTERCLASS(x) ( (((x)>>1)<<3)&(0x08) )
#define CS_SET_CLASS(x) ( ((x)<<5)& 0xE0 )
#define CS_SET_CHANGEUP(x) ( ((x) << 4) & 0x10 )
#define CS_SET_CHANGEUP2(x) ( ((x) << 3) & 0x08 )

#define CS_SET_HELMET1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_HELMET2(x) ( ((x) & 0x10 ) << 3 )
#define CS_SET_HELMET3(x) ( ((x) & 0x0F ) << 4 )

#define CS_SET_ARMOR1(x) ( ((x) & 0x1E0) >> 1 )
#define CS_SET_ARMOR2(x) ( ((x) & 0x10 ) << 2 )
#define CS_SET_ARMOR3(x) ( ((x) & 0x0F )      )

#define CS_SET_PANTS1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_PANTS2(x) ( ((x) & 0x10 ) << 1 )
#define CS_SET_PANTS3(x) ( ((x) & 0x0F ) << 4 )

#define CS_SET_GLOVES1(x) ( ((x) & 0x1E0) >> 1 )
#define CS_SET_GLOVES2(x) ( ((x) & 0x10 )      )
#define CS_SET_GLOVES3(x) ( ((x) & 0x0F )      )

#define CS_SET_BOOTS1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_BOOTS2(x) ( ((x) & 0x10 ) >> 1 )
#define CS_SET_BOOTS3(x) ( ((x) & 0x0F ) << 4 )

#define CS_SET_WING1(x)  ( ((x) & 0x03 ) << 2 )
#define CS_SET_HELPER(x) ( ((x) & 0x03 )      )

#define CS_SET_SMALLLEVEL_RH(x)		( (x)       )
#define CS_SET_SMALLLEVEL_LH(x)		( (x) << 3  ) 
#define CS_SET_SMALLLEVEL_HELMET(x)	( (x) << 6  )
#define CS_SET_SMALLLEVEL_ARMOR(x)	( (x) << 9  )
#define CS_SET_SMALLLEVEL_PANTS(x)	( (x) << 12 )
#define CS_SET_SMALLLEVEL_GLOVES(x)	( (x) << 15 )
#define CS_SET_SMALLLEVEL_BOOTS(x)	( (x) << 18 )

#define CS_SET_SMALLLEVEL1(x)		( ((x) >> 16) & 0xFF )
#define CS_SET_SMALLLEVEL2(x)		( ((x) >> 8 ) & 0xFF )
#define CS_SET_SMALLLEVEL3(x)		((x) & 0xFF )


//-- Include --//
#include "ViewPortGuild.h"


typedef union tagPATHTABLE
{
	short sPathTable[16];
	char cPathTable[32];
} PATHTABLE, * LPPATHTABLE;


typedef struct tagActionState
{
	unsigned long Rest:1;	// 0
	unsigned long Attack:1;	// 1
	unsigned long Move:1;	// 2
	unsigned long Escape:1;	// 3
	unsigned long Emotion:4;	// 4
	unsigned long EmotionCount:8;	// 8

} ACTION_STATE, * LPACTION_STATE;


typedef struct tagInterfaceState
{
	DWORD use	: 2;
	DWORD state : 4;
	DWORD type	: 10;

} INTERFACE_STATE, * LPINTERFACE_STATE;



typedef struct
{
	DWORD unk0 : 1;
	DWORD unk1 : 1;
	DWORD unk2 : 1;
	DWORD unk3 : 1;
	DWORD unk4 : 1;
	DWORD unk5 : 1;
	DWORD unk6 : 1;
	DWORD unk7 : 1;
	DWORD unk8 : 1;
	DWORD unk9 : 1;
	DWORD unkA : 1;
	DWORD unkB : 1;
	DWORD unkC : 1;
	DWORD unkD : 1;
	DWORD unkE : 1;
	DWORD unkF : 1;
	DWORD unk10: 1;

}  ST_SKILL_STATE;


union STU_SKILL_STATE
{
	ST_SKILL_STATE SkillStates;
	DWORD AllData;
};


extern short RoadPathTable[MAX_ROAD_PATH_TABLE];
extern int  gServerMaxUser;	
extern BOOL g_EnergyCheckOff;


struct HITDAMAGE_STRUCT
{
	short number;		// 0
	int HitDamage;		// 4
	DWORD LastHitTime;	// 8
};


struct VIEWPORT_STRUCT
{
	char state;			// 0
	short number;		// 2
	unsigned char type;	// 4
	short index;		// 6
	int dis;			// 8
};

struct VIEWPORT_PLAYER_STRUCT
{
	char state;			// 0
	short number;		// 2
	unsigned char type;	// 4
	short index;		// 6
	int dis;			// 8
};


struct MessageStateMachine
{
	int MsgCode;		// 0
	int SendUser;		// 4
	DWORD MsgTime;		// 8
	int SubCode;		// C
};


struct ExMessageStateMachine
{
	int MsgCode;		// 0
	int SendUser;		// 4
	DWORD MsgTime;		// 8
	int SubCode;		// C
	int SubCode2;		// 10
};

typedef struct tagSEND_MONSTER_ATTACK_MENSSAGE
{
	int iMsgCode;		// unk0
	int iIndex;			// unk4
	DWORD dwDelay;		// unk8
	int iSubCode;		// unkC
	int iSubCode2;		// unk10
}SEND_MONSTER_ATTACK_MENSSAGE;

///////////////////////////////////////////////////////
//=========== STRUCT  MASTER SKILL SYSTEM ===========//
///////////////////////////////////////////////////////
struct MASTER_SKILL_TREE_EFFECT
{
    short RiseAttackSuccessRate; //0
	short AdvancedAttackSuccessRate; //1
	short AdvancedDefenseSuccessRate; //2
	short RepairLevel1; //3
	short RepairLevel2; //4
	short PoisonResistance; //5
	short LightningResistance; //6
	short IceResistance; //7
	short IncreaseAutoRegeneration; //8
	short IncreaseZen; //9
	short IncreaseDefense; //10
	short IncreaseMaxHP; //11
	short IncreaseMaxAG; //12
	short IncreaseRecoverMana; //13
	short IncreaseRecoverLife; //14
	short IncreaseRecoverSD; //15
	short IncreaseExperience; //16
	short IncreaseMaxSD; //17
	short IncreaseSDRecoveryRate; //18
	short IncreaseMaxDamage; //19
	short IncreaseMinDamage; //20
	short IncreaseManaReduction; //21
	short IncreaseMaxSkillPower; //22
	short IncreaseMinSkillPower; //23
	short DecreasePetDurabilityWane; //24
};


extern ExMessageStateMachine gSMAttackProcMsg[OBJMAX][MAX_MONSTER_SEND_ATTACK_MSG];
extern MessageStateMachine gSMMsg[OBJMAX][MAX_MONSTER_SEND_MSG];




#if (!GS_CASTLE)
struct OBJECTSTRUCT
{
	unsigned short m_Index;
	char Connected;
	char LoginMsgSnd;
	char LoginMsgCount;
	char CloseCount;
	char CloseType;
	unsigned char bEnableDelCharacter;
	_PER_SOCKET_CONTEXT* PerSocketContext;
	unsigned int m_socket;
	char Ip_addr[16];
	int UserNumber;
	short DBNumber;
	unsigned char Magumsa;
	DWORD AutoSaveTime;
	DWORD ConnectCheckTime;
	DWORD CheckTick;
	unsigned char CheckSpeedHack;
	DWORD CheckTick2;
	unsigned char CheckTickCount;
	DWORD SaveTimeForStatics;
	int iPingTime;
	unsigned char m_TimeCount;
	DWORD m_dwPKTimer;
	short CheckSumTableNum;
	DWORD CheckSumTime;
	char Type;
	char Live;
	char CharacterPos;
	char AccountID[11];
	char Name[11];
	char LastJoominNumber[14];
	bool PlusStatQuestClear;
	bool ComboSkillquestClear;
	ComboSkillData comboSkill;
	WORD  Class;
	unsigned char DbClass;
	unsigned char ChangeUP;
	unsigned char ChangeUP2;
	short Level;
	short MasterLevel;
	int LevelUpPoint;
	short MasterPoints;
	int iFruitPoint;
	unsigned long Experience;
	unsigned long NextExp;
	unsigned long MLExperience;
	unsigned long MLNextExp;
	int   Money;
	unsigned short Strength;
	unsigned short Dexterity;
	unsigned short Vitality;
	unsigned short Energy;
	float Life;
	float MaxLife;
	int   m_iScriptMaxLife;
	float FillLife;
	float FillLifeMax;
	float Mana;
	float MaxMana;
	unsigned short Leadership;
	short AddLeadership;
	WORD  ChatLimitTime;
	BYTE  ChatLimitTimeSec;
	BYTE  FillLifeCount;
	short AddStrength;
	short AddDexterity;
	short AddVitality;
	short AddEnergy;
	int   BP;
	int   MaxBP;
	int	  AddBP;
	float VitalityToLife;
	float EnergyToMana;
	char  m_PK_Count;
	char  m_PK_Level;
	int   m_PK_Time;
	short X;
	short Y;
	BYTE  Dir;
	BYTE  MapNumber;
	int   AddLife;
	int   AddMana;
	int   iShield;
	int   iMaxShield;
	int   iAddShield;
	int   iFillShieldMax;
	int   iFillShield;
	int   iFillShieldCount;
	DWORD dwShieldAutoRefillTimer;
	BYTE  DamageMinus;
	BYTE  DamageReflect;
	short MonsterDieGetMoney;
	BYTE  MonsterDieGetLife;
	BYTE  MonsterDieGetMana;
	unsigned char StartX;
	unsigned char StartY;
	unsigned char m_OldX;
	unsigned char m_OldY;
	unsigned char TX;
	unsigned char TY;
	unsigned char MTX;
	unsigned char MTY;
	int   PathCount;
	int   PathCur;
	char  PathStartEnd;
	short PathOri[15];
	short PathX[15];
	short PathY[15];
	char  PathDir[15];
	DWORD PathTime;
	BYTE  m_MoveGateNumber;
	DWORD Authority;
	DWORD AuthorityCode;
	DWORD Penalty;
	BYTE  m_cAccountItemBlock;
	ACTION_STATE m_ActState;
	BYTE  m_ActionNumber;
	DWORD m_State;
	char m_StateSub;
	unsigned char m_Rest;
	char m_ViewState;
	unsigned char m_ViewSkillState[MAX_VIEWSKILL];
	DWORD m_LastMoveTime;
	DWORD m_LastAttackTime;
	BYTE m_FriendServerOnline;
	int m_DetectSpeedHackTime;
	unsigned long m_SumLastAttackTime;
	unsigned long m_DetectCount;
	int m_DetectedHackKickCount;
	int m_SpeedHackPenalty;
	unsigned char m_AttackSpeedHackDetectedCount;
	unsigned long m_PacketCheckTime;
	unsigned char m_ShopTime;
	unsigned long m_TotalAttackTime;
	int m_TotalAttackCount;
	unsigned long TeleportTime;
	char Teleport;
	char KillerType;
	char DieRegen;
	char RegenOk;
	unsigned char RegenMapNumber;
	unsigned char RegenMapX;
	unsigned char RegenMapY;
	DWORD RegenTime;
	DWORD MaxRegenTime;
	short m_PosNum;
	DWORD LifeRefillTimer;

	DWORD CurActionTime;
	DWORD NextActionTime;
	DWORD DelayActionTime;
	char DelayLevel;
	char m_PoisonType;
	char m_IceType;
	char m_PoisonBeattackCount;
	char m_ColdBeattackCount;
	char m_ImmuneToMagicCount;
	char m_ImmuneToHarmCount;
	char m_iMonsterBattleDelay;
	char m_cKalimaGateExist;
	int m_iKalimaGateIndex;
	char m_cKalimaGateEnterCount;
	OBJECTSTRUCT * lpAttackObj;
	short m_SkillNumber;
	DWORD m_SkillTime;
	bool m_bAttackerKilled;
	char m_ManaFillCount;
	char m_LifeFillCount;
	int SelfDefense[MAX_SELF_DEFENSE];
	DWORD SelfDefenseTime[MAX_SELF_DEFENSE];
	DWORD MySelfDefenseTime;
	char m_Drink;
	int m_SkillDefense;
	char m_SkillDefenseTime;
	int m_SkillAttack;
	char m_SkillAttackTime;
	int m_SkillAttack2;
	char m_SkillAttackTime2;
	int m_SkillAddLife;
	int m_SkillAddLifeTime;
	int m_SkillHarden;
	int m_SkillHardenTime;
	int m_SkillMagumReduceDefense;
	int m_SkillMagumReduceDefenseTime;
	int PartyNumber;
	int PartyTargetUser;
	int GuildNumber;
	_GUILD_INFO_STRUCT * lpGuild;
	char GuildName[11];
	int GuildStatus;
	int iGuildUnionTimeStamp;
	int m_RecallMon;
	int m_Change;
	short TargetNumber;
	short TargetShopNumber;	// 2F6
	short ShopNumber;	// 2F8
	short LastAttackerID;	// 2FA
	int m_AttackDamageMin;	// 2FC
	int m_AttackDamageMax;	// 300
	int m_MagicDamageMin;	// 304
	int m_MagicDamageMax;	// 308
	int m_AttackDamageLeft;	// 30C
	int m_AttackDamageRight;	// 310
	int m_AttackDamageMaxLeft;	// 314
	int m_AttackDamageMinLeft;	// 318
	int m_AttackDamageMaxRight;	// 31C
	int m_AttackDamageMinRight;	// 320
	int m_AttackRating;	// 324
	int m_AttackSpeed;	// 328
	int m_MagicSpeed;	// 32C
	int m_Defense;	// 330
	int m_MagicDefense;	// 334
	int m_SuccessfulBlocking;	// 338
	short m_MoveSpeed;	// 33C
	short m_MoveRange;	// 33E
	short m_AttackRange;	// 340
	short m_AttackType;	// 342
	short m_ViewRange;	// 344
	short m_Attribute;	// 346
	short m_ItemRate;	// 348
	short m_MoneyRate;	// 34A
	int m_CriticalDamage;	// 34C
	int m_ExcelentDamage;	// 350

	CMagicInf * m_lpMagicBack;	// 354
	CMagicInf * Magic;	// 358
	char MagicCount;	// 35C
	unsigned short UseMagicNumber;	// 35D
	unsigned long UseMagicTime;	// 360
	char UseMagicCount;	// 364
	short OSAttackSerial;	// 366
	unsigned char SASCount;	// 368
	DWORD SkillAttackTime;	// 36C
	char CharSet[18]; 	// 370
	int m_PeriodItemTime;
	char m_Resistance[MAX_RESISTENCE_TYPE];	// 382
	char m_AddResistance[MAX_RESISTENCE_TYPE];	// 389
	int FrustrumX[MAX_ARRAY_FRUSTRUM];	// 390
	int FrustrumY[MAX_ARRAY_FRUSTRUM]; // 3A4
	VIEWPORT_STRUCT VpPlayer[MAX_VIEWPORT];	// 3B0
	VIEWPORT_PLAYER_STRUCT VpPlayer2[MAX_VIEWPORT];	// 734
	int VPCount;						// AB8
	int VPCount2;						// ABC
	HITDAMAGE_STRUCT sHD[MAX_ST_HIT_DAMAGE];	// AC0
	short sHDCount;								// CA0
	tagInterfaceState m_IfState;		// CA4
	DWORD m_InterfaceTime;				// CA8
	CItem * pInventory;					// CAC
	LPBYTE  pInventoryMap;				// CB0
	char * pInventoryCount;				// CB4
	char pTransaction;					// CB8
	CItem * Inventory1;					// CBC
	LPBYTE  InventoryMap1;				// CC0
	char InventoryCount1;				// CC4
	CItem * Inventory2;					// CC8
	LPBYTE InventoryMap2;				// CCC
	char InventoryCount2;				// CD0
	CItem * Trade;						// CD4
	LPBYTE TradeMap;					// unkCD8
	int TradeMoney;						// CDC
	bool TradeOk;						// CE0
	CItem * pWarehouse;					// CE4
	LPBYTE pWarehouseMap;				// CE8
	char WarehouseCount;				// CEC
	short WarehousePW;					// CEE
	BYTE WarehouseLock;					// CF0
	BYTE WarehouseUnfailLock;			// CF1
	int WarehouseMoney;					// CF4
	int WarehouseSave;					// CF8
	CItem * pChaosBox;					// CFC
	LPBYTE pChaosBoxMap;				// D00
	int ChaosMoney;						// D04
	int ChaosSuccessRate;				// D08
	bool ChaosLock;						// D0C
	DWORD m_Option;						// D10
	int m_nEventScore;					// D14
	int m_nEventExp;					// D18
	int m_nEventMoney;					// D1C
	BYTE m_bDevilSquareIndex;			// D20
	bool m_bDevilSquareAuth;			// D21
	char m_cBloodCastleIndex;			// D22
	char m_cBloodCastleSubIndex;		// D23
	int m_iBloodCastleEXP;				// D24
	bool m_bBloodCastleComplete;		// D28
	char m_cChaosCastleIndex;			// D29
	char m_cChaosCastleSubIndex;		// D2A
	int m_iChaosCastleBlowTime;			// D2C
	char m_cKillUserCount;				// D30
	char m_cKillMonsterCount;			// D31
	char m_cIllusionTempleIndex;
	char m_cIllusionTempleSubIndex;
	////////////////////////////////
	//Skills Illusion temple
	////////////////////////////////
	int	 m_btSkillShieldOrderTime;
	BOOL m_btSkillShieldOrder;
	int	 m_btSkillRestraintOrderTime;
	BOOL m_btSkillRestraintOrder;
	int m_iIllusionTempleExp;
	short m_iDuelUserReserved;			// D34
	short m_iDuelUserRequested;			// D38
	short m_iDuelUser;					// D3C
	BYTE m_btDuelScore;					// D40
	int m_iDuelTickCount;				// D44
	bool m_bPShopOpen;					// D48
	bool m_bPShopTransaction;			// D49	
	bool m_bPShopItemChange;			// D4A
	bool m_bPShopRedrawAbs;				// D4B
	char m_szPShopText[36];				// D4C
	bool m_bPShopWantDeal;				// D70
	int m_iPShopDealerIndex;			// D74
	char m_szPShopDealerName[10];		// D78
	CRITICAL_SECTION m_critPShopTrade;	// D84
	int m_iVpPShopPlayer[MAX_VIEWPORT];	// D9C
	WORD m_wVpPShopPlayerCount;			// EC8
	bool IsInBattleGround;				// ECA
	bool HaveWeaponInHand;				// ECB
	short EventChipCount;				// ECC
	int MutoNumber;						// ED0
	bool UseEventServer;
	bool LoadWareHouseInfo;				// ED8
	int iStoneCount;					// EDC
	unsigned char m_Quest[50];
	bool m_SendQuestInfo;
	int m_SkyBossMonSheildLinkIndex;
	int m_SkyBossMonSheild;
	int m_SkyBossMonSheildTime;
	int m_MaxLifePower;
	int m_WizardSkillDefense;
	int m_WizardSkillDefenseTime;
	int m_PacketChecksumTime;
	int m_CheckLifeTime;
	unsigned char m_MoveOtherServer;
	char BackName[11];
	char m_BossGoldDerconMapNumber;
	bool m_InWebzen;
	char m_LastTeleportTime;	// F42
	BYTE m_ClientHackLogCount;	// F43
	char m_bIsInMonsterHerd;	// F44
	char m_bIsMonsterAttackFirst;	// F48
	class MonsterHerd * m_lpMonsterHerd;	// F4C
	bool m_bSentGGAuth;	// F7C
	DWORD m_NPggCheckSumSendTime;	// F80
	int fSkillFrustrumX[MAX_ARRAY_FRUSTRUM];	// F84
	int fSkillFrustrumY[MAX_ARRAY_FRUSTRUM];	// F94
	BYTE SkillHellFire2State;	// FA4
	BYTE SkillHellFire2Count;	// FA5
	DWORD SkillHellFire2Time;	// FA8
	unsigned char m_ReqWarehouseOpen;	// FAC
	int m_NotAttackAreaCount;	// FB0
	short SetOpAddSkillAttack;	// FB4
	short SetOpAddExDamage;	// FB6
	short SetOpAddExDamageSuccessRate;	// FB8
	short SetOpAddCriticalDamage;	// FBA
	short SetOpAddCriticalDamageSuccessRate;	// FBC
	short SetOpIncAGValue;	// FBE
	short SetOpAddDamage;	// FC0
	short SetOpAddMinAttackDamage;	// FC2
	short SetOpAddMaxAttackDamage;	// FC4
	short SetOpAddAttackDamage;	// FC6
	short SetOpAddDefence;	// FC8
	short SetOpAddDefenceRate;	// FCA
	short SetOpAddMagicPower;	// FCC
	BYTE SetOpIgnoreDefense;	// FCE
	BYTE SetOpDoubleDamage;	// FCF
	BYTE SetOpTwoHandSwordImproveDamage;	// FD0
	BYTE SetOpImproveSuccessAttackRate;	// FD1
	BYTE SetOpReflectionDamage;	// FD2
	BYTE SetOpImproveSheldDefence;	// FD3
	BYTE SetOpDecreaseAG;	// FD4
	BYTE SetOpImproveItemDropRate;	// FD5
	bool IsFullSetItem;	// FD6
	TDurMagicKeyChecker DurMagicKeyChecker;	// FD8
	WORD SkillRecallParty_Time;	// 1104
	BYTE SkillRecallParty_MapNumber;	// 1106
	BYTE SkillRecallParty_X;	// 1107
	BYTE SkillRecallParty_Y;	// 1108
	WORD SkillAddCriticalDamageTime;	// 110A
	WORD SkillAddCriticalDamage;	// 110C
	bool bIsChaosMixCompleted;	// 110E
	bool SkillLongSpearChange;	// 110F
	CSkillDelay SkillDelay;	// 1110
	int iObjectSecTimer;	// 1514
	bool m_bMapSvrMoveQuit;	// 1518
	bool m_bMapSvrMoveReq;	// 1519
	DWORD m_dwMapSvrQuitTick;	// 151C
	short m_sPrevMapSvrCode;	// 1520
	short m_sDestMapNumber;	// 1522
	unsigned char m_btDestX;
	unsigned char m_btDestY;
	bool m_bIsCastleNPCUpgradeCompleted;	// 1526
	BYTE m_btWeaponState;	// 1527
	int m_iWeaponUser;	// 1528
	BYTE m_btKillCount;	// 152C
	int m_iSkillStunTime;	// 1530
	int m_iSkillBrandOfSkillTime;	// 1534
	int m_iSkillInvisibleTime;	// 1538
	int m_iSkillManaSwellTime;	// 153C
	int m_iSkillManaSwell;	// 1540
	int m_iAccumulatedDamage;	// 1544
	int m_iPotionBlessTime;	// 1548
	int m_iPotionSoulTime;	// 154C
	int m_iAccumulatedCrownAccessTime;	// 1550
	TMonsterSkillElementInfo m_MonsterSkillElementInfo;	// 1554
	int m_iBasicAI;
	int m_iCurrentAI;
	int m_iCurrentAIState;
	int m_iLastAIRunTime;
	int m_iGroupNumber;
	int m_iSubGroupNumber;
	int m_iGroupMemberGuid;
	int m_iRegenType;
	TMonsterAIAgro m_Agro;
	int m_iLastAutomataRuntime;
	int m_iLastAutomataDelay;
	int m_iCrywolfMVPScore;
	DWORD m_dwLastCheckTick;
	int m_iAutoRecuperationTime;
	int m_iSkillNPCHelpTime;
	int m_iSkillNPCDefense;
	int m_iSkillNPCAttack;
	int m_iMuseElfInfinityArrowSkillTime;
	int m_iSkillDistanceErrorCount;
	DWORD m_dwSkillDistanceErrorTick;
	JEWELOFHARMONY_ITEM_EFFECT m_JewelOfHarmonyEffect;
	ITEMOPTION_FOR380ITEM_EFFECT m_ItemOptionExFor380;
	char m_bKanturuEntranceByNPC;
	WORD m_wItemEffectType;
	int m_iItemEffectValidTime;
	WORD m_wCashPoint;
	int m_iPeriodItemEffectIndex;
	WORD m_wExprienceRate;
	WORD m_wItemDropRate;
	BYTE m_btMoveMapBound;
	BYTE m_btSealIndex;
	ITEM_SOCKET_EFFECT ItemSocketEffect;
	int m_IllusionIndex;
	char m_IllusionTeam;
	int m_IllusionKillCount;
	int m_IllusionShieldSkillTime;
	char m_iDuelRoom;

	int m_SkillSleepTime;
	int m_SkillReflex;
	int m_SkillReflexTime;
	int m_SkillWeakness;
	int m_SkillWeaknessTime;
	int m_SkillInnovation;
	int m_SkillInnovationTime;
	int m_SkillBerserker;
	int m_SkillBerserkerTime;
	int m_SkillNightTime;
	int m_SkillNight;

	int m_RageFighterSkillDefenseRate;
	int m_RageFighterSkillDefenseRateTime;
	int m_RageFighterSkillFitnessAddLife;
	int m_RageFighterSkillFitnessTime;
	int m_RageFighterSkillRedArmorIgnore;
	int m_RageFighterSkillRedArmorIgnoreTime;
	int m_RageFighterSkillPowerIncrease;
	int m_RageFighterSkillPowerIncreaseTime;

	int GladiatorsGlory;
	int GladiatorsGloryTime;

	unsigned char GenFamily;
	unsigned char GenLevel;
	unsigned char GenReward;
	int GenContribution;
	int GenNextContribution;
	int GenRanking;
	short GenNPC;

	short QuestNPC;

	short LittleSanta1;
	int LittleSantaTime1;
	short LittleSanta2;
	int LittleSantaTime2;
	short LittleSanta3;
	int LittleSantaTime3;
	short LittleSanta4;
	int LittleSantaTime4;
	short LittleSanta5;
	int LittleSantaTime5;
	short LittleSanta6;
	int LittleSantaTime6;

	char SpecialBuff[7];
	char SpecialBuffEffectType1[7];
	char SpecialBuffEffectType2[7];
	short SpecialBuffEffectValue1[7];
	short SpecialBuffEffectValue2[7];
	int SpecialBuffEffectTime[7];
	int SpecialBuffItem[7];

	unsigned char QuestRegistered;
	unsigned char QuestTutorial;

	unsigned char NewQuestInfo[600];

	int SkillMagicCircleTime;
	int SkillMagicCircle;

	int SkillRecovery;

	unsigned char IsMarried;
	unsigned char SendRequest;
	unsigned char RecvRequest;
	char SendRequestName[11];
	char RecvRequestName[11];
	char MarryName[11];

	char ExpandedInventory;
	char ExpandedWarehouse;
	
	MASTER_SKILL_TREE_EFFECT MasterSkillTreeEffect;

	int	DelayTimeMSG;

};

#else

struct OBJECTSTRUCT {
  /*<thisrel this+0x0>*/ /*|0x4|*/ int m_Index;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int Connected;
  /*<thisrel this+0x8>*/ /*|0x1|*/ char LoginMsgSnd;
  /*<thisrel this+0x9>*/ /*|0x1|*/ char LoginMsgCount;
  /*<thisrel this+0xa>*/ /*|0x1|*/ char CloseCount;
  /*<thisrel this+0xb>*/ /*|0x1|*/ char CloseType;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int bEnableDelCharacter;
  /*<thisrel this+0x10>*/ /*|0x4|*/ struct _PER_SOCKET_CONTEXT* PerSocketContext;
  /*<thisrel this+0x14>*/ /*|0x4|*/ unsigned int m_socket;
  /*<thisrel this+0x18>*/ /*|0x10|*/ char Ip_addr[16];
  /*<thisrel this+0x28>*/ /*|0x4|*/ int UserNumber;
  /*<thisrel this+0x2c>*/ /*|0x4|*/ int DBNumber;
  /*<thisrel this+0x30>*/ /*|0x1|*/ unsigned char Magumsa;
  /*<thisrel this+0x34>*/ /*|0x4|*/ unsigned long AutoSaveTime;
  /*<thisrel this+0x38>*/ /*|0x4|*/ unsigned long ConnectCheckTime;
  /*<thisrel this+0x3c>*/ /*|0x4|*/ unsigned long CheckTick;
  /*<thisrel this+0x40>*/ /*|0x1|*/ unsigned char CheckSpeedHack;
  /*<thisrel this+0x44>*/ /*|0x4|*/ unsigned long CheckTick2;
  /*<thisrel this+0x48>*/ /*|0x1|*/ unsigned char CheckTickCount;
  /*<thisrel this+0x4c>*/ /*|0x4|*/ unsigned long SaveTimeForStatics;
  /*<thisrel this+0x50>*/ /*|0x4|*/ int iPingTime;
  /*<thisrel this+0x54>*/ /*|0x1|*/ unsigned char m_TimeCount;
  /*<thisrel this+0x58>*/ /*|0x4|*/ unsigned long m_dwPKTimer;
  /*<thisrel this+0x5c>*/ /*|0x2|*/ short CheckSumTableNum;
  /*<thisrel this+0x60>*/ /*|0x4|*/ unsigned long CheckSumTime;
  /*<thisrel this+0x64>*/ /*|0x2|*/ unsigned short Type;
  /*<thisrel this+0x66>*/ /*|0x1|*/ unsigned char Live;
  /*<thisrel this+0x67>*/ /*|0x1|*/ char CharacterPos;
  /*<thisrel this+0x68>*/ /*|0xb|*/ char AccountID[11];
  /*<thisrel this+0x73>*/ /*|0xb|*/ char Name[11];
  /*<thisrel this+0x7e>*/ /*|0xe|*/ char LastJoominNumber[14];
  /*<thisrel this+0x8c>*/ /*|0x1|*/ unsigned char PlusStatQuestClear;
  /*<thisrel this+0x8d>*/ /*|0x1|*/ unsigned char ComboSkillquestClear;
  /*<thisrel this+0x90>*/ /*|0xc|*/ struct ComboSkillData comboSkill;
  /*<thisrel this+0x9c>*/ /*|0x2|*/ unsigned short Class;
  /*<thisrel this+0x9e>*/ /*|0x1|*/ unsigned char DbClass;
  /*<thisrel this+0x9f>*/ /*|0x1|*/ unsigned char ChangeUP;
	BYTE  ChangeUP2;
	short Level; // A0
	short MasterLevel;
	int   LevelUpPoint; // A4
	short MasterPoints;
	unsigned long MLExperience;
	unsigned long MLNextExp;
  /*<thisrel this+0xa8>*/ /*|0x4|*/ int iFruitPoint;
  /*<thisrel this+0xac>*/ /*|0x4|*/ unsigned long Experience;
  /*<thisrel this+0xb0>*/ /*|0x4|*/ unsigned long NextExp;
  /*<thisrel this+0xb4>*/ /*|0x4|*/ int Money;
  /*<thisrel this+0xb8>*/ /*|0x2|*/ short Strength;
  /*<thisrel this+0xba>*/ /*|0x2|*/ short Dexterity;
  /*<thisrel this+0xbc>*/ /*|0x2|*/ short Vitality;
  /*<thisrel this+0xbe>*/ /*|0x2|*/ short Energy;
  /*<thisrel this+0xc0>*/ /*|0x4|*/ float Life;
  /*<thisrel this+0xc4>*/ /*|0x4|*/ float MaxLife;
  /*<thisrel this+0xc8>*/ /*|0x4|*/ int m_iScriptMaxLife;
  /*<thisrel this+0xcc>*/ /*|0x4|*/ float FillLife;
  /*<thisrel this+0xd0>*/ /*|0x4|*/ float FillLifeMax;
  /*<thisrel this+0xd4>*/ /*|0x4|*/ float Mana;
  /*<thisrel this+0xd8>*/ /*|0x4|*/ float MaxMana;
  /*<thisrel this+0xdc>*/ /*|0x2|*/ unsigned short Leadership;
  /*<thisrel this+0xde>*/ /*|0x2|*/ unsigned short AddLeadership;
  /*<thisrel this+0xe0>*/ /*|0x2|*/ unsigned short ChatLimitTime;
  /*<thisrel this+0xe2>*/ /*|0x1|*/ unsigned char ChatLimitTimeSec;
  /*<thisrel this+0xe3>*/ /*|0x1|*/ unsigned char FillLifeCount;
  /*<thisrel this+0xe4>*/ /*|0x2|*/ short AddStrength;
  /*<thisrel this+0xe6>*/ /*|0x2|*/ short AddDexterity;
  /*<thisrel this+0xe8>*/ /*|0x2|*/ short AddVitality;
  /*<thisrel this+0xea>*/ /*|0x2|*/ short AddEnergy;
  /*<thisrel this+0xec>*/ /*|0x4|*/ int BP;
  /*<thisrel this+0xf0>*/ /*|0x4|*/ int MaxBP;
  /*<thisrel this+0xf4>*/ /*|0x4|*/ int AddBP;
  /*<thisrel this+0xf8>*/ /*|0x4|*/ float VitalityToLife;
  /*<thisrel this+0xfc>*/ /*|0x4|*/ float EnergyToMana;
  /*<thisrel this+0x100>*/ /*|0x1|*/ char m_PK_Count;
  /*<thisrel this+0x101>*/ /*|0x1|*/ char m_PK_Level;
  /*<thisrel this+0x104>*/ /*|0x4|*/ int m_PK_Time;
	unsigned char X;
	unsigned char Y;
  /*<thisrel this+0x10c>*/ /*|0x1|*/ unsigned char Dir;
  /*<thisrel this+0x10d>*/ /*|0x1|*/ unsigned char MapNumber;
  /*<thisrel this+0x110>*/ /*|0x4|*/ int AddLife;
  /*<thisrel this+0x114>*/ /*|0x4|*/ int AddMana;
  /*<thisrel this+0x118>*/ /*|0x4|*/ int iShield;
  /*<thisrel this+0x11c>*/ /*|0x4|*/ int iMaxShield;
  /*<thisrel this+0x120>*/ /*|0x4|*/ int iAddShield;
  /*<thisrel this+0x124>*/ /*|0x4|*/ int iFillShieldMax;
  /*<thisrel this+0x128>*/ /*|0x4|*/ int iFillShield;
  /*<thisrel this+0x12c>*/ /*|0x4|*/ int iFillShieldCount;
  /*<thisrel this+0x130>*/ /*|0x4|*/ unsigned long dwShieldAutoRefillTimer;
  /*<thisrel this+0x134>*/ /*|0x1|*/ unsigned char DamageMinus;
  /*<thisrel this+0x135>*/ /*|0x1|*/ unsigned char DamageReflect;
  /*<thisrel this+0x136>*/ /*|0x2|*/ short MonsterDieGetMoney;
  /*<thisrel this+0x138>*/ /*|0x1|*/ unsigned char MonsterDieGetLife;
  /*<thisrel this+0x139>*/ /*|0x1|*/ unsigned char MonsterDieGetMana;
  /*<thisrel this+0x13a>*/ /*|0x1|*/ unsigned char StartX;
  /*<thisrel this+0x13b>*/ /*|0x1|*/ unsigned char StartY;
  /*<thisrel this+0x13c>*/ /*|0x2|*/ short m_OldX;
  /*<thisrel this+0x13e>*/ /*|0x2|*/ short m_OldY;
  /*<thisrel this+0x140>*/ /*|0x2|*/ short TX;
  /*<thisrel this+0x142>*/ /*|0x2|*/ short TY;
  /*<thisrel this+0x144>*/ /*|0x2|*/ short MTX;
  /*<thisrel this+0x146>*/ /*|0x2|*/ short MTY;
  /*<thisrel this+0x148>*/ /*|0x4|*/ int PathCount;
  /*<thisrel this+0x14c>*/ /*|0x4|*/ int PathCur;
  /*<thisrel this+0x150>*/ /*|0x1|*/ char PathStartEnd;
  /*<thisrel this+0x152>*/ /*|0x1e|*/ short PathOri[15];
  /*<thisrel this+0x170>*/ /*|0x1e|*/ short PathX[15];
  /*<thisrel this+0x18e>*/ /*|0x1e|*/ short PathY[15];
  /*<thisrel this+0x1ac>*/ /*|0xf|*/ char PathDir[15];
  /*<thisrel this+0x1bc>*/ /*|0x4|*/ unsigned long PathTime;
  /*<thisrel this+0x1c0>*/ /*|0x1|*/ char m_MoveGateNumber;
  /*<thisrel this+0x1c4>*/ /*|0x4|*/ unsigned long Authority;
  /*<thisrel this+0x1c8>*/ /*|0x4|*/ unsigned long AuthorityCode;
  /*<thisrel this+0x1cc>*/ /*|0x4|*/ unsigned long Penalty;
  /*<thisrel this+0x1d0>*/ /*|0x1|*/ unsigned char m_cAccountItemBlock;
  /*<thisrel this+0x1d4>*/ /*|0x4|*/ struct tagActionState m_ActState;
  /*<thisrel this+0x1d8>*/ /*|0x1|*/ unsigned char m_ActionNumber;
  /*<thisrel this+0x1dc>*/ /*|0x4|*/ unsigned long m_State;
  /*<thisrel this+0x1e0>*/ /*|0x1|*/ char m_StateSub;
  /*<thisrel this+0x1e1>*/ /*|0x1|*/ unsigned char m_Rest;
  /*<thisrel this+0x1e2>*/ /*|0x1|*/ char m_ViewState;
  /*<thisrel this+0x1e4>*/ /*|0x4|*/ BYTE m_ViewSkillState[MAX_VIEWSKILL];
  /*<thisrel this+0x1e8>*/ /*|0x4|*/ unsigned long m_LastMoveTime;
  /*<thisrel this+0x1ec>*/ /*|0x4|*/ unsigned long m_LastAttackTime;
  /*<thisrel this+0x1f0>*/ /*|0x1|*/ unsigned char m_FriendServerOnline;
  /*<thisrel this+0x1f4>*/ /*|0x4|*/ int m_DetectSpeedHackTime;
  /*<thisrel this+0x1f8>*/ /*|0x4|*/ unsigned long m_SumLastAttackTime;
  /*<thisrel this+0x1fc>*/ /*|0x4|*/ unsigned long m_DetectCount;
  /*<thisrel this+0x200>*/ /*|0x4|*/ int m_DetectedHackKickCount;
  /*<thisrel this+0x204>*/ /*|0x4|*/ int m_SpeedHackPenalty;
  /*<thisrel this+0x208>*/ /*|0x1|*/ unsigned char m_AttackSpeedHackDetectedCount;
  /*<thisrel this+0x20c>*/ /*|0x4|*/ unsigned long m_PacketCheckTime;
  /*<thisrel this+0x210>*/ /*|0x1|*/ unsigned char m_ShopTime;
  /*<thisrel this+0x214>*/ /*|0x4|*/ unsigned long m_TotalAttackTime;
  /*<thisrel this+0x218>*/ /*|0x4|*/ int m_TotalAttackCount;
  /*<thisrel this+0x21c>*/ /*|0x4|*/ unsigned long TeleportTime;
  /*<thisrel this+0x220>*/ /*|0x1|*/ char Teleport;
  /*<thisrel this+0x221>*/ /*|0x1|*/ char KillerType;
  /*<thisrel this+0x222>*/ /*|0x1|*/ char DieRegen;
  /*<thisrel this+0x223>*/ /*|0x1|*/ char RegenOk;
  /*<thisrel this+0x224>*/ /*|0x1|*/ unsigned char RegenMapNumber;
  /*<thisrel this+0x225>*/ /*|0x1|*/ unsigned char RegenMapX;
  /*<thisrel this+0x226>*/ /*|0x1|*/ unsigned char RegenMapY;
  /*<thisrel this+0x228>*/ /*|0x4|*/ unsigned long RegenTime;
  /*<thisrel this+0x22c>*/ /*|0x4|*/ unsigned long MaxRegenTime;
  /*<thisrel this+0x230>*/ /*|0x2|*/ short m_PosNum;
  /*<thisrel this+0x234>*/ /*|0x4|*/ unsigned long LifeRefillTimer;
  /*<thisrel this+0x238>*/ /*|0x4|*/ unsigned long CurActionTime;
  /*<thisrel this+0x23c>*/ /*|0x4|*/ unsigned long NextActionTime;
  /*<thisrel this+0x240>*/ /*|0x4|*/ unsigned long DelayActionTime;
  /*<thisrel this+0x244>*/ /*|0x1|*/ char DelayLevel;
  /*<thisrel this+0x245>*/ /*|0x1|*/ char m_PoisonType;
  /*<thisrel this+0x246>*/ /*|0x1|*/ char m_IceType;
  /*<thisrel this+0x247>*/ /*|0x1|*/ char m_PoisonBeattackCount;
  /*<thisrel this+0x248>*/ /*|0x1|*/ char m_ColdBeattackCount;
  /*<thisrel this+0x249>*/ /*|0x1|*/ char m_ImmuneToMagicCount;
  /*<thisrel this+0x24a>*/ /*|0x1|*/ char m_ImmuneToHarmCount;
  /*<thisrel this+0x24b>*/ /*|0x1|*/ char m_iMonsterBattleDelay;
  /*<thisrel this+0x24c>*/ /*|0x1|*/ char m_cKalimaGateExist;
  /*<thisrel this+0x250>*/ /*|0x4|*/ int m_iKalimaGateIndex;
  /*<thisrel this+0x254>*/ /*|0x1|*/ char m_cKalimaGateEnterCount;
  /*<thisrel this+0x258>*/ /*|0x4|*/ struct OBJECTSTRUCT* lpAttackObj;
  /*<thisrel this+0x25c>*/ /*|0x2|*/ short m_SkillNumber;
  /*<thisrel this+0x260>*/ /*|0x4|*/ unsigned long m_SkillTime;
  /*<thisrel this+0x264>*/ /*|0x1|*/ unsigned char m_bAttackerKilled;
  /*<thisrel this+0x265>*/ /*|0x1|*/ char m_ManaFillCount;
  /*<thisrel this+0x266>*/ /*|0x1|*/ char m_LifeFillCount;
  /*<thisrel this+0x268>*/ /*|0x14|*/ int SelfDefense[5];
  /*<thisrel this+0x27c>*/ /*|0x14|*/ unsigned long SelfDefenseTime[5];
  /*<thisrel this+0x290>*/ /*|0x4|*/ unsigned long MySelfDefenseTime;
  /*<thisrel this+0x294>*/ /*|0x1|*/ char m_Drink;
  /*<thisrel this+0x298>*/ /*|0x4|*/ int m_SkillDefense;
  /*<thisrel this+0x29c>*/ /*|0x1|*/ char m_SkillDefenseTime;
  /*<thisrel this+0x2a0>*/ /*|0x4|*/ int m_SkillAttack;
  /*<thisrel this+0x2a4>*/ /*|0x1|*/ char m_SkillAttackTime;
  /*<thisrel this+0x2a8>*/ /*|0x4|*/ int m_SkillAttack2;
  /*<thisrel this+0x2ac>*/ /*|0x1|*/ char m_SkillAttackTime2;
  /*<thisrel this+0x2b0>*/ /*|0x4|*/ int m_SkillAddLife;
  /*<thisrel this+0x2b4>*/ /*|0x4|*/ int m_SkillAddLifeTime;
  /*<thisrel this+0x2b8>*/ /*|0x4|*/ int m_SkillHarden;
  /*<thisrel this+0x2bc>*/ /*|0x4|*/ int m_SkillHardenTime;
  /*<thisrel this+0x2c0>*/ /*|0x4|*/ int m_SkillMagumReduceDefense;
  /*<thisrel this+0x2c4>*/ /*|0x4|*/ int m_SkillMagumReduceDefenseTime;
  /*<thisrel this+0x2c8>*/ /*|0x4|*/ int PartyNumber;
  /*<thisrel this+0x2cc>*/ /*|0x4|*/ int PartyTargetUser;
  /*<thisrel this+0x2d0>*/ /*|0x4|*/ int GuildNumber;
  /*<thisrel this+0x2d4>*/ /*|0x4|*/ struct _GUILD_INFO_STRUCT* lpGuild;
  /*<thisrel this+0x2d8>*/ /*|0xb|*/ char GuildName[11];
  /*<thisrel this+0x2e4>*/ /*|0x4|*/ int GuildStatus;
  /*<thisrel this+0x2e8>*/ /*|0x4|*/ int iGuildUnionTimeStamp;
  /*<thisrel this+0x2ec>*/ /*|0x4|*/ int m_RecallMon;
  /*<thisrel this+0x2f0>*/ /*|0x4|*/ int m_Change;
  /*<thisrel this+0x2f4>*/ /*|0x2|*/ short TargetNumber;
  /*<thisrel this+0x2f6>*/ /*|0x2|*/ short TargetShopNumber;
  /*<thisrel this+0x2f8>*/ /*|0x2|*/ short ShopNumber;
  /*<thisrel this+0x2fa>*/ /*|0x2|*/ short LastAttackerID;
  /*<thisrel this+0x2fc>*/ /*|0x4|*/ int m_AttackDamageMin;
  /*<thisrel this+0x300>*/ /*|0x4|*/ int m_AttackDamageMax;
  /*<thisrel this+0x304>*/ /*|0x4|*/ int m_MagicDamageMin;
  /*<thisrel this+0x308>*/ /*|0x4|*/ int m_MagicDamageMax;
  /*<thisrel this+0x30c>*/ /*|0x4|*/ int m_AttackDamageLeft;
  /*<thisrel this+0x310>*/ /*|0x4|*/ int m_AttackDamageRight;
  /*<thisrel this+0x314>*/ /*|0x4|*/ int m_AttackDamageMaxLeft;
  /*<thisrel this+0x318>*/ /*|0x4|*/ int m_AttackDamageMinLeft;
  /*<thisrel this+0x31c>*/ /*|0x4|*/ int m_AttackDamageMaxRight;
  /*<thisrel this+0x320>*/ /*|0x4|*/ int m_AttackDamageMinRight;
  /*<thisrel this+0x324>*/ /*|0x4|*/ int m_AttackRating;
  /*<thisrel this+0x328>*/ /*|0x4|*/ int m_AttackSpeed;
  /*<thisrel this+0x32c>*/ /*|0x4|*/ int m_MagicSpeed;
  /*<thisrel this+0x330>*/ /*|0x4|*/ int m_Defense;
  /*<thisrel this+0x334>*/ /*|0x4|*/ int m_MagicDefense;
  /*<thisrel this+0x338>*/ /*|0x4|*/ int m_SuccessfulBlocking;
  /*<thisrel this+0x33c>*/ /*|0x2|*/ short m_MoveSpeed;
  /*<thisrel this+0x33e>*/ /*|0x2|*/ short m_MoveRange;
  /*<thisrel this+0x340>*/ /*|0x2|*/ short m_AttackRange;
  /*<thisrel this+0x342>*/ /*|0x2|*/ short m_AttackType;
  /*<thisrel this+0x344>*/ /*|0x2|*/ short m_ViewRange;
  /*<thisrel this+0x346>*/ /*|0x2|*/ short m_Attribute;
  /*<thisrel this+0x348>*/ /*|0x2|*/ short m_ItemRate;
  /*<thisrel this+0x34a>*/ /*|0x2|*/ short m_MoneyRate;
  /*<thisrel this+0x34c>*/ /*|0x4|*/ int m_CriticalDamage;
  /*<thisrel this+0x350>*/ /*|0x4|*/ int m_ExcelentDamage;
  /*<thisrel this+0x354>*/ /*|0x4|*/ class CMagicInf* m_lpMagicBack;
  /*<thisrel this+0x358>*/ /*|0x4|*/ class CMagicInf* Magic;
  /*<thisrel this+0x35c>*/ /*|0x1|*/ char MagicCount;
  /*<thisrel this+0x35d>*/ /*|0x1|*/ unsigned char UseMagicNumber;
  /*<thisrel this+0x360>*/ /*|0x4|*/ unsigned long UseMagicTime;
  /*<thisrel this+0x364>*/ /*|0x1|*/ char UseMagicCount;
  /*<thisrel this+0x366>*/ /*|0x2|*/ short OSAttackSerial;
  /*<thisrel this+0x368>*/ /*|0x1|*/ unsigned char SASCount;
  /*<thisrel this+0x36c>*/ /*|0x4|*/ unsigned long SkillAttackTime;
  /*<thisrel this+0x370>*/ /*|0x12|*/ unsigned char CharSet[18];
  /*<thisrel this+0x382>*/ /*|0x7|*/ unsigned char m_Resistance[7];
  /*<thisrel this+0x389>*/ /*|0x7|*/ unsigned char m_AddResistance[7];
  /*<thisrel this+0x390>*/ /*|0x10|*/ int FrustrumX[4];
  /*<thisrel this+0x3a0>*/ /*|0x10|*/ int FrustrumY[4];
  /*<thisrel this+0x3b0>*/ /*|0x384|*/ struct VIEWPORT_STRUCT VpPlayer[75];
  /*<thisrel this+0x734>*/ /*|0x384|*/ struct VIEWPORT_PLAYER_STRUCT VpPlayer2[75];
  /*<thisrel this+0xab8>*/ /*|0x4|*/ int VPCount;
  /*<thisrel this+0xabc>*/ /*|0x4|*/ int VPCount2;
  /*<thisrel this+0xac0>*/ /*|0x1e0|*/ struct HITDAMAGE_STRUCT sHD[40];
  /*<thisrel this+0xca0>*/ /*|0x2|*/ short sHDCount;
  /*<thisrel this+0xca4>*/ /*|0x4|*/ struct tagInterfaceState m_IfState;
  /*<thisrel this+0xca8>*/ /*|0x4|*/ unsigned long m_InterfaceTime;
  /*<thisrel this+0xcac>*/ /*|0x4|*/ class CItem* pInventory;
  /*<thisrel this+0xcb0>*/ /*|0x4|*/ unsigned char* pInventoryMap;
  /*<thisrel this+0xcb4>*/ /*|0x4|*/ char* pInventoryCount;
  /*<thisrel this+0xcb8>*/ /*|0x1|*/ char pTransaction;
  /*<thisrel this+0xcbc>*/ /*|0x4|*/ class CItem* Inventory1;
  /*<thisrel this+0xcc0>*/ /*|0x4|*/ unsigned char* InventoryMap1;
  /*<thisrel this+0xcc4>*/ /*|0x1|*/ char InventoryCount1;
  /*<thisrel this+0xcc8>*/ /*|0x4|*/ class CItem* Inventory2;
  /*<thisrel this+0xccc>*/ /*|0x4|*/ unsigned char* InventoryMap2;
  /*<thisrel this+0xcd0>*/ /*|0x1|*/ char InventoryCount2;
  /*<thisrel this+0xcd4>*/ /*|0x4|*/ class CItem* Trade;
  /*<thisrel this+0xcd8>*/ /*|0x4|*/ unsigned char* TradeMap;
  /*<thisrel this+0xcdc>*/ /*|0x4|*/ int TradeMoney;
  /*<thisrel this+0xce0>*/ /*|0x1|*/ bool TradeOk;
  /*<thisrel this+0xce4>*/ /*|0x4|*/ class CItem* pWarehouse;
  /*<thisrel this+0xce8>*/ /*|0x4|*/ unsigned char* pWarehouseMap;
  /*<thisrel this+0xcec>*/ /*|0x1|*/ char WarehouseCount;
  /*<thisrel this+0xcee>*/ /*|0x2|*/ short WarehousePW;
  /*<thisrel this+0xcf0>*/ /*|0x1|*/ unsigned char WarehouseLock;
  /*<thisrel this+0xcf1>*/ /*|0x1|*/ unsigned char WarehouseUnfailLock;
  /*<thisrel this+0xcf4>*/ /*|0x4|*/ int WarehouseMoney;
  /*<thisrel this+0xcf8>*/ /*|0x4|*/ int WarehouseSave;
  /*<thisrel this+0xcfc>*/ /*|0x4|*/ class CItem* pChaosBox;
  /*<thisrel this+0xd00>*/ /*|0x4|*/ unsigned char* pChaosBoxMap;
  /*<thisrel this+0xd04>*/ /*|0x4|*/ int ChaosMoney;
  /*<thisrel this+0xd08>*/ /*|0x4|*/ int ChaosSuccessRate;
  /*<thisrel this+0xd0c>*/ /*|0x4|*/ int ChaosLock;
  /*<thisrel this+0xd10>*/ /*|0x4|*/ unsigned long m_Option;
  /*<thisrel this+0xd14>*/ /*|0x4|*/ int m_nEventScore;
  /*<thisrel this+0xd18>*/ /*|0x4|*/ int m_nEventExp;
  /*<thisrel this+0xd1c>*/ /*|0x4|*/ int m_nEventMoney;
  /*<thisrel this+0xd20>*/ /*|0x1|*/ unsigned char m_bDevilSquareIndex;
  /*<thisrel this+0xd21>*/ /*|0x1|*/ unsigned char m_bDevilSquareAuth;
  /*<thisrel this+0xd22>*/ /*|0x1|*/ char m_cBloodCastleIndex;
  /*<thisrel this+0xd23>*/ /*|0x1|*/ char m_cBloodCastleSubIndex;
  /*<thisrel this+0xd24>*/ /*|0x4|*/ int m_iBloodCastleEXP;
  /*<thisrel this+0xd28>*/ /*|0x1|*/ bool m_bBloodCastleComplete;
  /*<thisrel this+0xd29>*/ /*|0x1|*/ char m_cChaosCastleIndex;
  /*<thisrel this+0xd2a>*/ /*|0x1|*/ char m_cChaosCastleSubIndex;
  /*<thisrel this+0xd2c>*/ /*|0x4|*/ int m_iChaosCastleBlowTime;
  /*<thisrel this+0xd30>*/ /*|0x1|*/ char m_cKillUserCount;
  /*<thisrel this+0xd31>*/ /*|0x1|*/ char m_cKillMonsterCount;
  /*<thisrel this+0xd34>*/ /*|0x4|*/ int m_iDuelUserReserved;
  /*<thisrel this+0xd38>*/ /*|0x4|*/ int m_iDuelUserRequested;
  /*<thisrel this+0xd3c>*/ /*|0x4|*/ int m_iDuelUser;
  /*<thisrel this+0xd40>*/ /*|0x1|*/ unsigned char m_btDuelScore;
  /*<thisrel this+0xd44>*/ /*|0x4|*/ int m_iDuelTickCount;
  /*<thisrel this+0xd48>*/ /*|0x1|*/ bool m_bPShopOpen;
  /*<thisrel this+0xd49>*/ /*|0x1|*/ bool m_bPShopTransaction;
  /*<thisrel this+0xd4a>*/ /*|0x1|*/ unsigned char m_bPShopItemChange;
  /*<thisrel this+0xd4b>*/ /*|0x1|*/ unsigned char m_bPShopRedrawAbs;
  /*<thisrel this+0xd4c>*/ /*|0x24|*/ char m_szPShopText[36];
  /*<thisrel this+0xd70>*/ /*|0x1|*/ unsigned char m_bPShopWantDeal;
  /*<thisrel this+0xd74>*/ /*|0x4|*/ int m_iPShopDealerIndex;
  /*<thisrel this+0xd78>*/ /*|0xa|*/ char m_szPShopDealerName[10];
  /*<thisrel this+0xd84>*/ /*|0x18|*/ struct _RTL_CRITICAL_SECTION m_critPShopTrade;
  /*<thisrel this+0xd9c>*/ /*|0x12c|*/ int m_iVpPShopPlayer[75];
  /*<thisrel this+0xec8>*/ /*|0x2|*/ unsigned short m_wVpPShopPlayerCount;
  /*<thisrel this+0xeca>*/ /*|0x1|*/ unsigned char IsInBattleGround;
  /*<thisrel this+0xecb>*/ /*|0x1|*/ unsigned char HaveWeaponInHand;
  /*<thisrel this+0xecc>*/ /*|0x2|*/ short EventChipCount;
  /*<thisrel this+0xed0>*/ /*|0x4|*/ int MutoNumber;
  /*<thisrel this+0xed4>*/ /*|0x4|*/ int UseEventServer;
  /*<thisrel this+0xed8>*/ /*|0x1|*/ unsigned char LoadWareHouseInfo;
  /*<thisrel this+0xedc>*/ /*|0x4|*/ int iStoneCount;
  /*<thisrel this+0xee0>*/ /*|0x32|*/ unsigned char m_Quest[50];
  /*<thisrel this+0xf12>*/ /*|0x1|*/ unsigned char m_SendQuestInfo;
  /*<thisrel this+0xf14>*/ /*|0x4|*/ int m_SkyBossMonSheildLinkIndex;
  /*<thisrel this+0xf18>*/ /*|0x4|*/ int m_SkyBossMonSheild;
  /*<thisrel this+0xf1c>*/ /*|0x4|*/ int m_SkyBossMonSheildTime;
  /*<thisrel this+0xf20>*/ /*|0x4|*/ int m_MaxLifePower;
  /*<thisrel this+0xf24>*/ /*|0x4|*/ int m_WizardSkillDefense;
  /*<thisrel this+0xf28>*/ /*|0x4|*/ int m_WizardSkillDefenseTime;
  /*<thisrel this+0xf2c>*/ /*|0x4|*/ int m_PacketChecksumTime;
  /*<thisrel this+0xf30>*/ /*|0x4|*/ int m_CheckLifeTime;
  /*<thisrel this+0xf34>*/ /*|0x1|*/ unsigned char m_MoveOtherServer;
  /*<thisrel this+0xf35>*/ /*|0xb|*/ char BackName[11];
  /*<thisrel this+0xf40>*/ /*|0x1|*/ char m_BossGoldDerconMapNumber;
  /*<thisrel this+0xf41>*/ /*|0x1|*/ unsigned char m_InWebzen;
  /*<thisrel this+0xf42>*/ /*|0x1|*/ char m_LastTeleportTime;
  /*<thisrel this+0xf43>*/ /*|0x1|*/ unsigned char m_ClientHackLogCount;
  /*<thisrel this+0xf44>*/ /*|0x4|*/ int m_bIsInMonsterHerd;
  /*<thisrel this+0xf48>*/ /*|0x4|*/ int m_bIsMonsterAttackFirst;
  /*<thisrel this+0xf4c>*/ /*|0x4|*/ class MonsterHerd* m_lpMonsterHerd;
  /*<thisrel this+0xf7c>*/ /*|0x1|*/ unsigned char m_bSentGGAuth;
  /*<thisrel this+0xf80>*/ /*|0x4|*/ unsigned long m_NPggCheckSumSendTime;
  /*<thisrel this+0xf84>*/ /*|0x10|*/ int fSkillFrustrumX[4];
  /*<thisrel this+0xf94>*/ /*|0x10|*/ int fSkillFrustrumY[4];
  /*<thisrel this+0xfa4>*/ /*|0x1|*/ unsigned char SkillHellFire2State;
  /*<thisrel this+0xfa5>*/ /*|0x1|*/ unsigned char SkillHellFire2Count;
  /*<thisrel this+0xfa8>*/ /*|0x4|*/ unsigned long SkillHellFire2Time;
  /*<thisrel this+0xfac>*/ /*|0x1|*/ unsigned char m_ReqWarehouseOpen;
  /*<thisrel this+0xfb0>*/ /*|0x4|*/ int m_NotAttackAreaCount;
  /*<thisrel this+0xfb4>*/ /*|0x2|*/ short SetOpAddSkillAttack;
  /*<thisrel this+0xfb6>*/ /*|0x2|*/ short SetOpAddExDamage;
  /*<thisrel this+0xfb8>*/ /*|0x2|*/ short SetOpAddExDamageSuccessRate;
  /*<thisrel this+0xfba>*/ /*|0x2|*/ short SetOpAddCriticalDamage;
  /*<thisrel this+0xfbc>*/ /*|0x2|*/ short SetOpAddCriticalDamageSuccessRate;
  /*<thisrel this+0xfbe>*/ /*|0x2|*/ short SetOpIncAGValue;
  /*<thisrel this+0xfc0>*/ /*|0x2|*/ short SetOpAddDamage;
  /*<thisrel this+0xfc2>*/ /*|0x2|*/ short SetOpAddMinAttackDamage;
  /*<thisrel this+0xfc4>*/ /*|0x2|*/ short SetOpAddMaxAttackDamage;
  /*<thisrel this+0xfc6>*/ /*|0x2|*/ short SetOpAddAttackDamage;
  /*<thisrel this+0xfc8>*/ /*|0x2|*/ short SetOpAddDefence;
  /*<thisrel this+0xfca>*/ /*|0x2|*/ short SetOpAddDefenceRate;
  /*<thisrel this+0xfcc>*/ /*|0x2|*/ short SetOpAddMagicPower;
  /*<thisrel this+0xfce>*/ /*|0x1|*/ unsigned char SetOpIgnoreDefense;
  /*<thisrel this+0xfcf>*/ /*|0x1|*/ unsigned char SetOpDoubleDamage;
  /*<thisrel this+0xfd0>*/ /*|0x1|*/ unsigned char SetOpTwoHandSwordImproveDamage;
  /*<thisrel this+0xfd1>*/ /*|0x1|*/ unsigned char SetOpImproveSuccessAttackRate;
  /*<thisrel this+0xfd2>*/ /*|0x1|*/ unsigned char SetOpReflectionDamage;
  /*<thisrel this+0xfd3>*/ /*|0x1|*/ unsigned char SetOpImproveSheldDefence;
  /*<thisrel this+0xfd4>*/ /*|0x1|*/ unsigned char SetOpDecreaseAG;
  /*<thisrel this+0xfd5>*/ /*|0x1|*/ unsigned char SetOpImproveItemDropRate;
  /*<thisrel this+0xfd6>*/ /*|0x1|*/ unsigned char IsFullSetItem;
  /*<thisrel this+0xfd8>*/ /*|0x12c|*/ class TDurMagicKeyChecker DurMagicKeyChecker;
  /*<thisrel this+0x1104>*/ /*|0x2|*/ unsigned short SkillRecallParty_Time;
  /*<thisrel this+0x1106>*/ /*|0x1|*/ unsigned char SkillRecallParty_MapNumber;
  /*<thisrel this+0x1107>*/ /*|0x1|*/ unsigned char SkillRecallParty_X;
  /*<thisrel this+0x1108>*/ /*|0x1|*/ unsigned char SkillRecallParty_Y;
  /*<thisrel this+0x110a>*/ /*|0x2|*/ unsigned short SkillAddCriticalDamageTime;
  /*<thisrel this+0x110c>*/ /*|0x2|*/ unsigned short SkillAddCriticalDamage;
  /*<thisrel this+0x110e>*/ /*|0x1|*/ bool bIsChaosMixCompleted;
  /*<thisrel this+0x110f>*/ /*|0x1|*/ unsigned char SkillLongSpearChange;
  /*<thisrel this+0x1110>*/ /*|0x404|*/ class CSkillDelay SkillDelay;
  /*<thisrel this+0x1514>*/ /*|0x4|*/ int iObjectSecTimer;
  /*<thisrel this+0x1518>*/ /*|0x1|*/ bool m_bMapSvrMoveQuit;
  /*<thisrel this+0x1519>*/ /*|0x1|*/ bool m_bMapSvrMoveReq;
  /*<thisrel this+0x151c>*/ /*|0x4|*/ unsigned long m_dwMapSvrQuitTick;
  /*<thisrel this+0x1520>*/ /*|0x2|*/ short m_sPrevMapSvrCode;
  /*<thisrel this+0x1522>*/ /*|0x2|*/ short m_sDestMapNumber;
  /*<thisrel this+0x1524>*/ /*|0x1|*/ unsigned char m_btDestX;
  /*<thisrel this+0x1525>*/ /*|0x1|*/ unsigned char m_btDestY;
  /*<thisrel this+0x1528>*/ /*|0x1|*/ unsigned char m_btCsNpcExistVal1;
  /*<thisrel this+0x1529>*/ /*|0x1|*/ unsigned char m_btCsNpcExistVal2;
  /*<thisrel this+0x152a>*/ /*|0x1|*/ unsigned char m_btCsNpcExistVal3;
  /*<thisrel this+0x152b>*/ /*|0x1|*/ unsigned char m_btCsNpcExistVal4;
  /*<thisrel this+0x1528>*/ /*|0x4|*/ int m_iCsNpcExistVal;
  /*<thisrel this+0x152c>*/ /*|0x1|*/ unsigned char m_btCsNpcType;
  /*<thisrel this+0x152d>*/ /*|0x1|*/ unsigned char m_btCsGateOpen;
  /*<thisrel this+0x1530>*/ /*|0x4|*/ int m_iCsGateLeverLinkIndex;
  /*<thisrel this+0x1534>*/ /*|0x1|*/ unsigned char m_btCsNpcDfLevel;
  /*<thisrel this+0x1535>*/ /*|0x1|*/ unsigned char m_btCsNpcRgLevel;
  /*<thisrel this+0x1536>*/ /*|0x1|*/ unsigned char m_btCsJoinSide;
  /*<thisrel this+0x1537>*/ /*|0x1|*/ unsigned char m_bCsGuildInvolved;
  /*<thisrel this+0x1538>*/ /*|0x1|*/ bool m_bIsCastleNPCUpgradeCompleted;
  /*<thisrel this+0x1539>*/ /*|0x1|*/ unsigned char m_btWeaponState;
  /*<thisrel this+0x153c>*/ /*|0x4|*/ int m_iWeaponUser;
  /*<thisrel this+0x1540>*/ /*|0x1|*/ unsigned char m_btKillCount;
  /*<thisrel this+0x1544>*/ /*|0x4|*/ int m_iSkillStunTime;
  /*<thisrel this+0x1548>*/ /*|0x4|*/ int m_iSkillBrandOfSkillTime;
  /*<thisrel this+0x154c>*/ /*|0x4|*/ int m_iSkillInvisibleTime;
  /*<thisrel this+0x1550>*/ /*|0x4|*/ int m_iSkillManaSwellTime;
  /*<thisrel this+0x1554>*/ /*|0x4|*/ int m_iSkillManaSwell;
  /*<thisrel this+0x1558>*/ /*|0x4|*/ int m_iAccumulatedDamage;
  /*<thisrel this+0x155c>*/ /*|0x4|*/ int m_iPotionBlessTime;
  /*<thisrel this+0x1560>*/ /*|0x4|*/ int m_iPotionSoulTime;
  /*<thisrel this+0x1564>*/ /*|0x1|*/ unsigned char m_btLifeStoneCount;
  /*<thisrel this+0x1565>*/ /*|0x1|*/ unsigned char m_btCreationState;
  /*<thisrel this+0x1568>*/ /*|0x4|*/ int m_iCreatedActivationTime;
  /*<thisrel this+0x156c>*/ /*|0x4|*/ int m_iAccumulatedCrownAccessTime;
  /*<thisrel this+0x1570>*/ /*|0x50|*/ class TMonsterSkillElementInfo m_MonsterSkillElementInfo;
  /*<thisrel this+0x15c0>*/ /*|0x4|*/ int m_iBasicAI;
  /*<thisrel this+0x15c4>*/ /*|0x4|*/ int m_iCurrentAI;
  /*<thisrel this+0x15c8>*/ /*|0x4|*/ int m_iCurrentAIState;
  /*<thisrel this+0x15cc>*/ /*|0x4|*/ int m_iLastAIRunTime;
  /*<thisrel this+0x15d0>*/ /*|0x4|*/ int m_iGroupNumber;
  /*<thisrel this+0x15d4>*/ /*|0x4|*/ int m_iSubGroupNumber;
  /*<thisrel this+0x15d8>*/ /*|0x4|*/ int m_iGroupMemberGuid;
  /*<thisrel this+0x15dc>*/ /*|0x4|*/ int m_iRegenType;
  /*<thisrel this+0x15e0>*/ /*|0x324|*/ class TMonsterAIAgro m_Agro;
  /*<thisrel this+0x1904>*/ /*|0x4|*/ int m_iLastAutomataRuntime;
  /*<thisrel this+0x1908>*/ /*|0x4|*/ int m_iLastAutomataDelay;
  /*<thisrel this+0x190c>*/ /*|0x4|*/ int m_iCrywolfMVPScore;
  /*<thisrel this+0x1910>*/ /*|0x4|*/ unsigned long m_dwLastCheckTick;
  /*<thisrel this+0x1914>*/ /*|0x4|*/ int m_iAutoRecuperationTime;
  /*<thisrel this+0x1918>*/ /*|0x4|*/ int m_iSkillNPCHelpTime;
  /*<thisrel this+0x191c>*/ /*|0x4|*/ int m_iSkillNPCDefense;
  /*<thisrel this+0x1920>*/ /*|0x4|*/ int m_iSkillNPCAttack;
  /*<thisrel this+0x1924>*/ /*|0x4|*/ int m_iMuseElfInfinityArrowSkillTime;
  /*<thisrel this+0x1928>*/ /*|0x4|*/ int m_iSkillDistanceErrorCount;
  /*<thisrel this+0x192c>*/ /*|0x4|*/ unsigned long m_dwSkillDistanceErrorTick;
  /*<thisrel this+0x1930>*/ /*|0x26|*/ struct JEWELOFHARMONY_ITEM_EFFECT m_JewelOfHarmonyEffect;
  /*<thisrel this+0x1956>*/ /*|0x10|*/ struct ITEMOPTION_FOR380ITEM_EFFECT m_ItemOptionExFor380;
  /*<thisrel this+0x1966>*/ /*|0x2|*/ unsigned short m_wItemEffectType;
  /*<thisrel this+0x1968>*/ /*|0x4|*/ int m_iItemEffectValidTime;
  /*<thisrel this+0x196c>*/ /*|0x2|*/ unsigned short m_wCashPoint;
	int m_iPeriodItemEffectIndex;	// 195C
	WORD m_wExprienceRate;	// 1960
	WORD m_wItemDropRate;	// 1962
	BYTE m_btMoveMapBound;	// 1964
	BYTE m_btSealIndex;  // 1968
	ITEM_SOCKET_EFFECT ItemSocketEffect;
	int PCPoints;
	DWORD PCPointsTime;
	float PCBangAddExperience;
	int m_iDuelRoom; // novo
	int m_PeriodItemTime;
	int m_SummonerSkillSleepTime;
	int m_SummonerSkillReflex;
	int m_SummonerSkillReflexTime;
	int m_SummonerSkillSpeedUp;
	int m_SummonerSkillSpeedUpTime;
	int m_SummonerSkillDefenseUp;
	int m_SummonerSkillDefenseUpTime;
	int m_SummonerSkillSwordPower;
	int m_SummonerSkillSwordPowerTime;
	int m_SummonerSkillNightTime;
	int m_SummonerSkillNight;

	int m_RageFighterSkillDefenseRate;
	int m_RageFighterSkillDefenseRateTime;
	int m_RageFighterSkillFitnessAddLife;
	int m_RageFighterSkillFitnessTime;
	int m_RageFighterSkillRedArmorIgnore;
	int m_RageFighterSkillRedArmorIgnoreTime;
	int m_RageFighterSkillPowerIncrease;
	
	int GenFamily;
	int GenLevel;
	int GenReward;
	int GenContribution;
	int GenNextContribution;
	int GenRanking;
	int GenNPC;

	int QuestNPC;

	int LittleSanta1;
	int LittleSantaTime1;
	int LittleSanta2;
	int LittleSantaTime2;
	int LittleSanta3;
	int LittleSantaTime3;
	int LittleSanta4;
	int LittleSantaTime4;
	int LittleSanta5;
	int LittleSantaTime5;
	int LittleSanta6;
	int LittleSantaTime6;

	int CherryBlossom1;
	int CherryBlossom2;
	int CherryBlossom3;

	int Halloween1;
	int Halloween2;
	int Halloween3;
	int Halloween4;
	int Halloween5;

	int OldShoeTime;
	int OldShoeValue;

	int QuestRegistered;
	int QuestTutorial;

	BYTE NewQuestInfo[600];

	int SkillMagicCircleTime;
	int SkillMagicCircle;

	int SpecialScrollEffect1[20];
	int SpecialScrollEffect2[20];
	int SpecialScrollTime[20];

	int SpecialElixirEffect1[20];
	int SpecialElixirEffect2[20];
	int SpecialElixirTime[20];

	int SkillRecoveryTime;
	int SkillRecovery;

	int GladiatorsGlory;
	int GladiatorsGloryTime;

	unsigned char IsMarried;
	unsigned char SendRequest;
	unsigned char RecvRequest;
	char SendRequestName[11];
	char RecvRequestName[11];
	char MarryName[11];

	int m_SkillSleepTime;
	int m_SkillReflex;
	int m_SkillReflexTime;
	int m_SkillWeakness;
	int m_SkillWeaknessTime;
	int m_SkillInnovation;
	int m_SkillInnovationTime;
	int m_SkillBerserker;
	int m_SkillBerserkerTime;
	int m_SkillNightTime;
	int m_SkillNight;

	MASTER_SKILL_TREE_EFFECT MasterSkillTreeEffect;
};
#endif

typedef OBJECTSTRUCT * LPOBJ;


//-- SizeOf ( BILL_CLASS ) == 0x18
class BILL_CLASS
{
private:
	char cCertifyType;	// 0
	BYTE PayCode;		// 1
	int EndTime;		// 4
	char EndsDays[13];	// 8

public:
	BILL_CLASS()
	{
		this->Init();
	}

	void Init()
	{
		this->cCertifyType=-1;
		this->PayCode=0;
		this->EndTime=0;
		this->EndsDays[0]=0;
	}

	BOOL SetBill(BYTE certify_type, BYTE pay, DWORD end_time, char* ends_days)	// line : 228
	{
		BOOL bRet=0;

		if ( this->cCertifyType == -1 )
		{
			bRet = 1;
		}

		this->cCertifyType = certify_type;
		this->PayCode = pay;
		this->EndTime = end_time;
		strcpy(this->EndsDays, ends_days);
		return bRet;
	}

	BYTE GetPayCode() {return this->PayCode;}
	char* GetEndsDays() {return &this->EndsDays[0];}	
	int GetEndTime() { return this->EndTime;}
	int GetCertify() {return this->cCertifyType;}
};

//-- Include --//
#include "protocol.h"

//extern CViewportGuild ViewGuildMng;
extern OBJECTSTRUCT gObj[OBJMAX];
extern BILL_CLASS m_ObjBill[OBJMAX];


extern int gObjCSFlag;
extern int gItemLoop;
extern int gItemLoop2;
extern int gItemLoopMax;
extern int gObjTotalUser;
extern int gDisconnect;
extern int gCurConnectUser; 
extern int skillSuccess;
extern int GuildInfoOfs;
extern int GuildInfoCount;
extern int GuilUserOfs;
extern int GuildUserCount;
extern int lOfsChange;
extern int ChangeCount;

extern int gObjCallMonCount;
extern int gObjMonCount;
extern int gObjCount;

// ----------------------------------------------------------------------------------------------------------------------------------------//
// ::: MathLib																															   //
// ----------------------------------------------------------------------------------------------------------------------------------------//
typedef float vec_t;
typedef vec_t vec3_t[3];
// ------------------------------------------------------------------------------------------------------------------------------------------
#define	PITCH				0									// UP / DOWN
#define	YAW					1									// LEFT / RIGHT
#define	ROLL				2									// FALL OVER
#define M_PI				3.14159265358979323846				// MATCHES VALUES IN GCC V2 MATHLIB.H
// ------------------------------------------------------------------------------------------------------------------------------------------
#define MT_IN1				in1[3*4]
#define MT_IN2				in2[3*4]
#define MT_OUT				out[3*4]
// ----------------------------------------------------------------------------------------------------------------------------------------//

void gObjCalCharacter(int aIndex);
void gObjSkillUseProcTime500(LPOBJ lpObj);
void MonsterAndMsgProc();
void gObjInit();
void gObjEnd();

void gObjClearViewportOfMine(LPOBJ lpObj);
void gObjClearViewport(LPOBJ lpObj);
void gObjCloseSet(int aIndex, int Flag);
void gObjCharTradeClear(LPOBJ lpObj);
void gObjCharZeroSet(int aIndex);
int gObjGetSocket(SOCKET socket);
void gObjSetTradeOption(int aIndex, int option);
void gObjSetDuelOption(int aIndex, int option);

void DbItemSetInByte(LPOBJ lpObj, struct SDHP_DBCHAR_INFORESULT* lpMsg, int ItemDbByte, bool* bAllItemExist);
int gObjSetCharacter(unsigned char* lpdata, int aIndex);
void ItemIsBufExOption(unsigned char* buf, class CItem* lpItem);
bool gObjSetPosMonster(int aIndex, int PosTableNum);
int gObjSetMonster(int aIndex, int MonsterClass);
void gObjDestroy(SOCKET aSocket, int client);
short gObjAddSearch(unsigned int aSocket, char* ip);
short gObjAdd(SOCKET aSocket, char* ip, int aIndex);
short gObjMonsterRecall(int iMapNumber);
short gObjAddMonster(int iMapNumber);
short gObjAddCallMon();
void gObjUserKill(int aIndex);
void gObjTradeSave(LPOBJ lpObj, int index);
void gObjBillRequest(LPOBJ lpObj);
short gObjMemFree(int index);
int gObjGameClose(int aIndex);
short gObjDel(int index);
short gObjSometimeClose(int index);

int gObjGetUserNumber(int aIndex);

LPSTR gObjGetAccountId(int aIndex);
int gObjGetIndex(char* szId);
unsigned char GetPathPacketDirPos(int px, int py);
void gObjSetInventory1Pointer(LPOBJ lpObj);
void gObjSetInventory2Pointer(LPOBJ lpObj);
void gObjAddMsgSend(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode);
void gObjAddMsgSendDelay(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode);
void gObjAddMsgSendDelayInSpecificQPos(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int iQPosition);
void gObjAddAttackProcMsgSendDelay(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int SubCode2);
void gObjMsgProc(LPOBJ lpObj);
void gObjStateProc(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode);
void gObjStateAttackProc(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode, int SubCode2);
int gObjBackSpring(LPOBJ lpObj, LPOBJ lpTargetObj);
int BackSpringCheck(int& x, int& y, int& dir, unsigned char map);
int gObjBackSpring2(LPOBJ lpObj, LPOBJ lpTargetObj, int count);
bool gObjLevelUp(LPOBJ lpObj, int addexp, int iMonsterType, int iEventType);
void gObjMonsterItemLoadScript(LPOBJ lpObj, char* script_file);
unsigned char gObjWarehouseInsertItem(LPOBJ lpObj, int type, int index, int level, int pos);
bool gObjGuildMasterCapacity(LPOBJ lpObj);
void gObjNextExpCal(LPOBJ lpObj);
bool gObjAttackQ(LPOBJ lpObj);
void gObjPlayerKiller(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjUserDie(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjInterfaceTimeCheck(LPOBJ lpObj);
void gObjSpriteDamage(LPOBJ lpObj, int damage);
void gObjSecondDurDown(LPOBJ lpObj);
void gObjChangeDurProc(LPOBJ lpObj);
void gObjPenaltyDurDown(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjWeaponDurDown(LPOBJ lpObj, LPOBJ lpTargetObj, int type);
void gObjArmorRandomDurDown(LPOBJ lpObj, LPOBJ lpAttackObj);
bool gObjIsSelfDefense(LPOBJ lpObj, int aTargetIndex);
void gObjCheckSelfDefense(LPOBJ lpObj, int aTargetIndex);
void gObjTimeCheckSelfDefense(LPOBJ lpObj);
int gObjAttack(LPOBJ lpObj, LPOBJ lpTargetObj, class CMagicInf* lpMagic, int magicsend, unsigned char MSBFlag, int AttackDamage, BOOL bCombo);
void gObjMonsterExpDivision(LPOBJ lpMonObj, LPOBJ lpObj, int AttackDamage, int MSBFlag);
int gObjMonsterExpSingle(LPOBJ lpObj, LPOBJ lpTargetObj, int dmg, int tot_dmg, bool& bSendExp);
void gObjExpParty(LPOBJ lpObj, LPOBJ lpTargetObj, int AttackDamage, int MSBFlag);
void gObjMonsterDieLifePlus(LPOBJ lpObj, LPOBJ lpTartObj);
void gObjLifeCheck(LPOBJ lpTargetObj, LPOBJ lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, unsigned short Skill, int iShieldDamage);
void gObjInventoryItemSet(int aIndex, int itempos, unsigned char set_byte);
void gObjInventoryItemBoxSet(int aIndex, int itempos, int xl, int yl, unsigned char set_byte);
bool gObjFixInventoryPointer(int aIndex);
void gObjInventoryItemSet_PShop(int aIndex, int itempos, unsigned char set_byte);
void gObjInventoryItemBoxSet_PShop(int aIndex, int itempos, int xl, int yl, unsigned char set_byte);
unsigned char gObjInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int iSerial, int iDur);
DWORD gGetItemNumber();
void gPlusItemNumber();
unsigned char gObjInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int iSerial, int iDur);
unsigned char gObjOnlyInventoryInsertItem(int aIndex, class CItem item);
unsigned char gObjInventoryInsertItem(LPOBJ lpObj, int type, int index, int level);
unsigned char gObjShopBuyInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int iSerial, int iDur);
unsigned char gObjShopBuyInventoryInsertItem(int aIndex, class CItem item);
unsigned char gObjInventoryInsertItemTemp(LPOBJ lpObj, class CMapItem* Item);
unsigned char gObjInventoryInsertItem(int aIndex, class CMapItem* item);
unsigned char gObjInventoryInsertItem(int aIndex, class CItem item);
unsigned char gObjMonsterInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int op1, int op2, int op3,BYTE * SocketOption);
unsigned char gObjInventoryRectCheck(int aIndex, int sx, int sy, int width, int height);
unsigned char gObjOnlyInventoryRectCheck(int aIndex, int sx, int sy, int width, int height);
bool CheckInventoryEmptySpace(LPOBJ lpObj, int iItemHeight, int iItemWidth);
unsigned char gObjWerehouseRectCheck(int aIndex, int sx, int sy, int width, int height);
unsigned char gObjMapRectCheck(unsigned char* lpMapBuf, int sx, int sy, int ex, int ey, int width, int height);
void gObjWarehouseItemBoxSet(int aIndex, int itempos, int xl, int yl, unsigned char set_byte);
void gObjWarehouseItemSet(int aIndex, int itempos, unsigned char set_byte);
void gObjChaosItemBoxSet(int aIndex, int itempos, int xl, int yl, unsigned char set_byte);
void gObjChaosItemSet(int aIndex, int itempos, unsigned char set_byte);
unsigned char gObjChaosBoxInsertItemPos(int aIndex, class CItem item, int pos, int source);
unsigned char gObjWarehouseInsertItemPos(int aIndex, class CItem item, int pos, int source);
unsigned char gObjInventoryInsertItemPos(int aIndex, class CItem item, int pos, int RequestCheck);
unsigned char gObjInventoryMoveItem(int aIndex, unsigned char source, unsigned char target, int& durSsend, int& durTsend, unsigned char sFlag, unsigned char tFlag, unsigned char* siteminfo);
unsigned char gObjTradeRectCheck(int aIndex, int sx, int sy, int width, int height);
int gObjTradeItemBoxSet(int aIndex, int itempos, int xl, int yl, unsigned char set_byte);
unsigned char gObjTradeInventoryMove(LPOBJ lpObj, unsigned char source, unsigned char target);
unsigned char gObjInventoryTradeMove(LPOBJ lpObj, unsigned char source, unsigned char target);
unsigned char gObjTradeTradeMove(LPOBJ lpObj, unsigned char source, unsigned char target);
void gObjTradeCancel(int aIndex);
void gObjTempInventoryItemBoxSet(unsigned char* TempMap, int itempos, int xl, int yl, unsigned char set_byte);
unsigned char gObjTempInventoryRectCheck(unsigned char* TempMap, int sx, int sy, int width, int height);
unsigned char gObjTempInventoryInsertItem(LPOBJ lpObj, class CItem item, unsigned char* TempMap);
void gObjTradeOkButton(int aIndex);
void gObjMakePreviewCharSet(int aIndex);
void gObjViewportPaint(HWND hWnd, short aIndex);
void InitFrustrum();
void CreateFrustrum(int x, int y, int aIndex);
void gObjViewportClose(LPOBJ lpObj);
void gObjViewportListCreate(short aIndex);
int ViewportAdd(int aIndex, int aAddIndex, int aType);
int ViewportAdd2(int aIndex, int aAddIndex, int aType);
void ViewportDel(short aIndex, int aDelIndex);
void Viewport2Del(short aIndex, int aDelIndex);
void gObjViewportAllDel(short aIndex);
void gObjViewportListDestroy(short aIndex);
void PShop_ViewportListRegenarate(short aIndex);
bool PShop_CheckInventoryEmpty(short aIndex);
void gObjStateSetCreate(int aIndex);
void gObjSetState();
void gObjSecondProc();
void gObjManaPotionFill(LPOBJ lpObj);
void gObjRestPotionFill(LPOBJ lpObj);
void gObjUseDrink(LPOBJ lpObj, int level);
void gObjViewportListProtocolDestroy(LPOBJ lpObj);
void gObjViewportListProtocolCreate(LPOBJ lpObj);
void gObjViewportListProtocol(short aIndex);
void gObjSkillUseProc(LPOBJ lpObj);
void gObjSkillBeAttackProc(LPOBJ lpObj);
int gObjMoveGate(int aIndex, int gt);
void gObjAbilityReSet(LPOBJ lpObj);
void gObjGuildWarEndSend(struct _GUILD_INFO_STRUCT* lpGuild1, struct _GUILD_INFO_STRUCT* lpGuild2, unsigned char Result1, unsigned char Result2);
void gObjGuildWarEndSend(LPOBJ lpObj, unsigned char Result1, unsigned char Result2);
void gObjGuildWarEnd(struct _GUILD_INFO_STRUCT* lpGuild, struct _GUILD_INFO_STRUCT* lpTargetGuild);
bool gObjMonsterCall(int aIndex, int MonsterType, int x, int y);
void gObjMonsterCallKill(int aIndex);
bool gObjCheckAttackAreaUsedPath(int aIndex, int TarObjIndex);
bool gObjCheckattackAreaUsedViewPort(int aIndex, int TarObjIndex);
int gObjCheckAttackArea(int aIndex, int TarObjIndex);
bool gUserFindDevilSquareInvitation(int aIndex);
bool gUserFindDevilSquareKeyEyes(int aIndex);
void gObjSendUserStatistic(int aIndex, int startLevel, int endLevel);
LPOBJ gObjFind(char* targetcharname);
int gObjGetItemCountInEquipment(int aIndex, int itemtype, int itemindex, int itemlevel);
int gObjGetItemCountInIventory(int aIndex, int itemnum);
int gObjGetItemCountInIventory(int aIndex, int itemtype, int itemindex, int itemlevel);
int gObjGetManaItemPos(int aIndex);
void gObjDelteItemCountInInventory(int aIndex, int itemtype, int itemindex, int count);
void gObjGetStatPointState(int aIndex, short& AddPoint, short& MaxAddPoint, short& MinusPoint, short& MaxMinusPoint);
bool gObjCheckStatPointUp(int aIndex);
bool gObjCheckStatPointDown(int aIndex);
void gObjUseCircle(int aIndex, int pos);
void gObjUsePlusStatFruit(int aIndex, int pos);
void gObjUseMinusStatFruit(int aIndex, int pos);
void gObjDelayLifeCheck(int aIndex);
void gObjCheckAllUserDuelStop();
void SkillFrustrum(unsigned char bangle, int aIndex);
bool SkillTestFrustrum(int x, int y, int aIndex);
void MakeRandomSetItem(int aIndex);
void MakeRewardSetItem(int aIndex, BYTE cDropX, BYTE cDropY, int iRewardType, BYTE iMapnumber);
void gObjRecall(int aIndex, int mapnumber, int x, int y);
void gObjSetExpPetItem(int aIndex, int exp);
int gObjCheckAttackTypeMagic(int iClass, int iSkill);
int gObjGetGuildUnionNumber(LPOBJ lpObj);
void gObjGetGuildUnionName(LPOBJ lpObj, char* szUnionName, int iUnionNameLen);
int gObjCheckRival(LPOBJ lpObj, LPOBJ lpTargetObj);
int gObjGetRelationShip(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjNotifyUpdateUnionV1(LPOBJ lpObj);
void gObjNotifyUpdateUnionV2(LPOBJ lpObj);
void gObjUnionUpdateProc(int aIndex);
void gObjSetKillCount(int aIndex, int iOption);
void gObjNotifyUseWeaponV1(LPOBJ lpOwnerObj, LPOBJ lpWeaponObj, int iTargetX, int iTargetY);
void gObjNotifyUseWeaponDamage(LPOBJ lpWeaponObj, int iTargetX, int iTargetY);
void gObjUseBlessAndSoulPotion(int aIndex, int iItemLevel);
void gObjWeaponDurDownInCastle(LPOBJ lpObj, LPOBJ lpTargetObj, int iDecValue);
void gObjReady4Relife(LPOBJ lpObj);
void gObjCheckTimeOutValue(LPOBJ lpObj, DWORD& rNowTick);
void MsgOutput(int aIndex, char* msg, ...);
void gProcessAutoRecuperation(LPOBJ lpObj);
void gObjShieldAutoRefill(LPOBJ lpObj);
int gObjCheckOverlapItemUsingDur(int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel);
int gObjOverlapItemUsingDur(class CItem* lpItem, int iMapNumber, int iItemNumber, int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel);
bool gCheckSkillDistance(int aIndex, int aTargetIndex, int iSkillNum);
void gObjSaveChaosBoxItemList(LPOBJ lpObj);
int gObjGetRandomFreeArea(int iMapNumber,unsigned char & cX,unsigned char & cY,int iSX,int iSY,int iDX,int iDY,int iLoopCount);
int gObjViewSkillBufferConvert(LPOBJ lpObj, LPWORD lpViewSkillBuffer);
#if(!GS_CASTLE)
void gObjSetIllusionKillCount(int aIndex, int iOption, int iCount);
#endif

void VectorRotate (const float MT_IN1,const float MT_IN2,float MT_OUT);
void AngleMatrix (const float *angles, float (*matrix)[4] );

// -----------------------------------------------------------------------------------------------------------------------------------------------------
// Checked Functions...
// -----------------------------------------------------------------------------------------------------------------------------------------------------
// ---- Bools
bool gObjCheckMaxZen(int aIndex, int nAddZen);
bool gObjCheckTeleportArea(int aIndex, BYTE x, BYTE y);
bool gObjLevelUpPointAdd(LPOBJ lpObj, BYTE Type);
bool gObjCanItemTouch(LPOBJ lpObj, int Type);
bool gObjItemLevelUp(LPOBJ lpObj, int SourceSlot, int TargetSlot);
bool gObjItemRandomLevelUp(LPOBJ lpObj, int SourceSlot, int TargetSlot);
bool gObjItemRandomOption3Up(LPOBJ lpObj, int SourceSlot, int TargetSlot);
// -----------------------------------------------------------------------------------------------------------------------------------------------------
// ---- Voids
void gObjTeleportMagicUse(int aIndex, BYTE x, BYTE y);
void gObjAuthorityCodeSet(LPOBJ lpObj);
void gObjCalcMaxLifePower(int aIndex);
void gObjSetBP(int aIndex);
void ElfSoldierBuf(LPOBJ lpObj);
int gObjGetCompleteItemCountInIventory(int aIndex, int itemtype, int itemindex, int itemlevel, int itemluck, int itemskill, int itemadd, int itemexe);
// -----------------------------------------------------------------------------------------------------------------------------------------------------
// ---- Ints || BOOLS
int gObjCalDistance(LPOBJ lpObj,LPOBJ lpTarget);
int LevelSmallConvert(int LevelReturn);
int LevelSmallConvert(int aIndex, int InventoryIndex);
// -----------------------------------------------------------------------------------------------------------------------------------------------------
// ---- LOGS
void gObjItemTextSave(LPOBJ lpObj);
void gObjWarehouseTextSave(LPOBJ lpObj);

// ---- PETS
bool gObjWingSprite(LPOBJ lpObj);
bool gObjAngelSprite(LPOBJ lpObj);
bool gObjSatanSprite(LPOBJ lpObj);
bool gObjUniriaSprite(LPOBJ lpObj);
bool gObjDenorantSprite(LPOBJ lpObj);
bool gObjDarkHorse(LPOBJ lpObj);
bool gObjFenrir(LPOBJ lpObj);
bool gObjDemonSprite(LPOBJ lpObj);
bool gObjGuardianSprite(LPOBJ lpObj);
bool gObjRudolfSprite(LPOBJ lpObj);
bool gObjPandaSprite(LPOBJ lpObj);
bool gObjSkeletonSprite(LPOBJ lpObj);
// ---- MAGICS & SKILLS
bool gObjCheckMagic(LPOBJ lpObj, WORD aSkill);
class CMagicInf* gObjGetMagic(LPOBJ lpObj, int mIndex);
class CMagicInf* gObjGetMagicSearch(LPOBJ lpObj, unsigned short skillnumber);
int gObjMagicManaUse(LPOBJ lpObj, class CMagicInf* lpMagic);
int gObjMagicBPUse(LPOBJ lpObj, class CMagicInf* lpMagic);
void gObjMagicAddEnergyCheckOnOff(int flag);
int gObjMagicAdd(LPOBJ lpObj, unsigned short aSkill, unsigned char Level);
int gObjMagicDel(LPOBJ lpObj, unsigned short aSkill, unsigned char Level);
int gObjMagicAdd(LPOBJ lpObj, unsigned char Type, unsigned char Index, unsigned char Level, unsigned short& SkillNumber);
int gObjWeaponMagicAdd(LPOBJ lpObj, WORD aSkill, BYTE Level);
int gObjMonsterMagicAdd(LPOBJ lpObj, WORD aSkill, BYTE Level);
bool gObjMagicEnergyCheck(LPOBJ lpObj, unsigned short aSkill);

bool IsSelfDefense(LPOBJ lpObj,LPOBJ lpTargetObj);

// ---- CHANGED

bool gObjIsGamePlaing(LPOBJ lpObj);
bool gObjIsConnectedGP(int aIndex, char* CharName);
bool gObjIsConnectedGP(int aIndex);
bool gObjIsAccontConnect(int aIndex, char* accountid);
bool gObjJoominCheck(int aIndex, char* szInJN);
bool gObjUserIdConnectCheck(char* szId, int index);
bool gObjGetNumber(int aIndex, int& UN, int& DBN);
bool gObjSetAccountLogin(int aIndex, char* szId, int aUserNumber, int aDBNumber, char* JoominNumber);

bool retResistance(LPOBJ lpObj, int Resistance_Type);
bool retCalcSkillResistance(LPOBJ lpTargetObj, int isDouble);

bool gObjInventoryTrans(int aIndex);
bool gObjInventoryCommit(int aIndex);
bool gObjInventoryRollback(int aIndex);

void gObjInventoryDeleteItem(int aIndex, int itempos);
void gObjWarehouseDeleteItem(int aIndex, int itempos);
void gObjChaosBoxDeleteItem(int aIndex, int itempos);

bool gObjSearchItem(LPOBJ lpObj, int item_type, int add_dur, int nLevel);
bool gObjSearchItemMinus(LPOBJ lpObj, int pos, int m_dur);

bool gObjIsItemPut(LPOBJ lpObj, class CItem* lpItem, int pos);

bool gObjInventorySearchSerialNumber(LPOBJ lpObj, unsigned long serial);
bool gObjWarehouseSearchSerialNumber(LPOBJ lpObj, unsigned long sirial);

bool TradeItemInventoryPutTest(int aIndex);
bool TradeitemInventoryPut(int aIndex);

bool gObjCheckViewport(int aIndex, int x, int y);

bool gObjTargetGuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj);

bool gObjGuildWarProc(struct _GUILD_INFO_STRUCT* lpGuild1, struct _GUILD_INFO_STRUCT* lpGuild2, int score);
bool gObjGuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj);
bool gObjGuildWarMasterClose(LPOBJ lpObj);
bool gObjGuildWarItemGive(struct _GUILD_INFO_STRUCT* lpWinGuild, struct _GUILD_INFO_STRUCT* lpLoseGuild);
bool gObjGetPkTime(LPOBJ lpObj, int& hour, int& min);

bool gObjFind10EventChip(int aIndex);
bool gObjDelete10EventChip(int aIndex);

bool gObjGetRandomItemDropLocation(int iMapNumber, BYTE& cX, BYTE& cY, int iRangeX, int iRangeY, int iLoopCount);
bool gObjGetRandomFreeLocation(int iMapNumber, BYTE& cX, BYTE& cY, int iRangeX, int iRangeY, int iLoopCount);

class CUser
{
public:
	CUser(void);
	~CUser(void);
	// ---- MAP RANGE
	bool BloodCastleMapRange(int MapNumber);
	bool DevilSquareMapRange(int MapNumber);
	bool ChaosCastleMapRange(int MapNumber);
	bool KalimaMapRange(int MapNumber);
	bool IllusionTempleMapRange(int MapNumber);
	// ---- PLAYER CLASS
	bool gObjIsMasteringLevel(LPOBJ lpObj);
	bool IsMasterClass(short Class);
	bool gObjIsSecondClassComplete(LPOBJ lpObj);
	bool gObjIsRightPlayerClass(BYTE Class);
	// ----
	void WarehouseDBItemSetByte(LPOBJ lpObj, struct SDHP_GETWAREHOUSEDB_SAVE* lpMsg, int ItemDbByte);
	// ---- Used on Quest System
	void gObjInventoryDeleteItemComplete(int aIndex, int ItemType, int Level, int Skill, int Luck, int AddOpt, int Exe);
	// ---- User Buff Time Check
	void BuffTimer(LPOBJ lpObj);
	// ----
	unsigned char ExpandedInventory1InsertItem(LPOBJ lpObj, int type, int index, int level);
	unsigned char ExpandedInventory2InsertItem(LPOBJ lpObj, int type, int index, int level);
	void ExpandedInventory1ItemBoxSet(LPOBJ lpObj,int itempos,int xl,int yl, unsigned char set_byte);
	bool CheckExpandedInventory1EmptySpace(LPOBJ lpObj, int iItemHeight, int iItemWidth);
	bool CheckExpandedInventory2EmptySpace(LPOBJ lpObj, int iItemHeight, int iItemWidth);
	bool CheckExpandedWarehouseEmptySpace(LPOBJ lpObj, int iItemHeight, int iItemWidth);
};

extern CUser gUser;

#endif