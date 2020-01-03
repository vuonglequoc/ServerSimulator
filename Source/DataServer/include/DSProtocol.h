/*#############################################################################
# Zodea MU Suite - AIO
#
# RealCoderZ Software Incorpration.	http://www.realcoderz.com
#
# Copyright © 2011 RealCoderZ Software Incorpration.
#
# Author: WolF & M.E.S
#
# [19.08.2011] This Script For DataServer Protocol Class.
#
#############################################################################*/
#ifndef DATA_PROTO_HEADER
#define DATA_PROTO_HEADER

#include "StdAfx.H"
#include "DataProtocol.h"


void DSProtocolCore(DServerManager * thisManager, int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize);
void DataServerLogin(DServerManager * thisManager, int aIndex, SDHP_SERVERINFO * lpMsg);
void DataServerLogin(DServerManager * thisManager, int aIndex, SDHP_SERVERINFO * aRecv);
void JGPGetCharList(DServerManager * thisManager, int aIndex, SDHP_GETCHARLIST * aRecv);
void JGCharacterCreateRequest(DServerManager * thisManager, int aIndex, SDHP_CREATECHAR * aRecv);
void JGCharDelRequest(DServerManager * thisManager, int aIndex, SDHP_CHARDELETE * aRecv);
void JGGetCharacterInfo(DServerManager * thisManager, int aIndex, SDHP_DBCHARINFOREQUEST * aRecv);
void GJSetCharacterInfo(DServerManager * thisManager, int aIndex, SDHP_DBCHAR_INFOSAVE * aRecv);
void DGGetWarehouseList(DServerManager * thisManager, int aIndex, SDHP_GETWAREHOUSEDB * aRecv);
void GDSetWarehouseList(DServerManager * thisManager, int aIndex, SDHP_GETWAREHOUSEDB_SAVE * aRecv);
void DGGetQuestMonsterKill(DServerManager * thisManager, int aIndex, SDHP_GETQUESTMONSTER_KILL* aRecv);
void GDQuestMonsterKillSave(DServerManager * thisManager, int aIndex, SDHP_DBQUEST_KILLSAVE* aRecv);
void GDUserItemSave(DServerManager * thisManager, int aIndex, SDHP_DBCHAR_ITEMSAVE * aRecv);
void GDSetWarehouseMoney(DServerManager * thisManager, int aIndex, SDHP_WAREHOUSEMONEY_SAVE * aRecv);
void ItemSerialCreateRecv(DServerManager * thisManager, int aIndex, SDHP_ITEMCREATE * aRecv);
void PetItemSerialCreateRecv(DServerManager * thisManager, int aIndex, SDHP_ITEMCREATE * aRecv);
void DGRecvPetItemInfo(DServerManager * thisManager, int aIndex, SDHP_REQUEST_PETITEM_INFO * aRecv);
void GDSavePetItemInfo(DServerManager * thisManager, int aIndex, SDHP_SAVE_PETITEM_INFO * aRecv);
void DGOptionDataRecv(DServerManager * thisManager, int aIndex, SDHP_SKILLKEYDATA * aRecv);
void DGMoveOtherServer(DServerManager * thisManager, int aIndex, SDHP_CHARACTER_TRANSFER * aRecv);
//-----------------------------------------------------------------------------------------------------------
void GS_DGAnsOwnerGuildMaster(DServerManager * thisManager, int aIndex, CSP_REQ_OWNERGUILDMASTER * aRecv);
void GS_DGAnsCastleNpcBuy(DServerManager * thisManager, int aIndex, CSP_REQ_NPCBUY * aRecv);
void GS_DGAnsCastleNpcRepair(DServerManager * thisManager, int aIndex, CSP_REQ_NPCREPAIR * aRecv);
void GS_DGAnsCastleNpcUpgrade(DServerManager * thisManager, int aIndex, CSP_REQ_NPCUPGRADE * aRecv);
void GS_DGAnsTaxInfo(DServerManager * thisManager, int aIndex, CSP_REQ_TAXINFO * aRecv);
void GS_DGAnsTaxRateChange(DServerManager * thisManager, int aIndex, CSP_REQ_TAXRATECHANGE * aRecv);
void GS_DGAnsCastleMoneyChange(DServerManager * thisManager, int aIndex, CSP_REQ_MONEYCHANGE * aRecv);
void GS_DGAnsSiegeDateChange(DServerManager * thisManager, int aIndex, CSP_REQ_SDEDCHANGE * aRecv);
void GS_DGAnsGuildMarkRegInfo(DServerManager * thisManager, int aIndex, CSP_REQ_GUILDREGINFO * aRecv);
void GS_DGAnsSiegeEndedChange(DServerManager * thisManager, int aIndex, CSP_REQ_SIEGEENDCHANGE * aRecv);
void GS_DGAnsCastleOwnerChange(DServerManager * thisManager, int aIndex, CSP_REQ_CASTLEOWNERCHANGE * aRecv);
void GS_DGAnsRegAttackGuild(DServerManager * thisManager, int aIndex, CSP_REQ_REGATTACKGUILD * aRecv);
void GS_DGAnsRestartCastleState(DServerManager * thisManager, int aIndex, CSP_REQ_CASTLESIEGEEND * aRecv);
void GS_DGAnsMapSvrMsgMultiCast(DServerManager * thisManager, int aIndex, CSP_REQ_MAPSVRMULTICAST * aRecv);
void GS_DGAnsRegGuildMark(DServerManager * thisManager, int aIndex, CSP_REQ_GUILDREGMARK * aRecv);
void GS_DGAnsGuildMarkReset(DServerManager * thisManager, int aIndex, CSP_REQ_GUILDRESETMARK * aRecv);
void GS_DGAnsGuildSetGiveUp(DServerManager * thisManager, int aIndex, CSP_REQ_GUILDSETGIVEUP * aRecv);
void GS_DGAnsNpcRemove(DServerManager * thisManager, int aIndex, CSP_REQ_NPCREMOVE * aRecv);
void GS_DGAnsCastleStateSync(DServerManager * thisManager, int aIndex, CSP_REQ_CASTLESTATESYNC * aRecv);
void GS_DGAnsCastleTributeMoney(DServerManager * thisManager, int aIndex, CSP_REQ_CASTLETRIBUTEMONEY * aRecv);
void GS_DGAnsResetCastleTaxInfo(DServerManager * thisManager, int aIndex, CSP_REQ_RESETCASTLETAXINFO * aRecv);
void GS_DGAnsResetSiegeGuildInfo(DServerManager * thisManager, int aIndex, CSP_REQ_RESETSIEGEGUILDINFO * aRecv);
void GS_DGAnsResetRegSiegeInfo(DServerManager * thisManager, int aIndex, CSP_REQ_RESETREGSIEGEINFO * aRecv);
void GS_DGAnsCastleInitData(DServerManager * thisManager, int aIndex, CSP_REQ_CSINITDATA * aRecv);
void GS_DGAnsAllGuildMarkRegInfo(DServerManager * thisManager, int aIndex, CSP_REQ_ALLGUILDREGINFO * aRecv);
void GS_DGAnsFirstCreateNPC(DServerManager * thisManager, int aIndex, CSP_REQ_NPCSAVEDATA * aRecv);
void GS_DGAnsCalcRegGuildList(DServerManager * thisManager, int aIndex, CSP_REQ_CALCREGGUILDLIST * aRecv);
void GS_DGAnsCsGulidUnionInfo(DServerManager * thisManager, int aIndex, CSP_REQ_CSGUILDUNIONINFO * aRecv);
void GS_DGAnsCsSaveTotalGuildInfo(DServerManager * thisManager, int aIndex, CSP_REQ_CSSAVETOTALGUILDINFO * aRecv);
void GS_DGAnsCsLoadTotalGuildInfo(DServerManager * thisManager, int aIndex, CSP_REQ_CSLOADTOTALGUILDINFO * aRecv);
void GS_DGAnsCastleNpcUpdate(DServerManager * thisManager, int aIndex, CSP_REQ_NPCUPDATEDATA * aRecv);
void DGAnsCrywolfSync(DServerManager * thisManager, int aIndex, CWP_REQ_CRYWOLFSYNC * aRecv);
void DGAnsCrywolfInfoLoad(DServerManager * thisManager, int aIndex, CWP_REQ_CRYWOLFINFOLOAD * aRecv);
//-----------------------------------------------------------------------------------------------------------
#endif /* GDS_PRTOCOOL */
//-----------------------------------------------------------------------------------------------------------