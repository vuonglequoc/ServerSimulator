//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Chaos Box Structures & Functions						//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef CHAOSBOX_H
#define CHAOSBOX_H

#include "protocol.h"
#include "zzzitem.h"

#define CHAOS_BOX_SIZE 32
#define MIN_CHAOS_ITEM_LEVEL 4

#define CHAOS_BOX_RANGE(x) (((x)<0)?FALSE:((x)>CHAOS_BOX_SIZE-1)?FALSE:TRUE )

enum CHAOS_TYPE
{
	CHAOS_TYPE_DEFAULT									= 0x01,
	CHAOS_TYPE_DEVILSQUARE								= 0x02,
	CHAOS_TYPE_UPGRADE_10								= 0x03,
	CHAOS_TYPE_UPGRADE_11								= 0x04,
	CHAOS_TYPE_UPGRADE_12								= 0x16,
	CHAOS_TYPE_UPGRADE_13								= 0x17,
	CHAOS_TYPE_DINORANT									= 0x05,
	CHAOS_TYPE_FRUIT									= 0x06,
	CHAOS_TYPE_SECOND_WING								= 0x07,
	CHAOS_TYPE_BLOODCATLE								= 0x08,
	CHAOS_TYPE_FIRST_WING								= 0x0B,
	CHAOS_TYPE_SETITEM									= 0x0C,
	CHAOS_TYPE_DARKHORSE								= 0x0D,
	CHAOS_TYPE_DARKSPIRIT								= 0x0E,
	CHAOS_TYPE_CLOAK									= 0x18,
	CHAOS_TYPE_BLESS_POTION								= 0x0F,
	CHAOS_TYPE_SOUL_POTION								= 0x10,
	CHAOS_TYPE_LIFE_STONE								= 0x11,
#if(GS_CASTLE)
  CHAOS_TYPE_CASTLE_ITEM								= 0x12,
#endif
	CHAOS_TYPE_HT_BOX									= 0x14,
	CHAOS_TYPE_FENRIR_01								= 0x19,
	CHAOS_TYPE_FENRIR_02								= 0x1A,
	CHAOS_TYPE_FENRIR_03								= 0x1B,
	CHAOS_TYPE_FENRIR_04								= 0x1C,
	CHAOS_TYPE_COMPOUNDPOTION_LV1						= 0x1E,
	CHAOS_TYPE_COMPOUNTPOTION_LV2						= 0x1F,
	CHAOS_TYPE_COMPOUNTPOTION_LV3						= 0x20,
	CHAOS_TYPE_JEWELOFHARMONY_PURITY					= 0x21,
	CHAOS_TYPE_JEWELOFHARMONY_MIX_SMELTINGITEM			= 0x22,
	CHAOS_TYPE_JEWELOFHARMONY_RESTORE_ITEM				= 0x23,
	CHAOS_TYPE_380_OPTIONITEM							= 0x24,
	CHAOS_TYPE_LOTTERY_MIX								= 0x25,
	CHAOS_TYPE_CONDOR_FEATHER							= 0x26,
	CHAOS_TYPE_THIRD_WING								= 0x27,
	CHAOS_CARD_MIX										= 0x28,
	CHERRY_BLOSSOM_MIX									= 0x29,
	SEED_TYPE_SEEDEXTRACT								= 0x2A,
	SEED_TYPE_SEEDSPHERE								= 0x2B,
	SEED_TYPE_SEEDCALC									= 0x2C,
	SEED_TYPE_NEWMIX2									= 0x2D,
	CHAOS_TYPE_EMPIRE_GUARDIAN_BOOK						= 0x2E,
	CHAOS_TYPE_SEALED_BOX								= 0x2F,
	CHAOS_TYPE_UPGRADE_14								= 0x31,
	CHAOS_TYPE_UPGRADE_15								= 0x32
};

struct PMSG_CHAOSMIXRESULT
{
	PBMSG_HEAD h;
	BYTE Result;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

struct CB_PMSG_REQ_PRESENT_RESULT
{
	PBMSG_HEAD h;
	char AccountId[10];
	char GameId[10];
	int Sequence;
	char present;
	char presentname[50];
	int Seq_present;
};

struct CBUPS_ITEMPRIZESEND	// Confirmed size
{
	PHEADB PHeader;
	char szAccountID[10];
	char szCharName[10];
	int iGameServerCode;
	BYTE btResult;
	int unk20;

};

#define CB_ERROR					0
#define CB_SUCCESS					1
#define CB_NOT_ENOUGH_ZEN			2
#define CB_TOO_MANY_ITEMS			3
#define CB_LOW_LEVEL_USER			4
#define CB_LACKING_MIX_ITEMS		6
#define CB_INCORRECT_MIX_ITEMS		7
#define CB_INVALID_ITEM_LEVEL		8
#define CB_USER_CLASS_LOW_LEVEL		9
#define CB_NO_BC_CORRECT_ITEMS		10
#define CB_BC_NOT_ENOUGH_ZEN		11


class CChaosBox
{
public:
	CChaosBox(void);
	~CChaosBox(void);
	// ----
	bool ChaosBoxCheck(LPOBJ lpObj);
	bool ChaosBoxInit(LPOBJ lpObj);
};

extern CChaosBox  gChaosBox;

BOOL ChaosBoxItemDown(LPOBJ lpObj);
int ChaosBoxMix(LPOBJ lpObj, int & Result2);
void CBUPS_ItemRequest(int aIndex, char* AccountId, char* Name);
void ChaosEventProtocolCore(BYTE protoNum, unsigned char* aRecv, int aLen);
void CBUPR_ItemRequestRecv( CB_PMSG_REQ_PRESENT_RESULT * lpMsg);
int  CheckDevilSquareItem(LPOBJ lpObj, int & eventitemcount, int & itemlevel);
void LogDQChaosItem(LPOBJ lpObj);
BOOL DevilSquareEventChaosMix(LPOBJ lpObj, BOOL bCheckType, int iItemLevel);
void LogPlusItemLevelChaosItem(LPOBJ lpObj, int iPlusMixLevel);
void LogChaosItem(LPOBJ lpObj, LPSTR sLogType);
BOOL PlusItemLevelChaosMix(LPOBJ lpObj, int mixType);
BOOL PegasiaChaosMix(LPOBJ lpObj);
BOOL CircleChaosMix(LPOBJ lpObj);
BOOL WingChaosMix(LPOBJ lpObj);
void DefaultChaosMix(LPOBJ lpObj);
void DevilSquareItemChaosMix(LPOBJ lpObj);
void BloodCastleItemChaosMix(LPOBJ lpObj);
void SetItemChaosMix(LPOBJ lpObj);
void DarkHorseChaosMix(LPOBJ lpObj);
void DarkSpiritChaosMix(LPOBJ lpObj);
void BlessPotionChaosMix(LPOBJ lpObj);
void SoulPotionChaosMix(LPOBJ lpObj);
void LifeStoneChaosMix(LPOBJ lpObj);
#if (GS_CASTLE)
void CastleSpecialItemMix(LPOBJ lpObj);
#endif
void HiddenTreasureBoxItemMix(LPOBJ lpObj);
void Fenrir_01Level_Mix(LPOBJ lpObj);
void Fenrir_02Level_Mix(LPOBJ lpObj);
void Fenrir_03Level_Mix(LPOBJ lpObj);
void Fenrir_04Upgrade_Mix(LPOBJ lpObj);
void ShieldPotionLv1_Mix(LPOBJ lpObj);
void ShieldPotionLv2_Mix(LPOBJ lpObj);
void ShieldPotionLv3_Mix(LPOBJ lpObj);
void LotteryItemMix(LPOBJ lpObj);
void CondorFeatherMix(LPOBJ lpObj);
void WingsLv3ChaosMix(LPOBJ lpObj);
void GetWingAttr(int type,int* Option2,int* Option3,int* NewOption);
BOOL IllusionTempleEventChaosMix(LPOBJ lpObj, BOOL bCheckType, int iItemLevel);
void IllusionTempleItemChaosMix(LPOBJ lpObj);
void CherryBlossomMix(LPOBJ lpObj);
void SecromiconMix(LPOBJ lpObj);
void SealedBoxMix(LPOBJ lpObj);
#endif