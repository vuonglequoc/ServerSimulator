//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Events Structures & Functions						//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef ___EVENT_H
#define ___EVENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"


#define MAX_EVENTCHIP_TYPE	5

#define EVENCHIP_TYPE_RANGE(x)  ( ((x)<0)?FALSE:((x)>MAX_EVENTCHIP_TYPE-1)?FALSE:TRUE )

struct PMSG_REGEVENTCHIP_RESULT
{
	PBMSG_HEAD h;
	BYTE Type;
	short ChipCount;
};

struct PMSG_GETMUTONUMBER_RESULT
{
	PBMSG_HEAD h;
	short MutoNum[3];
};

struct PMSG_EVENTCHIPINFO
{
	PBMSG_HEAD h;
	BYTE Type;
	WORD ChipCount;
	short MutoNum[3];
};

struct PMSG_ANS_VIEW_EC_MN
{
	PBMSG_HEAD h;
	int iINDEX;
	char szUID[11];
	char bSUCCESS;
	short nEVENT_CHIPS;
	int iMUTO_NUM;
};

struct PMSG_ANS_REGISTER_EVENTCHIP
{
	PBMSG_HEAD h;
	int iINDEX;
	BYTE Pos;
	char szUID[11];
	char bSUCCESS;
	short nEVENT_CHIPS;
};

struct PMSG_ANS_REGISTER_MUTONUM
{
	PBMSG_HEAD h;
	int iINDEX;
	char szUID[11];
	char bSUCCESS;
	int iMUTO_NUM;
};

struct PMSG_ANS_RESET_EVENTCHIP
{
	PBMSG_HEAD h;
	int iINDEX;
	char szUID[11];
	char bSUCCESS;
};

struct PMSG_ANS_VIEW_STONES
{
	PBMSG_HEAD h;
	int iINDEX;
	char szUID[11];
	char bSUCCESS;
	int iStoneCount;
};

struct PMSG_ANS_REGISTER_STONES
{
	PBMSG_HEAD h;
	int iINDEX;
	BYTE iPosition;
	char szUID[11];
	char bSUCCESS;
	int iStoneCount;
};

struct PMSG_ANS_DELETE_STONES
{
	PBMSG_HEAD h;
	int iINDEX;
	char szUID[11];
	char bSUCCESS;
	int iStoneCount;
};

struct PMSG_ANS_2ANIV_SERIAL
{
	PBMSG_HEAD h;
	int iINDEX;
	char szUID[11];
	BYTE btIsRegistered;
	int iGiftNumber;
};

struct PMSG_ANS_REG_RINGGIFT
{
	PBMSG_HEAD h;
	int iINDEX;
	char szUID[11];
	BYTE btIsRegistered;
	BYTE btGiftSection;
	BYTE btGiftKind;
};

struct PMSG_ANS_BLOODCASTLE_ENTERCOUNT
{
	PBMSG_HEAD h;
	char AccountID[10];
	char GameID[10];
	int ServerCode;
	int iObjIndex;
	int iLeftCount;
};

struct PMSG_ANS_REG_CC_OFFLINE_GIFT
{
	PBMSG_HEAD h;
	int iINDEX;
	char szUID[11];
	WORD wServerCode;
	char szNAME[11];
	int iResultCode;
	char szGIFT_NAME[50];
};

struct PMSG_ANS_REG_DL_OFFLINE_GIFT
{
	PBMSG_HEAD h;
	int iINDEX;
	char szUID[11];
	WORD wServerCode;
	char szNAME[11];
	int iResultCode;
	char szGIFT_NAME[50];
};

struct PMSG_ANS_REG_HT_OFFLINE_GIFT
{
	PBMSG_HEAD h;
	int iINDEX;
	char szUID[11];
	WORD wServerCode;
	char szNAME[11];
	int iResultCode;
	char szGIFT_NAME[50];
};

extern LPOBJ pEventObj;

void EventChipEventProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen);
void DataSendEventChip(PCHAR pMsg, int size);

#if(GS_CASTLE)
void CastleHuntZoneBossRewardOpen(LPOBJ lpObj,BYTE btMapNumber,BYTE cX, BYTE cY);
#endif
void EGRecvEventChipInfo(PMSG_ANS_VIEW_EC_MN * aRecv);
void EGResultRegEventChip(PMSG_ANS_REGISTER_EVENTCHIP * aRecv);
void EGRecvRegMutoNum( PMSG_ANS_REGISTER_MUTONUM* aRecv);
void EGRecvChangeRena( PMSG_ANS_RESET_EVENTCHIP* aRecv);
void EGRecvStoneInfo( PMSG_ANS_VIEW_STONES* aRecv);
void EGRecvRegStone( PMSG_ANS_REGISTER_STONES* aRecv);
void EGRecvDeleteStone( PMSG_ANS_DELETE_STONES* aRecv);
void EGRecvChangeStones( PMSG_ANS_RESET_EVENTCHIP* aRecv);
void EGRecv2AnvRegSerial( PMSG_ANS_2ANIV_SERIAL* aRecv);
void EGRecvRegRingGift( PMSG_ANS_REG_RINGGIFT* aRecv);
void EGReqBloodCastleEnterCount(int iIndex);
void EGAnsBloodCastleEnterCount( PMSG_ANS_BLOODCASTLE_ENTERCOUNT* lpMsg);
void EGReqRegCCOfflineGift(int iIndex);
void EGAnsRegCCOfflineGift( PMSG_ANS_REG_CC_OFFLINE_GIFT* lpMsg);
void EGReqRegDLOfflineGift(int iIndex);
void EGAnsRegDLOfflineGift( PMSG_ANS_REG_DL_OFFLINE_GIFT* lpMsg);
void EGReqRegHTOfflineGift(int iIndex);
void EGAnsRegHTOfflineGift( PMSG_ANS_REG_HT_OFFLINE_GIFT* lpMsg);
void Japan1StAnivBoxOpen(LPOBJ lpObj, int iBoxLevel);


// -----------------------------------------------------------------------------------------------------------------------------------------------------
// BOX
void LuckyBoxOpenEvent(LPOBJ lpObj);
void StarOfXMasOpenEvent(LPOBJ lpObj);
void FireCrackerOpenEvent(LPOBJ lpObj);
void HeartOfLoveOpenEvent(LPOBJ lpObj);
void SilverMedalOpenEvent(LPOBJ lpObj);
void GoldMedalOpenEvent(LPOBJ lpObj);
void HeavenBoxOpenEvent(LPOBJ lpObj);
void KundunBox01OpenEvent(LPOBJ lpObj);
void KundunBox02OpenEvent(LPOBJ lpObj);
void KundunBox03OpenEvent(LPOBJ lpObj);
void KundunBox04OpenEvent(LPOBJ lpObj);
void KundunBox05OpenEvent(LPOBJ lpObj);
void DarkLordHeartBoxOpenEvent(LPOBJ lpObj);
void WizardRing40OpenEvent(LPOBJ lpObj);
void WizardRing80OpenEvent(LPOBJ lpObj);
void SealedGoldenOpenEvent(LPOBJ lpObj);
void SealedSilverOpenEvent(LPOBJ lpObj);
void ShinningJewelryOpenEvent(LPOBJ lpObj);
void ElegantJewelryOpenEvent(LPOBJ lpObj);
void SteelJewelryOpenEvent(LPOBJ lpObj);
void OldJewelryOpenEvent(LPOBJ lpObj);
void RedRibbonBoxOpenEvent(LPOBJ lpObj);
void GreenRibbonBoxOpenEvent(LPOBJ lpObj);
void BlueRibbonBoxOpenEvent(LPOBJ lpObj);
void PinkChocolateBoxOpenEvent(LPOBJ lpObj);
void RedChocolateBoxOpenEvent(LPOBJ lpObj);
void BlueChocolateBoxOpenEvent(LPOBJ lpObj);
void LightPurpleCandyBoxOpenEvent(LPOBJ lpObj);
void VermilionCandyBoxOpenEvent(LPOBJ lpObj);
void DeepBlueCandyBoxOpenEvent(LPOBJ lpObj);
void HalloweenDayBoxOpenEvent(LPOBJ lpObj);
void CherryBlossomOpenEvent(LPOBJ lpObj);
void GreenBoxOpenEvent(LPOBJ lpObj);
void RedBoxOpenEvent(LPOBJ lpObj);
void PurpleBoxOpenEvent(LPOBJ lpObj);
void GreenChaosBoxOpenEvent(LPOBJ lpObj);
void RedChaosBoxOpenEvent(LPOBJ lpObj);
void PurpleChaosBoxOpenEvent(LPOBJ lpObj);
void GameMasterBoxOpenEvent(LPOBJ lpObj);
void BlueHappyPouchOpenEvent(LPOBJ lpObj);
void RedHappyPouchOpenEvent(LPOBJ lpObj);
// -----------------------------------------------------------------------------------------------------------------------------------------------------
// BOSS
void SelupanBossOpenEvent(LPOBJ lpObj,int MaxHitUser,BYTE x,BYTE y);

extern LPOBJ pEventObj;

#endif