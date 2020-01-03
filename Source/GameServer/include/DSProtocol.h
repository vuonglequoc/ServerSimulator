#ifndef DSPROTOCOL_H
#define DSPROTOCOL_H

#include "prodef.h"
#include "user.h"


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Char List Count Packet
 *	Direction : DataServer -> GameServer
 *  Code : 0xC2
 *	HeadCode : 0x01
 *	BASE PACKET - Complete with SDHP_CHARLIST
 */
struct SDHP_CHARLISTCOUNT
{
	PWMSG_HEAD h;
	short Number;
	BYTE Count;
	int DbNumber;
	BYTE Magumsa;
	char AccountId[11];
	BYTE MoveCnt;
};

struct SDHP_CHARLIST
{
	BYTE Index;
	char Name[10];
	WORD Level;
	BYTE Class;
	BYTE CtlCode;
	BYTE dbInventory[48];
	BYTE DbVersion;
	BYTE btGuildStatus;
};

struct SDHP_CREATECHARRESULT
{
	PBMSG_HEAD h;
	unsigned char Result;
	short Number;
	char AccountId[10];
	char Name[10];
	BYTE Pos;
	BYTE ClassSkin;
	BYTE Equipment[24];
	WORD Level;
};

struct SDHP_CHARDELETERESULT
{
	PBMSG_HEAD h;
	unsigned char Result;
	short Number;
	char AccountID[10];
};

struct SDHP_DBCHAR_INFORESULT
{
	PWMSG_HEAD h;
   	UCHAR result;
   	short Number;
  	char AccountID[10];
   	char Name[11];
   	BYTE Class;
   	short Level;
  	int LevelUpPoint;
   	int Exp;
   	int Money;
   	short Str;
   	short Dex;
   	short Vit;
   	short Energy;
   	WORD Life;
   	WORD MaxLife;
   	WORD Mana;
   	WORD MaxMana;
   	BYTE dbInventory[3776];
   	BYTE dbMagicList[180];
   	BYTE MapNumber;
   	BYTE MapX;
   	BYTE MapY;
   	BYTE Dir;
   	int PkCount;
   	int PkLevel;
   	int PkTime;
   	BYTE CtlCode;
	BYTE DbVersion;
   	BYTE AccountCtlCode;
   	BYTE dbQuest[50];
    WORD Leadership;
    WORD ChatLitmitTime;
	WORD CommandLitmitTime;
    int iFruitPoint;
	BYTE GenFamily;
	BYTE GenLevel;
	int GenRanking;
	int GenContribution;
	BYTE GenReward;
	int MasterLevel;
	int MasterExp;
	int MasterNextExp;
	int MasterPoints;
	BYTE NewQuest[600];
	BYTE TutorialReg;
	BYTE MercenaryReg;
	unsigned char ExpandedInventory;
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Warehouse DB Save Packet
 *	Direction : GameServer -> DataServer [0x09]
 *	            DataServer -> GameServer [0x08]
 *  Code      : 0xC2
 *	HeadCode  : 0x08, 0x09
 */
struct SDHP_GETWAREHOUSEDB_SAVE
{
	PWMSG_HEAD h;
	char AccountID[10];	// 4
	short aIndex;	// E
	int Money;	// 10
	BYTE dbItems[3840];	// 14
	BYTE DbVersion;	// 794
	short pw;	// 796
};



struct SDHP_GETWAREHOUSEDB_RESULT
{
	PBMSG_HEAD h;
	char AccountID[10];	// 3
	short aIndex;	// E
};



struct SDHP_ITEMCREATERECV
{
	PBMSG_HEAD h;
	BYTE x;	// 3
	BYTE y;	// 4
	BYTE MapNumber;	// 5 [235:Mu_2nd_Aniv], [236:CashShop]
	DWORD m_Number;	// 8
	short Type;	// C
	BYTE Level;	// E
	BYTE Dur;	// F
	BYTE Op1;	// 10
	BYTE Op2;	// 11
	BYTE Op3;	// 12
	BYTE NewOption;	// 13
	int aIndex;	// 14
	short lootindex;	// 18
	BYTE SetOption;	// 1A
	BYTE Socket[5];
};


struct SDHP_SKILLKEYDATA_SEND
{
	PBMSG_HEAD h;
	int aIndex;	// 4
	char Name[10];	// 8
	BYTE SkillKeyBuffer[10];	// 12
	BYTE GameOption;	// 1C
	BYTE QkeyDefine;	// 1D
	BYTE WkeyDefine;	// 1E
	BYTE EkeyDefine;	// 1F
	BYTE ChatWindow;	// 20
};



void DataServerProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen);
void DataServerLogin(int server);
void DataServerLoginResult( SDHP_RESULT* lpMsg);
void JGPGetCharList(unsigned char * lpRecv);
void DataServerGetCharListRequest( short aIndex);
void JGCharacterCreateRequest( SDHP_CREATECHARRESULT* lpMsg);
void JGCharacterCreateFailSend(int aIndex, char* id);
void JGCharDelRequest( SDHP_CHARDELETERESULT* lpMsg);
void JGGetCharacterInfo( SDHP_DBCHAR_INFORESULT* lpMsg);
void GCItemListSend(int aIndex);
void GJSetCharacterInfo(struct OBJECTSTRUCT* lpObj, int aIndex, BOOL bMapServerMove);
void GDGetWarehouseList(int aIndex, char* AccountID);
void DGGetWarehouseList( SDHP_GETWAREHOUSEDB_SAVE* lpMsg);
void GDGetWarehouseNoItem( SDHP_GETWAREHOUSEDB_RESULT* lpMsg);
void GDSetWarehouseList(int aIndex);
void GDUserItemSave( OBJECTSTRUCT* lpObj);
void GDSetWarehouseMoney(int aIndex);
void GDGameServerInfoSave();
void ItemSerialCreateSend(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption, BYTE * SocketOption);
void PetItemSerialCreateSend(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level,BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption);
void ItemSerialCreateRecv( SDHP_ITEMCREATERECV* lpMsg);
void ItemMovePathSave(char* ActID, char* Name, BYTE level, BYTE mapnumber, BYTE x, BYTE y, char* Item, BYTE op1, BYTE op2, BYTE op3, DWORD serial, BYTE * Socket);
void DGOptionDataSend(int aIndex, char* szName, LPBYTE KeyBuffer, BYTE GO,BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd);
void DGOptionDataRecv( SDHP_SKILLKEYDATA_SEND* lpMsg);
void gObjRequestPetItemInfo(int aIndex, int inventype);
void DGRecvPetItemInfo(LPBYTE lpData);
void gObjSavePetItemInfo(int aIndex, int inventype);
void GS_GDReqCastleTotalInfo(int iMapSvrGroup, int iCastleEventCycle);
void GS_GDReqOwnerGuildMaster(int iMapSvrGroup, int aIndex);
void GS_GDReqCastleNpcBuy(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iNpcDfLevel, int iNpcRgLevel, int iNpcMaxHP, int iNpcHP, BYTE btNpcX, BYTE btNpcY, BYTE btNpcDIR,  int iBuyCost);
void GS_GDReqCastleNpcRepair(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iRepairCost);
void GS_GDReqCastleNpcUpgrade(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iNpcUpType, int iNpcUpValue, int iNpcUpIndex);
void GS_GDReqTaxInfo(int iMapSvrGroup, int aIndex);
void GS_GDReqTaxRateChange(int iMapSvrGroup, int aIndex, int iTaxType, int iTaxRate);
void GS_GDReqCastleMoneyChange(int iMapSvrGroup, int aIndex, int iMoneyChange);
void GS_GDReqSiegeDateChange(int iMapSvrGroup, int aIndex, WORD wStartYear, BYTE btStartMonth, BYTE btStartDay, WORD wEndYear, BYTE btEndMonth, BYTE btEndDay);
void GS_GDReqResetRegSiegeInfo(int iMapSvrGroup);
void GS_GDReqGuildMarkRegInfo(int iMapSvrGroup, int aIndex);
void GS_GDReqSiegeEndedChange(int iMapSvrGroup, int bIsSiegeEnded);
void GS_GDReqCastleOwnerChange(int iMapSvrGroup, int bIsCastleOccupied, char* lpszGuildName);
void GS_GDReqRegAttackGuild(int iMapSvrGroup, int aIndex);
void GS_GDReqRestartCastleState(int iMapSvrGroup);
void GS_GDReqMapSvrMsgMultiCast(int iMapSvrGroup, char* lpszMsgText);
void GS_GDReqRegGuildMark(int iMapSvrGroup, int aIndex, int iItemPos);
void GS_GDReqGuildMarkReset(int iMapSvrGroup, int aIndex, char* lpszGuildName);
void GS_GDReqGuildSetGiveUp(int iMapSvrGroup, int aIndex, int bIsGiveUp);
void GS_GDReqNpcRemove(int iMapSvrGroup, int iNpcNumber, int iNpcIndex);
void GS_GDReqCastleStateSync(int iMapSvrGroup, int iCastleState, int iTaxRateChaos, int iTaxRateStore, int iTaxHuntZone, char* lpszOwnerGuild);
void GS_GDReqCastleTributeMoney(int iMapSvrGroup, int iCastleTributeMoney);
void GS_GDReqResetCastleTaxInfo(int iMapSvrGroup);
void GS_GDReqResetSiegeGuildInfo(int iMapSvrGroup);
void GS_GDReqResetRegSiegeInfo(int iMapSvrGroup);
void GS_GDReqCastleInitData(int iMapSvrGroup, int iCastleEventCycle);
void GS_GDReqCastleNpcInfo(int iMapSvrGroup, int aIndex);
void GS_GDReqAllGuildMarkRegInfo(int iMapSvrGroup, int aIndex);
void GS_GDReqCalcRegGuildList(int iMapSvrGroup);
void GS_GDReqCsLoadTotalGuildInfo(int iMapSvrGroup);
void GS_DGAnsCastleTotalInfo(LPBYTE lpRecv);
void GS_DGAnsOwnerGuildMaster(LPBYTE lpRecv);
void GS_DGAnsCastleNpcBuy(LPBYTE lpRecv);
void GS_DGAnsCastleNpcRepair(LPBYTE lpRecv);
void GS_DGAnsCastleNpcUpgrade(LPBYTE lpRecv);
void GS_DGAnsTaxInfo(LPBYTE lpRecv);
void GS_DGAnsTaxRateChange(LPBYTE lpRecv);
void GS_DGAnsCastleMoneyChange(LPBYTE lpRecv);
void GS_DGAnsSiegeDateChange(LPBYTE lpRecv);
void GS_DGAnsGuildMarkRegInfo(LPBYTE lpRecv);
void GS_DGAnsSiegeEndedChange(LPBYTE lpRecv);
void GS_DGAnsCastleOwnerChange(LPBYTE lpRecv);
void GS_DGAnsRegAttackGuild(LPBYTE lpRecv);
void GS_DGAnsRestartCastleState(LPBYTE lpRecv);
void GS_DGAnsMapSvrMsgMultiCast(LPBYTE lpRecv);
void GS_DGAnsRegGuildMark(LPBYTE lpRecv);
void GS_DGAnsGuildMarkReset(LPBYTE lpRecv);
void GS_DGAnsGuildSetGiveUp(LPBYTE lpRecv);
void GS_DGAnsNpcRemove(LPBYTE lpRecv);
void GS_DGAnsCastleStateSync(LPBYTE lpRecv);
void GS_DGAnsCastleTributeMoney(LPBYTE lpRecv);
void GS_DGAnsResetCastleTaxInfo(LPBYTE lpRecv);
void GS_DGAnsResetSiegeGuildInfo(LPBYTE lpRecv);
void GS_DGAnsResetRegSiegeInfo(LPBYTE lpRecv);
void GS_DGAnsCastleInitData(LPBYTE lpRecv);
void GS_DGAnsCastleNpcInfo(LPBYTE lpRecv);
void GS_DGAnsAllGuildMarkRegInfo(LPBYTE lpRecv);
void GS_DGAnsFirstCreateNPC(LPBYTE lpRecv);
void GS_DGAnsCalcREgGuildList(LPBYTE lpRecv);
void GS_DGAnsCsGulidUnionInfo(LPBYTE lpRecv);
void GS_DGAnsCsSaveTotalGuildInfo(LPBYTE lpRecv);
void GS_DGAnsCsLoadTotalGuildInfo(LPBYTE lpRecv);
void GS_DGAnsCastleNpcUpdate(LPBYTE lpRecv);
void GDReqCrywolfSync(int iMapSvrGroup, int iCrywolfState, int iOccupationState);
void DGAnsCrywolfSync(LPBYTE lpRecv);
void GDReqCrywolfInfoLoad(int iMapSvrGroup);
void DGAnsCrywolfInfoLoad(LPBYTE lpRecv);
void GDReqCrywolfInfoSave(int iMapSvrGroup, int iCrywolfState, int iOccupationState);
void DGAnsCrywolfInfoSave(LPBYTE lpRecv);


#endif /*DSPROTOCOL_H*/