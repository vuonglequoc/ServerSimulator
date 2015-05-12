//-----------------------------------------------------------------------------------------------------------
#ifndef GDS_PRTOCOOL
#define GDS_PROTOCOL
//-----------------------------------------------------------------------------------------------------------

#include "..\..\Protocol\DataProtocol.h"
#include "TDSManager.h"
//-----------------------------------------------------------------------------------------------------------

#define INVENTORY_BINARY_SIZE 1728
// -----
#define CS_CLASS				0
#define CS_WEAPON1_TYPE			1
#define CS_WEAPON2_TYPE			2
#define CS_WEAPON1_DATA			12
#define CS_WEAPON2_DATA			13
#define CS_HELMET1				13
#define CS_HELMET2				9
#define CS_HELMET3				3
#define CS_ARMOR1				14
#define CS_ARMOR2				9
#define CS_ARMOR3				3
#define CS_PANTS1				14
#define CS_PANTS2				9
#define CS_PANTS3				4
#define CS_GLOVES1				15
#define CS_GLOVES2				9
#define CS_GLOVES3				4
#define CS_BOOTS1				15
#define CS_BOOTS2				9
#define CS_BOOTS3				5
// -----
#define CHECK_CLASS			(value, type) ( ((value)!=(type))?FALSE:TRUE )
// -----
#define CS_GET_CLASS		(x) ( (((x)>>4)<<5)&(0xE0) )
#define CS_GET_CHANGEUP		(x) ( ((x)&0x07) )
#define CS_SET_CLASS		(x) ( ((x)<<5)& 0xE0 )
#define CS_SET_CHANGEUP		(x) ( ((x) << 4) & 0x10 )
#define CS_SET_HELMET1		(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_HELMET2		(x) ( ((x) & 0x10 ) << 3 )
#define CS_SET_HELMET3		(x) ( ((x) & 0x0F ) << 4 )
#define CS_SET_ARMOR1		(x) ( ((x) & 0x1E0) >> 1 )
#define CS_SET_ARMOR2		(x) ( ((x) & 0x10 ) << 2 )
#define CS_SET_ARMOR3		(x) ( ((x) & 0x0F )      )
#define CS_SET_PANTS1		(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_PANTS2		(x) ( ((x) & 0x10 ) << 1 )
#define CS_SET_PANTS3		(x) ( ((x) & 0x0F ) << 4 )
#define CS_SET_GLOVES1		(x) ( ((x) & 0x1E0) >> 1 )
#define CS_SET_GLOVES2		(x) ( ((x) & 0x10 )      )
#define CS_SET_GLOVES3		(x) ( ((x) & 0x0F )      )
#define CS_SET_BOOTS1		(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_BOOTS2		(x) ( ((x) & 0x10 ) >> 1 )
#define CS_SET_BOOTS3		(x) ( ((x) & 0x0F ) << 4 )
#define CS_SET_WING1		(x)  ( ((x) & 0x03 ) << 2 )
#define CS_SET_HELPER		(x) ( ((x) & 0x03 )      )
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
// -----
#define DBI_GET_TYPE(x) ( ((x)&((MAX_TYPE_ITEMS-1)<<8))>>4 )
#define DBI_GET_INDEX(x) ( (x) & 0xFF )
#define DBI_GET_LEVEL(x)  ( ((x)>>3)& MAX_ITEM_LEVEL )
#define DBI_GET_SKILL(x)  ( ((x) >> 7) & 0x01)
#define DBI_GET_LUCK(x)  ( ((x) >> 2) & 0x01)
#define DBI_GET_OPTION(x)  ( ( (x) ) & 0x03 )
#define DBI_GET_OPTION16(x) ( (x) & 0x40  )
#define DBI_GET_DINO_OPTION(x)  ( DBI_GET_OPTION16((x)) >> 4 )
#define DBI_GET_NOPTION(x)  ( (x) & 0x3F  )
#define DBI_GET_380OPTION(x) ( ((x) & 0x08) << 4  )
#define ITEM_GET_TYPE(x)  ( (x)/MAX_SUBTYPE_ITEMS  )
#define ITEM_GET_INDEX(x) ( (x)%MAX_SUBTYPE_ITEMS  )
//-----------------------------------------------------------------------------------------------------------

void DataServerLogin(CDSManager * thisManager, int aIndex, SDHP_SERVERINFO * lpMsg);
void DSProtocolCore(CDSManager * thisManager, int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize);
void DataServerLogin(CDSManager * thisManager, int aIndex, SDHP_SERVERINFO * aRecv);
void JGPGetCharList(CDSManager * thisManager, int aIndex, SDHP_GETCHARLIST * aRecv);
void JGCharacterCreateRequest(CDSManager * thisManager, int aIndex, SDHP_CREATECHAR * aRecv);
void JGCharDelRequest(CDSManager * thisManager, int aIndex, SDHP_CHARDELETE * aRecv);
void JGGetCharacterInfo(CDSManager * thisManager, int aIndex, SDHP_DBCHARINFOREQUEST * aRecv);
void GJSetCharacterInfo(CDSManager * thisManager, int aIndex, SDHP_DBCHAR_INFOSAVE * aRecv);
void DGGetWarehouseList(CDSManager * thisManager, int aIndex, SDHP_GETWAREHOUSEDB * aRecv);
void GDSetWarehouseList(CDSManager * thisManager, int aIndex, SDHP_GETWAREHOUSEDB_SAVE * aRecv);
void DGGetQuestMonsterKill(CDSManager * thisManager, int aIndex, SDHP_GETQUESTMONSTER_KILL* aRecv);
void GDQuestMonsterKillSave(CDSManager * thisManager, int aIndex, SDHP_DBQUEST_KILLSAVE* aRecv);
void GDUserItemSave(CDSManager * thisManager, int aIndex, SDHP_DBCHAR_ITEMSAVE * aRecv);
void GDSetWarehouseMoney(CDSManager * thisManager, int aIndex, SDHP_WAREHOUSEMONEY_SAVE * aRecv);
void ItemSerialCreateRecv(CDSManager * thisManager, int aIndex, SDHP_ITEMCREATE * aRecv);
void PetItemSerialCreateRecv(CDSManager * thisManager, int aIndex, SDHP_ITEMCREATE * aRecv);
void DGRecvPetItemInfo(CDSManager * thisManager, int aIndex, SDHP_REQUEST_PETITEM_INFO * aRecv);
void GDSavePetItemInfo(CDSManager * thisManager, int aIndex, SDHP_SAVE_PETITEM_INFO * aRecv);
void DGOptionDataRecv(CDSManager * thisManager, int aIndex, SDHP_SKILLKEYDATA * aRecv);
void DGMoveOtherServer(CDSManager * thisManager, int aIndex, SDHP_CHARACTER_TRANSFER * aRecv);
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsOwnerGuildMaster(CDSManager * thisManager, int aIndex, CSP_REQ_OWNERGUILDMASTER * aRecv);
void GS_DGAnsCastleNpcBuy(CDSManager * thisManager, int aIndex, CSP_REQ_NPCBUY * aRecv);
void GS_DGAnsCastleNpcRepair(CDSManager * thisManager, int aIndex, CSP_REQ_NPCREPAIR * aRecv);
void GS_DGAnsCastleNpcUpgrade(CDSManager * thisManager, int aIndex, CSP_REQ_NPCUPGRADE * aRecv);
void GS_DGAnsTaxInfo(CDSManager * thisManager, int aIndex, CSP_REQ_TAXINFO * aRecv);
void GS_DGAnsTaxRateChange(CDSManager * thisManager, int aIndex, CSP_REQ_TAXRATECHANGE * aRecv);
void GS_DGAnsCastleMoneyChange(CDSManager * thisManager, int aIndex, CSP_REQ_MONEYCHANGE * aRecv);
void GS_DGAnsSiegeDateChange(CDSManager * thisManager, int aIndex, CSP_REQ_SDEDCHANGE * aRecv);
void GS_DGAnsGuildMarkRegInfo(CDSManager * thisManager, int aIndex, CSP_REQ_GUILDREGINFO * aRecv);
void GS_DGAnsSiegeEndedChange(CDSManager * thisManager, int aIndex, CSP_REQ_SIEGEENDCHANGE * aRecv);
void GS_DGAnsCastleOwnerChange(CDSManager * thisManager, int aIndex, CSP_REQ_CASTLEOWNERCHANGE * aRecv);
void GS_DGAnsRegAttackGuild(CDSManager * thisManager, int aIndex, CSP_REQ_REGATTACKGUILD * aRecv);
void GS_DGAnsRestartCastleState(CDSManager * thisManager, int aIndex, CSP_REQ_CASTLESIEGEEND * aRecv);
void GS_DGAnsMapSvrMsgMultiCast(CDSManager * thisManager, int aIndex, CSP_REQ_MAPSVRMULTICAST * aRecv);
void GS_DGAnsRegGuildMark(CDSManager * thisManager, int aIndex, CSP_REQ_GUILDREGMARK * aRecv);
void GS_DGAnsGuildMarkReset(CDSManager * thisManager, int aIndex, CSP_REQ_GUILDRESETMARK * aRecv);
void GS_DGAnsGuildSetGiveUp(CDSManager * thisManager, int aIndex, CSP_REQ_GUILDSETGIVEUP * aRecv);
void GS_DGAnsNpcRemove(CDSManager * thisManager, int aIndex, CSP_REQ_NPCREMOVE * aRecv);
void GS_DGAnsCastleStateSync(CDSManager * thisManager, int aIndex, CSP_REQ_CASTLESTATESYNC * aRecv);
void GS_DGAnsCastleTributeMoney(CDSManager * thisManager, int aIndex, CSP_REQ_CASTLETRIBUTEMONEY * aRecv);
void GS_DGAnsResetCastleTaxInfo(CDSManager * thisManager, int aIndex, CSP_REQ_RESETCASTLETAXINFO * aRecv);
void GS_DGAnsResetSiegeGuildInfo(CDSManager * thisManager, int aIndex, CSP_REQ_RESETSIEGEGUILDINFO * aRecv);
void GS_DGAnsResetRegSiegeInfo(CDSManager * thisManager, int aIndex, CSP_REQ_RESETREGSIEGEINFO * aRecv);
void GS_DGAnsCastleInitData(CDSManager * thisManager, int aIndex, CSP_REQ_CSINITDATA * aRecv);
void GS_DGAnsAllGuildMarkRegInfo(CDSManager * thisManager, int aIndex, CSP_REQ_ALLGUILDREGINFO * aRecv);
void GS_DGAnsFirstCreateNPC(CDSManager * thisManager, int aIndex, CSP_REQ_NPCSAVEDATA * aRecv);
void GS_DGAnsCalcRegGuildList(CDSManager * thisManager, int aIndex, CSP_REQ_CALCREGGUILDLIST * aRecv);
void GS_DGAnsCsGulidUnionInfo(CDSManager * thisManager, int aIndex, CSP_REQ_CSGUILDUNIONINFO * aRecv);
void GS_DGAnsCsSaveTotalGuildInfo(CDSManager * thisManager, int aIndex, CSP_REQ_CSSAVETOTALGUILDINFO * aRecv);
void GS_DGAnsCsLoadTotalGuildInfo(CDSManager * thisManager, int aIndex, CSP_REQ_CSLOADTOTALGUILDINFO * aRecv);
void GS_DGAnsCastleNpcUpdate(CDSManager * thisManager, int aIndex, CSP_REQ_NPCUPDATEDATA * aRecv);
void DGAnsCrywolfSync(CDSManager * thisManager, int aIndex, CWP_REQ_CRYWOLFSYNC * aRecv);
void DGAnsCrywolfInfoLoad(CDSManager * thisManager, int aIndex, CWP_REQ_CRYWOLFINFOLOAD * aRecv);
//-----------------------------------------------------------------------------------------------------------
#endif /* GDS_PRTOCOOL */
//-----------------------------------------------------------------------------------------------------------