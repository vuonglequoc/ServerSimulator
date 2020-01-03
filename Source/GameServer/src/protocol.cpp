#include "stdafx.h"
#include <stdio.h>
#include "user.h"
#include "protocol.h"
#include "../Test/Network.H"
#include "Event.h"
#include "LogProc.H"
#include "GameMain.h"
#include "QuestInfo.h"
#include "ConMember.h"
#include "Main.H"
#include "BattleSoccerManager.h"
#include "ChaosBox.h"
#include "CastleSiegeSync.h"
#include "TUnion.h"
#include "JewelMixSystem.h"
#include "Weapon.h"
#include "SendHackLog.h"
#include "DBSockMng.h"
#include "KalimaGate.h"
#include "ObjUseSkill.h"
#include "ObjCalCharacter.h"
#include "NpcTalk.h"
#include "Shop.h"
#include "CashItemPeriodSystem.h"
#include "BattleSoccer.h"
#include "BattleSoccerManager.h"
#include "gObjMonster.h"
#include "ComboAttack.h"
#include "Gate.h"
#include "MultiAttackHackCheck.h"
#include "ItemAddOption.h"
#include "JewelOfHarmonySystem.h"
#include "PacketCheckSum.h"
#include "DarkSpirit.h"
#include "Crywolf.h"
#include "CrywolfAltar.h"
#include "CrywolfSync.h"
#include "DevilSquare.h"
#include "MapServerManager.h"
#include "KanturuEntranceNPC.h"
#include "CashShop.h"
#include "JSProtocol.H"
#include "DSProtocol.h"
#include "EDSProtocol.h"
#include "Mercenary.h"
#include "Guardian.h"
#include "LifeStone.h"
#include "CastleSiege.h"
#include "ChaosCastle.h"
#include "MasterSkillSystem.h"
#include "IllusionTemple.h"
#include "DuelSystem.h"

// New Headers!
#include "Packets.h"
#include "Notice.H"
#include "MoveSystem.h"
#include "GenSystem.h"
#include "Functions.h"
#include "PersonalShop.h"
#include "SQuest.h"
#include "BloodCastle.h"
#include "Party.h"
#include "ImperialFort.h"
#include "DoubleGoer.h"
#include "CherryBlossom.h"
#include "Halloween.h"
#include "SpecialItems.h"
#include "ChaosCard.h"
#include "LuckyCoin.h"
#include "Firewall.h"
#include "MossMerchant.h"
#include "SendProto.H"


int cIndex;
int iCount;

void ProtocolCore(BYTE protoNum, unsigned char * aRecv, int aLen, int aIndex, BOOL Encrypt, int Serial)
{
	LPOBJ lpObj = &gObj[aIndex];


	char sBuf[255] = {0};
	//char _debuginfo[1024]="\x00";

	//ToHex((unsigned char *)_debuginfo, aRecv, aRecv[1]);
	//CLog.Debug("[Data RECV] %s", _debuginfo);
	

	//-- Serial Checking
	if ( Serial >= 0 )
	{
		if ( gObj[aIndex].Type == OBJ_USER )
		{
			if ( gNSerialCheck[aIndex].InCheck(Serial) == FALSE )
			{
				CLog.Error("Error-L1 : Socket Serial %s %d N_Serial:%d Serial:%d",gObj[aIndex].AccountID, protoNum, gNSerialCheck[aIndex].GetSerial(), Serial);
				gNet.CloseClient(aIndex);
			}
		}
	}

	if ( protoNum == 0xF1 || protoNum == 0xF3 || protoNum == 0x0E )
	{
		switch ( protoNum )
		{
			case 0x0E:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGLiveClient((PMSG_CLIENTTIME *)aRecv, aIndex);									//-- DonE!
					cIndex = aIndex;
				}break;
			
			case 0xF1:
				{
					PMSG_DEFAULT2 * lpMsg = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpMsg->subcode )
					{
						case 0x01:
							if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
							{
								CSCheckMainInclusions((PMSG_CHECKMAIN_INC *)aRecv, aIndex);			//-- DonE!
							}
							break;
						case 0x02:
							if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
							{
								CGCloseGamePlay((PMSG_CLIENTCLOSE *)aRecv, aIndex);					//-- DonE!
							}
							break;
						case 0x03:
							if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
							{
								CGHackCheckClient((PMSG_CLIENTMSG *)aRecv, aIndex);					//-- DonE!
							}
							break;
					}
				}
				break;

			case 0xF3:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							DataServerGetCharListRequest(aIndex);									//-- DonE!
							break;
						case 0x01:
							CGCharacterCreate((PMSG_CHARCREATE *)aRecv, aIndex);					//-- DonE!
							break;
						case 0x02:
							CGCharacterDelete((PMSG_CHARDELETE *)aRecv, aIndex);					//-- DonE!
							break;
						case 0x03:
							CGCharacterJoinRequest((PMSG_CHARMAPJOIN *)aRecv, aIndex);				//-- DonE!
							break;
						case 0x06:
							CGLevelUpPointAdd((PMSG_LVPOINTADD *)aRecv, aIndex);					//-- DonE!
							break;
						case 0x12:
							if(gObj[aIndex].RegenOk == 1) gObj[aIndex].RegenOk = 2;					//-- DonE! 
							break;
						case 0x15:
							DSCheckPJAccount(aIndex);												//-- DonE!
							break;
						case 0x30:
							GCSkillKeyRecv((PMSG_SKILLKEY *)aRecv, aIndex);							//-- DonE!
							break;
						case 0x52:
							gSkillTree.SkillAddRecv((PMSG_SKILL_ADD_RECV *)aRecv,aIndex);									//-- DonE!
							break;
					}
				}
				break;
		}
	}
	else
	{
		LPOBJ lpObj = &gObj[aIndex];

		switch ( protoNum )
		{
			case 0x00:
				TChat.GameChatProcess((PMSG_CHATDATA *)aRecv, aIndex);									//-- DonE!
				break;
			case 0x01:
				TChat.GameChatRecieve((PMSG_CHATDATA_NUMBER *)aRecv, aIndex);								//-- DonE!
				break;
			case 0x02:
				TChat.ChatWhisperRecieve((PMSG_CHATDATA_WHISPER *)aRecv, aIndex);							//-- DonE!
				break;
			case 0x03:
				CheckMainRecieve((PMSG_CHECK_MAINEXE_RESULT *)aRecv, aIndex);						//-- DonE!
				break;
			case 0x0E:
				CGLiveClient((PMSG_CLIENTTIME *)aRecv, aIndex);										//-- DonE!
				break;
			case PMSG_MOVE_HEADCODE:
				PMoveProc((PMSG_MOVE *)aRecv, aIndex);
				break;
			case PMSG_POSISTION_SET_HEADCODE:
				RecvPositionSetProc((PMSG_POSISTION_SET *)aRecv, aIndex);
				break;
			case PMSG_ATTACK_HEADCODE:
				CGAttack((PMSG_ATTACK *)aRecv, aIndex);												//-- DonE!
				break;
			case 0x18:
				CGActionRecv((PMSG_ACTION *)aRecv, aIndex);
				break;
			case 0x19:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGMagicAttack((PMSG_MAGICATTACK *)aRecv, aIndex);								//-- DonE!
				}
				break;
			case 0x1B:
				CGMagicCancel((PMSG_MAGICCANCEL *)aRecv, aIndex);
				break;
			case 0x1C:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGTeleportRecv((PMSG_TELEPORT *)aRecv, aIndex);
				}
				break;
			case 0xB0:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGTargetTeleportRecv((PMSG_TARGET_TELEPORT *)aRecv, aIndex);
				}
				break;
			case PMSG_BEATTACK_HEADCODE:
				CGBeattackRecv(aRecv, aIndex, FALSE);
				break;
			case 0x1E:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGDurationMagicRecv((PMSG_DURATION_MAGIC_RECV *)aRecv, aIndex);					//-- DonE!
				}
				break;
			case 0x22:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGItemGetRequest((PMSG_ITEMGETREQUEST *)aRecv, aIndex);
				}
				break;
			case 0x23:
				CGItemDropRequest((PMSG_ITEMTHROW *)aRecv, aIndex, 0);
				break;
			case 0x24:
				CGInventoryItemMove((PMSG_INVENTORYITEMMOVE *)aRecv, aIndex);
				break;
			case 0x26:
				CGUseItemRecv((PMSG_USEITEM *)aRecv, aIndex);
				break;
			case 0x30:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGTalkRequestRecv((PMSG_TALKREQUEST *)aRecv, aIndex);
				}
				break;
			case 0x31:
				CGCloseWindow(aIndex);
				break;
			case 0x32:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					if ( lpObj->TargetShopNumber == 492 )
					{
						gMossMerchant.ItemBuy(aIndex,aRecv[3]);
					}
					else
					{
						CGBuyRequestRecv((PMSG_BUYREQUEST *)aRecv, aIndex);
					}
				}
				break;
			case 0x33:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGSellRequestRecv((PMSG_SELLREQUEST *)aRecv, aIndex);
				}
				break;
			case 0x34:
				CGModifyRequestItem((PMSG_ITEMDURREPAIR *)aRecv, aIndex);
				break;
			case 0x36:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGTradeRequestSend((PMSG_TRADE_REQUEST *)aRecv, aIndex);
				}
				break;
			case 0x37:
				CGTradeResponseRecv((PMSG_TRADE_RESPONSE *)aRecv, aIndex);
				break;
			case 0x3A:
				CGTradeMoneyRecv((PMSG_TRADE_GOLD *)aRecv, aIndex);
				break;
			case 0x3C:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGTradeOkButtonRecv((PMSG_TRADE_OKBUTTON *)aRecv, aIndex);
				}
				break;
			case 0x3D:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGTradeCancelButtonRecv(aIndex);
				}
				break;
			case 0x3F:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							CPShop.SetPersonalShopItemPrice((PMSG_REQ_PSHOP_SETITEMPRICE *)aRecv, aIndex);
							break;
						case 0x02:
							CPShop.OpenPersonalShop((PMSG_REQ_PSHOP_OPEN *)aRecv, aIndex);
							break;
						case 0x03:
							CPShop.ClosePersonalShop(aIndex);
							break;
						case 0x05:
							CPShop.BuyListPersonalShop((PMSG_REQ_BUYLIST_FROM_PSHOP *)aRecv, aIndex);
							break;
						case 0x06:
							CPShop.BuyItemPersonalShop((PMSG_REQ_BUYITEM_FROM_PSHOP *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0x40:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGPartyRequestRecv((PMSG_PARTYREQUEST *)aRecv, aIndex);
				}
				break;
			case 0x41:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGPartyRequestResultRecv((PMSG_PARTYREQUESTRESULT *)aRecv, aIndex);
				}
				break;
			case 0x42:
				CGPartyList(aIndex);
				break;
			case 0x43:
				if ( PacketCheckTime(&gObj[aIndex]) == TRUE )
				{
					CGPartyDelUser((PMSG_PARTYDELUSER *)aRecv, aIndex);
				}
				break;
			case 0x50:
				CGGuildRequestRecv((PMSG_GUILDJOINQ *)aRecv, aIndex);
				break;
			case 0x51:
				CGGuildRequestResultRecv((PMSG_GUILDQRESULT *)aRecv, aIndex);
				break;
			case 0x52:
				CGGuildListAll(aIndex);
				break;
			case 0x53:
				CGGuildDelUser((PMSG_GUILDDELUSER *)aRecv, aIndex);
				break;
			case 0x54:
				CGGuildMasterAnswerRecv((PMSG_GUILDMASTERANSWER *)aRecv, aIndex);
				break;
			case 0x55:
				CGGuildMasterInfoSave(aIndex, (PMSG_GUILDINFOSAVE *)aRecv);
				break;
			case 0x57:
				CGGuildMasterCreateCancel(aIndex);
				break;
			case 0x61:
				GCGuildWarRequestSendRecv((PMSG_GUILDWARSEND_RESULT *)aRecv, aIndex);
				break;
			case 0x66:
				GCGuildViewportInfo((PMSG_REQ_GUILDVIEWPORT *)aRecv, aIndex);
				break;
			case 0xB1:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;
					
					switch ( lpDef->subcode )
					{
						case 0x01:
							if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
							{
								CGReqMapSvrAuth((PMSG_REQ_MAPSERVERAUTH *)aRecv, aIndex);
							}
							break;
					}
				}
				break;
			case 0xB2:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							CGReqCastleSiegeState((PMSG_REQ_CASTLESIEGESTATE *)aRecv, aIndex);
							break;
						case 0x01:
							CGReqRegCastleSiege((PMSG_REQ_REGCASTLESIEGE *)aRecv, aIndex);
							break;
						case 0x02:
							CGReqGiveUpCastleSiege((PMSG_REQ_GIVEUPCASTLESIEGE *)aRecv, aIndex);
							break;
						case 0x03:
							CGReqGuildRegInfo((PMSG_REQ_GUILDREGINFO *)aRecv, aIndex);
							break;
						case 0x04:
							CGReqRegGuildMark((PMSG_REQ_REGGUILDMARK *)aRecv, aIndex);
							break;
						case 0x05:
							CGReqNpcBuy((PMSG_REQ_NPCBUY *)aRecv, aIndex);
							break;
						case 0x06:
							CGReqNpcRepair((PMSG_REQ_NPCREPAIR *)aRecv, aIndex);
							break;
						case 0x07:
							CGReqNpcUpgrade((PMSG_REQ_NPCUPGRADE *)aRecv, aIndex);
							break;
						case 0x08:
							CGReqTaxMoneyInfo((PMSG_REQ_TAXMONEYINFO *)aRecv, aIndex);
							break;
						case 0x09:
							CGReqTaxRateChange((PMSG_REQ_TAXRATECHANGE *)aRecv, aIndex);
							break;
						case 0x10:
							CGReqMoneyDrawOut((PMSG_REQ_MONEYDRAWOUT *)aRecv, aIndex);
							break;
						case 0x12:
							CGReqCsGateOperate((PMSG_REQ_CSGATEOPERATE *)aRecv, aIndex);
							break;
						case 0x1B:
							CGReqCsMiniMapData((PMSG_REQ_MINIMAPDATA *)aRecv, aIndex);
							break;
						case 0x1C:
							CGReqStopCsMiniMapData((PMSG_REQ_STOPMINIMAPDATA *)aRecv, aIndex);
							break;
						case 0x1D:
							CGReqCsSendCommand((PMSG_REQ_CSCOMMAND *)aRecv, aIndex);
							break;
						case 0x1F:
							CGReqCsSetEnterHuntZone((PMSG_REQ_CSHUNTZONEENTER *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xB3:
				CGReqNpcDbList((PMSG_REQ_NPCDBLIST *)aRecv, aIndex);
				break;
			case 0xB4:
				CGReqCsRegGuildList((PMSG_REQ_CSREGGUILDLIST *)aRecv, aIndex);
				break;
			case 0xB5:
				CGReqCsAttkGuildList((PMSG_REQ_CSATTKGUILDLIST *)aRecv, aIndex);
				break;
			case 0xB7:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							CGReqWeaponUse((PMSG_REQ_USEWEAPON *)aRecv, aIndex);
							break;
						case 0x04:
							CGReqWeaponDamageValue((PMSG_REQ_WEAPON_DAMAGE_VALUE *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xB9:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x02:
							CGReqGuildMarkOfCastleOwner((PMSG_REQ_GUILDMARK_OF_CASTLEOWNER *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xBC:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							CGReqJewelMix((PMSG_REQ_JEWEL_MIX *)aRecv, aIndex);
							break;
						case 0x01:
							CGReqJewelUnMix((PMSG_REQ_JEWEL_UNMIX *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xBD:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							CGReqCrywolfInfo((PMSG_REQ_CRYWOLF_INFO *)aRecv, aIndex);
							break;
						case 0x03:
							CGReqAlatrContract((PMSG_REQ_CRYWOLF_ALTAR_CONTRACT *)aRecv, aIndex);
							break;
						case 0x09:
							CGReqPlusChaosRate((PMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xBE:
				CGGuildAssignStatus((PMSG_GUILD_ASSIGN_STATUS_REQ *)aRecv, aIndex);
				break;
			case 0xDC:
				NewSkillPacket(aIndex,aRecv);
				break;
			case 0xE2:
				CGGuildAssignType((PMSG_GUILD_ASSIGN_TYPE_REQ *)aRecv, aIndex);
				break;
			case 0xE5:
				CGRelationShipReqJoinBreakOff((PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ *)aRecv, aIndex);
				break;
			case 0xE6:
				CGRelationShipAnsJoinBreakOff((PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS *)aRecv, aIndex);
				break;
			case 0xE9:
				CGUnionList((PMSG_UNIONLIST_REQ *)aRecv, aIndex);
				break;
			case 0xEB:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							CGRelationShipReqKickOutUnionMember((PMSG_KICKOUT_UNIONMEMBER_REQ *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0x71:
				GCPingSendRecv((PMSG_PING_RESULT *)aRecv, aIndex);
				break;
			case 0x72:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					GCPacketCheckSumRecv((PMSG_PACKETCHECKSUM *)aRecv, aIndex);
				}
				break;
			case 0x81:
				CGWarehouseMoneyInOut(aIndex, (PMSG_WAREHOUSEMONEYINOUT *)aRecv);
				break;
			case 0x82:
				CGWarehouseUseEnd(aIndex);
				break;
			case 0x83:
				GCWarehouseRecivePassword(aIndex, (PMSG_WAREHOUSEPASSSEND *)aRecv);
				break;
			case 0x86:
				CGChaosBoxItemMixButtonClick((PMSG_CHAOSMIX *)aRecv, aIndex);
				break;
			case 0x87:
				CGChaosBoxUseEnd(aIndex);
				break;
			case 0x8E:
				gMove.RandWarp(aIndex,aRecv[8]);
				Gens.GenCheck(lpObj);
				return;
				break;
			case 0x90:
				GCReqmoveDevilSquare((PMSG_REQ_MOVEDEVILSQUARE *)aRecv, aIndex);
				break;
			case 0x91:
				GCReqEventRemainTime((PMSG_REQ_EVENT_REMAINTIME *)aRecv, aIndex);
				break;
			case 0x95:
				GCRegEventChipRecv((PMSG_REGEVENTCHIP *)aRecv, aIndex);
				break;
			case 0x96:
				GCGetMutoNumRecv((PMSG_GETMUTONUMBER *)aRecv, aIndex);
				break;
			case 0x97:
				GCUseEndEventChipRescv(aIndex);
				break;
			case 0x98:
				GCUseRenaChangeZenRecv((PMSG_EXCHANGE_EVENTCHIP *)aRecv, aIndex);
				break;
			case 0x99:
				CGReqMoveOtherServer((PMSG_REQ_MOVE_OTHERSERVER *)aRecv, aIndex);
				break;
			case 0xA0:
				CGRequestQuestInfo(aIndex);
				break;
			case 0xA2:
				CGSetQuestState((PMSG_SETQUEST *)aRecv, aIndex);
				break;
			case 0xA7:
				CGRequestPetItemCommand((PMSG_REQUEST_PET_ITEM_COMMAND *)aRecv, aIndex);
				break;
			case 0xA9:
				CGRequestPetItemInfo((PMSG_REQUEST_PET_ITEMINFO *)aRecv, aIndex);
				break;
			case 0xAA:
				gDuel.DuelProtocolManager(aRecv,aIndex);
				return;
				break;
			case 0x9A:
				g_BloodCastle.RequestEnterBloodCastle((PMSG_REQ_MOVEBLOODCASTLE*)aRecv, aIndex);
				break;
			case 0x9B:
				// Blood Castle Request Time
				break;
			case 0x9F:
				CGRequestEventEnterCount((PMSG_REQ_CL_ENTERCOUNT *)aRecv, aIndex);
				break;
			case 0x9D:
				CGRequestLottoRegister((PMSG_REQ_2ANV_LOTTO_EVENT *)aRecv, aIndex);
				break;
			case 0xAF:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							g_ChaosCastle.CGRequestEnterChaosCastle((PMSG_REQ_MOVECHAOSCASTLE *)aRecv, aIndex);
							break;
						case 0x02:
							g_ChaosCastle.CGRequestRepositionUserInChaosCastle((PMSG_REQ_REPOSUSER_IN_CC *)aRecv, aIndex);
							break;
					}
				}
				break;
#if(!GS_CASTLE)
			case 0xBF: /* Illusion Temple & Loren Market & Double Goer */
				if ( aRecv[1] == 0x05 && aRecv[3] == 0x0E )
				{
					gDoubleGoer.CheckCanEnter(aIndex);
				}
				else if ( aRecv[1] == 0x04 && aRecv[3] == 0x17 )
				{
					if (lpObj->MapNumber != 79)
					{
						gObjMoveGate(aIndex,333);
					}
					else
					{
						gObjMoveGate(aIndex,17);
					}
				}
				else if ( aRecv[1] == 0x06 && aRecv[3] == 0x20 )
				{
					int Item = gObj[aIndex].pInventory[aRecv[4]].m_Type;
					gObjInventoryItemSet(aIndex, aRecv[4], -1);
					gObj[aIndex].pInventory[aRecv[4]].Clear();
					GCInventoryItemDeleteSend(aIndex, aRecv[4], 1);
					gSpecialItem.UseItem(&gObj[aIndex],4,Item);
				}
				else if ( aRecv[3] == 0x0B )
				{
					// -- Lucky Coin Register Window Open
					gLuckyCoin.OpenRegisterWindow(&gObj[aIndex]);
				}
				else if ( aRecv[3] == 0x0C )
				{
					// -- Lucky Coin Register Coin
					gLuckyCoin.RegisterCoin(&gObj[aIndex]);
				}
				else if ( aRecv[1] == 0x08 && aRecv[3] == 0x0D && aRecv[4] == 0x0A )
				{
					// -- Lucky Coin Exchange 10 Coin
					gLuckyCoin.ExchangeCoin(&gObj[aIndex],0);
					return;
				}
				else if ( aRecv[1] == 0x08 && aRecv[3] == 0x0D && aRecv[4] == 0x14 )
				{
					// -- Lucky Coin Exchange 20 Coin
					gLuckyCoin.ExchangeCoin(&gObj[aIndex],1);
					return;
				}
				else if ( aRecv[1] == 0x08 && aRecv[3] == 0x0D && aRecv[4] == 0x1E )
				{
					// -- Lucky Coin Exchange 30 Coin
					gLuckyCoin.ExchangeCoin(&gObj[aIndex],2);
					return;
				}
				else
				{
					g_IllusionTemple.ProtocolCore(aRecv, aIndex);
				}
				break;
#endif
			case 0xC0:
				FriendListRequest(aIndex);
				break;
			case 0xC1:
				FriendAddRequest((PMSG_FRIEND_ADD_REQ *)aRecv, aIndex);
				break;
			case 0xC2:
				WaitFriendAddRequest((PMSG_FRIEND_ADD_SIN_RESULT *)aRecv, aIndex);
				break;
			case 0xC3:
				FriendDelRequest((PMSG_FRIEND_DEL_REQ *)aRecv, aIndex);
				break;
			case 0xC4:
				FriendStateClientRecv((PMSG_FRIEND_STATE_C *)aRecv, aIndex);
				break;
			case 0xC5:
				FriendMemoSend((PMSG_FRIEND_MEMO *)aRecv, aIndex);
				break;
			case 0xC7:
				FriendMemoReadReq((PMSG_FRIEND_READ_MEMO_REQ *)aRecv, aIndex);
				break;
			case 0xC8:
				FriendMemoDelReq((PMSG_FRIEND_MEMO_DEL_REQ *)aRecv, aIndex);
				break;
			case 0xC9:
				FriendMemoListReq(aIndex);
				break;
			case 0xCA:
				FriendChatRoomCreateReq((PMSG_FRIEND_ROOMCREATE_REQ *)aRecv, aIndex);
				break;
			case 0xCB:
				FriendRoomInvitationReq((PMSG_ROOM_INVITATION *)aRecv, aIndex);
				break;
			case 0xD0:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
					case 0x07: //WereWolf
						CGWereWolfQuarrelCheck(aIndex);
						break;
					case 0x08: //GateKeeper
						CGGateKeeperCheck(aIndex);
						break;
					}
				}
				break;
			case 0xD1:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							CGReqKanturuStateInfo((PMSG_REQ_KANTURU_STATE_INFO *)aRecv, aIndex);
							break;
						case 0x01:
							GCReqEnterKanturuBossMap((PMSG_REQ_ENTER_KANTURU_BOSS_MAP *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xD2:	/* Cash Shop -> New Protocol 0xF5 */
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							g_CashShop.CGCashShopOpen(lpObj, (PMSG_REQ_CASHSHOPOPEN *)aRecv);
							break;
						case 0x03:
							g_CashShop.CGCashPoint(lpObj);
							break;
						case 0x05:
							g_CashShop.GCCashItemListSend(&gObj[aIndex], (PMSG_REQ_CASHITEMLIST *)aRecv);
							break;
						case 0x07:
							g_CashShop.CGCashItemBuy(&gObj[aIndex], (PMSG_REQ_CASHITEM_BUY *)aRecv);
							break;
					}
				}
				break;
			case 0xF2:
				GCServerMsgStringSend("Re Initializate",aIndex,1);
				break;

			case 0xF6: /* Quest System */
				QuestNew.ProtocolManager(aRecv,aIndex);
				return;
				break;

			case 0xF7: /* Imperial Fort */
				gImperialFort.ProtocolManager(aRecv,aIndex);
				return;
				break;

			case 0xF8: /* Gen System */
				Gens.GenSystemManager(lpObj,aRecv);
				return;
				break;

			case 0x4A:
				if ( DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGMagicAttack((PMSG_MAGICATTACK *)aRecv, aIndex);
				}
				return;
				break;

			case 0x4B:
				NewSeason6Attack((PMSG_NEWATTACK_S6 *)aRecv,aIndex);
				return;
				break;

			default:
				CLog.Debug("[ProtocolCore]: [Account:%s][Name:%s][HEAD:%x][State:%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, protoNum, gObj[aIndex].Connected);
				gNet.CloseClient(aIndex);
		}	
	}
}
// -----------------------------------------------------------------------------------------------
void NewSeason6Attack(PMSG_NEWATTACK_S6 *lpMsg,int aIndex)
{
	LPOBJ lpObj;

	if ( gObj[aIndex].CloseCount >= 0 ){
		CLog.Debug("[NewSeason6Attack]: Clouse count > -1");
		return;
	}

	lpObj = &gObj[aIndex];
		
	CMagicInf * lpMagic;

	int iMagicNumber = MAKE_NUMBERW(lpMsg->MagicNumberH, lpMsg->MagicNumberL);

	lpMagic = gObjGetMagicSearch(lpObj, iMagicNumber);

	if (!lpMagic){
		CLog.Debug("[NewSeason6Attack]: User Magic Error");
		return;
	}

	PMSG_DURATION_MAGIC_SEND pMsg;

	C3HeadSet((PBYTE)&pMsg,0x4A,sizeof(pMsg));

	pMsg.MagicNumberH = SET_NUMBERH(lpMagic->m_Skill);
	pMsg.MagicNumberL = SET_NUMBERL(lpMagic->m_Skill);
	pMsg.X = gObj[aIndex].X;
	pMsg.Y = gObj[aIndex].Y;
	pMsg.Dir = gObj[aIndex].Dir;
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);

	if(lpObj->Type == OBJ_USER){
		gSendProto.DataSend(lpObj->m_Index,(PBYTE)&pMsg,pMsg.h.size);
	}

}
// -----------------------------------------------------------------------------------------------
void CGLiveClient(PMSG_CLIENTTIME * lpClientTime, short aIndex)
{
	if ( gObj[aIndex].Connected > PLAYER_CONNECTED )
	{
		gObj[aIndex].ConnectCheckTime = GetTickCount();
	}
}
// -----------------------------------------------------------------------------------------------
struct PMSG_CHECK_MAINEXE
{
	PBMSG_HEAD h;	// C1:03
	WORD m_wKey;	// 4
};
// -----------------------------------------------------------------------------------------------
void GCCheckMainExeKeySend(int aIndex)
{
	PMSG_CHECK_MAINEXE pMsg;

	gObj[aIndex].CheckSumTableNum = rand() % MAX_CHECKSUM_KEY;
	gObj[aIndex].CheckSumTime = GetTickCount();
	C1HeadSet((LPBYTE)&pMsg, 0x03, sizeof(pMsg));
	pMsg.m_wKey = EncryptCheckSumKey(gObj[aIndex].CheckSumTableNum);

	gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
WORD EncryptCheckSumKey(WORD wSource)
{
	WORD wRandom = rand() % 64;
	WORD wAcc = ((wSource & 0x3F0) * 64) | (wRandom *16) | (wSource & 0x0F);
	return wAcc ^ 0xB479;
}
// -----------------------------------------------------------------------------------------------
void CheckMainRecieve(PMSG_CHECK_MAINEXE_RESULT * lpMsg, int aIndex)
{
	char Message[256];
	LPOBJ lpObj = &gObj[aIndex];

	if ( strcmp(lpObj->Ip_addr, "127.0.0.1") == 0 )
	{
		lpObj->CheckSumTime = 0;
		lpObj->m_InWebzen = TRUE;
		return;
	}

	if ( HRB_EnableCheckSum == TRUE )
	{
		if (lpObj->CheckSumTableNum < 0 || lpObj->CheckSumTableNum > MAX_CHECKSUM_KEY-1 )
		{
			CLog.LogAddC(TColor.Red(), "Error-L1 : [%s][%s] CheckSum-Exe Error ", lpObj->AccountID, lpObj->Name);
			gNet.CloseClient(aIndex);
			return;
		}

		if (dwgCheckSum[gObj[aIndex].CheckSumTableNum] != lpMsg->m_dwKey)
		{
			CLog.LogAddC(TColor.Red(), "Error-L1 : CheckSum-Exe Error %d %d %d [%s]", dwgCheckSum[lpObj->CheckSumTableNum], lpMsg->m_dwKey, lpObj->CheckSumTableNum, lpObj->AccountID);
			gNet.CloseClient(aIndex);
			return;
		}

		if ( lpObj->CheckSumTableNum < 0 || lpObj->CheckSumTableNum > MAX_CHECKSUM_KEY-1 )
		{
			CLog.LogAddC(TColor.Red(), "Error-L1 : [%s][%s] CheckSum-Exe Error ", lpObj->AccountID, lpObj->Name);
			gNet.CloseClient(aIndex);
			return;
		}

		if ( dwgCheckSum[lpObj->CheckSumTableNum] != lpMsg->m_dwKey)
		{
			CLog.LogAddC(TColor.Red(), "Error-L1 : CheckSum-Exe Error %d %d %d [%s]",dwgCheckSum[lpObj->CheckSumTableNum], lpMsg->m_dwKey, lpObj->CheckSumTableNum, lpObj->AccountID);
			gNet.CloseClient(aIndex);
			return;
		}

	}

	lpObj->CheckSumTime = 0;

	sprintf(Message,ConnectGameNotice,lpObj->Name);
	GCServerMsgStringSend(Message, aIndex, 0);
	Gens.GenCheck(lpObj);
	QuestNew.CheckQuest(aIndex);
	gObjNextExpCal(lpObj);
	gSkillTree.SendMasterData(aIndex);
	gSkillTree.SendMasterSkills(aIndex);
}
// -----------------------------------------------------------------------------------------------
void PEchoProc(unsigned char * aMsg, int aLen, short aIndex)
{
	for ( int n = 0 ; n< OBJMAX ; n++)
	{
		if ( gObj[n].Connected >= PLAYER_CONNECTED )
		{
			gSendProto.DataSend(n, aMsg, aLen);
		}
	}
}
// -----------------------------------------------------------------------------------------------
void GCResultSend(int aIndex, BYTE headcode, BYTE result)
{
	PMSG_DEFRESULT pResult;

	C1HeadSet((LPBYTE)&pResult, headcode, sizeof(pResult));
	pResult.result = result;
	
	gSendProto.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);
}
// -----------------------------------------------------------------------------------------------
void ChatSend(LPOBJ lpObj, char* szChat)
{
	int len = strlen(szChat);

	if ( len < 1 )
		return;

	if ( len > MAX_CHAT_LEN-1 )
		return;

	PMSG_CHATDATA_NUMBER pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x01, len+6);
	strcpy(pMsg.chatmsg, szChat);
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);

	gSendProto.VPMsgSend(lpObj, (UCHAR*)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
void vChatSend(LPOBJ lpObj, char* szChat, ...)
{
	char szBuffer[256] = "";
	va_list pArguments;

	va_start(pArguments, szChat);
	vsprintf(szBuffer, szChat, pArguments);
	va_end(pArguments);

	int len = strlen(szBuffer);

	if ( len < 1 )
		return;

	if ( len > MAX_CHAT_LEN-1 )
		return;

	PMSG_CHATDATA_NUMBER pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x01, len+6);
	strcpy(pMsg.chatmsg, szBuffer);
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);

	gSendProto.VPMsgSend(lpObj, (UCHAR*)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
void ChatTargetSend(LPOBJ lpObj, char * szChat, int senduser)
{
	int len = strlen(szChat);

	if ( len < 1 || len > MAX_CHAT_LEN-1)
		return;

	PMSG_CHATDATA_NUMBER pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x01, len+6);
	strcpy(pMsg.chatmsg, szChat);
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);
	pMsg.chatmsg[len+1] = 0;

	gSendProto.DataSend(senduser, (UCHAR*)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
void GCServerMsgStringSend(LPSTR  szMsg, int aIndex, BYTE type) 
{
	//PMSG_NOTICE pNotice;

	SendMsg.MessageOut(aIndex,type,szMsg);
	
	//TNotice::MakeNoticeMsg((TNotice*)&pNotice, type, szMsg);
	//gSendProto.DataSend(aIndex, (UCHAR*)&pNotice, pNotice.h.size);
}
// -----------------------------------------------------------------------------------------------
void GCServerMsgStringSendGuild(_GUILD_INFO_STRUCT* lpNode, LPSTR szMsg, BYTE type)
{
	if ( lpNode == NULL )
	{
		return;
	}

	for ( int n = 0; n<MAX_USER_GUILD ; n++)
	{
		if (lpNode->Use[n] > 0 && lpNode->Index[n] >= 0 )
		{
			GCServerMsgStringSend((char*)szMsg, lpNode->Index[n], type);
		}
	}
}
// -----------------------------------------------------------------------------------------------
struct PMSG_EVENT
{
	PBMSG_HEAD h;	// C1:0B
	BYTE State;	// 3
	BYTE Event;	// 4
};
// -----------------------------------------------------------------------------------------------
void GCEventStateSend(int aIndex, BYTE state, BYTE Event)
{
	PMSG_EVENT pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x0B, sizeof(pMsg));
	pMsg.Event = Event;
	pMsg.State = state;

	gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
void GCMapEventStateSend(int map, BYTE state, BYTE Event)
{
	PMSG_EVENT pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x0B, sizeof(pMsg));
	pMsg.Event = Event;
	pMsg.State = state;

	for ( int n=0; n<OBJMAX ; n++ )
	{
		if ( gObj[n].Connected == PLAYER_PLAYING && gObj[n].Type == OBJ_USER && map == gObj[n].MapNumber )
		{
			gSendProto.DataSend(n, (UCHAR*)&pMsg, pMsg.h.size);
		}
	}
}
// -----------------------------------------------------------------------------------------------
struct PMSG_JOINRESULT
{
	PBMSG_HEAD h;	// C1:F1
	BYTE scode;	// 3
	BYTE result;	// 4
	BYTE NumberH;	// 5
	BYTE NumberL;	// 6
	BYTE CliVersion[5];	// 7
};
// -----------------------------------------------------------------------------------------------
void SCPJoinResultSend(int aIndex, BYTE result)
{
	PMSG_JOINRESULT pResult;

	memset(&pResult, 0, sizeof(pResult));

	pResult.h.size = sizeof(pResult);
	pResult.h.c = 0xC1;
	pResult.h.headcode = 0xF1;
	pResult.scode = 0x00;
	pResult.result = result;
	pResult.NumberH = SET_NUMBERH(aIndex);
	pResult.NumberL = SET_NUMBERL(aIndex);
	pResult.CliVersion[0] = szClientVersion[0];
	pResult.CliVersion[1] = szClientVersion[1];
	pResult.CliVersion[2] = szClientVersion[2];
	pResult.CliVersion[3] = szClientVersion[3];
	pResult.CliVersion[4] = szClientVersion[4];

	gSendProto.DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
	gObj[aIndex].ConnectCheckTime = GetTickCount();
}
// -----------------------------------------------------------------------------------------------
void CSCheckMainInclusions(PMSG_CHECKMAIN_INC* lpMsg, int aIndex)
{
	char MainSerial[17];
	char UserID[11];

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpMsg->CliVersion[0] != szClientVersion[0] ||
		 lpMsg->CliVersion[1] != szClientVersion[1] ||
		 lpMsg->CliVersion[2] != szClientVersion[2] ||
		 lpMsg->CliVersion[3] != szClientVersion[3] ||
		 lpMsg->CliVersion[4] != szClientVersion[4] )
	{
		GCJoinResult(JS_BAD_CLIENT_VERSION, aIndex);
		gNet.CloseClient(aIndex);
		return;
	}

	MainSerial[16] = 0;

	memcpy(MainSerial, lpMsg->CliSerial, sizeof(lpMsg->CliSerial));
	
	UserID[10] = 0;
	
	memcpy(UserID, lpMsg->Id, sizeof(lpMsg->Id));
	
	BuxConvert(UserID, MAX_ACCOUNT_LEN);

	if ( strcmp(MainSerial, szGameServerExeSerial) != NULL )
	{
		CLog.Connect("Error-L1: [%s] Inavlid Serial [%s]", UserID, MainSerial);
		GCJoinResult(JS_BAD_CLIENT_VERSION, aIndex);
		gNet.CloseClient(aIndex);
		return;
	}

	if(!FirewallC.CheckIP(aIndex))
	{
		CLog.Connect("Error: [%s][%s][BLOCKED] Was trying to enter The Server.", UserID, lpObj->Ip_addr, MainSerial);
		gNet.CloseClient(aIndex);
		return;
	}

	if ( bCanConnectMember == TRUE )
	{
		if ( ConMember.IsMember(UserID) == FALSE )
		{
			CLog.Connect("(%d)[%s] Trying to Enter VIP Server.",aIndex, gObj[aIndex].AccountID);
			GCJoinResult(JS_ONLY_VIP_MEMBERS, aIndex);
			return;
		}
	}

	if (PacketCheckTime(lpObj) == FALSE )
	{
		CLog.Connect("Error-L2 : Login Time is over. (%d)(%s)", aIndex, UserID);
		gNet.CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].Connected != PLAYER_CONNECTED )
	{
		CLog.Connect("Error-L2 : Authentication is allowed After Connected. (%d)(%s)", aIndex, UserID);
		gNet.CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].LoginMsgSnd != FALSE )
	{
		if ( gObj[aIndex].LoginMsgSnd == TRUE )
		{
			CLog.Connect("Error : It's after Authentication Information is Sent.(%d)(%s)(%s)", aIndex, UserID,
				gObj[aIndex].Ip_addr );
		}
		else
		{
			CLog.Connect("Error : %d %s %d", gObj[aIndex].LoginMsgSnd, __FILE__, __LINE__);
		}

		return;
	}

	SDHP_CHECKMAIN_INC spMsg;

	C1HeadSet((LPBYTE)&spMsg, 0x01, sizeof(spMsg));
	spMsg.Number = aIndex;
	memcpy(spMsg.ID, lpMsg->Id, sizeof(spMsg.ID));
	memcpy(spMsg.Password, lpMsg->Pass, sizeof(spMsg.Password));
	strcpy(spMsg.IpAddress, gObj[aIndex].Ip_addr);
	gObj[aIndex].CheckTick = lpMsg->TickCount;
	gObj[aIndex].CheckTick2 = GetTickCount();
	gObj[aIndex].ConnectCheckTime = GetTickCount();
	gObj[aIndex].CheckSpeedHack = true;
	gObj[aIndex].ConnectCheckTime = GetTickCount();
	gObj[aIndex].LoginMsgSnd = 1;
	gObj[aIndex].LoginMsgCount++;
	gObj[aIndex].AccountID[MAX_ACCOUNT_LEN] = 0;
	memcpy(gObj[aIndex].AccountID, UserID, MAX_ACCOUNT_LEN);
	gObj[aIndex].m_cAccountItemBlock = 0;

	wsJServerCli.DataSend((char*)&spMsg, spMsg.hEaD.size);

	CLog.Connect("(%d)[%s] Send Join Request.", aIndex, gObj[aIndex].AccountID);

	lpObj->m_bMapSvrMoveReq = FALSE;
	lpObj->m_sPrevMapSvrCode = -1; 
	lpObj->m_sDestMapNumber = -1;
	lpObj->m_btDestX = 0;
	lpObj->m_btDestY = 0;
}
// -----------------------------------------------------------------------------------------------
void GCJoinResult(BYTE result, int aIndex)
{
	PMSG_RESULT  pResult;

	C1SubHeadSet((LPBYTE)&pResult, 0xF1, 0x01, sizeof(pResult));
	pResult.result = result;
	gSendProto.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);
}
// -----------------------------------------------------------------------------------------------
void GCJoinSocketResult(BYTE result, SOCKET Socket)
{
	PMSG_RESULT pResult;

	C1SubHeadSet((LPBYTE)&pResult , 0xF1, 0x01, sizeof(pResult) );
	pResult.result= result;

	wsGServer.DataSocketSend(Socket, (char*)&pResult, pResult.h.size);
}
// -----------------------------------------------------------------------------------------------
void CGCloseGamePlay(PMSG_CLIENTCLOSE * lpMsg, int aIndex)
{
	switch ( lpMsg->Flag )
	{
		if ( aIndex < 0 || aIndex > OBJMAX-1)
		{
			return;
		}

		//-- GOTO :: Close Game
		case 0:
		{
			gObjCloseSet(aIndex, 0);
			CLog.Disconnect("[GameExit][%s][%s] Map:[%d][%d/%d]",gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y);
		}break;

		//-- GOTO :: Select Character
		case 1:
		{
			gObjCloseSet(aIndex, 1);
			CLog.Disconnect("[CharSelect][%s][%s] Map:[%d][%d/%d]",gObj[aIndex].AccountID,gObj[aIndex].Name, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y);
		}break;

		//-- GOTO :: Server List
		case 2:
		{
			gObjCloseSet(aIndex, 2);
			CLog.Disconnect("[ServerList] [%s][%s] Map:[%d][%d/%d]",gObj[aIndex].AccountID,gObj[aIndex].Name, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y);
		}break;
	}
}
// -----------------------------------------------------------------------------------------------
void CGHackCheckClient(PMSG_CLIENTMSG * lpMsg, int aIndex)
{
	char szTemp[255];

	if ( lpMsg->Flag )
	{
		CLog.LogAddC(TColor.Red(),"Error-L1 : HackCheck [%s][%s] 0x%x 0x%x",gObj[aIndex].AccountID, gObj[aIndex].Name, lpMsg->Flag, lpMsg->subFlag);
	}
	else 
	{
		if ( gObj[aIndex].m_ClientHackLogCount < 100 )
		{
			CLog.LogAddC(TColor.Red(), "Error-L1 : HackCheck [%s][%s] 0x%x 0x%x",gObj[aIndex].AccountID, gObj[aIndex].Name, lpMsg->Flag, lpMsg->subFlag);
		}

		gObj[aIndex].m_ClientHackLogCount++;

		if ( gObj[aIndex].m_ClientHackLogCount >= 100 )
		{
			gObj[aIndex].m_ClientHackLogCount = 100;
		}
	}

	wsprintf(szTemp, "Client HackCheck %d", lpMsg->Flag);
	gSendHackLog.Send(aIndex, 0, szTemp);
}
// -----------------------------------------------------------------------------------------------
void CGCharacterCreate( PMSG_CHARCREATE * lpMsg, int aIndex)
{
	char szTempA[20];
	char szTempB[20];

	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}

	if ( gObj[aIndex].Connected < PLAYER_LOGGED )
	{
		CLog.LogAddC(TColor.Red(), "[CreateCharacter][%s] Requestd to Create Character Before Authentication. (%d) IP:(%s)", gObj[aIndex].AccountID, aIndex, gObj[aIndex].Ip_addr);
		gNet.CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].Connected == PLAYER_PLAYING )
	{
		CLog.LogAddC(TColor.Red(), "[ANTI-HACK][CreateCharacter][%s][%s] Attempted Creating Character During GamePlay.", gObj[aIndex].AccountID, gObj[aIndex].Name);
		gNet.CloseClient(aIndex);
		return;
	}

	if ( gCreateCharacter == FALSE)
	{
		CLog.LogAddC(TColor.Red(),"[ANTI-HACK][CreateCharacter][%s][%s] Attempting Creating Character During Prevented Time.", gObj[aIndex].AccountID, gObj[aIndex].Name);
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}

	if (lpMsg->ClassSkin == 0x00 || lpMsg->ClassSkin == 0x10 || 
		lpMsg->ClassSkin == 0x20 || lpMsg->ClassSkin == 0x30 || 
		lpMsg->ClassSkin == 0x40 || lpMsg->ClassSkin == 0x50 || 
		lpMsg->ClassSkin == 0x60)
	{
	}
	else
	{
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}

	//******************************************
	// Magic Gladiator - Dark Lord - Rage Fighter
	// RF = OBJECTSTRUCT::Magusa[1]
	// MG = OBJECTSTRUCT::Magusa[2]
	// DL = OBJECTSTRUCT::Magusa[3]
	// ELSE :: ERROR
	// ******************************************
	if ( lpMsg->ClassSkin == 0x60 ) // Rage Fighter
	{
		if ( gObj[aIndex].Magumsa < 1 )
		{
			CLog.LogAddC(TColor.Red(), "[CGCharacterCreate]: [%s] Try Creating RageFighter Character.", gObj[aIndex].AccountID);
			JGCharacterCreateFailSend(aIndex, lpMsg->Name);
			return;
		}
	}

	if ( lpMsg->ClassSkin == 0x30 ) // Magic Gladiator
	{
		if ( gObj[aIndex].Magumsa < 2 )
		{
			CLog.LogAddC(TColor.Red(), "[CGCharacterCreate]: [%s] Try Creating MagicGladiator Character.", gObj[aIndex].AccountID);
			JGCharacterCreateFailSend(aIndex, lpMsg->Name);
			return;
		}
	}

	if ( lpMsg->ClassSkin == 0x40 ) // Dark Lord
	{
		if ( gObj[aIndex].Magumsa < 3 )
		{
			CLog.LogAddC(TColor.Red(), "[CGCharacterCreate]: [%s] Try Creating DarkLord Character.", gObj[aIndex].AccountID);
			JGCharacterCreateFailSend(aIndex, lpMsg->Name);
			return;
		}
	}

	SDHP_CREATECHAR sCreate;

	memset(&sCreate, 0, sizeof(sCreate));
	C1HeadSet((LPBYTE)&sCreate, 0x04, sizeof(sCreate));
	sCreate.ClassSkin = lpMsg->ClassSkin;
	sCreate.Number = aIndex;
	memcpy(sCreate.AccountID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);
	memcpy(sCreate.PlayerName, lpMsg->Name, MAX_ACCOUNT_LEN);
	szTempA[10]=0;
	szTempB[10]=0;
	memcpy(szTempA, sCreate.AccountID, MAX_ACCOUNT_LEN);
	memcpy(szTempB, sCreate.PlayerName, MAX_ACCOUNT_LEN);
	
	CLog.LogAddC(TColor.Blue(), "[%s] Created Character Name: %s", szTempA, szTempB);

	cDBSMng.Send((char*)&sCreate, sCreate.hEaD.size);
}
// -----------------------------------------------------------------------------------------------
void CGCharacterDelete(PMSG_CHARDELETE* lpMsg, int aIndex)
{
	char szJoomin[11];
	char szTempA[20];
	char szTempB[20];

	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}

	if ( gObj[aIndex].Connected < PLAYER_LOGGED )
	{
		CLog.LogAddC(TColor.Red(), "[DeleteCharacter][%s] Requestd to Delete Character Before Authentication. (%d) IP:(%s)", gObj[aIndex].AccountID, aIndex, gObj[aIndex].Ip_addr);
		gNet.CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].Connected == PLAYER_PLAYING )
	{
		CLog.LogAddC(TColor.Red(), "[ANTI-HACK][DeleteCharacter][%s][%s] Attempted Deleting Character During GamePlay.", gObj[aIndex].AccountID, gObj[aIndex].Name);
		gNet.CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].bEnableDelCharacter == FALSE )
	{
		CLog.LogAdd("[ANTI-HACK][DeleteCharacter][%s][%s] Attempting Deleting Character During Prevented Time.", gObj[aIndex].AccountID, gObj[aIndex].Name);
		gNet.CloseClient(aIndex);
		return;
	}

	SDHP_CHARDELETE sDelete;
	PMSG_RESULT pResult;

	C1SubHeadSet((LPBYTE)&pResult, 0xF3, 0x02, sizeof(pResult));
	pResult.result = 0;

	if ( gObj[aIndex].m_cAccountItemBlock )
	{
		pResult.result = 3;
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( gGuildDestroy == FALSE )
	{
		pResult.result = 0;
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	memset(szJoomin, 0, sizeof(szJoomin));
	memcpy(szJoomin, lpMsg->LastJoominNumber, sizeof(lpMsg->LastJoominNumber));

	if (HRB_DeleteCharNoPass == FALSE)
	{
		if ( gObjJoominCheck(aIndex, szJoomin) == FALSE )
		{
			pResult.result = 2;
			gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}

	C1HeadSet((LPBYTE)&sDelete, 0x05, sizeof(sDelete));
	sDelete.Number = aIndex;
	memcpy(sDelete.AccountID, gObj[aIndex].AccountID, sizeof(sDelete.AccountID));
	memcpy(sDelete.Name, lpMsg->Name, sizeof(sDelete.Name));
	sDelete.Guild = 0;

	if ( gObj[aIndex].GuildNumber > 0 && gObj[aIndex].lpGuild != NULL)
	{
		if ( !strcmp(gObj[aIndex].Name, gObj[aIndex].lpGuild->Names[0] ))
		{
			sDelete.Guild = 1;	// Master
		}
		else
		{
			sDelete.Guild = 2;	// Member
		}

		memcpy(sDelete.GuildName, gObj[aIndex].lpGuild->Name, MAX_GUILD_LEN);
	}

	g_CashItemPeriodSystem.GDReqPeriodItemDelete(&gObj[aIndex], lpMsg->Name);
	szTempA[10]=0;
	szTempB[10]=0;
	memcpy(szTempA, sDelete.AccountID, MAX_ACCOUNT_LEN);
	memcpy(szTempB, sDelete.Name, MAX_ACCOUNT_LEN);

	CLog.LogAddC(TColor.Blue(),"[%s] Deleted Character Name: %s", szTempA, szTempB);

	cDBSMng.Send((PCHAR)&sDelete, sDelete.h.size);
	gObj[aIndex].Level = 0;
}
// -----------------------------------------------------------------------------------------------
void CGCharacterJoinRequest( PMSG_CHARMAPJOIN * lpMsg, int aIndex)
{
	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		return;
	}

	if (!gObjIsAccontConnect(aIndex, gObj[aIndex].AccountID))
	{
		return;
	}

	if (gObj[aIndex].Connected == PLAYER_PLAYING)
	{
		return;
	}

	gObj[aIndex].bEnableDelCharacter = FALSE;

	char PlayerName[MAX_ACCOUNT_LEN+1];
	SDHP_DBCHARINFOREQUEST pCRequest;

	C1HeadSet((LPBYTE)&pCRequest, 0x06, sizeof(pCRequest));
	memset(PlayerName, 0, MAX_ACCOUNT_LEN);
	memcpy(PlayerName, lpMsg->Name, MAX_ACCOUNT_LEN);
	BuxConvert(PlayerName, MAX_ACCOUNT_LEN);
	memcpy(pCRequest.Name, PlayerName, MAX_ACCOUNT_LEN);
	strcpy(pCRequest.AccountID, gObj[aIndex].AccountID);
	pCRequest.Number = aIndex;

	cDBSMng.Send((char*)&pCRequest, pCRequest.h.size);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_LEVELUP
{
	PBMSG_HEAD h;	// C1:F3:05
	unsigned char subcode;
	unsigned short Level;
	unsigned short LevelUpPoint;
	unsigned short MaxLife;
	unsigned short MaxMana;
	unsigned short wMaxShield;
	unsigned short MaxBP;
	short AddPoint;
	short MaxAddPoint;
	short MinusPoint;
	short MaxMinusPoint;
};
// -----------------------------------------------------------------------------------------------
void GCLevelUpMsgSend(int aIndex, int iSendEffect)
{
	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	short AddPoint=0;
	short MaxAddPoint=0;
	short MinusPoint=0;
	short MaxMinusPoint=0;

	gObjGetStatPointState(gObj[aIndex].m_Index, AddPoint, MaxAddPoint, MinusPoint, MaxMinusPoint);
	
	PMSG_LEVELUP pMsg;

	C1SubHeadSet((LPBYTE)&pMsg, 0xF3, 0x05, sizeof(pMsg));
	pMsg.Level = gObj[aIndex].Level;
	pMsg.LevelUpPoint = gObj[aIndex].LevelUpPoint;
	pMsg.MaxLife = (WORD)((float)gObj[aIndex].MaxLife+(float)gObj[aIndex].AddLife);
	pMsg.MaxMana = (WORD)((float)gObj[aIndex].MaxMana+(float)gObj[aIndex].AddMana);
	pMsg.MaxBP = gObj[aIndex].MaxBP + gObj[aIndex].AddBP;
	pMsg.wMaxShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
	pMsg.AddPoint = AddPoint;
	pMsg.MaxAddPoint = MaxAddPoint;
	pMsg.MinusPoint = MinusPoint;
	pMsg.MaxMinusPoint = MaxMinusPoint;

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	if ( iSendEffect == 1 )
	{
		GCSendEffectInfo(aIndex, 0x10);
	}
}
// -----------------------------------------------------------------------------------------------
void CGLevelUpPointAdd(PMSG_LVPOINTADD * lpMsg, int aIndex)
{
	if ( gObjIsConnected(aIndex) == FALSE)
	{
		gNet.CloseClient(aIndex);
		return;
	}

	PMSG_LVPOINTADDRESULT pMsg;

	C1SubHeadSet((LPBYTE)&pMsg, 0xF3, 0x06, sizeof(pMsg));
	pMsg.ResultType = 0;
	
	if (::gObjLevelUpPointAdd(&gObj[aIndex],lpMsg->Type) == TRUE )
	{
		pMsg.ResultType = 0x10;
		pMsg.ResultType += lpMsg->Type;

		switch ( lpMsg->Type )
		{
			//-- Vitality
			case 2:
				pMsg.MaxLifeAndMana = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
				break;

			//-- Energy
			case 3:
				pMsg.MaxLifeAndMana = gObj[aIndex].MaxMana + gObj[aIndex].AddMana;
				break;
		}

		pMsg.wMaxShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
		gObjSetBP(aIndex);
		pMsg.MaxBP = gObj[aIndex].MaxBP + gObj[aIndex].AddBP;
	}

	gSendProto.DataSend(aIndex, (UCHAR *)&pMsg, pMsg.hEaD.size);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_INVENTORYITEMMODIFY
{
	PBMSG_HEAD h;	// C1:F3:14
	BYTE subcode;	// 3
	BYTE Pos;	// 4
	BYTE ItemInfo[MAX_ITEM_INFO];	// 5

};
// -----------------------------------------------------------------------------------------------
void GCInventoryItemOneSend(int aIndex, int pos)
{
	if ( !gObj[aIndex].pInventory[pos].IsItem())
		return;

	PMSG_INVENTORYITEMMODIFY pMsg;

	C1SubHeadSet((LPBYTE)&pMsg, 0xF3, 0x14, sizeof(pMsg));
	pMsg.Pos = pos;
	ItemByteConvert(pMsg.ItemInfo, gObj[aIndex].pInventory[pos]);

	gSendProto.DataSend(aIndex, (UCHAR *)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_PKLEVEL
{
	PBMSG_HEAD h;	// C1:F3:08
	BYTE subcode;	// 3
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
	BYTE PkLevel;	// 6
};
// -----------------------------------------------------------------------------------------------
void GCPkLevelSend(int aIndex, BYTE pklevel)
{
	PMSG_PKLEVEL pMsg;

	C1SubHeadSet((LPBYTE)&pMsg, 0xF3, 0x08, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	pMsg.PkLevel = pklevel;

	gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
	gSendProto.VPMsgSend(&gObj[aIndex], (UCHAR*)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
// Magic List Send And Delete
// -----------------------------------------------------------------------------------------------
struct PMSG_MAGICLIST
{
	char Pos;	// 0
	BYTE MagicInfo[MAX_MAGIC_INFO];	// 1
};
// -----------------------------------------------------------------------------------------------
struct PMSG_MAGICLISTCOUNT
{
	PBMSG_HEAD h;	// C1:F3:11
	BYTE subcode;	// 3
	BYTE Count;	// [0xFE:AddOne] [0xFF:DelOne] 4
	BYTE btListType;	// 5
};
// -----------------------------------------------------------------------------------------------
void GCMagicListOneSend(int aIndex, char Pos, WORD type, BYTE level, WORD skill, BYTE btListType)
{
	PMSG_MAGICLISTCOUNT pCount;
	PMSG_MAGICLIST pList;
	BYTE sendbuf[1000];
	int lOfs = sizeof(pCount);

	C1SubHeadSet((LPBYTE)&pCount, 0xF3, 0x11, 0);
	pCount.Count = 0xFE;
	pCount.btListType = btListType;
	pList.Pos = Pos;

	pList.MagicInfo[DBM_TYPE] = SET_NUMBERL(type);
	pList.MagicInfo[DBM_TYPE+1] = SET_NUMBERH(type);
	pList.MagicInfo[DBM_LEVEL] = level<<3;
	pList.MagicInfo[DBM_LEVEL] |= skill & 0x07;
	memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
	lOfs+= sizeof(pList);
	pCount.h.size = lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));

	gSendProto.DataSend(aIndex, sendbuf, lOfs);
}
// -----------------------------------------------------------------------------------------------
void GCMagicListOneDelSend(int aIndex, char Pos, WORD type, BYTE level, WORD skill, BYTE btListType)
{
	PMSG_MAGICLISTCOUNT pCount;
	PMSG_MAGICLIST pList;
	BYTE sendbuf[1000];
	int lOfs = sizeof(pCount);

	C1SubHeadSet((LPBYTE)&pCount, 0xF3, 0x11, 0);
	pCount.h.size = 0;
	pCount.Count = 0xFF;
	pCount.btListType = btListType;
	pList.Pos = Pos;

	pList.MagicInfo[DBM_TYPE] = SET_NUMBERL(type);
	pList.MagicInfo[DBM_TYPE+1] = SET_NUMBERH(type);
	pList.MagicInfo[DBM_LEVEL] = level<<3;
	pList.MagicInfo[DBM_LEVEL] |= skill & 0x07;
	memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
	lOfs+= sizeof(pList);
	pCount.h.size = lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));

	gSendProto.DataSend(aIndex, sendbuf, lOfs);
}
// -----------------------------------------------------------------------------------------------
void GCMagicListMultiSend(LPOBJ lpObj, BYTE btListType)
{
	PMSG_MAGICLISTCOUNT pCount;
	PMSG_MAGICLIST pList;
	BYTE sendbuf[1000];
	int lOfs = sizeof(pCount);
	int Count = 0;
	pCount.btListType = btListType;

	C1SubHeadSet((LPBYTE)&pCount, 0xF3, 0x11, 0);

	for ( int n=0;n<MAX_MAGIC;n++)
	{
		if ( lpObj->Magic[n].IsMagic() == true && lpObj->Magic[n].m_Skill < 300 )
		{
			pList.Pos = n;
			pList.MagicInfo[DBM_TYPE] = SET_NUMBERL(lpObj->Magic[n].m_Skill);
			pList.MagicInfo[DBM_TYPE+1] = SET_NUMBERH(lpObj->Magic[n].m_Skill);
			pList.MagicInfo[DBM_LEVEL] = lpObj->Magic[n].m_Level<<3;
			pList.MagicInfo[DBM_LEVEL] |= lpObj->Magic[n].m_Skill & 0x07;
			memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
			lOfs+= sizeof(pList);
			Count++;
		}
	}

	pCount.Count = Count;
	pCount.h.size = lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));

	gSendProto.DataSend(lpObj->m_Index, sendbuf, lOfs);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_EQUIPMENTLIST
{
	PBMSG_HEAD h;	// C1:F3:13
	BYTE subcode;	// 3
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
	BYTE Equipment[CHAR_SET_SIZE];	// 6
};
// -----------------------------------------------------------------------------------------------
void GCEquipmentSend(int aIndex)
{
	PMSG_EQUIPMENTLIST pMsg;

	C1SubHeadSet((LPBYTE)&pMsg, 0xF3, 0x13, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	gObjMakePreviewCharSet(aIndex);
	LPOBJ lpObj = &gObj[aIndex];
	memcpy(pMsg.Equipment, lpObj->CharSet, sizeof(lpObj->CharSet));

	gSendProto.VPMsgSend(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_RECALLMONLIFE
{
	PBMSG_HEAD h;	// C1:F3:20
	BYTE subcode;	// 3
	BYTE Life;	// 4
};
// -----------------------------------------------------------------------------------------------
void GCRecallMonLife(int aIndex, int maxlife, int life)
{
	if ( maxlife <= 0 )
		return;
	
	PMSG_RECALLMONLIFE pMsg;
	BYTE per = life * 100 / maxlife;
	C1SubHeadSet((LPBYTE)&pMsg, 0xF3, 0x20, sizeof(pMsg));
	pMsg.Life = per;

	gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_TIMEVIEW
{
	PBMSG_HEAD h;	// C1:F3:22
	BYTE subcode;	// 3
	WORD Second;	// 4
};
// -----------------------------------------------------------------------------------------------
void GCTimeViewSend(int aIndex, int second)
{
	PMSG_TIMEVIEW pMsg;

	C1SubHeadSet((LPBYTE)&pMsg, 0xF3, 0x22, sizeof(pMsg));
	pMsg.Second = second;

	gSendProto.DataSend(aIndex, (UCHAR *)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_GOALSEND
{
	PBMSG_HEAD h;	// C1:F3:23
	BYTE subcode;	// 3
	char RedTeamName[8];	// 4
	BYTE RedTeamScore;	// C
	char BlueTeamName[8];	// D
	BYTE BlueTeamScore;	// 15
};
// -----------------------------------------------------------------------------------------------
void GCGoalSend(int aIndex, char* Name1, BYTE score1, char* Name2, BYTE score2)
{
	PMSG_GOALSEND pMsg;

	C1SubHeadSet((LPBYTE)&pMsg, 0xF3, 0x23, sizeof(pMsg));
	memcpy(pMsg.RedTeamName, Name1, sizeof(pMsg.RedTeamName));
	pMsg.RedTeamScore = score1;
	memcpy(pMsg.BlueTeamName, Name2, sizeof(pMsg.BlueTeamName));
	pMsg.BlueTeamScore = score2;

	gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
void GCSkillKeyRecv(PMSG_SKILLKEY * lpMsg, int aIndex)
{
	if ( !gObjIsGamePlaing(&gObj[aIndex])){
		return;
	}
	DGOptionDataSend(aIndex, gObj[aIndex].Name, lpMsg->SKillKey, lpMsg->GameOption, lpMsg->QkeyDefine,  lpMsg->WkeyDefine, lpMsg->EkeyDefine, lpMsg->ChatWindow);
}
// -----------------------------------------------------------------------------------------------
void GCSkillKeySend(int aIndex, LPBYTE keybuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd)
{
	PMSG_SKILLKEY pMsg;

	C1SubHeadSet((LPBYTE)&pMsg, 0xF3, 0x30, sizeof(pMsg));
	memcpy(pMsg.SKillKey, keybuffer, sizeof(pMsg.SKillKey));
	pMsg.GameOption = GO;
	pMsg.QkeyDefine = Qk;
	pMsg.WkeyDefine = Wk;
	pMsg.EkeyDefine = Ek;
	pMsg.ChatWindow = ChatWnd;

	gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
	CLog.LogAdd("Option Send %d %d %d %d", GO, Qk, Wk, Ek);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_ITEMGETRESULT
{
	PBMSG_HEAD h;	// C1:22
	BYTE result;	// [0xFE:Money] 3
	BYTE Data[MAX_ITEM_INFO];	// 4
};
// -----------------------------------------------------------------------------------------------
void GCMoneySend(int aIndex, DWORD money)
{
	PMSG_ITEMGETRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x22;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = 0xFE;

	WORD hiWord = SET_NUMBERHW(money);
	WORD loWord = SET_NUMBERLW(money);
	pMsg.Data[0] = SET_NUMBERH(hiWord);
	pMsg.Data[1] = SET_NUMBERL(hiWord);
	pMsg.Data[2] = SET_NUMBERH(loWord);
	pMsg.Data[3] = SET_NUMBERL(loWord);

	gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
void GCItemInventoryPutSend(int aIndex,BYTE result, BYTE iteminfo1, BYTE iteminfo2)
{
	PMSG_ITEMGETRESULT pResult;

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x22;
	pResult.h.size = sizeof(pResult);
	pResult.result = result;
	pResult.Data[0] = iteminfo1;
	pResult.Data[1] = iteminfo2;
	pResult.h.size -= 2;

	gSendProto.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);
}
// -----------------------------------------------------------------------------------------------
void CGItemGetRequest(PMSG_ITEMGETREQUEST * lpMsg, int aIndex)
{
	int item_num, map_num;
	PMSG_ITEMGETRESULT pResult;
	int type;
	int level;
	int special;
	int NOption;
	char szItemName[50];
	CMapItem * lpItem;

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x22;
	pResult.h.size = sizeof(pResult);
	pResult.result = -1;

	if ( !gObjIsConnected(aIndex))
	{
		gNet.CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		return;
	}

	if ( gObj[aIndex].DieRegen != 0 )
	{
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		gSendProto.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);
		return;
	}

	if ( gObj[aIndex].m_IfState.use != 0 )
	{
		if ( gObj[aIndex].m_IfState.type != 3 )
		{
			::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 94)), aIndex, 1);
			pResult.result = -1;
			pResult.h.size -= sizeof(pResult.Data);
			gSendProto.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);
			return;
		}
	}

	if ( !::gObjFixInventoryPointer(aIndex))
	{
		CLog.LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].pTransaction == TRUE )
	{
		CLog.LogAdd("[%s][%s] CGItemGetRequest() Failed : Transaction == 1, IF_TYPE : %d",	gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
		return;
	}

	item_num = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( MAX_ITEM_TYPE_RANGE(item_num) == FALSE )
	{
		CLog.LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		gSendProto.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);
		return;
	}

	map_num = gObj[aIndex].MapNumber;

	if ( MAX_MAP_RANGE(map_num) == FALSE )
	{
		CLog.LogAdd("error-L3 : %s %d", __FILE__, __LINE__);
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		gSendProto.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);
		return;
	}

	lpItem = &MapC[map_num].m_cItem[item_num];

	if ( lpItem->IsItem() == TRUE && lpItem->Give == false && lpItem->live == true)
	{
		if ( lpItem->m_QuestItem != false )
		{
			int ItemCount = gObjGetItemCountInIventory(aIndex, lpItem->m_Type);

			if ( ItemCount > 0 )
			{
				CLog.LogAdd("[Quest] Too many quest item [%s][%s] (%s) %d", gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), ItemCount);
				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);
				gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				return;
			}
		}

		if ( lpItem->m_Type == ITEMGET(13,20) )	// Wizard Ring
		{
			switch ( lpItem->m_Level )
			{
				case 0:
				{
					int iWRCount = gObjGetItemCountInIventory(aIndex, lpItem->m_Type/MAX_SUBTYPE_ITEMS, lpItem->m_Type%MAX_SUBTYPE_ITEMS, lpItem->m_Level);

					if ( iWRCount > 0 )
					{
						CLog.LogAdd("[Ring Event] Too many Magician's Ring [%s][%s] (Name:%s, Count:%d)", gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), iWRCount);

						pResult.result = -1;
						pResult.h.size -= sizeof(pResult.Data);

						gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
						::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 186)), aIndex, 1);

						return;
					}
					break;
				}
				case 1:
				{
					pResult.result = -1;
					pResult.h.size -= sizeof(pResult.Data);

					gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return;
					break;
				}	
			}
		}

		if ( lpItem->m_Type == ITEMGET(13,20) && lpItem->m_Level == 2 )	// Wizard Ring
		{
			pResult.result = -1;
			pResult.h.size -= sizeof(pResult.Data);

			gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			

			return;
		}

		if ( lpItem->m_Type == ITEMGET(13,38) )	// Moonstone Ring
		{
			int count = gObjGetItemCountInIventory(aIndex, lpItem->m_Type/MAX_SUBTYPE_ITEMS,
				lpItem->m_Type % MAX_SUBTYPE_ITEMS, lpItem->m_Level);

			if( count > 0 )
			{
				CLog.LogAdd("[MoonStone Ring] Too many MoonStone Ring [%s][%s] ( Name:%s, Count:%d )", gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), count);

				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);

				gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);	
				GCServerMsgStringSend(lMsg.Get(MSGGET(13, 56)), aIndex, 1);
				return;
			}
		}
		else if ( lpItem->m_Type == ITEMGET(13,39) ) // Skeleton Warrior Ring
		{
			int count = gObjGetItemCountInIventory(aIndex, lpItem->m_Type/MAX_SUBTYPE_ITEMS, lpItem->m_Type % MAX_SUBTYPE_ITEMS, lpItem->m_Level);

			if ( count > 0 )
			{
				CLog.LogAdd("[ChangeRing] Too many Skeleton Warrior Ring [%s][%s] ( Name:%s, Count:%d )", gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), count);
				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);
				gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);	
				GCServerMsgStringSend(lMsg.Get(MSGGET(13, 63)), aIndex, 1);
				return;
			}
		}
		else if ( lpItem->m_Type == ITEMGET(13,40) ) // Jack O'Lantern Ring
		{
			int count = gObjGetItemCountInIventory(aIndex, lpItem->m_Type/MAX_SUBTYPE_ITEMS, lpItem->m_Type % MAX_SUBTYPE_ITEMS, lpItem->m_Level);

			if ( count > 0 )
			{
				CLog.LogAdd("[ChangeRing] Too many Jack O'Lantern Ring [%s][%s] ( Name:%s, Count:%d )", gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), count);
				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);
				gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);	
				GCServerMsgStringSend(lMsg.Get(MSGGET(13, 63)), aIndex, 1);
				return;
			}
		}
		else if ( lpItem->m_Type == ITEMGET(13,41) ) // Santa Girl Ring
		{
			int count = gObjGetItemCountInIventory(aIndex, lpItem->m_Type/MAX_SUBTYPE_ITEMS, lpItem->m_Type % MAX_SUBTYPE_ITEMS, lpItem->m_Level);

			if ( count > 0 )
			{
				CLog.LogAdd("[ChangeRing] Too many Santa Girl Ring [%s][%s] ( Name:%s, Count:%d )", gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), count);
				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);
				gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);	
				GCServerMsgStringSend(lMsg.Get(MSGGET(13, 63)), aIndex, 1);
				return;
			}
		}
		else if ( lpItem->m_Type == ITEMGET(13,122) ) // Skeleton Ring
		{
			int count = gObjGetItemCountInIventory(aIndex, lpItem->m_Type/MAX_SUBTYPE_ITEMS, lpItem->m_Type % MAX_SUBTYPE_ITEMS, lpItem->m_Level);

			if ( count > 0 )
			{
				CLog.LogAdd("[ChangeRing] Too many Skeleton Ring [%s][%s] ( Name:%s, Count:%d )", gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), count);
				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);
				gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);	
				GCServerMsgStringSend(lMsg.Get(MSGGET(13, 63)), aIndex, 1);
				return;
			}
		}
		else if ( lpItem->m_Type == ITEMGET(13,76) ) // Panda Ring
		{
			int count = gObjGetItemCountInIventory(aIndex, lpItem->m_Type/MAX_SUBTYPE_ITEMS, lpItem->m_Type % MAX_SUBTYPE_ITEMS, lpItem->m_Level);

			if ( count > 0 )
			{
				CLog.LogAdd("[ChangeRing] Too many Panda Ring [%s][%s] ( Name:%s, Count:%d )", gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), count);
				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);
				gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);	
				GCServerMsgStringSend(lMsg.Get(MSGGET(13, 63)), aIndex, 1);
				return;
			}
		}

		type = lpItem->m_Type;
		level = lpItem->m_Level;
		special = lpItem->m_Special[0];
		NOption = lpItem->m_NewOption;
		strcpy(szItemName, lpItem->GetName());

		if ( lpItem->m_Type == ITEMGET(14,15) ) // Zen
		{
			if ( MapC[map_num].ItemGive(aIndex, item_num, false) == TRUE )
			{
				if ( !gObjCheckMaxZen(aIndex, lpItem->m_BuyMoney))
				{
					if ( gObj[aIndex].Money < MAX_ZEN )
					{
						gObj[aIndex].Money = MAX_ZEN;
						pResult.result = -2;
						WORD hiWord = SET_NUMBERHW(gObj[aIndex].Money);
						WORD loWord = SET_NUMBERLW(gObj[aIndex].Money);
						pResult.Data[0] = SET_NUMBERH(hiWord);
						pResult.Data[1] = SET_NUMBERL(hiWord);
						pResult.Data[2] = SET_NUMBERH(loWord);
						pResult.Data[3] = SET_NUMBERL(loWord);

						gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

						return;
					}

					pResult.result = -1;
					pResult.h.size -= sizeof(pResult.Data);

					gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			

					return;
				}

				gObj[aIndex].Money += lpItem->m_BuyMoney;
				pResult.result = -2;

				WORD hiWord = SET_NUMBERHW(gObj[aIndex].Money);
				WORD loWord = SET_NUMBERLW(gObj[aIndex].Money);
				pResult.Data[0] = SET_NUMBERH(hiWord);
				pResult.Data[1] = SET_NUMBERL(hiWord);
				pResult.Data[2] = SET_NUMBERH(loWord);
				pResult.Data[3] = SET_NUMBERL(loWord);
				pResult.h.size -= 3;
			}

			gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		}
		else
		{
			if ( lpItem->m_Type == ITEMGET(13,32) ) // Splinter of Armor
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, 20, ITEMGET(13,32), 0);

				if ( MAIN_INVENTORY_RANGE(pos) != FALSE )
				{
					pResult.result = -3;
					gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
					return;
				}
			}
			else if ( lpItem->m_Type == ITEMGET(13,33) ) // Bless of Guardian
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, 20, ITEMGET(13,33), 0);

				if ( MAIN_INVENTORY_RANGE(pos) != FALSE )
				{
					pResult.result = -3;
					gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);

					return;
				}
			}
			else if ( lpItem->m_Type == ITEMGET(13,34) ) // Claw of Beast
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, 10, ITEMGET(13,34), 0);

				if ( MAIN_INVENTORY_RANGE(pos) != FALSE )
				{
					pResult.result = -3;
					gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);

					return;
				}
			}
			else if ( lpItem->m_Type == ITEMGET(14,29) ) // Symbol of Kundun
			{
				for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
				{
					int pos = ::g_KalimaGate.CheckOverlapKundunMark(aIndex, lpItem->m_Level);

					if ( MAIN_INVENTORY_RANGE(pos) )
					{
						if ( MapC[map_num].ItemGive(aIndex, item_num, true) == TRUE )
						{
							BYTE NewOption[MAX_EXOPTION_SIZE];

							::ItemIsBufExOption(NewOption, (lpItem != NULL)?(CItem*)&lpItem->m_Number:NULL);

							CLog.LogAdd(lMsg.Get(MSGGET(1, 221)), gObj[aIndex].AccountID, gObj[aIndex].Name, map_num,
								gObj[aIndex].X, gObj[aIndex].Y, lpItem->m_Number, szItemName, type,
								level, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, (int)lpItem->m_Durability,
								NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5],
								NewOption[6], lpItem->m_SetOption, lpItem->m_ItemOptionEx>>7,
								g_kJewelOfHarmonySystem.GetItemStrengthenOption((lpItem)?((CItem *)&lpItem->m_Number):NULL), g_kJewelOfHarmonySystem.GetItemOptionLevel((lpItem)?((CItem *)&lpItem->m_Number):NULL));

							pResult.result = -3;

							gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			

							gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;

							if ( gObj[aIndex].pInventory[pos].m_Durability >= 5.0f )
							{
								int NewDur = gObj[aIndex].pInventory[pos].m_Durability - 5.0f;
								::gObjInventoryItemSet(aIndex, pos, -1);
								gObj[aIndex].pInventory[pos].Clear();
								::GCInventoryItemDeleteSend(aIndex, pos, 1);
								BYTE Socket[5];
								Socket[0] = 0xFF;
								Socket[1] = 0xFF;
								Socket[2] = 0xFF;
								Socket[3] = 0xFF;
								Socket[4] = 0xFF;
								::ItemSerialCreateSend(aIndex, 235, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(14, 28), lpItem->m_Level, 0, 0, 0, 0, aIndex, 0, 0, Socket);
								CLog.LogAdd("[Kalima] [%s][%s] Make Lost Kalima Map (Left Kundun Mark:%d)", gObj[aIndex].AccountID, gObj[aIndex].Name, NewDur);

								if ( NewDur > 0 )
								{
									lpItem->m_State = 2;
									lpItem->Give = false;
									lpItem->live = true;
									lpItem->m_Durability = NewDur;

									continue;

								}
							}
							else
							{
								GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
							}
							return;
						}
					}
					else
					{
						break;
					}
				}
			}
#if(!GS_CASTLE)
			else if ( lpItem->m_Type == ITEMGET(14,110) ) // Sign of Dimensions ( Double Goer )
			{
				for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
				{
					int pos = gDoubleGoer.CheckSignPos(&gObj[aIndex]);

					if ( MAIN_INVENTORY_RANGE(pos) == TRUE )
					{
						if ( MapC[map_num].ItemGive(aIndex, item_num, true) == TRUE )
						{
							BYTE NewOption[MAX_EXOPTION_SIZE];

							::ItemIsBufExOption(NewOption, (lpItem != NULL)?(CItem*)&lpItem->m_Number:NULL);

							pResult.result = -3;

							gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			

							gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;

							if ( gObj[aIndex].pInventory[pos].m_Durability >= 5.0f )
							{
								int NewDur = gObj[aIndex].pInventory[pos].m_Durability - 5.0f;
								::gObjInventoryItemSet(aIndex, pos, -1);
								gObj[aIndex].pInventory[pos].Clear();
								::GCInventoryItemDeleteSend(aIndex, pos, 1);
								BYTE Socket[5];
								Socket[0] = 0xFF;
								Socket[1] = 0xFF;
								Socket[2] = 0xFF;
								Socket[3] = 0xFF;
								Socket[4] = 0xFF;
								::ItemSerialCreateSend(aIndex, 235, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(14, 111), 0, 0, 0, 0, 0, aIndex, 0, 0, Socket);
								//CLog.LogAdd("[Kalima] [%s][%s] Make Lost Kalima Map (Left Kundun Mark:%d)", gObj[aIndex].AccountID, gObj[aIndex].Name, NewDur);

								if ( NewDur > 0 )
								{
									lpItem->m_State = 2;
									lpItem->Give = false;
									lpItem->live = true;
									lpItem->m_Durability = NewDur;

									continue;

								}
							}
							else
							{
								GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
							}
							return;
						}
					}
					else
					{
						break;
					}
				}
			}
#endif
			else if ( lpItem->m_Type == ITEMGET(14,101) ) // Suspicious crap ( Imperial Fort )
			{
				for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
				{
					int pos = gImperialFort.CheckScrapPaperPos(&gObj[aIndex]);

					if ( MAIN_INVENTORY_RANGE(pos) == TRUE )
					{
						if ( MapC[map_num].ItemGive(aIndex, item_num, true) == TRUE )
						{
							BYTE NewOption[MAX_EXOPTION_SIZE];

							::ItemIsBufExOption(NewOption, (lpItem != NULL)?(CItem*)&lpItem->m_Number:NULL);

							pResult.result = -3;

							gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			

							gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;

							if ( gObj[aIndex].pInventory[pos].m_Durability >= 5.0f )
							{
								int NewDur = gObj[aIndex].pInventory[pos].m_Durability - 5.0f;
								::gObjInventoryItemSet(aIndex, pos, -1);
								gObj[aIndex].pInventory[pos].Clear();
								::GCInventoryItemDeleteSend(aIndex, pos, 1);
								BYTE Socket[5];
								Socket[0] = 0xFF;
								Socket[1] = 0xFF;
								Socket[2] = 0xFF;
								Socket[3] = 0xFF;
								Socket[4] = 0xFF;
								::ItemSerialCreateSend(aIndex, 235, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(14, 102), 0, 0, 0, 0, 0, aIndex, 0, 0, Socket);
								//CLog.LogAdd("[Kalima] [%s][%s] Make Lost Kalima Map (Left Kundun Mark:%d)", gObj[aIndex].AccountID, gObj[aIndex].Name, NewDur);

								if ( NewDur > 0 )
								{
									lpItem->m_State = 2;
									lpItem->Give = false;
									lpItem->live = true;
									lpItem->m_Durability = NewDur;

									continue;

								}
							}
							else
							{
								GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
							}
							return;
						}
					}
					else
					{
						break;
					}
				}
			}
			else if ( lpItem->m_Type == ITEMGET(14,100) ) // Lucky Coin
			{
				for ( int n=12;n<MAIN_INVENTORY_SIZE;n++)
				{
					int pos = gLuckyCoin.GetCoinPos(&gObj[aIndex]);

					if ( MAIN_INVENTORY_RANGE(pos) )
					{
						int Dur = gObj[aIndex].pInventory[pos].m_Durability + lpItem->m_Durability;

						if ( Dur <= 255 )
						{
							if ( MapC[map_num].ItemGive(aIndex, item_num, true) == TRUE )
							{
								pResult.result = -3;
								gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
								gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
								GCItemDurSend( aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability,0);
								return;
							}
						}
						else
						{
							lpItem->m_Durability = Dur - 255;
							gObj[aIndex].pInventory[pos].m_Durability = 255.0f;
							GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
						}

					}
					else
					{
						break;
					}
				}
			}
			else if ( lpItem->m_Type == ITEMGET(14,21) && lpItem->m_Level == 3) // Sign of Lord
			{
				for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
				{
					int pos = ::g_CastleSiegeSync.CheckOverlapCsMarks(aIndex);

					if ( MAIN_INVENTORY_RANGE(pos) )
					{
						int Dur = gObj[aIndex].pInventory[pos].m_Durability + lpItem->m_Durability;

						if ( Dur <= 255 )
						{
							if ( MapC[map_num].ItemGive(aIndex, item_num, true) == TRUE )
							{
								pResult.result = -3;
								gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
								gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
								GCItemDurSend( aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability,0);
								return;
							}
						}
						else
						{
							lpItem->m_Durability = Dur - 255;
							gObj[aIndex].pInventory[pos].m_Durability = 255.0f;
							GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
						}

					}
					else
					{
						break;
					}
				}
			}

			pResult.result = ::gObjInventoryInsertItemTemp(&gObj[aIndex], lpItem);
			
			if ( pResult.result != 0xFF )
			{
				ItemByteConvert((LPBYTE)&pResult.Data[0], lpItem->m_Type, lpItem->m_Option1, lpItem->m_Option2,
					lpItem->m_Option3, lpItem->m_Level, lpItem->m_Durability, lpItem->m_NewOption,
					lpItem->m_SetOption, lpItem->m_JewelOfHarmonyOption, lpItem->m_ItemOptionEx, lpItem->m_ItemSocket);

				if ( MapC[map_num].ItemGive(aIndex, item_num, false) == TRUE )
				{
					BYTE pos = ::gObjInventoryInsertItem(aIndex, lpItem);

					if ( pos == 0xFF )
					{
						pResult.result = -1;
					}

					gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					if ( pos == 0xFF )
					{
						::GCItemListSend(aIndex);
					}
					else
					{
						BYTE NewOption[MAX_EXOPTION_SIZE];

						::ItemIsBufExOption(NewOption, (lpItem != NULL)?(CItem*)&lpItem->m_Number:NULL);

						CLog.LogAdd(lMsg.Get(MSGGET(1, 221)), gObj[aIndex].AccountID, gObj[aIndex].Name, map_num,
							gObj[aIndex].X, gObj[aIndex].Y, lpItem->m_Number, szItemName, type,
							level, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, (INT)lpItem->m_Durability,
							NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5],
							NewOption[6], lpItem->m_SetOption, lpItem->m_ItemOptionEx>>7,
							g_kJewelOfHarmonySystem.GetItemStrengthenOption((lpItem)?((CItem *)&lpItem->m_Number):NULL), g_kJewelOfHarmonySystem.GetItemOptionLevel((lpItem)?((CItem *)&lpItem->m_Number):NULL));
						::GCSendGetItemInfoForParty(aIndex, lpItem);

						if ( gUser.BloodCastleMapRange(gObj[aIndex].MapNumber) == true )
						{
							if ( g_BloodCastle.CheckQuestItemSerial(g_BloodCastle.GetBloodCastleIndex(gObj[aIndex].MapNumber), lpItem) == true )
							{
								g_BloodCastle.m_BridgeData[g_BloodCastle.GetBloodCastleIndex(gObj[aIndex].MapNumber)].m_iBC_QUEST_ITEM_USER_INDEX = aIndex;
								char szTempMsg[128];

								switch ( lpItem->m_Level )
								{
									case 0:	// Staff of Archangel
										wsprintf(szTempMsg, lMsg.Get(MSGGET(4, 150)), gObj[aIndex].Name);
										g_BloodCastle.SendNoticeMessage(g_BloodCastle.GetBloodCastleIndex(gObj[aIndex].MapNumber), szTempMsg);
										break;

									case 1:	// Sword of Archangel
										wsprintf(szTempMsg, lMsg.Get(MSGGET(4, 151)), gObj[aIndex].Name);
										g_BloodCastle.SendNoticeMessage(g_BloodCastle.GetBloodCastleIndex(gObj[aIndex].MapNumber), szTempMsg);
										break;

									case 2:	// Crossbow of Archangel
										wsprintf(szTempMsg, lMsg.Get(MSGGET(4, 152)), gObj[aIndex].Name);
										g_BloodCastle.SendNoticeMessage(g_BloodCastle.GetBloodCastleIndex(gObj[aIndex].MapNumber), szTempMsg);
										break;

									default:
										szTempMsg[0] = 0;
								}

								CLog.LogAdd("[Blood Castle] (%d) (Account:%s, Name:%s) picked up Angel King's Weapon (%d)", g_BloodCastle.GetBloodCastleIndex(gObj[aIndex].MapNumber)+1, gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->m_Level);
							}
						}
					}
				}
				else
				{
					pResult.result = -1;
					pResult.h.size -= sizeof(pResult.Data);

					gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				}
			}
			else
			{
				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);

				gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			}
		}
	}
	else
	{
		pResult.h.size -= sizeof(pResult.Data);
		gSendProto.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);
	}
}
// -----------------------------------------------------------------------------------------------
BOOL CGItemDropRequest(PMSG_ITEMTHROW * lpMsg, int aIndex, BOOL drop_type)
{
	BOOL CopyItem = FALSE;

	BYTE NewOption[MAX_EXOPTION_SIZE];

	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		return FALSE;
	}

	PMSG_ITEMTHROW_RESULT pResult;

	C1HeadSet((LPBYTE)&pResult, 0x23, sizeof(pResult));
	pResult.Result = true;
	pResult.Ipos = lpMsg->Ipos;

	if ( gObj[aIndex].m_IfState.use != 0 )
	{
		if ( gObj[aIndex].m_IfState.type != 3 )
		{
			pResult.Result = false;
			return FALSE;
		}
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		pResult.Result = false;
	}

	if ( gObj[aIndex].m_bMapSvrMoveQuit == true )
	{
		CLog.LogAdd("[ANTI-HACK][Item Duplication] - Item Drop during MapServer Move [%s][%s]", gObj[aIndex].AccountID, gObj[aIndex].Name);
		pResult.Result = false;
	}

	if ( !::gObjFixInventoryPointer(aIndex))
	{
		CLog.LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].pTransaction == 1 )
	{
		CLog.LogAdd("[%s][%s] CGItemDropRequest() Failed : Transaction == 1, IF_TYPE : %d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
		pResult.Result = false;
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpMsg->Ipos == 0xFF )	// Null Position
	{
		CLog.LogAdd("error:%s %d", __FILE__, __LINE__);
		pResult.Result = false;
	}

	if ( lpMsg->Ipos < 0 || lpMsg->Ipos > MAIN_INVENTORY_SIZE-1)	// Out of Bounds
	{	
		pResult.Result = false;
	}

	if (!::gObjCanItemTouch(lpObj, 0))
	{
		pResult.Result = false;
	}

	// Icarus Preventions about Dinorant
	if ( gObj[aIndex].MapNumber == MAP_INDEX_ICARUS)
	{
		if ( lpMsg->Ipos == 8 )
		{
			if ( lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,3) ) // Dinorant
			{
				if( lpObj->pInventory[7].IsItem() == FALSE )
				{
					pResult.Result = false;
				}
			}
		}
		else if ( lpMsg->Ipos == 7 )
		{
			if ( lpObj->pInventory[8].m_Type != ITEMGET(13,3) ) // Dinorant 
			{
				pResult.Result = false;
			}
		}
	}

	if ( lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(12,30) || lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(12,31) || lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,36) || lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,37) )
	{
		pResult.Result = false;
	}

	if ( (lpObj->pInventory[lpMsg->Ipos].m_Type >= ITEMGET(13,0) && lpObj->pInventory[lpMsg->Ipos].m_Type <= ITEMGET(13,5)) || // Angel, Imp, Uniria, Dinorant, Dark Raven & Dark Horse
		lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,64) || // Demon Pet
		lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,65) || // Spirit of Guardian Pet
		lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,67) || // Rudolf Pet
		lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,80) || // Panda Pet
		lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,123) || // Skeleton Pet
		lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,13) || // Jewel of Bless
		lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,14) || // Jewel Of Soul
		lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,16) || // Jewel of Life
		lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(12,15) || // Jewel of Chaos
		lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,22) || // Jewel of Creation
		lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,30) || // Cape of Lord
		(lpObj->pInventory[lpMsg->Ipos].m_Type >= ITEMGET(12,0) && lpObj->pInventory[lpMsg->Ipos].m_Type <= ITEMGET(12,6)) || // Wings Lvl 1 and 2
		(lpObj->pInventory[lpMsg->Ipos].m_Type >= ITEMGET(12,36) && lpObj->pInventory[lpMsg->Ipos].m_Type <= ITEMGET(12,43)) || // Wings Lvl 3 & Mantle of Emperor
		lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(12,49) || // Cloak of Warrior
		lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(12,50) || // Rage Fighter Cape Lvl 2
		(lpObj->pInventory[lpMsg->Ipos].m_Type >= ITEMGET(12,130) && lpObj->pInventory[lpMsg->Ipos].m_Type <= ITEMGET(12,135)) || // Mini Wings & Mini Capes
		(lpObj->pInventory[lpMsg->Ipos].m_Level > 6 && lpObj->pInventory[lpMsg->Ipos].m_Type < ITEMGET(12,0)) || // Weapons or Items Above Lvl 4
		lpObj->pInventory[lpMsg->Ipos].IsSetItem() != FALSE || // Check Exist Item
		lpObj->pInventory[lpMsg->Ipos].IsExtItem() != FALSE ) // Check Exist Item
	{
		/*if ( gPkLimitFree == FALSE )
		{
			pResult.Result = false;
		}*/
	}

	if ( g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[lpMsg->Ipos]) == TRUE )
	{
		pResult.Result = false;
		GCServerMsgStringSend(lMsg.Get(MSGGET(13, 43)), lpObj->m_Index, 1);
	}

	if ( g_kJewelOfHarmonySystem.IsJewelOfHarmonyOriginal(lpObj->pInventory[lpMsg->Ipos].m_Type) ||
		 g_kJewelOfHarmonySystem.IsJewelOfHarmonyPurity(lpObj->pInventory[lpMsg->Ipos].m_Type)   ||
		 g_kJewelOfHarmonySystem.IsJewelOfHarmonySmeltingItems(lpObj->pInventory[lpMsg->Ipos].m_Type) )
	{
		pResult.Result = false;
	}
	switch ( lpObj->pInventory[lpMsg->Ipos].m_Type )
	{
	case ITEMGET(13,39):	pResult.Result = false;	break; // Elite Skeleton Transformation Ring
	case ITEMGET(13,40):	pResult.Result = false;	break; // Jack Olantern Ring
	case ITEMGET(13,41):	pResult.Result = false;	break; // Christmas Ring
	case ITEMGET(13,42):	pResult.Result = false;	break; // GM Ring
	case ITEMGET(13,76):	pResult.Result = false;	break; // Panda Ring
	case ITEMGET(13,122):	pResult.Result = false;	break; // Skeleton Ring
	}

	if ( IsCashItem(lpObj->pInventory[lpMsg->Ipos].m_Type) == TRUE )
	{
		pResult.Result = false;
	}

	if ( pResult.Result == 1 )
	{
		::ItemIsBufExOption(NewOption, &lpObj->pInventory[lpMsg->Ipos]);

		int map = lpObj->MapNumber;
		int type = lpObj->pInventory[lpMsg->Ipos].m_Type;
		int level = lpObj->pInventory[lpMsg->Ipos].m_Level;
		float dur = lpObj->pInventory[lpMsg->Ipos].m_Durability;
		BOOL bIsItem = lpObj->pInventory[lpMsg->Ipos].IsItem();
		BYTE Option1 = lpObj->pInventory[lpMsg->Ipos].m_Option1;
		BYTE Option2 = lpObj->pInventory[lpMsg->Ipos].m_Option2;
		BYTE Option3 = lpObj->pInventory[lpMsg->Ipos].m_Option3;
		BYTE NOption = lpObj->pInventory[lpMsg->Ipos].m_NewOption;
		DWORD serial = lpObj->pInventory[lpMsg->Ipos].m_Number;
		int PetLevel = lpObj->pInventory[lpMsg->Ipos].m_PetItem_Level;
		int PetExp = lpObj->pInventory[lpMsg->Ipos].m_PetItem_Exp;
		BYTE SOption = lpObj->pInventory[lpMsg->Ipos].m_SetOption;
		BYTE btItemEffectEx = lpObj->pInventory[lpMsg->Ipos].m_ItemOptionEx;
		int serial2 = lpObj->pInventory[lpMsg->Ipos].m_Number;
		char szItemName[50] = "Item";
		BYTE JOHOption = g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pInventory[lpMsg->Ipos]);
		BYTE JOHOptionLevel = g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pInventory[lpMsg->Ipos]);
		BYTE Socket[5];
		Socket[0] = lpObj->pInventory[lpMsg->Ipos].m_ItemSocket[0];
		Socket[1] = lpObj->pInventory[lpMsg->Ipos].m_ItemSocket[1];
		Socket[2] = lpObj->pInventory[lpMsg->Ipos].m_ItemSocket[2];
		Socket[3] = lpObj->pInventory[lpMsg->Ipos].m_ItemSocket[3];
		Socket[4] = lpObj->pInventory[lpMsg->Ipos].m_ItemSocket[4];
		
		if ( !::gObjInventorySearchSerialNumber(lpObj, serial2))	// ANTI DUPE SYSTEM
		{
			CopyItem = TRUE;
			pResult.Result = false;
			bIsItem = FALSE;
		}

		if ( bIsItem == TRUE )
		{
			strcpy(szItemName, lpObj->pInventory[lpMsg->Ipos].GetName());

			if ( type == ITEMGET(14,11)) // Box of Luck With Its Different Levels
			{
				::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				pResult.Result = true;

				if( level == 0 )
				{
					::LuckyBoxOpenEvent(&gObj[aIndex]);
				}
				else if ( level == 1 )
				{
					::StarOfXMasOpenEvent(&gObj[aIndex]);
				}
				else if ( level == 2 )
				{
					::FireCrackerOpenEvent(&gObj[aIndex]);
					GCFireWorkSend(aIndex,3);
				}
				else if ( level == 3 )
				{
					::HeartOfLoveOpenEvent(&gObj[aIndex]);
				}
				else if ( level == 5 )
				{
					::SilverMedalOpenEvent(&gObj[aIndex]);
				}
				else if ( level == 6 )
				{
					::GoldMedalOpenEvent(&gObj[aIndex]);
				}
				else if ( level == 7 )
				{
					::HeavenBoxOpenEvent(&gObj[aIndex]);
				}
				else if ( level == 8 )
				{
					::KundunBox01OpenEvent(&gObj[aIndex]);
				}
				else if ( level == 9 )
				{
					::KundunBox02OpenEvent(&gObj[aIndex]);
				}
				else if ( level == 10 )
				{
					::KundunBox03OpenEvent(&gObj[aIndex]);
				}
				else if ( level == 11 )
				{
					::KundunBox04OpenEvent(&gObj[aIndex]);
				}
				else if ( level == 12 )
				{
					::KundunBox05OpenEvent(&gObj[aIndex]);
				}
				else if ( level == 13 )
				{
					::DarkLordHeartBoxOpenEvent(&gObj[aIndex]);
				}
				else if ( level == 14 )
				{
					::BlueHappyPouchOpenEvent(&gObj[aIndex]);
				}
				else if ( level == 15 )
				{
					::RedHappyPouchOpenEvent(&gObj[aIndex]);
				}
				else
				{
					pResult.Result = false;
					gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}
			}
			// ------
			else if ( type == ITEMGET(13,20) && level == 1 ) // Wizard Ring A
			{
				if ( gObj[aIndex].Level >= 40 )
				{
					::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
					::WizardRing40OpenEvent(&gObj[aIndex]);
				}
				else
				{
					/*PMSG_NOTICE pNotice;

					TNotice::MakeNoticeMsg((TNotice *)&pNotice, 10, "You must be over Level 40 to throw the Ring.");
					TNotice::SetNoticeProperty((TNotice *)&pNotice, 10, _ARGB(255, 128, 149, 196), 1, 0, 20);
					TNotice::SendNoticeToUser(aIndex, (TNotice *)&pNotice);*/
					pResult.Result = false;
					gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}
			}
			// ------
			else if ( type == ITEMGET(13,20) && level == 2 ) // Wizard Ring B
			{
				if ( gObj[aIndex].Level >= 80 )
				{
					::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
					::WizardRing80OpenEvent(&gObj[aIndex]);
				}
				else
				{
					/*PMSG_NOTICE pNotice;

					TNotice::MakeNoticeMsg((TNotice *)&pNotice, 10, "You must be over Level 80 to throw the Ring.");
					TNotice::SetNoticeProperty((TNotice *)&pNotice, 10, _ARGB(255, 128, 149, 196), 1, 0, 20);
					TNotice::SendNoticeToUser(aIndex, (TNotice *)&pNotice);*/
					pResult.Result = false;
					gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}
			}
			// ------
			else if ( type == ITEMGET(14,123) )	// Sealed Golden Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::SealedGoldenOpenEvent(&gObj[aIndex]);
			}
			// ------
			else if ( type == ITEMGET(14,124) ) // Sealed Silver Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::SealedSilverOpenEvent(&gObj[aIndex]);
			}
			// ------
			else if ( type == ITEMGET(14,141) ) // Shining Jewelry Case
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::ShinningJewelryOpenEvent(&gObj[aIndex]);
			}
			// ------
			else if ( type == ITEMGET(14,142) ) // Elegant Jewelry Case
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::ElegantJewelryOpenEvent(&gObj[aIndex]);
			}
			// ------
			else if ( type == ITEMGET(14,143) ) // Steel Jewelry Case
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::SteelJewelryOpenEvent(&gObj[aIndex]);
			}
			// ------
			else if ( type == ITEMGET(14,144) ) // Old Jewelry Case
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::OldJewelryOpenEvent(&gObj[aIndex]);
			}
			// ------
			else if ( type == ITEMGET(12,32) ) // Red Ribbon Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::RedRibbonBoxOpenEvent(&gObj[aIndex]);
			}
			else if ( type == ITEMGET(12,33) ) // Green Ribbon Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::GreenRibbonBoxOpenEvent(&gObj[aIndex]);
			}
			else if ( type == ITEMGET(12,34) ) // Blue Ribbon Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::BlueRibbonBoxOpenEvent(&gObj[aIndex]);
			}
			// ------
			else if ( type == ITEMGET(14,32) && level == 0 ) // Pink Chocolate Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::PinkChocolateBoxOpenEvent(&gObj[aIndex]);
			}
			else if ( type == ITEMGET(14,33)  && level == 0 ) // Red Chocolate Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::RedChocolateBoxOpenEvent(&gObj[aIndex]);
			}
			else if ( type == ITEMGET(14,34) && level == 0  ) // Blue Chocolate Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::BlueChocolateBoxOpenEvent(&gObj[aIndex]);
			}
			// ------
			else if ( type == ITEMGET(14,32)  && level == 1 ) // Light Purple Candy Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::LightPurpleCandyBoxOpenEvent(&gObj[aIndex]);
			}
			else if ( type == ITEMGET(14,33)  && level == 1  ) // Vermilion Candy Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::VermilionCandyBoxOpenEvent(&gObj[aIndex]);
			}
			else if ( type == ITEMGET(14,34)  && level == 1 ) // Deep Blue Candy Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::DeepBlueCandyBoxOpenEvent(&gObj[aIndex]);
			}
			// ------
			else if ( type == ITEMGET(14,45) ) // Halloween Day Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::HalloweenDayBoxOpenEvent(&gObj[aIndex]);
			}
			else if ( type == ITEMGET(14,84) ) // Cherry Blossom Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::CherryBlossomOpenEvent(&gObj[aIndex]);
			}
			else if ( type == ITEMGET(14,157) ) // Green Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::GreenBoxOpenEvent(&gObj[aIndex]);
			}
			else if ( type == ITEMGET(14,158) ) // Red Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::RedBoxOpenEvent(&gObj[aIndex]);
			}
			else if ( type == ITEMGET(14,159) ) // Purple Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::PurpleBoxOpenEvent(&gObj[aIndex]);
			}
			else if ( type == ITEMGET(14,55) ) // Green Chaos Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::GreenChaosBoxOpenEvent(&gObj[aIndex]);
			}
			else if ( type == ITEMGET(14,56) ) // Red Chaos Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::RedChaosBoxOpenEvent(&gObj[aIndex]);
			}
			else if ( type == ITEMGET(14,57) ) // Purple Chaos Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::PurpleChaosBoxOpenEvent(&gObj[aIndex]);
			}
			else if ( type == ITEMGET(14,52) ) // GM Box
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				::GameMasterBoxOpenEvent(&gObj[aIndex]);
			}
			else if ( type == ITEMGET(14,51) ) // Christmas Star
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				GCFireWorkSend(aIndex,3);
			}
			else if ( type == ITEMGET(14,63) ) // Christmas FireCracker
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				GCFireWorkSend(aIndex,3);
			}
			else if ( type == ITEMGET(14,99) ) // Christmas FireCracker
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				GCFireWorkSend(aIndex,6);
			}
			//GCFireWorkSend(aIndex,Amount);
			// ----------------------------------------------------------------------------------------------------------------------END HERE!
			else if ( type == ITEMGET(14,28) ) // Lost Map
			{
				if ( g_KalimaGate.CreateKalimaGate(aIndex, level, lpMsg->px, lpMsg->py))
				{
					gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				}
				else
				{
					pResult.Result = false;
					gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}
			}
			else if ( type == ITEMGET(14,20) && ( ((level-1<0)?FALSE:(level-1>4)?FALSE:TRUE) != FALSE ) ) // Remedy Of Love
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				Japan1StAnivBoxOpen(&gObj[aIndex], level);
			}
			else if ( type == ITEMGET(13,7) ) // Contract (Summon)
			{
				int iMonsterID = 0;

				if ( level == 0 )
					iMonsterID = 286;
				else if ( level == 1 )
					iMonsterID = 287;


				if ( g_CsNPC_Mercenary.CreateMercenary(aIndex, iMonsterID, lpMsg->px, lpMsg->py) )
				{
					::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				}
				else
				{
					pResult.Result = false;
					gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}
			}
			else if ( type == ITEMGET(13,11) ) // Guardian / LifeStone
			{
				BOOL bResult = FALSE;

				if ( level == 0 )
					bResult = g_CsNPC_Guardian.CreateGuardian(aIndex);
#if GS_CASTLE == 1
				else if ( level == 1 )
					bResult = g_CsNPC_LifeStone.CreateLifeStone(aIndex);
#endif
				if ( bResult == TRUE )
					::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				else
				{
					pResult.Result = false;
					gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}
			}
			else
			{
				int OwnerIndex = aIndex;

				if ( (type >= ITEMGET(14,0) && type <= ITEMGET(14,10)) || (type == ITEMGET(4,7) ) || type ==ITEMGET(4,15) )
				{
					OwnerIndex = -1;
				}

				if ( MapC[map].ItemDrop( type, level, dur, lpMsg->px, lpMsg->py, Option1, Option2, Option3,
					NOption, SOption, serial2, OwnerIndex, PetLevel, PetExp, btItemEffectEx, Socket) == TRUE )
				{
					gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
					pResult.Result = true;

					if ( drop_type == FALSE )
					{
						CLog.LogAdd(lMsg.Get(MSGGET(1, 223)), lpObj->AccountID, lpObj->Name, lpObj->MapNumber,
							lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2,
							Option3, (int)dur, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4],
							NewOption[5], NewOption[6], SOption, btItemEffectEx>>7, JOHOption, JOHOptionLevel);
					}
					else
					{
						CLog.LogAdd(lMsg.Get(MSGGET(1, 224)), lpObj->AccountID, lpObj->Name, lpObj->MapNumber,
							lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2,
							Option3, (int)dur, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4],
							NewOption[5], NewOption[6], SOption, btItemEffectEx, JOHOption, JOHOptionLevel );
					}
				}
				else
				{
					pResult.Result = false;
				}
			}
		}
		else	// bIsItem
		{
			CLog.LogAdd(lMsg.Get(MSGGET(1, 225)));
			pResult.Result = false;
		}
	}

	gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

	if ( CopyItem != FALSE )
		::GCItemListSend(aIndex);

	if ( pResult.Result == 1 )
	{
		if ( lpMsg->Ipos < 12 )
		{
			if ( lpMsg->Ipos == 10 || lpMsg->Ipos == 11 )
			{
				gObjUseSkill.SkillChangeUse(aIndex);
			}

			::gObjMakePreviewCharSet(aIndex);

			PMSG_USEREQUIPMENTCHANGED pChange;

			C1HeadSet((LPBYTE)&pChange, 0x25, sizeof(pChange));
			pChange.NumberH = SET_NUMBERH(aIndex);
			pChange.NumberL = SET_NUMBERL(aIndex);
			ItemByteConvert(pChange.ItemInfo, lpObj->pInventory[lpMsg->Ipos]);
			pChange.ItemInfo[I_OPTION] = lpMsg->Ipos <<  4;
			pChange.ItemInfo[I_OPTION] |= LevelSmallConvert(aIndex, lpMsg->Ipos) & 0x0F;

			gSendProto.DataSend(aIndex, (LPBYTE)&pChange, pChange.h.size);
			gSendProto.VPMsgSend(lpObj, (LPBYTE)&pChange, pChange.h.size);
		}
	}

	return pResult.Result;
}
// -----------------------------------------------------------------------------------------------
struct PMSG_INVENTORYITEMMOVE_RESULT
{
	PBMSG_HEAD h;	// C3:24
	BYTE result;	// 3
	BYTE Pos;	// 4
	BYTE ItemInfo[MAX_ITEM_INFO];	// 5
};
// -----------------------------------------------------------------------------------------------
void GCItemMoveResultSend(int aIndex, BYTE result, BYTE pos, LPBYTE const ItemInfo)
{
	PMSG_INVENTORYITEMMOVE_RESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x24;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = result;
	pMsg.Pos = pos;
	pMsg.ItemInfo[I_TYPE] = ItemInfo[I_TYPE];
	pMsg.ItemInfo[I_OPTION] = ItemInfo[I_OPTION];
	pMsg.ItemInfo[I_DUR] = ItemInfo[I_DUR];
	pMsg.ItemInfo[I_NOPTION] = ItemInfo[I_NOPTION];
	pMsg.ItemInfo[I_SOPTION] = ItemInfo[I_SOPTION];
	pMsg.ItemInfo[I_380OPTION] = ItemInfo[I_380OPTION];
	pMsg.ItemInfo[I_JOHOPTION] = ItemInfo[I_JOHOPTION];
	pMsg.ItemInfo[I_SOCKETOPT1] = ItemInfo[I_SOCKETOPT1];
	pMsg.ItemInfo[I_SOCKETOPT2] = ItemInfo[I_SOCKETOPT2];
	pMsg.ItemInfo[I_SOCKETOPT3] = ItemInfo[I_SOCKETOPT3];
	pMsg.ItemInfo[I_SOCKETOPT4] = ItemInfo[I_SOCKETOPT4];
	pMsg.ItemInfo[I_SOCKETOPT5] = ItemInfo[I_SOCKETOPT5];

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
void CGInventoryItemMove(PMSG_INVENTORYITEMMOVE * lpMsg, int aIndex)
{
	BYTE result;
	BYTE ItemInfo[MAX_ITEM_INFO];

	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		::GCItemMoveResultSend(aIndex, -1, 0, ItemInfo);
		return;
	}

	if ( !::gObjIsConnectedGP(aIndex))
	{
		CLog.LogAddC(TColor.Red(),"[%s][%d] error-L3", __FILE__, __LINE__);
		::GCItemMoveResultSend(aIndex, -1, 0, ItemInfo);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int it_type = 0;

	if ( lpObj->m_ReqWarehouseOpen != false )
	{
		CLog.LogAddC(TColor.Red(),"[%s][%d] error-L3", __FILE__, __LINE__);
		GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
		return;
	}

	int source = lpMsg->source_item_num;
	int target = lpMsg->target_item_num;

	if ( lpMsg->tFlag == 2 || lpMsg->sFlag == 2 )
	{
		it_type = 6;
	}

	if ( lpMsg->tFlag == 0 && lpMsg->sFlag == 0 )
	{
		it_type = 8;
	}

	if ( (lpMsg->tFlag == 0 && lpMsg->sFlag == 4) || (lpMsg->tFlag == 4 && lpMsg->sFlag == 0) || (lpMsg->tFlag == 4 && lpMsg->sFlag == 4) )
	{
		it_type = 8;
	}

	if (::gObjCanItemTouch(lpObj, it_type) == FALSE)
	{
		if ( lpMsg->sItemInfo[I_TYPE] == ITEMGET(4,7) || lpMsg->sItemInfo[I_TYPE] == ITEMGET(4,15) )
		{
			if ( target >= 12 || source < 12 )
			{
				::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
				return;
			}
		}
		else
		{
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			return;
		}
	}

	if ( lpObj->DieRegen != FALSE )
	{
		::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
		return;
	}

	ItemInfo[I_TYPE] = lpMsg->sItemInfo[I_TYPE];
	ItemInfo[I_OPTION] = lpMsg->sItemInfo[I_OPTION];
	ItemInfo[I_DUR] = lpMsg->sItemInfo[I_DUR];
	ItemInfo[I_NOPTION] = lpMsg->sItemInfo[I_NOPTION];
	ItemInfo[I_SOPTION] = lpMsg->sItemInfo[I_SOPTION];
	ItemInfo[I_380OPTION] = lpMsg->sItemInfo[I_380OPTION];
	ItemInfo[I_JOHOPTION] = lpMsg->sItemInfo[I_JOHOPTION];
	ItemInfo[I_SOCKETOPT1] = lpMsg->sItemInfo[I_SOCKETOPT1];
	ItemInfo[I_SOCKETOPT2] = lpMsg->sItemInfo[I_SOCKETOPT2];
	ItemInfo[I_SOCKETOPT3] = lpMsg->sItemInfo[I_SOCKETOPT3];
	ItemInfo[I_SOCKETOPT4] = lpMsg->sItemInfo[I_SOCKETOPT4];
	ItemInfo[I_SOCKETOPT5] = lpMsg->sItemInfo[I_SOCKETOPT5];

	if ( lpMsg->sFlag == 3 || lpMsg->tFlag == 3 )
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 7 )
		{
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			CLog.LogAdd("[%s][%s] error-L1 : used not Chaosbox", gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		if ( lpObj->ChaosLock == TRUE )
		{
			CLog.LogAdd("[%s][%s] error-L3 : CBMixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
		
			return;
		}
	}

	if ( lpMsg->sFlag == 5 || lpMsg->tFlag == 5 )
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 13 )
		{
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			CLog.LogAdd("[%s][%s] error-L1 : used not DarkTrainerBox", gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		if ( lpObj->ChaosLock == TRUE )
		{
			CLog.LogAdd("[%s][%s] error-L3 : DarkTrainerBoxMixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
		
			return;
		}
	}

	if ( lpMsg->sFlag == 6 || lpMsg->sFlag == 7 || lpMsg->sFlag == 8 || lpMsg->tFlag == 6 || lpMsg->tFlag == 7 || lpMsg->tFlag == 8 )
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type !=7 )
		{
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			CLog.LogAdd("[%s][%s] error-L1 : used not ChaosBox", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return;
		}

		if ( lpObj->ChaosLock == TRUE )
		{
			CLog.LogAdd("[%s][%s] error-L3 : JewelOfHarmony Mixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			return;
		}
	}

	if ( lpMsg->sFlag == 9 || lpMsg->tFlag == 9 )
	{
		if ( lpObj->m_IfState.use	 < 1 || lpObj->m_IfState.type !=7 )
		{
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			CLog.LogAdd("[%s][%s] error-L1 : used not ChaosBox", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return;
		}

		if ( lpObj->ChaosLock == TRUE )
		{
			CLog.LogAdd("[%s][%s] error-L3 : JewelOfHarmony Mixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			return;
		}
	}


	if ( lpMsg->sFlag == 2 || lpMsg->tFlag == 2 )
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 6 )
		{
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			CLog.LogAdd("[%s][%s] error-L1 : used not Warehouse", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return;
		}
	}

	if ( lpMsg->sFlag == 1 && lpMsg->tFlag == 0 )
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 1 )
		{
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			CLog.LogAdd("[%s][%s] error-L1 : used not Trade", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return;
		}
	}

	if ( lpMsg->sFlag == 2 && lpMsg->tFlag == 0 )
	{
		int money = GetWarehouseUsedHowMuch(lpObj->Level, lpObj->WarehousePW);

		if ( (lpObj->Money - money ) < 1 && (lpObj->WarehouseMoney - money) < 1)
		{
			::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			char szTemp[256];
			wsprintf(szTemp, lMsg.Get(MSGGET(6, 69)), money);
			::GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
			return;
		}

		if ( ::bCanWarehouseLock == TRUE )
		{
			if ( gObj[aIndex].WarehouseLock == 1 )
			{
				::GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
				::GCServerMsgStringSend(lMsg.Get(MSGGET(6, 70)), lpObj->m_Index, 1);
				return;
			}
		}
	}

	if( it_type != 8 )
	{
		int type = (ItemInfo[I_TYPE] + ((ItemInfo[I_NOPTION] & 0x80 )*2)) + ((ItemInfo[I_380OPTION] & 0xF0)<<5);
		LPITEM_ATTRIBUTE lpItemAttr = GetItemAttr(type);

		if ( lpItemAttr == NULL )
		{
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			return;
		}

		if ( lpItemAttr->QuestItem != false )
		{
			GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			return;
		}
	}

	if ( (lpMsg->sFlag == 0 && lpMsg->tFlag == 0) ||
 		 (lpMsg->sFlag == 2 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 2) ||
 		 (lpMsg->sFlag == 2 && lpMsg->tFlag == 2) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 3) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 5) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 6) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 7) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 8) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 9) ||
		 (lpMsg->sFlag == 9 && lpMsg->tFlag == 9) ||
		 (lpMsg->sFlag == 9 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 4) ||
		 (lpMsg->sFlag == 4 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 4 && lpMsg->tFlag == 4) ||
		 (lpMsg->sFlag == 5 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 5 && lpMsg->tFlag == 5) ||
		 (lpMsg->sFlag == 6 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 6 && lpMsg->tFlag == 6) ||
		 (lpMsg->sFlag == 7 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 7 && lpMsg->tFlag == 7) ||
		 (lpMsg->sFlag == 8 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 8 && lpMsg->tFlag == 8) ||
		 (lpMsg->sFlag == 3 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 3 && lpMsg->tFlag == 3) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 10)||
		 (lpMsg->sFlag == 10 && lpMsg->tFlag == 0)||
		 (lpMsg->sFlag == 10 && lpMsg->tFlag == 10)||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 11)||
		 (lpMsg->sFlag == 11 && lpMsg->tFlag == 0)||
		 (lpMsg->sFlag == 11 && lpMsg->tFlag == 11)||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 12)||
		 (lpMsg->sFlag == 12 && lpMsg->tFlag == 0)||
		 (lpMsg->sFlag == 12 && lpMsg->tFlag == 12)||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 13)||
		 (lpMsg->sFlag == 13 && lpMsg->tFlag == 0)||
		 (lpMsg->sFlag == 13 && lpMsg->tFlag == 13)||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 14)||
		 (lpMsg->sFlag == 14 && lpMsg->tFlag == 0)||
		 (lpMsg->sFlag == 14 && lpMsg->tFlag == 14))
	{
		BOOL DurSSend;
		BOOL DurTSend;

		result = gObjInventoryMoveItem(aIndex, source, target, DurSSend, DurTSend, lpMsg->sFlag, lpMsg->tFlag, (LPBYTE)&ItemInfo);
		::GCItemMoveResultSend(aIndex, result, target, (LPBYTE)&ItemInfo);

		if ( DurSSend != FALSE )
			::GCItemDurSend(aIndex, source, lpObj->pInventory[source].m_Durability, FALSE);

		if ( DurTSend != FALSE )
			::GCItemDurSend(aIndex, target, lpObj->pInventory[target].m_Durability, FALSE);

		if ( result != 0xFF && lpMsg->sFlag == 2 && lpMsg->tFlag == 0)
		{
			int money = GetWarehouseUsedHowMuch(lpObj->Level, lpObj->WarehousePW);

			if ( (lpObj->Money - money) > 0 )
			{
				int iZen = lpObj->Money;
				lpObj->Money -= money;
				GCMoneySend(lpObj->m_Index, lpObj->Money);
				CLog.LogAdd("Pay WareHouse Money(In Inventory) : %d - %d = %d", iZen, money, lpObj->Money);
			}
			else if ( (lpObj->WarehouseMoney - money) > 0 )
			{
				int iZen = lpObj->WarehouseMoney;
				lpObj->WarehouseMoney -= money;
				CLog.LogAdd("Pay WareHouse Money(In WareHouse) : %d - %d = %d", iZen, money, lpObj->WarehouseMoney);
				GCWarehouseInventoryMoneySend(aIndex, 1, lpObj->Money, lpObj->WarehouseMoney);
			}
		}

		return;
	}

	if ( lpMsg->sFlag == 1 && lpMsg->tFlag == 1 )
	{
		result = gObjTradeTradeMove(lpObj, source, target);
		GCItemMoveResultSend(aIndex, result, target, (LPBYTE)&ItemInfo);
		return;
	}

	if ( lpMsg->sFlag == 1 && lpMsg->tFlag == 0 )
	{
		result = gObjTradeInventoryMove(lpObj, source, target);

		if ( result == 0xFF )
		{
			GCItemMoveResultSend(aIndex, 0xFF, target, (LPBYTE)&ItemInfo);
			CGTradeCancelButtonRecv(aIndex);
		}
		else
		{
			GCItemMoveResultSend(aIndex, result, target, (LPBYTE)&ItemInfo);
		}

		if ( result == 0xFF )
		{
			lpObj->m_IfState.state = 0;
			GCItemListSend(aIndex);
		}
		
		if ( lpObj->TargetNumber >= 0 )
		{
			lpObj->TradeOk = false;
			gObj[gObj[aIndex].TargetNumber].TradeOk = false;
			GCTradeOkButtonSend(lpObj->TargetNumber, 2);
			GCTradeOkButtonSend(aIndex, 0);
		}

		return;
	}

	if ( lpMsg->sFlag == 0 && lpMsg->tFlag == 1 )
	{
		result = gObjInventoryTradeMove(lpObj, source, target);
		GCItemMoveResultSend(aIndex, result, target, (LPBYTE)&ItemInfo);
	}
}
// -----------------------------------------------------------------------------------------------
void GCEquipmentChange(int aIndex, int pos)
{
	PMSG_USEREQUIPMENTCHANGED pChange;
	LPOBJ lpObj = &gObj[aIndex];

	C1HeadSet((LPBYTE)&pChange, 0x25, sizeof(PMSG_USEREQUIPMENTCHANGED));
	pChange.NumberH = SET_NUMBERH(aIndex);
	pChange.NumberL = SET_NUMBERL(aIndex);
	ItemByteConvert(pChange.ItemInfo, lpObj->pInventory[pos]);
	pChange.ItemInfo[I_OPTION] = pos << 4;
	pChange.ItemInfo[I_OPTION] |= LevelSmallConvert(aIndex, pos) & MAX_ITEM_LEVEL;

	gSendProto.VPMsgSend(lpObj, (LPBYTE)&pChange, pChange.h.size);
}
// -----------------------------------------------------------------------------------------------
void CGTalkRequestRecv(PMSG_TALKREQUEST * lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int DealerNumber ;
	PMSG_TALKRESULT pResult;
	PMSG_SHOPITEMCOUNT pShopItemCount;

	if ( !PacketCheckTime(lpObj))
		return;

	if ( gObj[aIndex].CloseType != -1 )
		return;

	if ( !gObjIsConnectedGP(aIndex) )
	{
		CLog.LogAdd("[ANTI-HACK][NPC Talk] Couldn't talk.. (%s)(%s)", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	BYTE SendByte[1024];
	int lOfs = 0;

	if ( lpObj->m_IfState.use > 0 )
	{
		return;
	}

	DealerNumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
	
	if ( DealerNumber < 0 || DealerNumber > OBJMAX-1)
	{
		return;
	}

	if ( lpObj->MapNumber != gObj[DealerNumber].MapNumber )
	{
		return;
	}

#if(!GS_CASTLE)
	if ( gObj[DealerNumber].Class == 367 )
	{
		if ( (lpObj->X < (gObj[DealerNumber].X-5) )|| (lpObj->X> (gObj[DealerNumber].X+5) )||(lpObj->Y < (gObj[DealerNumber].Y-10)) ||(lpObj->Y > (gObj[DealerNumber].Y+5)))
		{
			return;
		}
	}
	else if ( (lpObj->X < (gObj[DealerNumber].X-5) )|| (lpObj->X> (gObj[DealerNumber].X+5) )||(lpObj->Y < (gObj[DealerNumber].Y-5)) ||(lpObj->Y > (gObj[DealerNumber].Y+5)))
	{
		return;
	}
#else
	if ( (lpObj->X < (gObj[DealerNumber].X-5) )|| (lpObj->X> (gObj[DealerNumber].X+5) )||(lpObj->Y < (gObj[DealerNumber].Y-5)) ||(lpObj->Y > (gObj[DealerNumber].Y+5)))
	{
		return;
	}
#endif

	int ShopNum = gObj[DealerNumber].ShopNumber;

	if ( gObj[DealerNumber].Type == OBJ_NPC )
	{
		if ( gNpcTalk.NpcTalk(&gObj[DealerNumber], lpObj) == true )
		{
			return;
		}
	}

	if ( ::gObjFixInventoryPointer(aIndex) == false )
	{
		CLog.LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[DealerNumber].Class == 234 )	// Npc Server Division
	{
		pResult.h.c = 0xC3;
		pResult.h.headcode = 0x30;
		pResult.h.size = sizeof(pResult);
		pResult.result = 0;
		pResult.result = 5;

		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( gObj[DealerNumber].Type == OBJ_NPC && ShopNum < 0)
	{
		return;
	}

	if ( ShopNum < MAX_SHOP )
	{
		if ( ShopC[ShopNum].ItemCount < TRUE )
		{
			if ( (rand()%2) != 0 )
			{
				ChatTargetSend(&gObj[DealerNumber], lMsg.Get(MSGGET(4, 119)), aIndex);
			}
			else
			{
				ChatTargetSend(&gObj[DealerNumber], lMsg.Get(MSGGET(4, 120)), aIndex);
			}
			return;
		}
	}

	lpObj->TargetShopNumber = ShopNum;
	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 3;
	lpObj->m_ShopTime = 0;
	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x30;
	pResult.h.size = sizeof(pResult);
	pResult.result = 0;

	if ( ShopNum == 100 )
	{
		if ( gObj[aIndex].m_ReqWarehouseOpen != false )
		{
			return;
		}
		
		gObj[aIndex].m_ReqWarehouseOpen = true;
		lpObj->m_IfState.type = 6;
		lpObj->m_IfState.state = 0;
	}
	else if ( ShopNum == 101 )
	{
		if ( bCanChaosBox == TRUE )
		{
			if ( lpObj->m_bPShopOpen == true )
			{
				CLog.LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
				GCServerMsgStringSend(lMsg.Get(MSGGET(4, 194)), lpObj->m_Index, 1);
				return;
			}

			lpObj->m_IfState.type = 7;
			lpObj->m_IfState.state = 0;
			pResult.result = 3;
			lpObj->bIsChaosMixCompleted = false;
			pResult.level1 = g_DevilSquare.DS1ChaosSuccessRate;
			pResult.level2 = g_DevilSquare.DS2ChaosSuccessRate;
			pResult.level3 = g_DevilSquare.DS3ChaosSuccessRate;
			pResult.level4 = g_DevilSquare.DS4ChaosSuccessRate;
			pResult.level5 = g_DevilSquare.DS5ChaosSuccessRate;
			pResult.level6 = g_DevilSquare.DS6ChaosSuccessRate;
			pResult.level7 = g_DevilSquare.DS7ChaosSuccessRate;

			gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			gObjInventoryTrans(lpObj->m_Index);

			CLog.LogAdd("[%s][%s] Open Chaos Box", lpObj->AccountID, lpObj->Name);
			gObjItemTextSave(lpObj);
			gObjWarehouseTextSave(lpObj);
			GCAnsCsMapSvrTaxInfo( lpObj->m_Index, 1, ::g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
		}
	}
	else
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	}

	if ( ShopNum == 100 )
	{
		gObj[aIndex].WarehouseCount = 0;
		GDGetWarehouseList(aIndex, gObj[aIndex].AccountID);
		return;
	}

	if ( ShopNum == 101 )
	{
		return;
	}
	else
	{
		lOfs = 0;

		lOfs += sizeof(pShopItemCount );

		int size = lOfs + ShopC[ShopNum].SendItemDataLen;
		C2HeadSet((LPBYTE)&pShopItemCount, 0x31, size);
		pShopItemCount.Type = 0;
		pShopItemCount.count = ShopC[ShopNum].ItemCount;
		memcpy(SendByte, &pShopItemCount, sizeof(pShopItemCount));
		memcpy(&SendByte[lOfs], ShopC[ShopNum].SendItemData, ShopC[ShopNum].SendItemDataLen);

		gSendProto.DataSend(aIndex, SendByte, size);
		GCAnsCsMapSvrTaxInfo(lpObj->m_Index,2,  ::g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index));
	}
}
// -----------------------------------------------------------------------------------------------
void GCUserWarehouseSend(LPOBJ lpObj)
{
	if ( lpObj->m_IfState.type != 6 )
	{
		return;
	}

	if ( lpObj->m_IfState.type == 6 && lpObj->m_IfState.state == 1 )
	{
		return;
	}

	PMSG_SHOPITEMCOUNT pMsg;
	BYTE SendByte[2048];
	int lOfs = sizeof(pMsg);

	C2HeadSet((LPBYTE)&pMsg, 0x31, 0);
	pMsg.count = 0;
	pMsg.Type = 0;

	for ( int n=0;n<WAREHOUSE_SIZE;n++)
	{
		if ( lpObj->pWarehouse[n].IsItem() == TRUE )
		{
			SendByte[lOfs] = n;
			lOfs++;
			ItemByteConvert(&SendByte[lOfs], lpObj->pWarehouse[n] );
			lOfs += MAX_ITEM_INFO;
			pMsg.count ++;
		}
	}

	pMsg.h.sizeH = SET_NUMBERH(lOfs);
	pMsg.h.sizeL = SET_NUMBERL(lOfs);
	memcpy(SendByte, &pMsg, sizeof(pMsg));

	gSendProto.DataSend(lpObj->m_Index, SendByte, lOfs);
	GCWarehouseInventoryMoneySend(lpObj->m_Index, 1, lpObj->Money, lpObj->WarehouseMoney);

	if ( lpObj->m_IfState.type == 6 )
	{
		lpObj->m_IfState.state = 1;
		lpObj->WarehouseSave = TRUE;
	}
}
// -----------------------------------------------------------------------------------------------
struct PMSG_BUYRESULT
{
	PBMSG_HEAD h;	// C1:32
	BYTE Result;	// 3
	BYTE ItemInfo[MAX_ITEM_INFO];	// 4
};
// -----------------------------------------------------------------------------------------------
void CGBuyRequestRecv(PMSG_BUYREQUEST * lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	PMSG_BUYRESULT pResult;

	C1HeadSet((LPBYTE)&pResult, 0x32, sizeof(pResult));
	pResult.Result = -1;

	if ( !PacketCheckTime(lpObj))
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !::gObjFixInventoryPointer(aIndex))
	{
		CLog.LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].m_ShopTime == 0 )
		gObj[aIndex].m_ShopTime = 1;

	if ( gObj[aIndex].m_ShopTime > 	60 )
	{
		if ( lpObj->m_IfState.use != 0 && lpObj->m_IfState.type == 3 )
		{
			lpObj->TargetShopNumber = -1;
			lpObj->m_IfState.use = 0;
			lpObj->m_IfState.type = 0;
		}

		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->m_IfState.use > 0 )
	{
		if ( lpObj->m_IfState.type != 3 )
		{
			gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}

	int tShop = lpObj->TargetShopNumber;

	if ( tShop < 0 || tShop > MAX_SHOP-1 )
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpMsg->Pos > MAX_ITEM_IN_SHOP-1 )
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	
	if ( ShopC[tShop].m_item[lpMsg->Pos].IsItem() == TRUE )
	{
		if ( ShopC[tShop].m_item[lpMsg->Pos].m_Type >= ITEMGET(13,71) && ShopC[tShop].m_item[lpMsg->Pos].m_Type <= ITEMGET(13,75) )
		{
			gMossMerchant.ItemBuy(aIndex,ShopC[tShop].m_item[lpMsg->Pos].m_Type);
		}
		else
		{
			int iStoreTaxMoney = ShopC[tShop].m_item[lpMsg->Pos].m_BuyMoney + (int)((__int64)ShopC[tShop].m_item[lpMsg->Pos].m_BuyMoney * (__int64)g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index) / (__int64)100);
		
			if ( iStoreTaxMoney < 0 )
				iStoreTaxMoney  = 0;

			int iStoreTaxMoney2 = (int)((__int64)ShopC[tShop].m_item[lpMsg->Pos].m_BuyMoney * (__int64)g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index) / (__int64)100);

			if ( iStoreTaxMoney2 < 0 )
				iStoreTaxMoney2  = 0;

			if ( lpObj->Money < iStoreTaxMoney )
				pResult.Result = -1;
			else
			{
				BOOL bNoItem = TRUE;

				if ( (ShopC[tShop].m_item[lpMsg->Pos].m_Type >= ITEMGET(14,0) && ShopC[tShop].m_item[lpMsg->Pos].m_Type <= ITEMGET(14,8)) ||
					 (ShopC[tShop].m_item[lpMsg->Pos].m_Type >= ITEMGET(14,35) && ShopC[tShop].m_item[lpMsg->Pos].m_Type <= ITEMGET(14,40)))
				{
					int dur = ShopC[tShop].m_item[lpMsg->Pos].m_Durability;

					if ( dur == 0 )
						dur = 1;

					if ( ::gObjSearchItem(&gObj[aIndex], ShopC[tShop].m_item[lpMsg->Pos].m_Type,dur, ShopC[tShop].m_item[lpMsg->Pos].m_Level) == TRUE )
					{
						bNoItem = FALSE;
						lpObj->Money -= iStoreTaxMoney;
						::g_CastleSiegeSync.AddTributeMoney(iStoreTaxMoney2);

						if ( lpObj->Money < 0 )
							lpObj->Money = 0;

		
						::GCMoneySend(aIndex, lpObj->Money);
					}
				}

				if ( bNoItem != FALSE )
				{
					pResult.Result = gObjShopBuyInventoryInsertItem(aIndex, ShopC[tShop].m_item[lpMsg->Pos]);
				
					if ( pResult.Result != 0xFF )
					{
						ItemByteConvert((LPBYTE)&pResult.ItemInfo, ShopC[tShop].m_item[lpMsg->Pos]);
						int lc64 = ShopC[tShop].m_item[lpMsg->Pos].m_BuyMoney;
						lpObj->Money -= iStoreTaxMoney;
						::g_CastleSiegeSync.AddTributeMoney(iStoreTaxMoney2);

						if ( lpObj->Money < 0 )
							lpObj->Money = 0;

						GCMoneySend(aIndex, lpObj->Money);
						int iTaxRate = g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index);

						CLog.LogAdd("[%s][%s] (%d) Shop buy [%d][%d][%d][%s] LEV:%d, DUR:%d, OP:[%d][%d][%d]",
							lpObj->AccountID, lpObj->Name, tShop, lc64, iTaxRate, lc64+iStoreTaxMoney2, ShopC[tShop].m_item[lpMsg->Pos].GetName(),
							ShopC[tShop].m_item[lpMsg->Pos].m_Level, ShopC[tShop].m_item[lpMsg->Pos].m_Durability,
							ShopC[tShop].m_item[lpMsg->Pos].m_Option1, ShopC[tShop].m_item[lpMsg->Pos].m_Option2,
							ShopC[tShop].m_item[lpMsg->Pos].m_Option3);
					}
				}
			}
		}
	}

	gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_SELLRESULT
{
	PBMSG_HEAD h;	// C1:33
	BYTE Result;	// 3
	DWORD Money;	// 4
};
// -----------------------------------------------------------------------------------------------
void CGSellRequestRecv(PMSG_SELLREQUEST * lpMsg, int aIndex)
{
	PMSG_SELLRESULT pResult;
	LPOBJ lpObj = &gObj[aIndex];

	C1HeadSet((LPBYTE)&pResult, 0x33, sizeof(pResult));
	pResult.Result = false;
	pResult.Money = 0;

	if ( gObj[aIndex].CloseType != -1 )
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->m_IfState.use > 0 && lpObj->m_IfState.type != 3 )
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !PacketCheckTime(lpObj))
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !::gObjFixInventoryPointer(aIndex))
	{
		CLog.LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].pTransaction == 1 )
	{
		CLog.LogAdd("[%s][%s] CGSellRequestRecv() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);

		return;
	}

	int tShop = lpObj->TargetShopNumber;

	if ( tShop < 0 || tShop > MAX_SHOP-1 )
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !gObjCanItemTouch(lpObj, 3))
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpMsg->Pos > MAIN_INVENTORY_SIZE-1 )
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		CLog.LogAdd(lMsg.Get(MSGGET(1, 227)), lpMsg->Pos);
		return;
	}

	if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(14,11) ) // Box of Luck
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->pInventory[lpMsg->Pos].m_QuestItem != false) 
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(13,20) && lpObj->pInventory[lpMsg->Pos].m_Level == 1) // Wizards Ring +1
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(13,20) && lpObj->pInventory[lpMsg->Pos].m_Level == 2) // Wizards Ring +2
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(12,26) &&
		(lpObj->pInventory[lpMsg->Pos].m_Level == 1 ||
		 lpObj->pInventory[lpMsg->Pos].m_Level == 2 || 
		 lpObj->pInventory[lpMsg->Pos].m_Level == 3 || 
		 lpObj->pInventory[lpMsg->Pos].m_Level == 4 || 
		 lpObj->pInventory[lpMsg->Pos].m_Level == 5 ))
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[lpMsg->Pos]) == TRUE )
	{
		GCServerMsgStringSend(lMsg.Get(MSGGET(13, 37)), lpObj->m_Index, 1);
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( IsCashItem(lpObj->pInventory[lpMsg->Pos].m_Type) == TRUE )
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(14,12) && lpObj->pInventory[lpMsg->Pos].m_Level == 1) // Heart+1
	{
		pResult.Result = true;
		pResult.Money = lpObj->Money;
		gObjInventoryItemSet(aIndex, lpMsg->Pos, -1);
		lpObj->pInventory[lpMsg->Pos].Clear();
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if ( lpObj->pInventory[lpMsg->Pos].IsItem() == FALSE )
	{
		pResult.Result = false;
		pResult.Money = lpObj->Money;
		CLog.LogAdd(lMsg.Get(MSGGET(1, 228)), __FILE__, __LINE__, lpObj->Name, lpMsg->Pos);
		CLog.LogAdd("ID:%s CharID:%s ", lpObj->AccountID, lpObj->Name);
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

	}
	else
	{
		BYTE NewOption[MAX_EXOPTION_SIZE];
		int iAddZen = 0;
		int type = lpObj->pInventory[lpMsg->Pos].m_Type;

		if ( type == ITEMGET(13,4) || type == ITEMGET(13,5)) // Pets
			lpObj->pInventory[lpMsg->Pos].PetValue();
		else
			lpObj->pInventory[lpMsg->Pos].Value();

		iAddZen = lpObj->pInventory[lpMsg->Pos].m_SellMoney;

		if ( gObjCheckMaxZen(aIndex, iAddZen) == FALSE )
		{
			gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		lpObj->Money += iAddZen;
		pResult.Result =true;
		pResult.Money = lpObj->Money;

		

		ItemIsBufExOption(NewOption, &lpObj->pInventory[lpMsg->Pos]);

		CLog.LogAdd(lMsg.Get(MSGGET(1, 229)), lpObj->AccountID, lpObj->Name, tShop,
			iAddZen, lpObj->pInventory[lpMsg->Pos].GetName(), 
			lpObj->pInventory[lpMsg->Pos].m_Level, lpObj->pInventory[lpMsg->Pos].m_Option1,
			lpObj->pInventory[lpMsg->Pos].m_Option2, lpObj->pInventory[lpMsg->Pos].m_Option3,
			lpObj->pInventory[lpMsg->Pos].m_Number, (int)lpObj->pInventory[lpMsg->Pos].m_Durability,
			NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6], 
			lpObj->pInventory[lpMsg->Pos].m_SetOption);

		::gObjInventoryDeleteItem(aIndex, lpMsg->Pos);

		if ( lpMsg->Pos == 10 || lpMsg->Pos == 11 )
		{
			::gObjUseSkill.SkillChangeUse(aIndex);
		}

		::gObjMakePreviewCharSet(aIndex);
	}

	gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}
// -----------------------------------------------------------------------------------------------
int GetNeedMoneyItemDurRepaire(CItem *  DurItem, BOOL RequestPos)
{
	int iMoney = 0;
	int type = DurItem->m_Type;
	float ItemDurability = DurItem->m_Durability;

	if ( type == ITEMGET(13,10) ) // Transformation Ring
		return 0;

	if ( ::HasItemDurability(type ) != FALSE )
	{
		float lc4;
		float lc5 = 0;
		lc4 = DurItem->m_BaseDurability;

		if ( ItemDurability == lc4 )
			return 0;

		float lc6 = 1.0f - ItemDurability / lc4;
		int lc7;

		if ( type == ITEMGET(13,4)|| type ==ITEMGET(13,5) ) // Pets
			lc7 = DurItem->m_BuyMoney;
		else
			lc7 = DurItem->m_BuyMoney/3;

		if ( lc7 > 400000000 ) lc7 = 400000000;

		if ( lc7 >= 1000 )
			lc7 = lc7/100*100;
		else if ( lc7 >= 100 )
			lc7 = lc7/10*10;

		float lc8 = sqrt((float)lc7);
		float lc9 = sqrt((float)sqrt((float)lc7)); 
		lc5 = 3.0f * lc8 * lc9;
		lc5 *= lc6;
		lc5 += 1.0f;

		if ( ItemDurability <= 0.0f )
		{
			if ( type == ITEMGET(13,4)|| type ==ITEMGET(13,5) ) // Pets
				lc5 *= 2;
			else
				lc5 *= ::GetAllRepairItemRate(type);

		}

		if ( RequestPos == TRUE )
		{
			lc5 += lc5 * 0.05f;
		}
		iMoney = lc5;

		if ( iMoney >= 1000 )
			iMoney = iMoney/100*100;
		else if ( iMoney > 10 )	// Deathway fix Here -> Must be 100 and >=
			iMoney = iMoney/10*10;
		
	}
	else
	{
		iMoney = -1;
	}

	return iMoney;
}

struct PMSG_ITEMDURREPAIR_RESULT
{
	PBMSG_HEAD h;	// C1:34
	int Money;	// 4
};

void ItemDurRepaire(LPOBJ lpObj, CItem * DurItem, int pos, int RequestPos)
{
	PMSG_ITEMDURREPAIR_RESULT pResult;
	int result = TRUE;

	C1HeadSet((LPBYTE)&pResult, 0x34, sizeof(pResult));
	int itemtype = DurItem->m_Type;

	if ( lpObj->m_IfState.type != 13 )
	{
		if ( itemtype ==ITEMGET(13,4)  || itemtype == ITEMGET(13,5))
		{
			pResult.Money = 0;
			gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

			return;
		}
	}

	if ( itemtype >= ITEMGET(14,0) 
		|| ( itemtype >= ITEMGET(13,0) &&itemtype  < ITEMGET(13,4) ) 
		||itemtype == ITEMGET(13,10) 
		|| (itemtype >= ITEMGET(12,7) && itemtype < ITEMGET(12,36)
		||itemtype == ITEMGET(13,51) ) 
		||itemtype ==ITEMGET(4,7) ||itemtype ==ITEMGET(4,15)  )
	{
		pResult.Money = 0;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if ( itemtype == ITEMGET(13,38) )
	{
		pResult.Money = 0;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if ( itemtype == ITEMGET(13,39) )
	{
		pResult.Money = 0;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if ( itemtype == ITEMGET(13,40) )
	{
		pResult.Money = 0;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if ( itemtype == ITEMGET(13,41) )
	{
		pResult.Money = 0;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if ( itemtype == ITEMGET(13,42) )
	{
		pResult.Money = 0;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	pResult.Money = GetNeedMoneyItemDurRepaire(DurItem, RequestPos);

	if ( pResult.Money <= 0 )
	{
		pResult.Money = 0;
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if ( (lpObj->Money - pResult.Money) < 1 )
		pResult.Money = 0;
	else
	{
		lpObj->Money -= pResult.Money;
		pResult.Money = lpObj->Money;
		DurItem->m_Durability = (float)((int)DurItem->m_BaseDurability);
		DurItem->Convert(DurItem->m_Type, DurItem->m_Option1, DurItem->m_Option2, DurItem->m_Option3,
			DurItem->m_NewOption, DurItem->m_SetOption,DurItem->m_ItemOptionEx,DurItem->m_ItemSocket);
		GCItemDurSend(lpObj->m_Index, pos, DurItem->m_Durability, FALSE);
	}

	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
}

void CGModifyRequestItem(PMSG_ITEMDURREPAIR * lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	PMSG_ITEMDURREPAIR_RESULT pResult;

	C1HeadSet((LPBYTE)&pResult, 0x34, sizeof(pResult));
	pResult.Money = 0;


	if ( gObj[aIndex].CloseType != -1 )
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !PacketCheckTime(lpObj))
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (lpMsg->Requestpos == 1 && gObj[aIndex].Level < 50 )
	{
		pResult.Money = 0;
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpMsg->Position == 0xFF )
	{
		for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
		{
			if ( lpObj->pInventory[n].IsItem())
			{
				if ( IsCashItem(lpObj->pInventory[n].m_Type ) == TRUE )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,20) && (lpObj->pInventory[n].m_Level == 0 ||lpObj->pInventory[n].m_Level == 1 ))
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,20) && lpObj->pInventory[n].m_Level == 2 )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,4) || lpObj->pInventory[n].m_Type == ITEMGET(13,5) )
					continue;
				
				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,37) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,38) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,39) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,40) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,41) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,42) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(12,26) &&
					(lpObj->pInventory[n].m_Level == 1 ||
					 lpObj->pInventory[n].m_Level == 2 || 
					 lpObj->pInventory[n].m_Level == 3 || 
					 lpObj->pInventory[n].m_Level == 4 || 
					 lpObj->pInventory[n].m_Level == 5 ))
					 continue;
					
				ItemDurRepaire(lpObj,&lpObj->pInventory[n] , n, lpMsg->Requestpos);
			}
		}

		gCalCharacter.gObjCalCharacter(lpObj->m_Index);
		return;
	}

	if ( lpMsg->Position > MAIN_INVENTORY_SIZE-1)
	{
		pResult.Money = 0;
		return;
	}

	if ( IsCashItem(lpObj->pInventory[lpMsg->Position].m_Type ) == TRUE )
		return;


	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,20) && (lpObj->pInventory[lpMsg->Position].m_Level == 0 ||lpObj->pInventory[lpMsg->Position].m_Level == 1 ))
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,20) && lpObj->pInventory[lpMsg->Position].m_Level == 2 )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,37) )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,38) )
		return;

	ItemDurRepaire(lpObj, &lpObj->pInventory[lpMsg->Position], lpMsg->Position, lpMsg->Requestpos);
	gCalCharacter.gObjCalCharacter(lpObj->m_Index);
}

struct PMSG_TRADE_REQUESTSEND
{
	PBMSG_HEAD h;	// C3:36
	char szId[10];	// 3
};

void CGTradeRequestSend(PMSG_TRADE_REQUEST * lpMsg, int aIndex)
{
	int number;

	if (bCanTrade == FALSE )
	{
		::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 125)), aIndex, 1);
		::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 126)), aIndex, 1);
		return;
	}

#if(!GS_CASTLE)
	if ( IT_MAP_RANGE(gObj[aIndex].MapNumber) == TRUE )
	{
		::GCServerMsgStringSend("Você não pode dar trade no templo.", aIndex, 1);
		return;
	}
#endif

	if ( gObj[aIndex].CloseType != -1 )
		return;

	LPOBJ lpObj = &gObj[aIndex];

	if ( !PacketCheckTime(lpObj))
		return;

	number = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( OBJMAX_RANGE(number) == FALSE )
	{
		CLog.LogAdd("error : %s %d (%d)", __FILE__, __LINE__, number);
		return;
	}

	if ( number == aIndex )
		return;

	if ( !gObjIsConnected(number))
		return;

	if ( gObj[number].Type == OBJ_MONSTER )
		return;

	if ( gObj[number].CloseCount >= 0 )
		return;

	if ( DS_MAP_RANGE(gObj[number].MapNumber) != FALSE )
	{
		::GCServerMsgStringSend(lMsg.Get(MSGGET(2, 199)), aIndex, 1);
		return;
	}

	if ( gObj[aIndex].m_bPShopOpen == true )
		return;

	if ( gObj[number].m_bPShopOpen == true )
		return;

	if ( gUser.ChaosCastleMapRange(gObj[number].MapNumber) == true )
	{
		::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 196)), aIndex, 1);
		return;
	}

	if ( gUser.BloodCastleMapRange(gObj[aIndex].MapNumber) == true )
	{
		if ( g_BloodCastle.GetCurrentState(g_BloodCastle.GetBloodCastleIndex(gObj[aIndex].MapNumber)) != 1 || g_BloodCastle.CheckCanEnter(g_BloodCastle.GetBloodCastleIndex(gObj[aIndex].MapNumber)) == false )
		{
			::GCServerMsgStringSend(lMsg.Get(MSGGET(4, 188)), aIndex, 1);
			return;
		}	
	}

	if ( (gObj[number].m_Option &1) != 1 )
	{
		::GCTradeResponseSend(0, aIndex, gObj[number].Name, 0, 0);
		return;
	}

	if ( lpObj->m_IfState.use > 0 )
	{
		::GCTradeResponseSend(3, aIndex, gObj[number].Name, 0, 0);
		return;
	}

	if (gObj[number].m_IfState.use > 0 )
	{
		::GCTradeResponseSend(2, aIndex, gObj[number].Name, 0, 0);
		return;
	}

	if ( ::gObjFixInventoryPointer(aIndex) == false )
	{
		CLog.LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].pTransaction == 1 )
	{
		CLog.LogAdd("[%s][%s] CGTradeRequestSend() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);

		return;
	}

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.type = 1;
	lpObj->TargetNumber = number;
	gObj[number].m_IfState.use = 1;
	gObj[number].m_IfState.state = 0;
	gObj[number].m_IfState.type = 1;
	gObj[number].TargetNumber = aIndex;
	lpObj->m_InterfaceTime = GetTickCount();
	gObj[number].m_InterfaceTime = GetTickCount();

	PMSG_TRADE_REQUESTSEND pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x36;
	pMsg.h.size = sizeof(pMsg);
	memcpy(pMsg.szId, gObj[aIndex].Name, sizeof(pMsg.szId));

	gSendProto.DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);

	CLog.LogAdd(lMsg.Get(MSGGET(1, 230)), gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Ip_addr,
		gObj[number].AccountID, gObj[number].Name, gObj[number].Ip_addr);
}

void CGTradeResponseRecv(PMSG_TRADE_RESPONSE * lpMsg, int aIndex)
{
	BOOL Result = TRUE;
	int number = gObj[aIndex].TargetNumber;

	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		CLog.LogAdd("error-L1 : %d (A_ID:%s) %s %d", number, gObj[aIndex].AccountID, __FILE__, __LINE__);
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( OBJMAX_RANGE(number) == FALSE )
	{
		if ( gObj[aIndex].m_IfState.use != FALSE )
		{
			if ( gObj[aIndex].m_IfState.type == 1 )
			{
				gObj[aIndex].m_IfState.use = FALSE;
				gObj[aIndex].TargetNumber = -1;
			}
		}

		CLog.LogAdd("error-L1 : target:%d (A_ID:%s) %s %d", number, gObj[aIndex].AccountID, __FILE__, __LINE__);
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( gObjIsConnected(number) == FALSE )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}
	
	if ( gObj[aIndex].CloseType != -1 )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( gObj[aIndex].m_bPShopOpen == true )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( gObj[number].m_bPShopOpen == true )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( gObj[aIndex].X < (gObj[number].X -2 ) || gObj[aIndex].X > (gObj[number].X +2 ) || gObj[aIndex].Y < (gObj[number].Y -2 ) || gObj[aIndex].Y > (gObj[number].Y +2 ) )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( gObj[aIndex].m_IfState.use == FALSE || gObj[aIndex].m_IfState.type != 1 || gObj[aIndex].m_IfState.state != 0 )
	{
		CLog.LogAdd("(%s)(%s) Trade Interface State Error : use:%d type:%d", gObj[aIndex].AccountID, gObj[aIndex].Name, 
			gObj[aIndex].m_IfState.use, gObj[aIndex].m_IfState.type);

		return;
	}

	if ( gObj[number].m_IfState.use == FALSE || gObj[number].m_IfState.type != 1 || gObj[number].m_IfState.state != 0 )
	{
		CLog.LogAdd("(%s)(%s) Trade Interface State Error : use:%d type:%d", gObj[number].AccountID, gObj[number].Name, 
			gObj[number].m_IfState.use, gObj[number].m_IfState.type);

		return;
	}

	if ( gObjFixInventoryPointer(aIndex) == false )
	{
		CLog.LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].pTransaction == 1 )
	{
		CLog.LogAdd("[%s][%s] CGTradeResponseRecv() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);

		return;
	}

	if ( lpMsg->Response == false )
	{
		GCTradeResponseSend(lpMsg->Response, number, gObj[aIndex].Name, 0, 0);
		Result = FALSE;
	}
	else
	{
		Result = TRUE;

		for ( int i=0;i<TRADE_BOX_SIZE;i++)
		{
			gObj[aIndex].Trade[i].Clear();
			gObj[number].Trade[i].Clear();
		}

		memset(gObj[aIndex].TradeMap, (BYTE)-1, TRADE_BOX_MAP_SIZE );
		memset(gObj[number].TradeMap, (BYTE)-1, TRADE_BOX_MAP_SIZE );
		gObj[aIndex].m_IfState.state = 1;
		gObj[number].m_IfState.state = 1;
		gObj[aIndex].TradeMoney = 0;
		gObj[number].TradeMoney = 0;

		if (::gObjInventoryTrans(aIndex) == FALSE )
			Result = 2;

		if (::gObjInventoryTrans(number) == FALSE )
			Result = 3;

		if ( Result != TRUE )
		{
			lpMsg->Response = false;
			GCTradeResponseSend(lpMsg->Response, number, gObj[aIndex].Name, 0, 0);
			GCTradeResponseSend(lpMsg->Response, aIndex, gObj[number].Name, 0, 0);

			if ( Result == 2 )
			{
				CLog.LogAdd("[%s][%s] Made Trade Error (in Transaction) [%s][%s]",
					gObj[aIndex].AccountID, gObj[aIndex].Name,
					gObj[number].AccountID, gObj[number].Name);
			}
			else if ( Result == 3 )
			{
				CLog.LogAdd("[%s][%s] Made Trade Error (in Transaction) [%s][%s]",
					gObj[number].AccountID, gObj[number].Name,
					gObj[aIndex].AccountID, gObj[aIndex].Name);
			}

			Result = FALSE;
		}
		else
		{
			lpMsg->Response = true;
			GCTradeResponseSend(lpMsg->Response, number, gObj[aIndex].Name, gObj[aIndex].Level, gObj[aIndex].GuildNumber);
			GCTradeResponseSend(lpMsg->Response, aIndex, gObj[number].Name, gObj[number].Level, gObj[number].GuildNumber);

			CLog.LogAdd("[%s][%s] Trade Ready [%s][%s]", 
				gObj[number].AccountID, gObj[number].Name,
				gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
	}

	if ( Result == FALSE )
	{
		if ( gObj[aIndex].m_IfState.use != FALSE && gObj[aIndex].m_IfState.type == 1 )
		{
			gObj[aIndex].m_IfState.use = FALSE;
			gObj[aIndex].TargetNumber = -1;
			CLog.LogAdd("Interface State : %d", gObj[aIndex].m_IfState.use);
		}

		if ( gObj[number].TargetNumber == aIndex )
		{
			if ( gObj[number].m_IfState.use != FALSE && gObj[number].m_IfState.type == 1 )
			{
				gObj[number].m_IfState.use = FALSE;
				gObj[number].TargetNumber = -1;
				CLog.LogAdd("Interface State : %d", gObj[number].m_IfState.use);
			}
		}
	}
}

void GCTradeResponseSend(BYTE response, int aIndex, LPSTR id, WORD level, int GuildNumber)
{
	PMSG_TRADE_RESPONSE pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x37, sizeof(pMsg));
	pMsg.Response = response;
	pMsg.Level = level;
	pMsg.GuildNumber = GuildNumber;
	memcpy(pMsg.Id, id, sizeof(pMsg.Id));

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

struct PMSG_TRADE_OTHER_DEL
{
	PBMSG_HEAD h;	// C1:38
	BYTE TradeItemIndex;	// 3
};

void GCTradeOtherDel(int aIndex, BYTE tradeindex)
{
	PMSG_TRADE_OTHER_DEL pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x38, sizeof(pMsg ));
	pMsg.TradeItemIndex = tradeindex;

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

struct PMSG_TRADE_OTHER_MOVE
{
	PBMSG_HEAD h;	// C1:39
	BYTE TradeItemIndex;	// 3
	BYTE ItemInfo[MAX_ITEM_INFO];	// 4
};

void GCTradeOtherAdd(int aIndex, BYTE tradeindex, LPBYTE iteminfo)
{
	PMSG_TRADE_OTHER_MOVE pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x39, sizeof(pMsg));
	pMsg.TradeItemIndex = tradeindex;
	pMsg.ItemInfo[I_TYPE] = iteminfo[I_TYPE];
	pMsg.ItemInfo[I_OPTION] = iteminfo[I_OPTION];
	pMsg.ItemInfo[I_DUR] = iteminfo[I_DUR];
	pMsg.ItemInfo[I_NOPTION] = iteminfo[I_NOPTION];
	pMsg.ItemInfo[I_SOPTION] = iteminfo[I_SOPTION];
	pMsg.ItemInfo[I_380OPTION] = iteminfo[I_380OPTION];
	pMsg.ItemInfo[I_JOHOPTION] = iteminfo[I_JOHOPTION];
	pMsg.ItemInfo[I_SOCKETOPT1] = iteminfo[I_SOCKETOPT1];
	pMsg.ItemInfo[I_SOCKETOPT2] = iteminfo[I_SOCKETOPT2];
	pMsg.ItemInfo[I_SOCKETOPT3] = iteminfo[I_SOCKETOPT3];
	pMsg.ItemInfo[I_SOCKETOPT4] = iteminfo[I_SOCKETOPT4];
	pMsg.ItemInfo[I_SOCKETOPT5] = iteminfo[I_SOCKETOPT5];
	
	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CGTradeMoneyRecv(PMSG_TRADE_GOLD * lpMsg, int aIndex)
{
	PMSG_DEFRESULT pMsg;
	int money = 0;
	int number;

	if ( !PacketCheckTime(&gObj[aIndex]))
		return;

	if ( !OBJMAX_RANGE(aIndex) )
	{
		CLog.LogAdd("error : %s %d (%d)", __FILE__, __LINE__, aIndex);
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 231)), gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	number = gObj[aIndex].TargetNumber;

	if ( !OBJMAX_RANGE(number) )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 232)), gObj[aIndex].AccountID, gObj[aIndex].Name, number);
		return;
	}

	if ( gObj[number].CloseType != -1 )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 233)), gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[number].AccountID, gObj[number].Name);
		return;
	}

	if ( gObj[aIndex].m_IfState.use == FALSE && gObj[aIndex].m_IfState.type != 1 )
		return;

	if ( ::gObjCanItemTouch(&gObj[aIndex], 1) == FALSE )
		return;

	if ( lpMsg->Money > 100000000 )
		return;

	money = lpMsg->Money;

	if ( (gObj[aIndex].Money - money) < 0 )
		return;

	GCMoneySend(aIndex, gObj[aIndex].Money - money);

	C1HeadSet((LPBYTE)&pMsg, 0x3A, sizeof(pMsg));
	pMsg.result = 1;

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	if ( number >= 0)
	{
		gObj[aIndex].TradeMoney = money;
		gObj[aIndex].TradeOk  = false;
		gObj[number].TradeOk  = false;

		GCTradeOkButtonSend(number, 2);
		GCTradeOkButtonSend(aIndex, 0);
		GCTradeMoneyOther(number, gObj[aIndex].TradeMoney);
	}
}

struct PMSG_TRADE_OTHER_GOLD
{
	PBMSG_HEAD h;	// C1:3B
	DWORD Money;	// 4
};

void GCTradeMoneyOther(int aIndex, DWORD money)
{
	PMSG_TRADE_OTHER_GOLD pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x3B, sizeof(pMsg));
	pMsg.Money = money;

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);	
}

void GCTradeOkButtonSend(int aIndex, BYTE flag)
{
	PMSG_TRADE_OKBUTTON pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x3C, sizeof(pMsg));
	pMsg.Flag = flag;

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CGTradeOkButtonRecv(PMSG_TRADE_OKBUTTON * lpMsg, int aIndex)
{
	int number = gObj[aIndex].TargetNumber;

	if ( number < 0 || number > OBJMAX-1 )
		return;

	if ( !gObjIsConnected(&gObj[number]))
		return;

	if ( gObj[aIndex].m_IfState.use < 1 || gObj[aIndex].m_IfState.state != 1 )
		return;

	if ( gObj[number].m_IfState.use < 1 || gObj[number].m_IfState.state != 1 )
		return;

	if ( gObj[aIndex].CloseType != -1 )
		return;

	if ( gObj[number].CloseType != -1 )
		return;
	
	if ( lpMsg->Flag == 1 )
	{
		if ( gObj[aIndex].TradeOk == false )
		{
			gObj[aIndex].TradeOk = true;
			CLog.LogAdd("[%s][%s] Trade Accept", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCTradeOkButtonSend(number, 1);
		}
	}
	else if ( lpMsg->Flag == 0 )
	{
		if ( gObj[aIndex].TradeOk  == true )
		{
			GCTradeOkButtonSend(number, 0);
			gObj[aIndex].TradeOk = false;
		}
	}

	gObjTradeOkButton(aIndex);
}

void CGTradeCancelButtonRecv(int aIndex)
{
	int number = gObj[aIndex].TargetNumber;
	gObjTradeCancel(aIndex);
	CGTradeResult(aIndex, 0);

	if ( number >= 0 )
	{
		gObjTradeCancel(number);
		CGTradeResult(number, 0);
	}
}

struct PMSG_TRADE_RESULT
{
	PBMSG_HEAD h;	// C1:3D
	BYTE Result;	// 3
};

void CGTradeResult(int aIndex , BYTE result )
{
	PMSG_TRADE_RESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x3D, sizeof(pMsg));
	pMsg.Result = result;

	gSendProto.DataSend(aIndex, (unsigned char*)&pMsg, pMsg.h.size);
}

struct PMSG_GUILDQUESTSEND
{
	PBMSG_HEAD h;	// C1:50
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
};

void CGGuildRequestRecv(PMSG_GUILDJOINQ * lpMsg, int aIndex)
{
	PMSG_GUILDQUESTSEND pMsg;
	int number = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( number < 0 || number > OBJMAX-1 )
		return;

	if ( !PacketCheckTime(&gObj[aIndex]))
		return;

#if(GS_CASTLE)
	
	if(g_CastleSiegeSync.GetCastleState() == 7)
	{
		MsgOutput(aIndex,(lMsg.Get(MSGGET(6, 194))));
		return;
	}

#endif

	if ( !gObjIsConnected(&gObj[number]))
	{
		GCResultSend(aIndex, 0x51, 0x03);
		return;
	}

	if ( gObj[aIndex].m_IfState.use > 0 )
	{
		GCResultSend(aIndex, 0x51, 0x06);
		return;
	}
	
	if ( gObj[number].m_IfState.use > 0 )
	{
		GCResultSend(aIndex, 0x51, 0x06);
		return;
	}

	if ( gObj[aIndex].Level < 6 )
	{
		GCResultSend(aIndex, 0x51, 0x07);
		return;
	}

	if ( gObj[aIndex].GuildNumber > 0 )
	{
		GCResultSend(aIndex, 0x51, 0x05);
		return;
	}

	if ( (gObj[number].m_Option&1 ) != 1 )
	{
		GCResultSend(aIndex, 0x51, 0x00);
		return;
	}

	if ( gObj[aIndex].GenFamily != gObj[number].GenFamily )
	{
		GCServerMsgStringSend("You can't join different Guild Gen Member",aIndex,1);
		return;
	}

	if ( gObj[number].GuildNumber > 0 )
	{
		if ( strcmp(gObj[number].lpGuild->Names[0], gObj[number].Name))
		{
			GCResultSend(aIndex, 0x51, 0x04);
			return;
		}

		if ( gObj[number].lpGuild->WarState )
		{
			GCResultSend(aIndex, 0x51, 0x06);
			return;
		}

		int MaxGuildMember;

		if ( gObj[number].Class == CLASS_DARKLORD )
			MaxGuildMember = gObj[number].Level / 10 + gObj[number].Leadership / 10;
		else
			MaxGuildMember = gObj[number].Level / 10;

		if ( MaxGuildMember > MAX_USER_GUILD )
			MaxGuildMember = MAX_USER_GUILD;

		if ( gObj[number].lpGuild->TotalCount >= MaxGuildMember )
		{
			GCResultSend(aIndex, 0x51, 0x02);
			return;
		}
	}
	else
	{
		GCResultSend(aIndex, 0x51, 0x04);
		return;
	}

	gObj[aIndex].m_IfState.use = 1;
	gObj[aIndex].m_IfState.type = 4;
	gObj[aIndex].m_IfState.state = 0;

	gObj[number].m_IfState.use = 1;
	gObj[number].m_IfState.type = 4;
	gObj[number].m_IfState.state = 0;

	gObj[aIndex].TargetNumber = number;
	gObj[number].TargetNumber = aIndex;
	gObj[aIndex].m_InterfaceTime = GetTickCount();
	gObj[number].m_InterfaceTime = GetTickCount();

	C1HeadSet((LPBYTE)&pMsg, 0x50, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);

	gSendProto.DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);

	CLog.Event("[%s][%s] Guild Request - Join Guild: %s", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[number].lpGuild->Name);
}

void CGGuildRequestResultRecv(PMSG_GUILDQRESULT * lpMsg, int aIndex)
{
	int number;
	BYTE result=0;
	int pnumber=-1;

	if ( !PacketCheckTime(&gObj[aIndex]))
		return;

	number = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( number < 0 || number > OBJMAX-1)
		return;

	if ( !gObjIsConnected(&gObj[number]))
	{
		GCResultSend(aIndex, 0x51, 0x02);
	}
	else
	{
		result = 1;
	}

	if ( lpMsg->Result == 0 )
	{
		result = 0;
		GCResultSend(number, 0x51, 0x00);
	}

	if ( result == 1 )
	{
		if ( gObj[aIndex].GuildNumber > 0 )
		{
			GDGuildMemberAdd(number, gObj[aIndex].lpGuild->Name, gObj[number].Name);
		}
	}

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 4)
		gObj[aIndex].m_IfState.use = 0;
		
	if ( gObj[number].m_IfState.use && gObj[number].m_IfState.type == 4)
		gObj[number].m_IfState.use = 0;
}

struct PMSG_GUILDLIST
{
	char Name[10];	// 0
	BYTE Number;	// A
	BYTE ConnectAServer;	// B
	BYTE btGuildStatus;	// C
};

struct PMSG_GUILDLISTCOUNT
{
	PWMSG_HEAD h;	// C2:52
	BYTE Result;	// 4
	BYTE Count;	// 5
	int TotalScore;	// 8
	BYTE Score;	// C
	char szRivalGuild[9];	// D
};

void CGGuildListAll(int pnumber)
{
	if ( gObj[pnumber].lpGuild == NULL )
		return;

	PMSG_GUILDLISTCOUNT pCount;
	PMSG_GUILDLIST pList;
	char sendbuf[1500];
	int lOfs = 0;
	int number;
	int n;
	int guildmaster = 0;
	
	C2HeadSet((LPBYTE)&pCount, 0x52, 0);
	pCount.Result = 0;
	pCount.Count = 0;
	lOfs += sizeof(pCount);
	pCount.h.sizeH = SET_NUMBERH(lOfs);
	pCount.h.sizeL = SET_NUMBERL(lOfs);

	if ( gObj[pnumber].GuildNumber < 1 )
	{
		gSendProto.DataSend(pnumber, (LPBYTE)&pCount, lOfs);
		return;
	}

	pCount.Count = gObj[pnumber].lpGuild->Count;
	pCount.TotalScore = gObj[pnumber].lpGuild->TotalScore;
	pCount.Score = gObj[pnumber].lpGuild->PlayScore;
	memset(pCount.szRivalGuild, 0, sizeof(pCount.szRivalGuild));

	if ( gObj[pnumber].lpGuild->iGuildRival )
		memcpy(pCount.szRivalGuild, gObj[pnumber].lpGuild->szGuildRivalName, MAX_GUILD_LEN);

	pCount.Result = 1;
	guildmaster = 1;

	if ( pCount.Count == 0 )
		return;

	pCount.Count = 0;

	for ( n=0;n<MAX_USER_GUILD;n++)
	{
		number = gObj[pnumber].lpGuild->Use[n];

		if ( number > 0 )
		{
			memset(&pList, 0, sizeof(pList));
			memcpy(pList.Name, gObj[pnumber].lpGuild->Names[n], sizeof(pList.Name));
			pList.Number = gObj[pnumber].lpGuild->pServer[n];
			pList.ConnectAServer = (short)gObj[pnumber].lpGuild->pServer[n] & 0x7F;

			if ( gObj[pnumber].lpGuild->pServer[n] >= 0 )
				pList.ConnectAServer |= 0x80;

			pList.btGuildStatus = gObj[pnumber].lpGuild->GuildStatus[n];
			memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
			lOfs+= sizeof(pList);
			pCount.Count++;
		}
	}

	pCount.h.sizeH = SET_NUMBERH(lOfs);
	pCount.h.sizeL = SET_NUMBERL(lOfs);
	memcpy(sendbuf, (LPBYTE)&pCount, sizeof(pCount));

	gSendProto.DataSend(pnumber, (LPBYTE)sendbuf, lOfs);
}

struct PMSG_GUILDDELUSER_RESULT
{
	PBMSG_HEAD h;	// C1:53
	BYTE Result;	// 3
};

void CGGuildDelUser(PMSG_GUILDDELUSER * lpMsg, int aIndex)
{
	if ( !PacketCheckTime(&gObj[aIndex]))
		return;

	if ( gObj[aIndex].GuildNumber < 1 )
		return;

	char joomin[14];
	char memberid[11];
	char guildname[11];
	PMSG_GUILDDELUSER_RESULT pMsg;
	
	C1HeadSet((LPBYTE)&pMsg, 0x53, sizeof(pMsg));
	pMsg.Result = 3;

	if ( !gGuildDestroy )
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	memset(memberid, 0, sizeof(memberid));
	memset(guildname, 0, sizeof(guildname));
	memcpy(memberid, lpMsg->Name, MAX_ACCOUNT_LEN);
	strcpy(guildname, gObj[aIndex].lpGuild->Name);

	if ( !strcmp(gObj[aIndex].lpGuild->Names[0], gObj[aIndex].Name ))
	{
		memset(joomin, 0, sizeof(joomin));
		memcpy(joomin, lpMsg->JoominNumber, 10);

		if ( gObjJoominCheck(aIndex, joomin) == TRUE )
		{
			if ( !strcmp(memberid, gObj[aIndex].Name ))
			{
				gObjGuildWarMasterClose(&gObj[aIndex]);
				GDGuildDestroySend(aIndex, gObj[aIndex].lpGuild->Name, gObj[aIndex].Name);
			}
			else
			{
				GDGuildMemberDel(aIndex, guildname, memberid);
			}

			CLog.Event("[%s][%s] Guild Request - Dismiss All : %s",
				gObj[aIndex].AccountID,gObj[aIndex].Name, guildname);
		}
		else
		{
			pMsg.Result = 0;
			gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}
	}
	else
	{
		if ( strcmp(memberid, gObj[aIndex].Name) )
		{
			pMsg.Result = 0;
			gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}

		memset(joomin, 0, sizeof(joomin));
		memcpy(joomin, lpMsg->JoominNumber, 10);

		if ( gObjJoominCheck(aIndex, joomin) == TRUE )
		{
			GDGuildMemberDel(aIndex, guildname, memberid);
		}
		else
		{
			GCServerMsgStringSend(lMsg.Get(MSGGET(4, 108)), aIndex, 1);
		}

		CLog.LogAdd("[%s][%s] Guild Request - Leave : %s",
			gObj[aIndex].AccountID, gObj[aIndex].Name, guildname);
	}
}

void GCGuildDelUserResult(int aIndex, BYTE Result)
{
	PMSG_GUILDDELUSER_RESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x53, sizeof(pMsg));
	pMsg.Result = Result;

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

struct PMSG_GUILDMASTERQUESTION
{
	PBMSG_HEAD h;	// C1:54
};

void GCGuildMasterQuestionSend(int aIndex)
{
	if ( gObj[aIndex].m_IfState.use > 0 )
	{
		return;
	}

	PMSG_GUILDMASTERQUESTION pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x54, sizeof(pMsg));
	gObj[aIndex].m_IfState.use = 1;
	gObj[aIndex].m_IfState.type = 5;
	gObj[aIndex].m_IfState.state = 0;

	gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}

void CGGuildMasterAnswerRecv(PMSG_GUILDMASTERANSWER * lpMsg, int aIndex)
{
	if ( lpMsg->Result == 1 )
	{
		if ( !gObjGuildMasterCapacity(&gObj[aIndex]))
		{
			if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
			{
				gObj[aIndex].m_IfState.use = 0;
			}

			return;
		}

		GCGuildMasterManagerRun(aIndex);
		return;
	}

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
	{
		gObj[aIndex].m_IfState.use = 0;
	}
}

struct PMSG_GUILDMASTERMANAGER_RUN
{
	PBMSG_HEAD h;	// C1:55
};

void GCGuildMasterManagerRun(int aIndex)
{
	PMSG_GUILDMASTERMANAGER_RUN pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x55, sizeof(pMsg));

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CGGuildMasterInfoSave(int aIndex, PMSG_GUILDINFOSAVE * lpMsg)
{
	char GuildName[MAX_GUILD_LEN+1];
	GuildName[MAX_GUILD_LEN] ='\0';
	memcpy(GuildName, lpMsg->GuildName, MAX_GUILD_LEN);
	int len = strlen(GuildName);

	if ( len <= 2 || len > MAX_GUILD_LEN)
	{
		PMSG_GUILDCREATED_RESULT pMsg;

		C1HeadSet((LPBYTE)&pMsg, 0x56, sizeof(pMsg));
		pMsg.Result = 2;
		
		gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
		{
			gObj[aIndex].m_IfState.use = 0;
		}

		return;
	}

	GDGuildCreateSend(aIndex, GuildName, gObj[aIndex].Name, lpMsg->Mark, lpMsg->btGuildType);

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
	{
		gObj[aIndex].m_IfState.use = 0;
	}
}

void CGGuildMasterCreateCancel(int aIndex)
{
	if ( gObj[aIndex].GuildNumber > 0 )
		return;

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
	{
		gObj[aIndex].m_IfState.use = 0;
	}
}

void GCGuildViewportNowPaint(int aIndex, char* guildname, BYTE* mark, BOOL isGuildMaster)
{
	_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild(guildname);

	if ( lpGuild == NULL )
		return;

	LPOBJ lpObj = &gObj[aIndex];
	BYTE _GuildInfoBuf[256] = {0};
	int _GuildInfoOfs = 5;

	if ( lpObj->lpGuild == NULL )
		return;


	PMSG_SIMPLE_GUILDVIEWPORT pMsg;
	pMsg.GuildNumber = lpObj->lpGuild->Number;
	pMsg.NumberH = SET_NUMBERH(aIndex)&0x7F;
	pMsg.NumberL = SET_NUMBERL(aIndex);

	if ( isGuildMaster != FALSE )
		pMsg.NumberH |= 0x80;


	if ( isGuildMaster != FALSE )
		pMsg.btGuildStatus = 0x80;
	else
		pMsg.btGuildStatus = lpObj->GuildStatus;

	pMsg.btGuildType = lpObj->lpGuild->btGuildType;
	pMsg.btGuildRelationShip = 0;
	lpObj->iGuildUnionTimeStamp = 0;
	memcpy(&_GuildInfoBuf[_GuildInfoOfs], &pMsg, sizeof(pMsg));
	_GuildInfoOfs += sizeof(pMsg);

	PMSG_SIMPLE_GUILDVIEWPORT_COUNT pGVCount;

	pGVCount.h.c = 0xC2;
	pGVCount.h.headcode = 0x65;
	pGVCount.h.sizeH = SET_NUMBERH(_GuildInfoOfs);
	pGVCount.h.sizeL = SET_NUMBERL(_GuildInfoOfs);
	pGVCount.Count = 1;
	memcpy(_GuildInfoBuf, &pGVCount, sizeof(pGVCount));

	if ( gUser.ChaosCastleMapRange(gObj[aIndex].MapNumber) == false )
	{
		gSendProto.DataSend(aIndex, _GuildInfoBuf, _GuildInfoOfs);
	}

	gSendProto.VPMsgSend(lpObj, _GuildInfoBuf, _GuildInfoOfs);
}

struct PMSG_GUILDDEL_VIEWPORT_NOW
{
	PBMSG_HEAD h;	// C1:5D
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
};

void GCGuildViewportDelNow(int aIndex, BOOL isGuildMaster)
{
	PMSG_GUILDDEL_VIEWPORT_NOW pMsg;

	LPOBJ lpObj = &gObj[aIndex];

	C1HeadSet((LPBYTE)&pMsg, 0x5D, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex) & 0x7F;
	pMsg.NumberL = SET_NUMBERL(aIndex);

	if ( isGuildMaster != FALSE )
	{
		pMsg.NumberH |= 0x80;
	}

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	gSendProto.VPMsgSend(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCManagerGuildWarEnd(char * GuildName)
{
	_GUILD_INFO_STRUCT * lpNode = Guild.SearchGuild(GuildName);

	if ( lpNode == NULL )
	{
		return;
	}

	int n=0;
	int warmaster = -1;

	while ( true )
	{
		if ( gObj[n].Type == OBJ_USER )
		{
			if ( gObj[n].Connected > PLAYER_LOGGED )
			{
				if ( gObj[n].Name[0] == lpNode->Names[0][0] )
				{
					if ( strcmp(gObj[n].Name, lpNode->Names[0]) == 0 )
					{
						warmaster = n;
						break;
					}
				}
			}
		}

		if ( n < OBJMAX-1 )
		{
			n++;
		}
		else
		{
			break;
		}
	}

	if ( warmaster >= 1 )
	{
		BYTE Result1 = 0;
		BYTE Result2 = 0;

		if ( lpNode != NULL )
		{
			if ( lpNode->lpTargetGuildNode != NULL )
			{
				if ( lpNode->PlayScore == 0 && lpNode->lpTargetGuildNode->PlayScore == 0 )
				{
					Result1 = 6;
					Result2 = 6;
				}
				else
				{
					Result1 = 0;
					Result2 = 1;
				}
			}
		}

		if ( lpNode->WarType == 1 )
		{
			gBattleGroundEnable(lpNode->BattleGroundIndex, FALSE );
			gObjGuildWarEndSend(lpNode, lpNode->lpTargetGuildNode, Result1, Result2);
			gObjGuildWarEnd(lpNode, lpNode->lpTargetGuildNode);
		}
		else
		{
			gObjGuildWarEndSend(lpNode, lpNode->lpTargetGuildNode, Result1, Result2);
			gObjGuildWarEnd(lpNode, lpNode->lpTargetGuildNode);
		}
	}
}

void GCManagerGuildWarSet(LPSTR GuildName1, LPSTR GuildName2, int type)
{
	_GUILD_INFO_STRUCT * lpNode = Guild.SearchGuild(GuildName1);

	if ( lpNode == NULL )
	{
		return;
	}

	int n=0;
	int warmaster = -1;

	while ( true )
	{
		if ( gObj[n].Type == OBJ_USER )
		{
			if ( gObj[n].Connected > PLAYER_LOGGED )
			{
				if ( gObj[n].Name[0] == lpNode->Names[0][0] )
				{
					if ( strcmp(gObj[n].Name, lpNode->Names[0]) == 0 )
					{
						warmaster = n;
						break;
					}
				}
			}
		}

		if ( n < OBJMAX-1 )
		{
			n++;
		}
		else
		{
			break;
		}
	}

	if ( warmaster >= 1 )
	{
		::GCGuildWarRequestResult(GuildName2, warmaster, type);
	}
}

struct PMSG_GUILDWARREQUEST_RESULT
{
	PBMSG_HEAD h;	// C1:60
	BYTE Result;	// 3
};

void GCGuildWarRequestResult(LPSTR GuildName, int aIndex, int type)
{
	PMSG_GUILDWARREQUEST_RESULT pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x60, sizeof(pMsg));
	pMsg.Result = 3;

	if ( gObj[aIndex].GuildNumber < 1)
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	_GUILD_INFO_STRUCT * lpMyGuild = gObj[aIndex].lpGuild;

	if ( !lpMyGuild )
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if ( lpMyGuild->WarState == 1 || lpMyGuild->WarDeclareState == 1 )
	{
		pMsg.Result = 4;

		//Error on declaring war : %d %d %s
		CLog.LogAdd(lMsg.Get(MSGGET(1, 234)), lpMyGuild->WarState,
			lpMyGuild->WarDeclareState, lpMyGuild->Name);
		gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		return;
	}

	if ( strcmp(lpMyGuild->Names[0], gObj[aIndex].Name ) )
	{
		pMsg.Result = 5;

		// Error on declaring war : 0x05 %s %s
		CLog.LogAdd(lMsg.Get(MSGGET(1, 235)),
			lpMyGuild->Names[0], gObj[aIndex].Name);
		gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		return;
	}

	char _guildname[MAX_GUILD_LEN+1];
	memset(_guildname, 0, sizeof(_guildname));
	memcpy(_guildname, GuildName, MAX_GUILD_LEN);

	if ( !strncmp(lpMyGuild->Name, GuildName, MAX_GUILD_LEN))
		return;

	_GUILD_INFO_STRUCT * lpNode = Guild.SearchGuild(_guildname);

	if ( lpNode )
	{
		if ( lpNode->WarState == 1 || lpNode->WarDeclareState == 1 )
		{
			pMsg.Result = 4;

			//Error on declaring war : %d %d %s
			CLog.LogAdd(lMsg.Get(MSGGET(1, 234)), lpNode->WarState,
				lpNode->WarDeclareState, lpNode->Name);
			gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

			return;
		}

		if ( lpMyGuild->iGuildUnion != 0 && lpMyGuild->iGuildUnion == lpNode->iGuildUnion )
		{
			CLog.LogAdd("[U.System][Guildwar] Can't declare guildwar between Same Union (%s / %d) <-> (%s / %d)",
				lpMyGuild->Name, lpMyGuild->iGuildUnion, lpNode->Name, lpMyGuild->iGuildUnion);

			return;
		}

		int n=0;	// #warning Change this 0 to OBJ_STARTUSERINDEX
		int warmaster=-1;

		while ( true )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				if ( gObj[n].Connected > PLAYER_LOGGED )
				{
					if ( gObj[n].Name[0] == lpNode->Names[0][0] )
					{
						if ( !strcmp(gObj[n].Name, lpNode->Names[0]))
						{

							warmaster = n;
							break;
						}
					}
				}
			}

			if ( n < OBJMAX-1 )
				n++;
			else
				break;
		}
		
		if ( warmaster >= 1 )
		{
			if ( gUser.ChaosCastleMapRange(gObj[aIndex].MapNumber) == true || gUser.ChaosCastleMapRange(gObj[warmaster].MapNumber) == true )
			{
				GCServerMsgStringSend(lMsg.Get(MSGGET(4, 199)), aIndex, 1);
				return;
			}

			if ( (gObj[warmaster].m_Option&1) != 1 )
			{
				pMsg.Result = 4;
				gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

				return;
			}

			pMsg.Result = 1;
			gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			GCGuildWarRequestSend(lpMyGuild->Name, warmaster, type);
			lpMyGuild->WarDeclareState = 1;
			lpNode->WarDeclareState = 1;
			lpMyGuild->WarType = type;
			lpNode->WarType = type;

			// Declared guild war [%s][%s] sGuild:(%s) TargetGuild:(%s)
			CLog.LogAdd(lMsg.Get(MSGGET(1, 236)), gObj[aIndex].AccountID, gObj[aIndex].Name,
				lpMyGuild->Name, lpNode->Name);

			strcpy(lpMyGuild->TargetGuildName, lpNode->Name);
			strcpy(lpNode->TargetGuildName, lpMyGuild->Name);
			lpMyGuild->lpTargetGuildNode = lpNode;
			lpNode->lpTargetGuildNode = lpMyGuild;
		}
		else
		{
			pMsg.Result = 2;
			gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

			//return;
		}
	}
	else
	{
		pMsg.Result = 0;
		gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		return;
	}
}

struct PMSG_GUILDWARSEND
{
	PBMSG_HEAD h;	// C1:61
	char GuildName[8];	// 3
	BYTE Type;	// B
};

void GCGuildWarRequestSend(LPSTR GuildName, int aIndex, int type)
{
	PMSG_GUILDWARSEND pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x61, sizeof(pMsg));
	pMsg.Type = type;
	memcpy(pMsg.GuildName, GuildName, MAX_GUILD_LEN);

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	// (%s) guild declared guild war
	CLog.LogAdd(lMsg.Get(MSGGET(1, 237)), GuildName);
}

struct PMSG_GUILDWAR_DECLARE
{
	PBMSG_HEAD h;	// C1:62
	char GuildName[8];	// 3
	BYTE Type;	// B
	BYTE TeamCode;	// C
};

void GCGuildWarRequestSendRecv(PMSG_GUILDWARSEND_RESULT * lpMsg, int aIndex)
{

	PMSG_GUILDWAR_DECLARE pMsg;
	int count=0;
	int g_call=0;
	
	C1HeadSet((LPBYTE)&pMsg, 0x62, sizeof(pMsg));
	pMsg.Type = 0;
	
	_GUILD_INFO_STRUCT * lpMyNode = gObj[aIndex].lpGuild;

	if ( !lpMyNode )
	{
		PMSG_GUILDWARREQUEST_RESULT pResult;

		pResult.h.c = 0xC1;
		pResult.h.headcode = 0x60;
		pResult.h.size = sizeof(pResult);
		pResult.Result = 0;

		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpMyNode->lpTargetGuildNode && lpMsg->Result )
	{
		int iTarGetIndex = lpMyNode->lpTargetGuildNode->Index[0];

		if ( gUser.BloodCastleMapRange(gObj[iTarGetIndex].MapNumber) == true ||
			 gUser.ChaosCastleMapRange(gObj[iTarGetIndex].MapNumber) == true ||
			 DS_MAP_RANGE(gObj[iTarGetIndex].MapNumber)  )
		{
			//PMSG_NOTICE pNotice;

			//TNotice::MakeNoticeMsgEx(&pNotice, 1, lMsg.Get(MSGGET(4, 199)));

			//gSendProto.DataSend(aIndex, (LPBYTE)&pNotice, pNotice.h.size);

			lpMsg->Result = 0;
		}
	}

	pMsg.Type = lpMyNode->WarType;

	if ( lpMsg->Result == 0 )
	{
		if ( lpMyNode->lpTargetGuildNode )
		{
			if ( lpMyNode->lpTargetGuildNode->WarDeclareState == 1 )
			{
				lpMyNode->lpTargetGuildNode->WarDeclareState = 0;
				lpMyNode->WarDeclareState = 0;

				PMSG_GUILDWARREQUEST_RESULT pResult;

				C1HeadSet((LPBYTE)&pResult, 0x60, sizeof(pResult));
				pResult.Result = 6;

				if ( lpMyNode->lpTargetGuildNode->Index[0] >= 0 )
				{
					gSendProto.DataSend(lpMyNode->lpTargetGuildNode->Index[0], (LPBYTE)&pResult, pResult.h.size);
				}
			}
		}
	}
	else //if ( lpMsg->Result == 1 )
	{
		if ( lpMyNode->WarDeclareState == 1 )
		{
			if ( lpMyNode->lpTargetGuildNode )
			{
				if (lpMyNode->lpTargetGuildNode->WarDeclareState == 1 )
				{
					PMSG_TELEPORT pTeleportMsg;

					if ( lpMyNode->WarType == 1 )
					{
						lpMyNode->BattleGroundIndex = gCheckBlankBattleGround();

						switch ( lpMyNode->BattleGroundIndex )
						{
							case 0xFF:
								lpMyNode->WarDeclareState = 0;
								lpMyNode->WarState = 0;
								lpMyNode->lpTargetGuildNode->WarDeclareState = 0;
								lpMyNode->lpTargetGuildNode->WarState = 0;

								PMSG_GUILDWARREQUEST_RESULT pResult;

								C1HeadSet((LPBYTE)&pResult, 0x60, sizeof(pResult));
								pResult.Result = 4;

								gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
								return;
						}
	
						pTeleportMsg.MoveNumber = 51;
						lpMyNode->lpTargetGuildNode->BattleGroundIndex = lpMyNode->BattleGroundIndex;

						if ( gBSGround[0]->m_BallIndex >= 0 )
							gObjMonsterRegen(&gObj[gBSGround[0]->m_BallIndex]);

						BattleSoccerGoalEnd(0);
						lpMyNode->PlayScore = 0;
						lpMyNode->lpTargetGuildNode->PlayScore = 0;
						gBattleGroundEnable(lpMyNode->BattleGroundIndex, TRUE);
						gSetBattleTeamMaster(lpMyNode->BattleGroundIndex, 0, lpMyNode->Name, lpMyNode);
						gSetBattleTeamMaster(lpMyNode->BattleGroundIndex, 1, lpMyNode->lpTargetGuildNode->Name, lpMyNode->lpTargetGuildNode);
					}

					lpMyNode->WarDeclareState = 2;
					lpMyNode->WarState = 1;
					lpMyNode->lpTargetGuildNode->WarDeclareState = 2;
					lpMyNode->lpTargetGuildNode->WarState = 1;
					lpMyNode->PlayScore = 0;
					lpMyNode->lpTargetGuildNode->PlayScore = 0;
					lpMyNode->BattleTeamCode = 0;
					lpMyNode->lpTargetGuildNode->BattleTeamCode = 1;
					memset(pMsg.GuildName, 0, sizeof(pMsg.GuildName));
					memcpy(pMsg.GuildName, lpMyNode->lpTargetGuildNode->Name, sizeof(pMsg.GuildName));
					pMsg.TeamCode = lpMyNode->BattleTeamCode;
					count = 0;

					for ( int n=0;n<MAX_USER_GUILD;n++)
					{
						if ( lpMyNode->Use[n] )
						{
							if ( lpMyNode->Index[n] >= 0 )
							{
								g_call = 0;

								if ( n > 0 )
								{
									if ( lpMyNode->WarType == 1 )
									{
										if ( gObj[lpMyNode->Index[0]].PartyNumber  >= 0 )
										{
											if ( gObj[lpMyNode->Index[0]].PartyNumber == gObj[lpMyNode->Index[n]].PartyNumber )
											{
												g_call = 1;
											}
										}
									}
									else 
									{
										g_call = 1;
									}
								}
								else if ( n== 0 )
								{
									g_call = 1;
								}

								if ( g_call )
								{
									gSendProto.DataSend(lpMyNode->Index[n], (LPBYTE)&pMsg, pMsg.h.size);

									// Declared guild war against (%s)
									CLog.LogAdd(lMsg.Get(MSGGET(1, 238)), lpMyNode->Name);
									GCGuildWarScore(lpMyNode->Index[n]);
									int x = 60;

									if ( lpMyNode->WarType == 1 )
									{
										gObj[lpMyNode->Index[n]].IsInBattleGround = true;
										pTeleportMsg.MoveNumber = 51;

										/*if ( gPkLimitFree || gObj[lpMyNode->Index[n]].m_PK_Level < 6 )
										{
											gObjTeleport(lpMyNode->Index[n], 6, x++, 153);
											count++;
										}*/
									}
								}
							}
						}
					}

					memset(pMsg.GuildName, 0, sizeof(pMsg.GuildName));
					memcpy(pMsg.GuildName, lpMyNode->Name, sizeof(pMsg.GuildName));

					pMsg.TeamCode = lpMyNode->lpTargetGuildNode->BattleTeamCode;
					count = 0;

					for ( int n=0;n<MAX_USER_GUILD;n++)
					{
						if ( lpMyNode->lpTargetGuildNode->Use[n] )
						{
							if ( lpMyNode->lpTargetGuildNode->Index[n] >= 0 )
							{
								g_call = 0;

								if ( n > 0 )
								{
									if ( lpMyNode->WarType == 1 )
									{
										if ( gObj[lpMyNode->lpTargetGuildNode->Index[0]].PartyNumber  >= 0 )
										{
											if ( gObj[lpMyNode->lpTargetGuildNode->Index[0]].PartyNumber == gObj[lpMyNode->lpTargetGuildNode->Index[n]].PartyNumber )
											{
												g_call = 1;
											}
										}
									}
									else 
									{
										g_call = 1;
									}
								}
								else if ( n== 0 )
								{
									g_call = 1;
								}

								if ( g_call )
								{
									gSendProto.DataSend(lpMyNode->lpTargetGuildNode->Index[n], (LPBYTE)&pMsg, pMsg.h.size);

									// Declared guild war against (%s)
									CLog.LogAdd(lMsg.Get(MSGGET(1, 238)), lpMyNode->lpTargetGuildNode->Name);
									GCGuildWarScore(lpMyNode->lpTargetGuildNode->Index[n]);
									int x = 59;

									if ( lpMyNode->lpTargetGuildNode->WarType == 1 )
									{

										/*if ( gPkLimitFree != 0 || gObj[lpMyNode->lpTargetGuildNode->Index[n]].m_PK_Level < 6 )
										{
											gObj[lpMyNode->lpTargetGuildNode->Index[n]].IsInBattleGround = true;
											gObjTeleport(lpMyNode->lpTargetGuildNode->Index[n], 6, x++, 164);
											count++;
										}*/
									}
								}
							}
						}
					}

					if ( lpMyNode->WarType == 1 )
					{
						gObjAddMsgSendDelay(&gObj[aIndex], 5, aIndex, 10000, 0);
						GCServerMsgStringSendGuild(lpMyNode->lpTargetGuildNode,lMsg.Get(MSGGET(4, 130)), 1);
						GCServerMsgStringSendGuild(lpMyNode,lMsg.Get(MSGGET(4, 130)), 1);
					}
				}
			}
		}
	}
}

void GCGuildWarDeclare(int aIndex, LPSTR _guildname)
{
	PMSG_GUILDWAR_DECLARE pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x62, sizeof(pMsg));
	memcpy(pMsg.GuildName, _guildname, sizeof(pMsg.GuildName));

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

struct PMSG_GUILDWAR_END
{
	PBMSG_HEAD h;	// C1:63
	BYTE Result;	// 3
	char GuildName[8];	// 4
};

void GCGuildWarEnd(int aIndex, BYTE result, char* _guildname)
{
	PMSG_GUILDWAR_END pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x63, sizeof(pMsg));
	pMsg.Result = result;
	memcpy(pMsg.GuildName, _guildname, sizeof(pMsg.GuildName));

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

struct PMSG_GUILDSCORE
{
	PBMSG_HEAD h;	// C1:64
	BYTE Score1;	// 3
	BYTE Score2;	// 4
	BYTE Type;	// 5
};

void GCGuildWarScore(int aIndex)
{
	if ( gObj[aIndex].GuildNumber < 1 )
		return;

	PMSG_GUILDSCORE pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x64, sizeof(pMsg));
	pMsg.Score1 = 0;
	pMsg.Score2 = 0;
	pMsg.Type = 0;

	if ( gObj[aIndex].lpGuild != NULL )
	{
		pMsg.Score1 = gObj[aIndex].lpGuild->PlayScore;
		CLog.LogAdd("Score %s %d", gObj[aIndex].Name, gObj[aIndex].lpGuild->PlayScore);
	}

	if ( gObj[aIndex].lpGuild->lpTargetGuildNode != NULL )
	{
		pMsg.Score2 = gObj[aIndex].lpGuild->lpTargetGuildNode->PlayScore;
		CLog.LogAdd("Target Score %s %d", gObj[aIndex].Name, gObj[aIndex].lpGuild->lpTargetGuildNode->PlayScore);
	}

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CGWarehouseMoneyInOut(int aIndex, PMSG_WAREHOUSEMONEYINOUT* lpMsg)
{
	int money = 0;

	if ( !gObjIsConnected(aIndex))
	{
		CLog.LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	money = lpMsg->Money;
	
	if ( lpObj->m_IfState.state == 0 )
	{
		GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
		return;
	}

	if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 6 )
	{
		GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
		CLog.LogAdd("[%s][%s] error-L1 : used not Warehouse",
			gObj[aIndex].AccountID, gObj[aIndex].Name);

		return;
	}

	switch ( lpMsg->Type )
	{
		case 0x00:
			if ( money <= 0 || money > MAX_WAREHOUSE_ZEN )
			{
				GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
				return;
			}

			if ( (lpObj->WarehouseMoney+money) > MAX_WAREHOUSE_ZEN )
			{
				GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
				return;
			}

			if ( money > lpObj->Money )
			{
				GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
				return;
			}

			lpObj->Money-=money;
			lpObj->WarehouseMoney+=money;
			break;
		case 0x01:
			{
				if ( bCanWarehouseLock == TRUE)
				{
					if ( lpObj->WarehouseLock == 1)
					{
						GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
						GCServerMsgStringSend(lMsg.Get(MSGGET(6, 70)), lpObj->m_Index,1);
						return;
					}
				}

				if ( money <= 0 || money > MAX_WAREHOUSE_ZEN )
				{
					GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
					return;
				}
				
				if ( money > lpObj->WarehouseMoney )
				{
					GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
					return;
				}

				lpObj->WarehouseMoney -= money;
				lpObj->Money += money;
				int rZen = GetWarehouseUsedHowMuch(lpObj->Level, lpObj->WarehousePW);

				if ( (lpObj->Money-rZen) > 0 )
				{
					int oldmoney = lpObj->Money;
					lpObj->Money -= rZen;
					GCMoneySend(lpObj->m_Index, lpObj->Money);
					
					CLog.LogAdd("Get WareHouse Money(In Inventory) : %d - %d = %d",
						oldmoney, rZen, lpObj->Money);
				}
				else if ( (lpObj->WarehouseMoney-rZen) > 0 )
				{
					int oldmoney = lpObj->WarehouseMoney;
					lpObj->WarehouseMoney -= rZen;

					CLog.LogAdd("Get WareHouse Money(In WareHouse) : %d - %d = %d",
						oldmoney, rZen, lpObj->WarehouseMoney);

					GCWarehouseInventoryMoneySend(aIndex, 1, lpObj->Money, lpObj->WarehouseMoney);
				}
				else
				{
					lpObj->WarehouseMoney +=money;
					lpObj->Money -= money;
					GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
					char msg[255];

					wsprintf(msg, lMsg.Get(MSGGET(6, 69)), rZen);
					GCServerMsgStringSend(msg, lpObj->m_Index, 1);
					return;
				}
			}
			break;

		default:
			GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
			return;
	}

	lpObj->WarehouseCount++;
	GCWarehouseInventoryMoneySend(aIndex, 1, lpObj->Money, lpObj->WarehouseMoney);
}

struct PMSG_MONEY
{
	PBMSG_HEAD h;	// C1:81
	BYTE Result;	// 3
	int wMoney;	// 4
	int iMoney;	// 8
};

void GCWarehouseInventoryMoneySend(int aIndex, BYTE result, int money, int wmoney)
{
	PMSG_MONEY pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x81, sizeof(pMsg));
	pMsg.Result = result;
	pMsg.iMoney = money;
	pMsg.wMoney = wmoney;

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CGWarehouseUseEnd(int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		CLog.LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	PMSG_DEFAULT pMsg;
	C1HeadSet((LPBYTE)&pMsg, 0x82, sizeof(pMsg));
	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	gObjItemTextSave(lpObj);
	GDSetWarehouseList(aIndex);
	GDUserItemSave(lpObj);

	if ( lpObj->m_IfState.use  && lpObj->m_IfState.type == 6 )
	{
		lpObj->m_IfState.use = 0;
		lpObj->m_IfState.state = 0;
		lpObj->WarehouseSave = FALSE;
	}
}

struct PMSG_WAREHOUSESTATE
{
	PBMSG_HEAD h;	// C1:83
	BYTE State;	// 3
};

void GCWarehouseStateSend(int aIndex, BYTE state)
{
	PMSG_WAREHOUSESTATE pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x83, sizeof(pMsg));
	pMsg.State = state;

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GCWarehouseRecivePassword(int aIndex, PMSG_WAREHOUSEPASSSEND* lpMsg)
{
	if ( bCanWarehouseLock == FALSE )
		return;

	int pw = lpMsg->Pass;

	switch ( lpMsg->Type )
	{
		case 0x00:
			if ( gObj[aIndex].WarehouseLock )
			{
				if ( gObj[aIndex].WarehousePW == pw )
				{
					gObj[aIndex].WarehouseLock = FALSE;
					GCWarehouseStateSend(aIndex, 0x0C);
				}
				else
				{
					GCWarehouseStateSend(aIndex, 0x0A);
				}
			}
			break;
		case 0x01:
			if ( !gObj[aIndex].WarehouseLock || gObj[aIndex].WarehouseLock == 0xFF )
			{
				char szJoomin[11];

				memset(szJoomin, 0, sizeof(szJoomin));
				memcpy(szJoomin, lpMsg->LastJoominNumber, 10);

				if ( gObjJoominCheck(aIndex, szJoomin) == FALSE )
				{
					GCWarehouseStateSend(aIndex, 0x0D);
					return;
				}

				gObj[aIndex].WarehousePW = pw;
				gObj[aIndex].WarehouseLock = FALSE;
				GCWarehouseStateSend(aIndex, 0x0C);
			}
			else
			{
				GCWarehouseStateSend(aIndex, 0x0B);
			}
			break;
		case 0x02:
			{
				char szJoomin[11];

				memset(szJoomin, 0, sizeof(szJoomin));
				memcpy(szJoomin, lpMsg->LastJoominNumber, 10);

				if ( gObjJoominCheck(aIndex, szJoomin) == FALSE )
				{
					GCWarehouseStateSend(aIndex, 0x0D);
					return;
				}

				gObj[aIndex].WarehouseLock = FALSE;
				gObj[aIndex].WarehousePW = 0;
				GCWarehouseStateSend(aIndex, gObj[aIndex].WarehouseLock);
			}
			break;
	}
}	

void GCUserChaosBoxSend(LPOBJ lpObj, int iChaosBoxType)
{
	if ( iChaosBoxType == 1  )
	{
		if ( lpObj->m_IfState.type != 13 )
		{
			return;
		}

		if ( lpObj->m_IfState.type == 13 && lpObj->m_IfState.state == 1 )
		{
			return;
		}
	}
	else 
	{
		if ( lpObj->m_IfState.type != 7 )
		{
			return;
		}

		if ( lpObj->m_IfState.type == 7 && lpObj->m_IfState.state == 1 )
		{
			return;
		}
	}

	PMSG_SHOPITEMCOUNT pMsg;
	BYTE SendByte[2048];
	int lOfs = sizeof(pMsg);
	C2HeadSet((LPBYTE)&pMsg, 0x31, 0);
	pMsg.count = 0;

	if ( iChaosBoxType == 1 )
	{
		pMsg.Type = 5;
	}
	else
	{
		pMsg.Type = 3;
	}

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			SendByte[lOfs] = n;
			lOfs++;
			ItemByteConvert(&SendByte[lOfs], lpObj->pChaosBox[n] );
			lOfs += MAX_ITEM_INFO;
			pMsg.count ++;
		}
	}

	pMsg.h.sizeH = SET_NUMBERH(lOfs);
	pMsg.h.sizeL = SET_NUMBERL(lOfs);
	memcpy(SendByte, &pMsg, sizeof(pMsg));

	gSendProto.DataSend(lpObj->m_Index, SendByte, lOfs);
}

void CGChaosBoxItemMixButtonClick(PMSG_CHAOSMIX* aRecv, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		CLog.LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->ChaosLock == TRUE )
	{
		CLog.LogAdd("[%s][%s] Already Used Chaos", lpObj->AccountID, lpObj->Name);
		GCServerMsgStringSend(lMsg.Get(MSGGET(2, 198)), lpObj->m_Index, 1);
		return;
	}

	if ( lpObj->m_bPShopOpen == true )
	{
		CLog.LogAdd("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
		GCServerMsgStringSend(lMsg.Get(MSGGET(4, 194)), lpObj->m_Index, 1);
		return;
	}

	lpObj->bIsChaosMixCompleted = true;
	int iMixType = aRecv->Type;

	switch ( iMixType )
	{
		case CHAOS_TYPE_DEVILSQUARE: // Devil Square Ticket Mix
			DevilSquareItemChaosMix(lpObj);
			break;
		case CHAOS_TYPE_UPGRADE_10: // Upgrade +10 Items Mix
			PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_10);
			break;
		case CHAOS_TYPE_UPGRADE_11: // Upgrade +11 Items Mix
			PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_11);
			break;
		case CHAOS_TYPE_UPGRADE_12: // Upgrade +12 Items Mix
			PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_12);
			break;
		case CHAOS_TYPE_UPGRADE_13: // Upgrade +13 Items Mix
			PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_13);
			break;
		case CHAOS_TYPE_UPGRADE_14: // Upgrade +14 Items Mix
			PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_14);
			break;
		case CHAOS_TYPE_UPGRADE_15: // Upgrade +15 Items Mix
			PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_15);
			break;
		case CHAOS_TYPE_DINORANT: // Dinorant Mix
			PegasiaChaosMix(lpObj);
			break;
		case CHAOS_TYPE_FRUIT: // Fruit Mix
			CircleChaosMix(lpObj);
			break;
		case CHAOS_TYPE_SECOND_WING: // Second Wings Mix
		case CHAOS_TYPE_CLOAK: // Cloaks Mix
			WingChaosMix(lpObj);
			break;
		case CHAOS_TYPE_BLOODCATLE: // Blood Castle Ticket Mix
			BloodCastleItemChaosMix(lpObj);
			break;
		case CHAOS_TYPE_DEFAULT:
		case CHAOS_TYPE_FIRST_WING:
			DefaultChaosMix(lpObj);
			break;
		//case CHAOS_TYPE_SETITEM:	// #warning Activate this to SetItemChaosMix
			//SetItemChaosMix(lpObj);
			//break;
		case CHAOS_TYPE_DARKHORSE: // Dark Horse Mix
			DarkHorseChaosMix(lpObj);
			break;
		case CHAOS_TYPE_DARKSPIRIT: // Dark Spirit Mix
			DarkSpiritChaosMix(lpObj);
			break;
		case CHAOS_TYPE_BLESS_POTION: // Bless Potion Mix
			BlessPotionChaosMix(lpObj);
			break;
		case CHAOS_TYPE_SOUL_POTION: // Soul Potion Mix
			SoulPotionChaosMix(lpObj);
			break;
		case CHAOS_TYPE_LIFE_STONE:
			LifeStoneChaosMix(lpObj);
			break;
#if(GS_CASTLE)
		case CHAOS_TYPE_CASTLE_ITEM:
			CastleSpecialItemMix(lpObj);
			break;
#endif
		case CHAOS_TYPE_HT_BOX:
			HiddenTreasureBoxItemMix(lpObj);
			break;
		case CHAOS_TYPE_FENRIR_01:
			Fenrir_01Level_Mix(lpObj);
			break;
		case CHAOS_TYPE_FENRIR_02:
			Fenrir_02Level_Mix(lpObj);
			break;
		case CHAOS_TYPE_FENRIR_03:
			Fenrir_03Level_Mix(lpObj);
			break;
		case CHAOS_TYPE_FENRIR_04:
			Fenrir_04Upgrade_Mix(lpObj);
			break;
		case CHAOS_TYPE_COMPOUNDPOTION_LV1:
			ShieldPotionLv1_Mix(lpObj);
			break;
		case CHAOS_TYPE_COMPOUNTPOTION_LV2:
			ShieldPotionLv2_Mix(lpObj);
			break;
		case CHAOS_TYPE_COMPOUNTPOTION_LV3:
			ShieldPotionLv3_Mix(lpObj);
			break;
		case CHAOS_TYPE_JEWELOFHARMONY_PURITY:
			g_kJewelOfHarmonySystem.PurityJewelOfHarmony(lpObj);
			break;
		case CHAOS_TYPE_JEWELOFHARMONY_MIX_SMELTINGITEM:
			g_kJewelOfHarmonySystem.MakeSmeltingStoneItem(lpObj);
			break;
		case CHAOS_TYPE_JEWELOFHARMONY_RESTORE_ITEM:
			g_kJewelOfHarmonySystem.RestoreStrengthenItem(lpObj);
			break;
		case CHAOS_TYPE_380_OPTIONITEM:
			g_Item380System.ChaosMix380ItemOption(lpObj);
			break;
		case CHAOS_TYPE_LOTTERY_MIX:
#if(SERVER_COUNTRY == INTERNATIONAL_JAPAN)
			LotteryItemMix(lpObj);
#elif (SERVER_COUNTRY == INTERNATIONAL_KOREA)
			IllusionTempleItemChaosMix(lpObj);
#endif
			break;
		case CHAOS_TYPE_CONDOR_FEATHER:
			CondorFeatherMix(lpObj);
			break;
		case CHAOS_TYPE_THIRD_WING:
			WingsLv3ChaosMix(lpObj);
			break;
		case CHAOS_CARD_MIX:
			gChaosCard.ChaosCardMix(lpObj);
			break;
		case CHERRY_BLOSSOM_MIX:
			CherryBlossomMix(lpObj);
			break;
		case SEED_TYPE_SEEDEXTRACT:
			gItemSocket.SeedExtract(lpObj);
			break;
		case SEED_TYPE_SEEDSPHERE:
			gItemSocket.SeedSphere(lpObj);
			break;
		case SEED_TYPE_SEEDCALC:
			gItemSocket.SeedCalc(lpObj, aRecv->SeedType);
			break;
		case SEED_TYPE_NEWMIX2:
			gItemSocket.NewMix4(lpObj, aRecv->SeedType);
			break;
		case CHAOS_TYPE_EMPIRE_GUARDIAN_BOOK:
			SecromiconMix(lpObj);
			break;
		case CHAOS_TYPE_SEALED_BOX:
			SealedBoxMix(lpObj);
			break;
		default:
			CLog.LogAdd("[%s][%s] Undefine chaosmix type detect %d", lpObj->AccountID, lpObj->Name, iMixType);
			break;
	}
}

void CGChaosBoxUseEnd(int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		CLog.LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	PMSG_DEFAULT pMsg;
	C1HeadSet((LPBYTE)&pMsg, 0x87, sizeof(pMsg));
	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	if ( lpObj->m_IfState.use && lpObj->m_IfState.type == 7 )
	{
		lpObj->m_IfState.use = 0;
		lpObj->m_IfState.state = 0;
	}

	gChaosBox.ChaosBoxInit(lpObj);
	gObjInventoryCommit(lpObj->m_Index);
	lpObj->m_bIsCastleNPCUpgradeCompleted = false;
}

void CGAttack(PMSG_ATTACK* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj;
	int usernumber = (lpMsg->NumberH*256) + lpMsg->NumberL;

	if ( usernumber < 0 || usernumber > OBJMAX-1)
	{
		CLog.LogAdd("[CGAttack] [UserIndex Error] :%s %d %d", __FILE__, __LINE__, usernumber);
		return;
	}

	lpTargetObj = &gObj[usernumber];

	if ( !gObj[usernumber].Live )
	{
		return;
	}

	/*int iTimeCalc = GetTickCount() - lpObj->m_LastAttackTime;

	if ( iTimeCalc < lpObj->m_DetectSpeedHackTime && iTimeCalc && lpObj->HaveWeaponInHand )
	{
		lpObj->m_DetectCount++;
		lpObj->m_SumLastAttackTime += iTimeCalc;

		if ( lpObj->m_DetectCount > gHackCheckCount )
		{
			lpObj->m_DetectedHackKickCount++;
			lpObj->m_SpeedHackPenalty = gSpeedHackPenalty;
			
			if ( gIsKickDetecHackCountLimit )
			{
				if ( lpObj->m_DetectedHackKickCount > gDetectedHackKickCount )
				{
					CLog.LogAdd("[%s][%s] %s Kick DetecHackCountLimit Over User (%d)", lpObj->AccountID, lpObj->Name, lMsg.Get(MSGGET(7, 108)+lpObj->Class), lpObj->m_DetectedHackKickCount);
					CloseClient(aIndex);
					return;
				}
			}
			CLog.LogAdd("[%s][%s] %s Attack Speed Is Wrong Normal (%d)(%d) Penalty %d", lpObj->AccountID, lpObj->Name, lMsg.Get(MSGGET(7, 108)+lpObj->Class), lpObj->m_DetectSpeedHackTime, lpObj->m_SumLastAttackTime/lpObj->m_DetectCount, lpObj->m_SpeedHackPenalty);
		}

		lpObj->m_LastAttackTime = GetTickCount();
	}
	else
	{
		lpObj->m_SumLastAttackTime = 0;
		lpObj->m_DetectCount = 0;
	}

	lpObj->m_LastAttackTime = GetTickCount();*/

	/*if ( bIsIgnorePacketSpeedHackDetect )
	{
		if ( lpObj->m_SpeedHackPenalty > 0 )
		{
			lpObj->m_SpeedHackPenalty--;
			
			CLog.LogAdd("[%s][%s] %s Apply Attack Speed Penalty (%d left)", lpObj->AccountID, lpObj->Name, lMsg.Get(MSGGET(7, 108)+lpObj->Class), lpObj->m_SpeedHackPenalty);
			return;
		}
	}*/
	
	lpObj->Dir = lpMsg->DirDis;
	GCActionSend(lpObj, lpMsg->AttackAction, aIndex, usernumber);
	gComboAttack.CheckCombo(aIndex, 0);
	gObjAttack(lpObj, lpTargetObj, 0, 0, 0, 0, 0);
	lpObj->UseMagicNumber = 0;
}

void GCDamageSend(int aIndex, int TargetIndex, int AttackDamage, int MSBFlag, int MSBDamage, int iShieldDamage)
{
	PMSG_ATTACKRESULT pResult;

	C1HeadSet((LPBYTE)&pResult, PMSG_ATTACK_HEADCODE, sizeof(pResult));
	pResult.NumberH = SET_NUMBERH(TargetIndex);
	pResult.NumberL = SET_NUMBERL(TargetIndex);
	pResult.DamageH = SET_NUMBERH(AttackDamage);
	pResult.DamageL = SET_NUMBERL(AttackDamage);
	pResult.btShieldDamageH = SET_NUMBERH(iShieldDamage);
	pResult.btShieldDamageL = SET_NUMBERL(iShieldDamage);

	if ( MSBFlag != FALSE )
	{
		pResult.NumberH &= 0x7F;
		pResult.NumberH |= 0x80;
	}

	pResult.DamageType = MSBDamage;

	if ( gObj[TargetIndex].Type == OBJ_USER )
	{
		gSendProto.DataSend(TargetIndex, (LPBYTE)&pResult, pResult.h.size);
	}

	if ( gObj[aIndex].Type == OBJ_USER )
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	}

}

struct PMSG_DAMAGE
{
	PBMSG_HEAD h;	// C1:F3:07
	BYTE subcode;	// 3
	BYTE DamageH;	// 4
	BYTE DamageL;	// 5
	BYTE btShieldDamageH;	// 6
	BYTE btShieldDamageL;	// 7
};

void GCDamageSend(int aIndex, int damage, int iShieldDamage)
{
	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}


	PMSG_DAMAGE pDamage;

	C1SubHeadSet((LPBYTE)&pDamage, 0xF3, 0x07, sizeof(pDamage));
	pDamage.DamageH = SET_NUMBERH(damage);
	pDamage.DamageL = SET_NUMBERL(damage);
	pDamage.btShieldDamageH = SET_NUMBERH(iShieldDamage);
	pDamage.btShieldDamageL = SET_NUMBERL(iShieldDamage);

	gSendProto.DataSend(aIndex, (LPBYTE)&pDamage, pDamage.h.size);
}

struct PMSG_KILLPLAYER
{
	PBMSG_HEAD h;
	unsigned char NumberH;
	unsigned char NumberL;
	unsigned char ExpH;
	unsigned char ExpL;
	unsigned char DamageH;
	unsigned char DamageL;
};

void GCKillPlayerExpSend(int aIndex, int TargetIndex, int exp, int AttackDamage, BOOL MSBFlag)
{
	PMSG_KILLPLAYER pkillMsg;

	C3HeadSet((LPBYTE)&pkillMsg, 0x16, sizeof(pkillMsg));
	pkillMsg.NumberH = SET_NUMBERH(TargetIndex);
	pkillMsg.NumberL = SET_NUMBERL(TargetIndex);
	pkillMsg.ExpH = SET_NUMBERH(exp);
	pkillMsg.ExpL = SET_NUMBERL(exp);
	pkillMsg.DamageH = SET_NUMBERH(AttackDamage);
	pkillMsg.DamageL = SET_NUMBERL(AttackDamage);

	if ( MSBFlag != FALSE )
	{
		pkillMsg.NumberH &= 0x7F;
		pkillMsg.NumberH |= 0x80;
	}

	if (  gUser.BloodCastleMapRange(gObj[aIndex].MapNumber) == true )
	{
		g_BloodCastle.AddExperience(aIndex, exp);
	}

	gSendProto.DataSend(aIndex, (UCHAR*)&pkillMsg, pkillMsg.h.size);
}

struct PMSG_DIEPLAYER
{
	PBMSG_HEAD h;	// C1:17
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE SkillH;	// 5
	BYTE SkillL;	// 5
	BYTE KillerNumberH;	// 6
	BYTE KillerNumberL;	// 7
};

void GCDiePlayerSend(LPOBJ lpObj, int TargetIndex, WORD skill, int KillerIndex)
{
	PMSG_DIEPLAYER pDieMsg;

	C1HeadSet((LPBYTE)&pDieMsg, 0x17, sizeof(pDieMsg));
	pDieMsg.NumberH = SET_NUMBERH(TargetIndex);
	pDieMsg.NumberL = SET_NUMBERL(TargetIndex);
	pDieMsg.SkillH = SET_NUMBERH(skill);
	pDieMsg.SkillL = SET_NUMBERL(skill);
	pDieMsg.KillerNumberH = SET_NUMBERH(KillerIndex);
	pDieMsg.KillerNumberL = SET_NUMBERL(KillerIndex);
	int iSize = pDieMsg.h.size;

	gSendProto.VPMsgSend(lpObj, (LPBYTE)&pDieMsg, iSize);

	if ( lpObj->Type == OBJ_USER )
	{
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pDieMsg, iSize);
	}
}

void GCActionSend(LPOBJ lpObj, BYTE ActionNumber, int aIndex, int aTargetIndex)
{
	PMSG_ACTIONRESULT pActionResult;

	C1HeadSet((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
	pActionResult.NumberH = SET_NUMBERH(aIndex);
	pActionResult.NumberL = SET_NUMBERL(aIndex);
	pActionResult.ActionNumber = ActionNumber;
	pActionResult.Dir = lpObj->Dir;
	pActionResult.TargetNumberH = SET_NUMBERH(aTargetIndex);
	pActionResult.TargetNumberL = SET_NUMBERL(aTargetIndex);

	gSendProto.VPMsgSend(lpObj, (LPBYTE)&pActionResult, pActionResult.h.size);
}

void CGActionRecv(PMSG_ACTION * lpMsg, int aIndex)
{
	if ( !OBJMAX_RANGE(aIndex))
	{
		CLog.LogAdd("error : move protocol index error %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_ACTIONRESULT pResult;
	LPOBJ lpObj = &gObj[aIndex];

	C1HeadSet((LPBYTE)&pResult, 0x18, sizeof(pResult));
	pResult.ActionNumber = lpMsg->ActionNumber;
	pResult.NumberH = SET_NUMBERH(aIndex);
	pResult.NumberL = SET_NUMBERL(aIndex);
	lpObj->m_ActionNumber = lpMsg->ActionNumber;
	lpObj->Dir = lpMsg->Dir;
	pResult.Dir = lpObj->Dir;
	pResult.TargetNumberH = lpMsg->iTargetIndexH;
	pResult.TargetNumberL = lpMsg->iTargetIndexL;

	switch ( lpObj->m_ActionNumber )
	{
	case 0x80:
		lpObj->m_ViewState = 2;
		lpObj->m_Rest = lpObj->m_ActionNumber;
		break;
	case 0x81:
		lpObj->m_ViewState = 3;
		lpObj->m_Rest = lpObj->m_ActionNumber;
		break;
	case 0x82:
		lpObj->m_ViewState = 4;
		lpObj->m_Rest = lpObj->m_ActionNumber;
		break;
	}
	
	int MVL = MAX_VIEWPORT;

	if ( lpObj->Type == OBJ_MONSTER )
		MVL = MAX_VIEWPORT_MONSTER;

	for (int n=0;n<MVL;n++)
	{
		if ( lpObj->VpPlayer2[n].type == OBJ_USER )
		{
			if ( lpObj->VpPlayer2[n].state != 0 && lpObj->VpPlayer2[n].state != 0x10 && lpObj->VpPlayer2[n].state != 0x08)
			{
				gSendProto.DataSend(lpObj->VpPlayer2[n].number, (LPBYTE)&pResult, pResult.h.size);
			}
		}
	}
}

void CGMagicAttack(PMSG_MAGICATTACK* lpMsg, int aIndex)
{
	LPOBJ lpObj;
	LPOBJ lpTargetObj;
	int usernumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( usernumber < 0 || usernumber > OBJMAX-1 )
	{
		CLog.LogAdd("[CGMagicAttack]: Index Error -> File: %s, Line: %d, Index: %d", __FILE__, __LINE__, usernumber);
		return;
	}

	if ( gObj[usernumber].Live == 0 || gObj[usernumber].Connected  < PLAYER_LOGGED )
	{
		return;
	}

	if ( gObj[aIndex].CloseCount >= 0 )
	{
		return;
	}

	lpObj = &gObj[aIndex];
	lpTargetObj = &gObj[usernumber];

	int iMagicNumber = MAKE_NUMBERW(lpMsg->MagicNumberH, lpMsg->MagicNumberL);

	CMagicInf * lpMagic;

	if ( iMagicNumber != 263 )
	{	
		int attackret = gObjCheckAttackArea(aIndex, usernumber);

		if ( attackret )
		{
			CLog.LogAdd("[CGMagicAttack]: [%s][%s] Try Attack In Not Attack Area [Protocol] (%s:%d,%d) errortype = %d",lpObj->AccountID, lpObj->Name, lMsg.Get(MSGGET(7, 208)+lpObj->MapNumber), lpObj->X, lpObj->Y,attackret);

			if ( lpObj->Class == 275 )	// Kundun
				lpObj->TargetNumber = -1;

			/*if ( bIsIgnorePacketSpeedHackDetect )
				return;*/
		}
	}

	if ( lpObj->Type == OBJ_USER )
	{
		lpMagic = gObjGetMagicSearch(lpObj, iMagicNumber);

		if ( !lpMagic )
			return;

		if ( lpMagic->m_Skill < 300 )
		{
			if ( MagicDamageC.SkillGetRequireClass(lpObj->m_Index, lpMagic->m_Skill) < 1 )
				return;

			if ( MagicDamageC.CheckStatus(lpMagic->m_Skill, lpObj->GuildStatus) == 0 )
			{
				CLog.LogAddC(2, "[0x19] CGMagicAttack() - Invalid Status");
				return;
			}

			if ( MagicDamageC.CheckKillCount(lpMagic->m_Skill, lpObj->m_btKillCount) < 0 )
			{
				CLog.LogAddC(2, "[0x19] CGMagicAttack() - Invalid KillCount");
				return;
			}
		}
		else
		{
			if ( gSkillTree.MasterSkillClassCheck(lpObj,lpMagic->m_Skill) == false )
				return;
		}
	}
	else
	{
		lpMagic = gObjGetMagic(lpObj, iMagicNumber);

		if ( !lpMagic )
			return;
	}

	if ( lpMagic->m_Skill != 24 && gObjUseSkill.SpeedHackCheck(aIndex) == false )
	{
		return;
	}

	if ( gCheckSkillDistance(aIndex, usernumber, lpMagic->m_Skill) == false )
	{
		DWORD dwTick = GetTickCount() - lpObj->m_dwSkillDistanceErrorTick;
		
		// VERIFICAR
		if ( (int)dwTick > (g_iSkillDiatanceKickCheckTime*1000) )
		{
			lpObj->m_iSkillDistanceErrorCount = 0;
			lpObj->m_dwSkillDistanceErrorTick = GetTickCount();
		}

		lpObj->m_iSkillDistanceErrorCount++;

		if ( lpObj->m_iSkillDistanceErrorCount > g_iSkillDistanceKickCount )
		{
			if ( g_iSkillDistanceKick )
			{
				CLog.LogAdd("[SKILL DISTANCE CHECK] [%s][%s] Kick Invalid Skill Area User. count(%d)",lpObj->AccountID, lpObj->Name, lpObj->m_iSkillDistanceErrorCount);

				gNet.CloseClient(lpObj->m_Index);
				return;
			}

			lpObj->m_iSkillDistanceErrorCount=0;
		}

		return;
	}
	
	gObjUseSkill.UseSkill(aIndex, usernumber, lpMagic);
}

void  GCMagicAttackNumberSend(LPOBJ lpObj, WORD MagicNumber, int usernumber,  unsigned char skillsuccess)
{
	PMSG_MAGICATTACK_RESULT pAttack;

	C3HeadSet((LPBYTE)&pAttack, 0x19, sizeof(pAttack));
	
	pAttack.MagicNumberH = HIBYTE(MagicNumber);
	pAttack.MagicNumberL = LOBYTE(MagicNumber);
	pAttack.SourceNumberH = HIBYTE(lpObj->m_Index);
	pAttack.SourceNumberL = LOBYTE(lpObj->m_Index);
	pAttack.TargetNumberH = HIBYTE(usernumber);
	pAttack.TargetNumberL = LOBYTE(usernumber);
	pAttack.TargetNumberH &= 0x7F;

	if ( skillsuccess )
		pAttack.TargetNumberH |= 0x80;

	if ( lpObj->Type == OBJ_USER )
		gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pAttack, pAttack.h.size);

	if ( gUser.ChaosCastleMapRange(lpObj->MapNumber) == true )
	{
		switch ( MagicNumber )
		{
			case 16:
			case 26:
			case 27:
			case 28:
			case 48:
			case 64:
			case 217:
			case 218:
			case 221:
			case 222:
			case 233:
			case 234:
			case 266:
			case 267:
			case 268:
			case 435:
			case 436:
			case 437:
			case 438:
			case 439:
			case 470:
			case 471:
			case 472:
			case 473:
			case 474:
			case 475:
			case 476:
			case 477:
			case 478:
			case 479:
			case 480:
			case 481:
			case 482:
			case 483:
			case 484:
			case 485:
			case 486:
			case 487:
			case 488:
			case 489:
				return;
		}
	}
	gSendProto.VPMsgSend(lpObj, (LPBYTE)&pAttack, pAttack.h.size);
}

void GCMagicCancelSend(LPOBJ lpObj, WORD MagicNumber)
{
	PMSG_MAGICCANCEL pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x1B, sizeof(pMsg));
	pMsg.MagicNumber = (BYTE)MagicNumber;
	pMsg.NumberH = HIBYTE(lpObj->m_Index);
	pMsg.NumberL = LOBYTE(lpObj->m_Index);

	if ( lpObj->Type == OBJ_USER )
	{
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
	}

	gSendProto.VPMsgSend(lpObj, (UCHAR*)&pMsg, pMsg.h.size);
}

struct PMSG_USEEFFECTITEM
{
	PBMSG_HEAD h;	// C1:2D
	WORD wOptionType;	// 4
	WORD wEffectType;	// 6
	BYTE byEffectOption;	// 8
	int iLeftTime;	// C
};

void GCUseEffectItem(LPOBJ lpObj, int iEffectUseOption, int iOptionType, int iEffectType, int iLeftTime)
{
	PMSG_USEEFFECTITEM pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x2D, sizeof(pMsg));
	pMsg.byEffectOption = iEffectUseOption;
	pMsg.wOptionType = iOptionType;
	pMsg.wEffectType = iEffectType;
	pMsg.iLeftTime = iLeftTime;

	if ( lpObj->Type == OBJ_USER )
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
}

void CGMagicCancel(PMSG_MAGICCANCEL * lpMsg, int aIndex)
{
	if ( !OBJMAX_RANGE(aIndex))
		return;

	LPOBJ lpObj = &gObj[aIndex];



	if ( lpMsg->MagicNumber == 77 )
	{
		if ( lpObj->m_iMuseElfInfinityArrowSkillTime > 0 )
		{
			lpObj->m_iMuseElfInfinityArrowSkillTime = 0;

			GCStateInfoSend(lpObj, 0, eVS_INFINITY_ARROW);
		}
	}
}

struct PMSG_USE_MONSTERSKILL
{
	PBMSG_HEAD h;	// C1:69
	BYTE btMonsterSkillNumber;	// 3
	WORD wObjIndex;	// 4
	WORD wTargetObjIndex;	// 6
};

void GCUseMonsterSkillSend(LPOBJ lpObj, LPOBJ lpTargetObj, int iSkillNumber)
{	
	PMSG_USE_MONSTERSKILL pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x69, sizeof(pMsg));
	pMsg.btMonsterSkillNumber = iSkillNumber;
	pMsg.wObjIndex = lpObj->m_Index;
	pMsg.wTargetObjIndex = lpTargetObj->m_Index;

	if ( lpObj->Type == OBJ_USER )
		gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

	gSendProto.VPMsgSend(lpObj, (UCHAR*)&pMsg, pMsg.h.size);
}

struct PMSG_VIEWSKILLSTATE
{
	PBMSG_HEAD h;	// C1:07
	BYTE State;	// 3
	BYTE NumberH;	// 8
	BYTE NumberL;	// 9
	BYTE SkillIndex;
};

void GCStateInfoSend(OBJECTSTRUCT* lpObj, BYTE state, BYTE ViewSkillState, BOOL Send, BOOL JustToViewport)
{ 
	
	lpObj->m_ViewSkillState[ViewSkillState] = state;

	if(!Send) return;

	PMSG_VIEWSKILLSTATE pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x07, sizeof(pMsg));
	pMsg.State = state;
	pMsg.SkillIndex = ViewSkillState;
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);

	if(!JustToViewport)
	{
		if ( lpObj->Type == OBJ_USER )
			gSendProto.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
	}

	if ( gUser.ChaosCastleMapRange(lpObj->MapNumber) == true )
	{
		return;
	}

	gSendProto.VPMsgSend(lpObj, (UCHAR*)&pMsg, pMsg.h.size);
}

void CGBeattackRecv(unsigned char* lpRecv, int aIndex, int magic_send)
{
	// Set the lpRecv to a Predeterminated Packet
	PMSG_BEATTACK_COUNT * lpCount = (PMSG_BEATTACK_COUNT *)lpRecv;

	// Check the Protocol
	if ( lpCount->h.headcode != PMSG_BEATTACK_HEADCODE )
	{
		CLog.LogAdd("[CGBeattackRecv]: Head code different than 0x10");
		return;
	}

	// Check if the count is leess than 1
	if ( lpCount->Count < 1 )
	{
		CLog.LogAdd("[CGBeattackRecv]: Count less than 1");
		return;
	}

	// Set the Max limitation to 5
	if ( lpCount->Count > 5 )
	{
		lpCount->Count = 5;
	}

	// Elf FiveShot Skill *added by kimdu
	int skill = (lpCount->MagicNumberH*256)+lpCount->MagicNumberL;

	// Set the Start of the Offset
	int lOfs = sizeof(PMSG_BEATTACK_COUNT);
	CMagicInf * lpMagic;
	int tNumber;
	PMSG_BEATTACK * lpMsg;

	if ( gObj[aIndex].Type == OBJ_USER )
	{
		if(skill == 0)
		{
			skill = 235;
		}
		lpMagic = gObjGetMagicSearch(&gObj[aIndex], skill);
	}
	else
	{
		lpMagic = gObjGetMagic(&gObj[aIndex], skill);
	}

	// Check if there is Magic
	if ( lpMagic == NULL )
	{
		CLog.LogAdd("[CGBeattackRecv] Magic doesn't exists");
		return;
	}

	// Avoid use Skill of the following types
	if ( lpMagic->m_Skill == 42 || lpMagic->m_Skill == 43 || lpMagic->m_Skill == 41 || (lpMagic->m_Skill >= 495 && lpMagic->m_Skill <= 499 ) || (lpMagic->m_Skill >= 455 && lpMagic->m_Skill <= 469 ) )	
	{
		return;
	}

	if ( gObj[aIndex].Type == OBJ_USER )
	{
		if ( lpMagic->m_Skill < 300 )
		{
			if ( MagicDamageC.SkillGetRequireClass(aIndex, lpMagic->m_Skill) < 1 )
			{
				CLog.LogAddC(2, "[CGBeattackRecv] [%s][%s] not allowed to use skill %d", gObj[aIndex].AccountID, gObj[aIndex].Name, lpMagic->m_Skill);
				gObjUserKill(aIndex);
				return;
			}
		}
		else
		{
			if ( gSkillTree.MasterSkillClassCheck(&gObj[aIndex],lpMagic->m_Skill) == false )
			{
				CLog.LogAddC(2, "[CGBeattackRecv] [%s][%s] not allowed to use skill %d", gObj[aIndex].AccountID, gObj[aIndex].Name, lpMagic->m_Skill);
				return;
			}
		}
		
		// Check the time cast Spell
		// No longer that 8 seconds
		if ( (GetTickCount()- gObj[aIndex].UseMagicTime) > 8000 )
		{
			CLog.LogAddC(2, "Too long time passed after casting magic. [%s][%s] (%d)(%d)", gObj[aIndex].AccountID, gObj[aIndex].Name, lpMagic->m_Skill, GetTickCount() - gObj[aIndex].UseMagicTime);
			return;
		}

		// Check if user cast to much Spells
		// Npo longer thant 4
		if ( gObj[aIndex].UseMagicCount > 4 )
		{
			CLog.LogAddC(2, "Magic Count is more than 4. [%s][%s] (%d)", gObj[aIndex].AccountID, gObj[aIndex].Name, lpMagic->m_Skill,gObj[aIndex].UseMagicCount);
			return;
		}
		gObj[aIndex].UseMagicCount++;
	}


	// Avoid Attack out of the predeterminated
	// Magic need to be one of the following
	//	
	//	8	"Twister"
	//	9	"Evil Spirit"
	//	10	"Hellfire"
	//	13	"Blast"
	//	14	"Inferno"
	//	5	"Flame"
	//	24	"Triple Shot"
	//	50	"Flame of Evil(Monster)"
	//	12	"Aquaflash"
	//	41	"Twisting Slash"
	//	47	"Impale"
	//	43	"Death Stab"
	//	42	"Rageful Blow"
	//	52	"Penetration"
	//	55	"Fire Slash"
	//	78	"Fire Scream"
	// 235  "Five Shot"
	
	if (	 lpMagic->m_Skill != 8
		  && lpMagic->m_Skill != 9
		  && lpMagic->m_Skill != 445
		  && lpMagic->m_Skill != 446
		  && lpMagic->m_Skill != 447
		  && lpMagic->m_Skill != 448
		  && lpMagic->m_Skill != 449
		  && lpMagic->m_Skill != 530
		  && lpMagic->m_Skill != 531
		  && lpMagic->m_Skill != 532
		  && lpMagic->m_Skill != 533
		  && lpMagic->m_Skill != 534
		  && lpMagic->m_Skill != 10
		  && lpMagic->m_Skill != 440
		  && lpMagic->m_Skill != 441
		  && lpMagic->m_Skill != 442
		  && lpMagic->m_Skill != 443
		  && lpMagic->m_Skill != 444
		  && lpMagic->m_Skill != 13
		  && lpMagic->m_Skill != 510
		  && lpMagic->m_Skill != 511
		  && lpMagic->m_Skill != 512
		  && lpMagic->m_Skill != 513
		  && lpMagic->m_Skill != 514
		  && lpMagic->m_Skill != 14
		  && lpMagic->m_Skill != 5
		  && lpMagic->m_Skill != 24
		  && lpMagic->m_Skill != 50
		  && lpMagic->m_Skill != 12
		  && lpMagic->m_Skill != 41
		  && lpMagic->m_Skill != 47
		  && lpMagic->m_Skill != 43
		  && lpMagic->m_Skill != 42
		  && lpMagic->m_Skill != 52
		  && lpMagic->m_Skill != 55
		  && lpMagic->m_Skill != 78
		  && lpMagic->m_Skill != 235
		  && lpMagic->m_Skill != 264 )
	{
		CLog.LogAdd("[CGBeattackRecv]: Skill Index [%d] not allowed",lpMagic->m_Skill);
		return;
	}

	/*if ( lpMagic->m_Skill == 24 || lpMagic->m_Skill == 78 )
	{
		int NSAttackSerial = lpCount->Serial;

		if ( gObj[aIndex].OSAttackSerial >= 255 && (gObj[aIndex].OSAttackSerial - lpCount->Serial) > 50 )
		{
			gObj[aIndex].OSAttackSerial = -1;
		}
	
		if ( NSAttackSerial > gObj[aIndex].OSAttackSerial )
		{
			gObj[aIndex].OSAttackSerial = NSAttackSerial;
			LPOBJ lpObj = &gObj[aIndex];
			int iTimeCalc = GetTickCount() - lpObj->m_LastAttackTime;

			if ( iTimeCalc < lpObj->m_DetectSpeedHackTime && iTimeCalc )
			{
				lpObj->m_DetectCount++;
				lpObj->m_SumLastAttackTime += iTimeCalc;

				if ( lpObj->m_DetectCount > gHackCheckCount )
				{
					lpObj->m_DetectedHackKickCount++;

					if ( gIsKickDetecHackCountLimit )
					{
						if ( lpObj->m_DetectedHackKickCount > gDetectedHackKickCount )
						{
							CLog.LogAdd("[%s][%s] %s Kick DetecHackCountLimit Over User (%d)", lpObj->AccountID, lpObj->Name, lMsg.Get(MSGGET(7, 108)+lpObj->Class), lpObj->m_DetectedHackKickCount);
							CloseClient(aIndex);
							return;
						}
					}
					CLog.LogAdd("[%s][%s] %s Attack Speed Is Wrong Magic3 (%d)(%d) Penalty %d", lpObj->AccountID, lpObj->Name, lMsg.Get(MSGGET(7, 108)+lpObj->Class), lpObj->m_DetectSpeedHackTime,lpObj->m_SumLastAttackTime/lpObj->m_DetectCount, lpObj->m_SpeedHackPenalty);
				}

				lpObj->m_LastAttackTime = GetTickCount();
			}
			else
			{
				lpObj->m_SumLastAttackTime = 0;
				lpObj->m_DetectCount = 0;
			}

			lpObj->m_LastAttackTime = GetTickCount();
		}
	}*/

	/*if ( bIsIgnorePacketSpeedHackDetect )
	{
		LPOBJ lpObj = &gObj[aIndex];

		if ( lpObj->m_SpeedHackPenalty > 0 )
		{
			lpObj->m_SpeedHackPenalty--;
			CLog.LogAdd("[%s][%s] %s Apply Attack Speed Penalty (%d left)", lpObj->AccountID, lpObj->Name, lMsg.Get(MSGGET(7, 108)+lpObj->Class), lpObj->m_SpeedHackPenalty);
			return;
		}
	}*/
	
	int lOfs2 = lOfs;
	int pTargetNumber[128];
	
	for (int i=0;i<lpCount->Count;i++)
	{
		lpMsg = (PMSG_BEATTACK *)&lpRecv[lOfs2];
		pTargetNumber[i] = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
		lOfs2 += sizeof(PMSG_BEATTACK);
	}

	for (int n=0;n<lpCount->Count;n++)
	{
		lpMsg = (PMSG_BEATTACK *)&lpRecv[lOfs];
		tNumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
		LPOBJ lpObj = &gObj[aIndex];

		if ( lpMsg->MagicKey  && ( lpMagic->m_Skill == 9 || (lpMagic->m_Skill >= 445 && lpMagic->m_Skill >= 449 ) || (lpMagic->m_Skill >= 530 && lpMagic->m_Skill >= 534 ) ) ) // Evil Spirit
		{
			if ( lpObj->DurMagicKeyChecker.IsValidDurationTime(lpMsg->MagicKey) == FALSE )
			{
				CLog.LogAddC(0, "[CGBeattackRecv] Invalid DurationTime Key = %d ( Time : %d) [%s][%s]", lpMsg->MagicKey, lpObj->DurMagicKeyChecker.GetValidDurationTime(lpMsg->MagicKey), lpObj->AccountID, lpObj->Name); 
				lOfs += sizeof(PMSG_BEATTACK);
				continue;
			}
			
			if ( lpObj->DurMagicKeyChecker.IsValidCount(lpMsg->MagicKey) == FALSE )
			{
				CLog.LogAddC(0, "[CGBeattackRecv] Invalid Count = %d ( Count : %d) [%s][%s]", lpMsg->MagicKey, lpObj->DurMagicKeyChecker.GetValidCount(lpMsg->MagicKey), lpObj->AccountID, lpObj->Name); 
				lOfs += sizeof(PMSG_BEATTACK);
				continue;
			}
		}

		CLog.LogAdd("[CGBeattackRecv]: Magic Attack:%d, Serial:%d, Target:%d, Count:%d", lpMagic->m_Skill, lpCount->Serial, tNumber, lpCount->Count);

		if ( gEnableCheckPenetrationSkill )
		{
			if ( lpMagic->m_Skill == 78 || lpMagic->m_Skill == 52 )
			{
				if ( gMultiAttackHackCheck[gObj[aIndex].m_Index].CheckFireScreamSkill(tNumber, lpMagic->m_Skill, lpCount->Serial) == FALSE )
				{
					return;
				}
			}
			else if ( gMultiAttackHackCheck[gObj[aIndex].m_Index].CheckPenetrationSkill(tNumber, lpMagic->m_Skill, lpCount->Serial)== FALSE )
			{
				return;
			}
		}

		/*for (int i=0;i<lpCount->Count;i++)
		{
			if ( n != i)
			{
				if ( pTargetNumber[i] == tNumber )
				{
					CLog.LogAdd("[%s][%s] %s Detect Hack : Multi Attack", gObj[aIndex].AccountID, gObj[aIndex].Name, lMsg.Get(MSGGET(7, 108)+gObj[aIndex].Class));
					CloseClient(gObj[aIndex].m_Index);
					return;
				}
			}
		}*/

		if ( tNumber >= 0 && tNumber < OBJMAX-1 )
		{
			if ( lpMagic->m_Skill == 55 ) // Fire Slash
			{
				gObjUseSkill.MaGumSkillDefenseDown(aIndex, tNumber, lpMagic->m_Level);
				gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, TRUE, 1, 0, FALSE);
			}
			else if ( lpMagic->m_Skill == 78 ) // Fire Scream
			{
				gObjUseSkill.SkillFireScream(aIndex, tNumber, lpMagic);
			}
			else if ( magic_send )
			{
				gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, TRUE, 1, 0, FALSE);
			}
			else
			{
				gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, FALSE, 1, 0, FALSE);
			}
		}

		lOfs += sizeof(PMSG_BEATTACK);
	}
}

void CGDurationMagicRecv(PMSG_DURATION_MAGIC_RECV* lpMsg, int aIndex)
{
	CMagicInf * lpMagic;
	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Teleport )
	{
		return;
	}

	if ( gObj[aIndex].Type == OBJ_USER )
	{
		lpMagic = gObjGetMagicSearch(lpObj, MAKE_NUMBERW(lpMsg->MagicNumberH, lpMsg->MagicNumberL));
	}
	else
	{
		lpMagic = gObjGetMagic( lpObj, MAKE_NUMBERW(lpMsg->MagicNumberH, lpMsg->MagicNumberL));
	}

	if ( lpMagic == NULL )
	{
		return;
	}

	if ( lpObj->Type == OBJ_USER )
	{
		if ( lpMagic->m_Skill < 300 )
		{
			if ( MagicDamageC.CheckStatus(lpMagic->m_Skill, lpObj->GuildStatus) == FALSE )
			{
				CLog.LogAddC(2, "[0x1E] CGDurationMagicRecv() - Invalid Status");	// #warning maybe protocol here change
				return;
			}

			if ( MagicDamageC.CheckKillCount(lpMagic->m_Skill, lpObj->m_btKillCount) < 0 )
			{
				CLog.LogAddC(2, "[0x1E] CGDurationMagicRecv() - Invalid KillCount");
				return;
			}
		}
	}

	lpObj->UseMagicNumber = 0;

	/*if ( lpMagic->m_Skill != 24 )
	{
		if ( !gObjUseSkill.SpeedHackCheck(aIndex))
		{
			return;
		}
	}*/

	if ( !gObjUseSkill.EnableSkill(lpMagic->m_Skill) )
	{
		return;
	}

	int aTargetIndex = lpMsg->NumberH*256 + lpMsg->NumberL;
	
	gObjUseSkill.UseSkill(aIndex, lpMagic,lpMsg->X, lpMsg->Y, lpMsg->Dir, lpMsg->TargetPos, aTargetIndex);

	if ( lpMsg->MagicKey )
	{
		lpObj->DurMagicKeyChecker.SetDurationTime(lpMsg->MagicKey, GetTickCount());
	}
}

struct PMSG_USERMAPJOIN
{
	PBMSG_HEAD h;	// C1:F3:03
	BYTE subcode;	// 3
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
	BYTE X;	// 6
	BYTE Y;	// 7
	BYTE MapNumber;	// 8
	BYTE Dir;	// 9
};

void ObjectMapJoinPositionSend(short aIndex)	// Not Used
{
	PMSG_USERMAPJOIN pObject;
	
	C1SubHeadSet((LPBYTE)&pObject, 0xF3, 0x03, sizeof(pObject));
	pObject.NumberH = SET_NUMBERH(aIndex);
	pObject.NumberL = SET_NUMBERL(aIndex);
	pObject.X = gObj[aIndex].X;
	pObject.Y = gObj[aIndex].Y;
	pObject.MapNumber = gObj[aIndex].MapNumber;
	pObject.Dir = gObj[aIndex].Dir;

	gSendProto.DataSend(aIndex, (LPBYTE)&pObject, pObject.h.size);
}

void CGUseItemRecv(PMSG_USEITEM* lpMsg, int aIndex)
{
	int pos;
	CItem * citem;
	int iItemUseType = lpMsg->btItemUseType;


	// Check User States
	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type != 3 )
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, TRUE, gObj[aIndex].iShield);
		CLog.LogAdd("[%s][%s] CGUseItemRecv()_If return %s %d", gObj[aIndex].AccountID, gObj[aIndex].Name, __FILE__, __LINE__);
		return;
	}

	// Check if User is exiting
	if ( gObj[aIndex].CloseType != -1 )
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, TRUE, gObj[aIndex].iShield);
		CLog.LogAdd("[%s][%s] CGUseItemRecv()_CloseType return %s %d", gObj[aIndex].AccountID, gObj[aIndex].Name, __FILE__, __LINE__);
		return;
	}

	// Fix Inventory Pointer
	if ( gObjFixInventoryPointer(aIndex) == false )
	{
		CLog.LogAdd("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
		return;
	}

	// Check if there is a Transaction
	// Dupe
	if ( gObj[aIndex].pTransaction == 1 )
	{
		CLog.LogAdd("[%s][%s] CGUseItemRecv() Failed : Transaction == 1, IF_TYPE : %d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
		return;
	}

	// Set Var Pos
	pos = lpMsg->inventoryPos;

	// Check if the pos is in bounds
	if ( pos > MAIN_INVENTORY_SIZE-1 )
	{
		// error : Location is over the range. %s/%d
		CLog.LogAdd(lMsg.Get(MSGGET(1, 241)), __FILE__, __LINE__);
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
		CLog.LogAdd("[%s][%s] CGUseItemRecv()_Inventory return %s %d", gObj[aIndex].AccountID,	gObj[aIndex].Name, __FILE__, __LINE__);
		return;
	}

	// check if the item have the same position as the new position
	if ( lpMsg->inventoryPos == lpMsg->invenrotyTarget )
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
		CLog.LogAdd("error-L1 : [%s][%s] CGUseItemRecv()_Pos return %s %d", gObj[aIndex].AccountID, gObj[aIndex].Name, __FILE__, __LINE__);
		return;
	}

	// Set Pointer
	citem = &gObj[aIndex].pInventory[pos];

	// The item is ITEM
	if ( citem->IsItem() )
	{
		// Check the item Serial
		if ( gObjInventorySearchSerialNumber(&gObj[aIndex], citem->GetNumber()) == false )
		{
			GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			CLog.LogAdd("error-L2 : CopyItem [%s][%s] return %s %d", gObj[aIndex].AccountID, gObj[aIndex].Name, __FILE__, __LINE__);
			return;
		}

		if ( citem->m_serial && !gObjCanItemTouch(&gObj[aIndex], 1) )
		{
			GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			return;
		}
		// ---------------------------------------------------------------------
		// HEALING POTIONS
		// ---------------------------------------------------------------------
		if ( citem->m_Type == ITEMGET(14,0) || // Apple
			 citem->m_Type == ITEMGET(14,1) || // Small Healing Potion
			 citem->m_Type == ITEMGET(14,2) || // Healing Potion
			 citem->m_Type == ITEMGET(14,3) || // Large Healing Potion
			 citem->m_Type == ITEMGET(14,70) ) // Elite Healing Potion
		{
			int tLife = (citem->m_Value*10) - (gObj[aIndex].Level*2);

			if ( tLife <  0 )
			{
				tLife = 0;
			}

			int nAddRate=0;

			switch ( citem->m_Type )
			{
				case ITEMGET(14,0):	// Apple
					nAddRate = 10;
					break;
				case ITEMGET(14,1):	// Small Healing Potion
					nAddRate = 20;
					break;
				case ITEMGET(14,2):	// Healing Potion
					nAddRate = 30;
					break;
				case ITEMGET(14,3):	// Large Healing Potion
					nAddRate = 40;
					break;
				case ITEMGET(14,70): // Elite Healing Potion
					nAddRate = 65;
					break;
			}

			if ( citem->m_Level == 1 )
			{
				nAddRate += 5;
			}

			tLife += ((int)gObj[aIndex].MaxLife * nAddRate) / 100;

			if ( citem->m_Type == ITEMGET(14,0) )
			{
				if ( citem->m_Level < 2 )
				{
					gObj[aIndex].FillLife += tLife;
					tLife = 0;
				}
			}

			/*if ( gObj[aIndex].FillLife > 0.0f )
			{
				
			}*/

			gObj[aIndex].Life += tLife; //gObj[aIndex].FillLife;

			if ( (gObj[aIndex].Life + tLife) > (gObj[aIndex].MaxLife+gObj[aIndex].AddLife) )
			{
				gObj[aIndex].Life = gObj[aIndex].MaxLife+gObj[aIndex].AddLife;
			}
			else
			{
				gObj[aIndex].Life += tLife;
			}

			/*if ( gObj[aIndex].Life > (gObj[aIndex].MaxLife ) )// +gObj[aIndex].AddLife) )
			{
				gObj[aIndex].Life = gObj[aIndex].MaxLife; //+ gObj[aIndex].AddLife;
				//gObj[aIndex].FillLife = 0;
			}*/

			GCReFillSend(gObj[aIndex].m_Index, gObj[aIndex].Life, 0xFF, FALSE, gObj[aIndex].iShield);

			//gObj[aIndex].FillLifeMax = tLife;
			//gObj[aIndex].FillLife = tLife;

			if ( citem->m_Type == ITEMGET(14,0) && citem->m_Level < 2 )
			{
				gObj[aIndex].FillLifeCount = 0;
			}
			else if ( citem->m_Level == 1 )
			{
				gObj[aIndex].FillLifeCount = 2;
			}
			else
			{
				gObj[aIndex].FillLifeCount = 3;
			}

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		// ---------------------------------------------------------------------
		// MANA POTIONS
		// ---------------------------------------------------------------------
		else if (citem->m_Type == ITEMGET(14,4) || // Small Mana Potion
				 citem->m_Type == ITEMGET(14,5) || // Mana Potion
				 citem->m_Type == ITEMGET(14,6) || // Large Mana Potion
				 citem->m_Type == ITEMGET(14,71) ) // Elite Mana Potion
		{
			int tMana = (citem->m_Value*10) - (gObj[aIndex].Level);

			if ( tMana < 0 )
			{
				tMana=0;
			}

			switch ( citem->m_Type )
			{
				case ITEMGET(14,4):	// Small Mana Potion
					tMana += ((int)(gObj[aIndex].MaxMana + gObj[aIndex].AddMana))*20/100; 
					break;
				case ITEMGET(14,5):	// Mana Potion
					tMana += ((int)(gObj[aIndex].MaxMana + gObj[aIndex].AddMana))*30/100; 
					break;
				case ITEMGET(14,6):	// Large Mana Potion
					tMana += ((int)(gObj[aIndex].MaxMana + gObj[aIndex].AddMana))*40/100; 
					break;
				case ITEMGET(14,71): // Elite Mana Potion
					tMana += ((int)(gObj[aIndex].MaxMana + gObj[aIndex].AddMana))*50/100; 
					break;
			}

			if ( (gObj[aIndex].Mana + tMana) > gObj[aIndex].MaxMana+gObj[aIndex].AddMana )
			{
				gObj[aIndex].Mana = gObj[aIndex].MaxMana+gObj[aIndex].AddMana;
			}
			else
			{
				gObj[aIndex].Mana += tMana;
			}

			/*gObj[aIndex].Mana += tMana;

			if ( gObj[aIndex].Mana > (gObj[aIndex].MaxMana+gObj[aIndex].AddMana-1.0f) )
			{
				gObj[aIndex].Mana = gObj[aIndex].MaxMana+gObj[aIndex].AddMana;
			}*/
			
			GCManaSend(aIndex, gObj[aIndex].Mana, 0xFF, 0, gObj[aIndex].BP);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		// ---------------------------------------------------------------------
		// SHIELD POTIONS
		// ---------------------------------------------------------------------
		else if ( citem->m_Type == ITEMGET(14,38) )	// Small Compound Potion 
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 5/100;
			int iHPGage = (int)((gObj[aIndex].MaxLife + gObj[aIndex].AddLife) * 10.0f / 100.0f );

			if ( gObj[aIndex].iShield < 0 )
			{
				gObj[aIndex].iShield = 0;
			}

			if ( gObj[aIndex].FillLife > 0.0f )
			{
				gObj[aIndex].Life += gObj[aIndex].FillLife;

				if ( gObj[aIndex].Life > (gObj[aIndex].MaxLife + gObj[aIndex].AddLife ) )
				{
					gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
				}
			}

			if ( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if (  gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield )  )
				{
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
				}
			}

			gObj[aIndex].FillLife = iHPGage;
			gObj[aIndex].FillLifeMax = iHPGage;
			gObj[aIndex].FillLifeCount = 4;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;
			gObj[aIndex].iFillShieldCount = 4;

			GCSendEffectInfo(aIndex, 3);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			CLog.LogAdd("[%s][%s]Use Compound Potion Lv1 - SD[%d] HP[%f] -> SD[%d] HP[%f]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].iShield, gObj[aIndex].Life, gObj[aIndex].iShield+iShieldGage, (double)(gObj[aIndex].Life+(float)iHPGage));
		}
		else if ( citem->m_Type == ITEMGET(14,39) )	// Compound Potion 
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 10/100;
			int iHPGage = (int)((gObj[aIndex].MaxLife + gObj[aIndex].AddLife) * 25.0f / 100.0f );

			if ( gObj[aIndex].iShield < 0 )
			{
				gObj[aIndex].iShield = 0;
			}

			if ( gObj[aIndex].FillLife > 0.0f )
			{
				gObj[aIndex].Life += gObj[aIndex].FillLife;

				if ( gObj[aIndex].Life > (gObj[aIndex].MaxLife + gObj[aIndex].AddLife ) )
				{
					gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
				}
			}

			if ( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if (  gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield )  )
				{
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
				}
			}

			gObj[aIndex].FillLife = iHPGage;
			gObj[aIndex].FillLifeMax = iHPGage;
			gObj[aIndex].FillLifeCount = 4;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;
			gObj[aIndex].iFillShieldCount = 4;

			GCSendEffectInfo(aIndex, 3);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			CLog.LogAdd("[%s][%s]Use Compound Potion Lv2 - SD[%d] HP[%f] -> SD[%d] HP[%f]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].iShield, gObj[aIndex].Life, gObj[aIndex].iShield+iShieldGage, (double)(gObj[aIndex].Life+(float)iHPGage));
		}
		else if ( citem->m_Type == ITEMGET(14,40) )	// Large Compound Potion 
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 20/100;
			int iHPGage = (int)((gObj[aIndex].MaxLife + gObj[aIndex].AddLife) * 45.0f / 100.0f );

			if ( gObj[aIndex].iShield < 0 )
			{
				gObj[aIndex].iShield = 0;
			}

			if ( gObj[aIndex].FillLife > 0.0f )
			{
				gObj[aIndex].Life += gObj[aIndex].FillLife;

				if ( gObj[aIndex].Life > (gObj[aIndex].MaxLife + gObj[aIndex].AddLife ) )
				{
					gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
				}
			}

			if ( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if (  gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield )  )
				{
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
				}
			}

			gObj[aIndex].FillLife = iHPGage;
			gObj[aIndex].FillLifeMax = iHPGage;
			gObj[aIndex].FillLifeCount = 4;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;
			gObj[aIndex].iFillShieldCount = 4;

			GCSendEffectInfo(aIndex, 3);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			CLog.LogAdd("[%s][%s]Use Compound Potion Lv3 - SD[%d] HP[%f] -> SD[%d] HP[%f]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].iShield, gObj[aIndex].Life, gObj[aIndex].iShield+iShieldGage, (double)(gObj[aIndex].Life+(float)iHPGage));
		}
		// ---------------------------------------------------------------------
		// SD POTIONS
		// ---------------------------------------------------------------------
		else if ( citem->m_Type == ITEMGET(14,35) ) // Small SD Potion
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 25 / 100;

			if ( gObj[aIndex].iShield < 0 )
			{
				gObj[aIndex].iShield = 0;
			}

			if ( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if ( gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield ) )
				{
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
				}
			}

			gObj[aIndex].iFillShieldCount = 2;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;

			GCSendEffectInfo(aIndex, 3);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,36) ) // SD Potion
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 35 / 100;

			if ( gObj[aIndex].iShield < 0 )
			{
				gObj[aIndex].iShield = 0;
			}

			if ( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if ( gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield ) )
				{
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
				}
			}

			gObj[aIndex].iFillShieldCount = 2;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;

			GCSendEffectInfo(aIndex, 3);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,37) ) // Large SD Potion
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 45 / 100;

			if ( gObj[aIndex].iShield < 0 )
			{
				gObj[aIndex].iShield = 0;
			}

			if ( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if ( gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield ) )
				{
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
				}
			}

			gObj[aIndex].iFillShieldCount = 2;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;

			GCSendEffectInfo(aIndex, 3);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,133) ) // Elite SD Potion
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 65 / 100;

			if ( gObj[aIndex].iShield < 0 )
			{
				gObj[aIndex].iShield = 0;
			}

			if ( gObj[aIndex].iFillShield > 0 )
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if ( gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield ) )
				{
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
				}
			}

			gObj[aIndex].iFillShieldCount = 2;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;

			GCSendEffectInfo(aIndex, 3);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		// ---------------------------------------------------------------------
		// ANTIDOTE POTIONS
		// ---------------------------------------------------------------------
		else if ( citem->m_Type == ITEMGET(14,8) ) // Antidote
		{
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			
			if ( gObj[aIndex].m_PoisonBeattackCount )
			{
				gObj[aIndex].m_PoisonType = 0;
				gObj[aIndex].m_PoisonBeattackCount = 0;
				gObj[aIndex].lpAttackObj = NULL;
				GCMagicCancelSend(&gObj[aIndex], 1);
			}

			if ( gObj[aIndex].m_ColdBeattackCount != 0 )
			{
				gObj[aIndex].m_ColdBeattackCount = 0;
				gObj[aIndex].DelayActionTime = 0;
				gObj[aIndex].DelayLevel = 0;
				gObj[aIndex].lpAttackObj = NULL;
				GCMagicCancelSend(&gObj[aIndex], 7);
			}
		}
		// ---------------------------------------------------------------------
		// JACK OLANTERN POTIONS
		// ---------------------------------------------------------------------
		else if ( citem->m_Type == ITEMGET(14,46) ) // Jack Olantern Blessings
		{
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],5,ITEMGET(14,46));
		}
		else if ( citem->m_Type == ITEMGET(14,47) ) // Jack Olantern Rage
		{
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],5,ITEMGET(14,47));
		}
		else if ( citem->m_Type == ITEMGET(14,48) ) // Jack Olantern Screem
		{
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],5,ITEMGET(14,48));
		}
		else if ( citem->m_Type == ITEMGET(14,49) ) // Jack Olantern Food
		{
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],5,ITEMGET(14,49));
		}
		else if ( citem->m_Type == ITEMGET(14,50) ) // Jack Olantern Drink
		{
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],5,ITEMGET(14,50));
		}
#if(GS_CASTLE)
		else if(citem->m_Type == ITEMGET(14,63) || citem->m_Type == ITEMGET(14,64)) // FireCracker & Cursed Castle Water
		{
			// Add Codes Here
		}
#endif
		// ---------------------------------------------------------------------
		// JEWELS
		// ---------------------------------------------------------------------
		else if ( g_kJewelOfHarmonySystem.IsJewelOfHarmonyPurity(citem->m_Type) == TRUE ) // Jewel Of Harmony
		{
			if ( g_kJewelOfHarmonySystem.StrengthenItemByJewelOfHarmony(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}
		else if ( g_kJewelOfHarmonySystem.IsJewelOfHarmonySmeltingItems(citem->m_Type) == TRUE ) // Jewel Of Harmony
		{
			if ( g_kJewelOfHarmonySystem.SmeltItemBySmeltingStone(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,13) ) // Jewel Of Bless
		{
			if ( gObjItemLevelUp(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,14) ) // Jewel Of Soul
		{
			if ( gObjItemRandomLevelUp(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,16) ) // Jewel Of Life
		{
			if ( gObjItemRandomOption3Up(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}
		// ---------------------------------------------------------------------
		// SCROLLS AND ORBS
		// ---------------------------------------------------------------------
		else if ( citem->m_Type >= ITEMGET(15,0) ||	// Group 15 - Scrolls
				  citem->m_Type == ITEMGET(12,7) || // Orb of Twisting Slash
				 (citem->m_Type >= ITEMGET(12,8) && citem->m_Type <= ITEMGET(12,24) && citem->m_Type != ITEMGET(12,15) ) || // Orbs
				  citem->m_Type == ITEMGET(12,35) || // Scroll of Fire Scream
				  (citem->m_Type >= ITEMGET(12,44) && citem->m_Type <= ITEMGET(12,48)) ) // S4 Orbs
		{
			if ( (gObj[aIndex].Strength + gObj[aIndex].AddStrength) < citem->m_RequireStrength )
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
				return;
			}
			
			if ( (gObj[aIndex].Dexterity + gObj[aIndex].AddDexterity) < citem->m_RequireDexterity )
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
				return;
			}

			if ( citem->m_Type == ITEMGET(15,18) ) // Scroll of Nova
			{
				if ( g_QuestInfo.GetQuestState(&gObj[aIndex], 2) != 2 )
				{
					GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
					return;
				}
			}
			else if (citem->m_Type >= ITEMGET(12,8) && citem->m_Type <= ITEMGET(12,24) && citem->m_Type != ITEMGET(12,15) ) // Orbs
			{
				if ( gObj[aIndex].Level < citem->m_RequireLevel )
				{
					GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
					return;
				}
			}
			else if (citem->m_Type >= ITEMGET(12,44) && citem->m_Type <= ITEMGET(12,48) ) // Master Orbs
			{
				if ( gObj[aIndex].Level < citem->m_RequireLevel )
				{
					GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
					return;
				}
			}

			WORD skillnumber;

			if ( citem->IsClass(gObj[aIndex].Class, gObj[aIndex].ChangeUP, gObj[aIndex].ChangeUP2) == FALSE )
			{	
				GCInventoryItemDeleteSend(aIndex, -1, 1);
			}
			else
			{
				int addskill = gObjMagicAdd(&gObj[aIndex], citem->m_Type>>(9), citem->m_Type %  MAX_SUBTYPE_ITEMS, citem->m_Level, skillnumber);

				if ( addskill >= 0 && gSkillTree.CheckMasterSkillCanAdd(&gObj[aIndex],skillnumber) == true )
				{
					gObjInventoryItemSet(aIndex, pos, -1);
					GCMagicListOneSend(aIndex, addskill, skillnumber, citem->m_Level, 0, 0);
					gObj[aIndex].pInventory[pos].Clear();
					GCInventoryItemDeleteSend(aIndex, pos, 1);
				}
				else
				{
					GCInventoryItemDeleteSend(aIndex, -1, 1);
				}
			}
		}
		// ---------------------------------------------------------------------
		// TOWN PORTAL SCROLL
		// ---------------------------------------------------------------------
		else if ( citem->m_Type == ITEMGET(14,10) ) // Town Portal Scroll
		{
			CLog.LogAdd("[Using Item] [return Scroll] [%s][%s] - Current Map:[%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].MapNumber);

			if ( gUser.BloodCastleMapRange(gObj[aIndex].MapNumber) == true )
			{
				if ( g_BloodCastle.GetCurrentState(g_BloodCastle.GetBloodCastleIndex(gObj[aIndex].MapNumber)) == 2 )
				{
					g_BloodCastle.SearchUserDropQuestItem(aIndex);
				}
				else
				{
					g_BloodCastle.SearchUserDeleteQuestItem(aIndex);
				}
			}

			if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 3 )
			{
				gObj[aIndex].TargetShopNumber = -1;
				gObj[aIndex].m_IfState.type = 0;
				gObj[aIndex].m_IfState.use = 0;
			}

			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);

			if ( gObj[aIndex].MapNumber == MAP_INDEX_DEVIAS ) // Devias
			{
				gObjMoveGate(aIndex, 22);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_NORIA ) // Noria
			{
				gObjMoveGate(aIndex, 27);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_LOSTTOWER ) // Lost Tower
			{
				gObjMoveGate(aIndex, 42);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_ATLANS ) // Atlans
			{
				gObjMoveGate(aIndex, 49);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_TARKAN ) // Tarkan
			{
				gObjMoveGate(aIndex, 57);
			}
			else if ( gUser.BloodCastleMapRange(gObj[aIndex].MapNumber) == true ) // Blood Caslte
			{
				gObjMoveGate(aIndex, 22);
			}
			else if ( gUser.ChaosCastleMapRange(gObj[aIndex].MapNumber) == true ) // Chaos Castle
			{
				gObjMoveGate(aIndex, 22);
			}
			else if ( KALIMA_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE ) // Kalima
			{
				gObjMoveGate(aIndex, 22);
			}
#if(!GS_CASTLE)
			else if ( IT_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE ) // Illusion Temple
			{
				gObjMoveGate(aIndex, 267); // Elbeland
			}
#endif
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_AIDA ) // Aida
			{
				gObjMoveGate(aIndex, 27);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE ) // Crywolf
			{
				gObjMoveGate(aIndex, 27);
			}
#if (GS_CASTLE)
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE ) // Castle Siege
            {
              if ( g_CastleSiege.GetCastleState() == 7 )
              {
                if ( gObj[aIndex].m_btCsJoinSide == 1 )
					gObjMoveGate(aIndex, 101);
                  else
					gObjMoveGate(aIndex, 100);
                }
                else
                {
                  if ( g_CastleSiege.CheckCastleOwnerMember(aIndex) != TRUE && g_CastleSiege.CheckCastleOwnerUnionMember(aIndex) != TRUE )
                    gObjMoveGate(aIndex, 100);
                  else
                    gObjMoveGate(aIndex, 101);
                }
              }
#endif
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_ELBELAND ) // Elbeland
			{
				gObjMoveGate(aIndex, 267);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_SWAMP_OF_CALMNESS ) // Swamp Of Calmness
			{
				gObjMoveGate(aIndex, 273);
			}
			else if ( gObj[aIndex].MapNumber == MAP_INDEX_RAKLION ) // Raklion
			{
				gObjMoveGate(aIndex, 287);
			}
			else
			{
				gObjMoveGate(aIndex, 17);
			}
		}
		// ---------------------------------------------------------------------
		// ALE & REMEDY OF LOVE
		// ---------------------------------------------------------------------
		else if ( citem->m_Type == ITEMGET(14,9) ) // Ale
		{
			int level = citem->m_Level;

			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gObjUseDrink(&gObj[aIndex], level);
		}
		else if ( citem->m_Type == ITEMGET(14,20) ) // Remedy of Love
		{
			if ( citem->m_Level == 0 )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
				gObjUseDrink(&gObj[aIndex], 2);
			}
		}
		// ---------------------------------------------------------------------
		// STATS FRUITS
		// ---------------------------------------------------------------------
		else if ( citem->m_Type == ITEMGET(13,15) ) // Fruits
		{
			if ( iItemUseType == 0 )
			{
				gObjUsePlusStatFruit(aIndex, pos);
			}
			else if ( iItemUseType == 1 )
			{
				gObjUseMinusStatFruit(aIndex, pos);
			}
		}
		// ---------------------------------------------------------------------
		// SIEGE POTION
		// ---------------------------------------------------------------------
		else if ( citem->m_Type == ITEMGET(14,7) )	// Siege Potion
		{
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			
			int iLevel = citem->m_Level;
			int iSerial = citem->GetNumber();
			int iDur = citem->m_Durability;

			if ( gObj[aIndex].lpGuild )
			{
				CLog.LogAdd("[CastleSiege] Using CastleSiege Potion Lv[%d]/Si[%d]/Dur[%d] - [%s][%s] - (Guild : %s)", iLevel, iSerial, iDur, gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].lpGuild->Name);
			}
			else
			{
				CLog.LogAdd("[CastleSiege] Using CastleSiege Potion Lv[%d]/Si[%d]/Dur[%d] - [%s][%s]", iLevel, iSerial, iDur, gObj[aIndex].AccountID, gObj[aIndex].Name);
			}

			if ( iLevel == 0 )
			{
				gObjUseBlessAndSoulPotion(aIndex, iLevel);
			}
			else if ( iLevel == 1 )
			{
				gObjUseBlessAndSoulPotion(aIndex, iLevel);
			}
		}
		// ---------------------------------------------------------------------
		// TICKETS USER
		// ---------------------------------------------------------------------
		else if ( citem->m_Type == ITEMGET(13,48) ) // Kalima Ticket
		{
			if ( (gObj[aIndex].MapNumber < MAP_INDEX_KALIMA1 || gObj[aIndex].MapNumber > MAP_INDEX_KALIMA6 ) && gObj[aIndex].MapNumber != MAP_INDEX_KALIMA7 )
			{
				int iLevel = g_KalimaGate.GetKalimaGateLevel2(aIndex);
				int iSerial = citem->GetNumber();
				int iDuration = citem->m_Durability;
				BOOL bKalimaGateCreateResult = FALSE;
				int iKalimaGateX = 0;
				int iKalimaGateY = 0;

				iLevel++;
				iKalimaGateX = gObj[aIndex].X + rand()%6 - 2;
				iKalimaGateY = gObj[aIndex].Y + rand()%6 - 2;

				bKalimaGateCreateResult = g_KalimaGate.CreateKalimaGate(aIndex, iLevel, iKalimaGateX, iKalimaGateY);

				if ( bKalimaGateCreateResult == TRUE )
				{
					if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
					{
						gObjInventoryItemSet(aIndex, pos, -1);
						gObj[aIndex].pInventory[pos].Clear();
						GCInventoryItemDeleteSend(aIndex, pos, 1);
					}

					CLog.LogAdd("[PCS] Use Free Kalima Ticket Success");
				}
				else
				{
					CLog.LogAdd("[PCS] Use Free Kalima Ticket Failed");
				}
			}
		}
		else if ( citem->m_Type == ITEMGET(13,66) ) // Santa Ticket
		{

			gObjTeleport(aIndex, 62, 220, 30); 

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			CLog.LogAdd("[Santa Event] Use Teleport Ticket Success");
		}
		switch ( citem->m_Type )
		{
		case ITEMGET(13,81):	// Talisman of Guardian
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			GCStateInfoSend(&gObj[aIndex],1,99);
			break;
		case ITEMGET(13,82):	// Talisman of Item Protection
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			GCStateInfoSend(&gObj[aIndex],1,100);
			break;
		case ITEMGET(14,72):	// Scroll of Quickness
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],3,ITEMGET(14,72));
			break;
		case ITEMGET(14,73):	// Scroll of Defense
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],3,ITEMGET(14,73));
			break;
		case ITEMGET(14,74):	// Scroll of Wrath
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],3,ITEMGET(14,74));
			break;
		case ITEMGET(14,75):	// Scroll of Wizardy
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],3,ITEMGET(14,75));
			break;
		case ITEMGET(14,76):	// Scroll of Health
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],3,ITEMGET(14,76));
			break;
		case ITEMGET(14,77):	// Scroll of Mana
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],3,ITEMGET(14,77));
			break;
		case ITEMGET(14,97):	// Scroll of Battle
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],3,ITEMGET(14,97));
			break;
		case ITEMGET(14,98):	// Scroll of Strenghtener
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],3,ITEMGET(14,98));
			break;
		case ITEMGET(14,78):	// Elixir of Strenght
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],1,ITEMGET(14,78));
			break;
		case ITEMGET(14,79):	// Elixir of Agility
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],1,ITEMGET(14,79));
			break;
		case ITEMGET(14,80):	// Elixir of Health
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],1,ITEMGET(14,80));
			break;
		case ITEMGET(14,81):	// Elixir of Energy
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],1,ITEMGET(14,81));
			break;
		case ITEMGET(14,82):	// Elixir of Leadership
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],1,ITEMGET(14,82));
			break;
		case ITEMGET(14,85):	// Cherry Blossom Wine
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],6,ITEMGET(14,85));
			break;
		case ITEMGET(14,86):	// Cherry Blossom Rice Cake
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],6,ITEMGET(14,86));
			break;
		case ITEMGET(14,87):	// Cherry Blossom Flower Petal
			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gSpecialItem.UseItem(&gObj[aIndex],6,ITEMGET(14,87));
			break;
		case ITEMGET(14,63):
			int x = gObj[aIndex].X;
	        int y = gObj[aIndex].Y;
			
			PMSG_SERVERCMD pMsg;

			C1SubHeadSet((LPBYTE)&pMsg,0xF3,0x40, sizeof(pMsg));

			for ( int i=0;i<15;i++)
			{
				pMsg.X = x+(rand() % 2)*1 - 2;
				pMsg.Y = y+(rand() % 2)*1 - 2;
				gSendProto.VPMsgSend(&gObj[aIndex],(UCHAR*)&pMsg, sizeof(pMsg));
				::gSendProto.DataSend(aIndex ,(UCHAR*)&pMsg, sizeof(pMsg));
			}

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			break;
		}
		/*if ( citem->m_Type == ITEMGET(14,63) ) // FireCracker
		{
			
		}
		else
		{
			CLog.LogAdd("error-L3 : %s return %s %d %d %s", gObj[aIndex].Name, __FILE__,__LINE__, pos, &CItem::GetName);	// #error Convert to other Name
			GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
		}*/
	}
	else
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
		CLog.LogAdd("error-L3 : %s return %s %d %d", gObj[aIndex].Name, __FILE__,__LINE__, pos);
	}
}
// -----------------------------------------------------------------------------------------------
struct PMSG_REFILL
{
	PBMSG_HEAD h;	// C1:26
	BYTE IPos;	// 3
	BYTE LifeH;	// 4
	BYTE LifeL;	// 5
	BYTE Flag;	// 6
	BYTE btShieldH;	// 7
	BYTE btShieldL;	// 8
};
// -----------------------------------------------------------------------------------------------
void GCReFillSend(int aIndex, WORD Life, BYTE Ipos, unsigned char flag,  WORD wShield)
{
	PMSG_REFILL pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x26, sizeof(pMsg));
	pMsg.IPos = Ipos;
	pMsg.LifeH = SET_NUMBERH(Life);
	pMsg.LifeL = SET_NUMBERL(Life);
	pMsg.btShieldH = SET_NUMBERH(wShield);
	pMsg.btShieldL = SET_NUMBERL(wShield);
	pMsg.Flag = flag;

	gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_MANASEND
{
	PBMSG_HEAD h;	// C1:27
	BYTE IPos;	// 3
	BYTE ManaH;	// 4
	BYTE ManaL;	// 5
	BYTE BPH;	// 6
	BYTE BPL;	// 7
};
// -----------------------------------------------------------------------------------------------
void GCManaSend(int aIndex, short Mana, BYTE Ipos, unsigned char flag,  WORD BP)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1)
	{
		CLog.LogAdd("GCManaSend() return %s %d", __FILE__, __LINE__);
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	PMSG_MANASEND pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x27, sizeof(pMsg));
	pMsg.IPos = Ipos;
	pMsg.ManaH = SET_NUMBERH(Mana);
	pMsg.ManaL = SET_NUMBERL(Mana);
	pMsg.BPH = SET_NUMBERH(BP);
	pMsg.BPL = SET_NUMBERL(BP);

	gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_INVENTORYDELETE
{
	PBMSG_HEAD h;	// C1:28
	BYTE IPos;	// 3
	BYTE Flag;	// 4
};
// -----------------------------------------------------------------------------------------------
void GCInventoryItemDeleteSend(int aIndex, BYTE pos, unsigned char flag)
{
	PMSG_INVENTORYDELETE pMsg;
	
	C1HeadSet((LPBYTE)&pMsg, 0x28, sizeof(pMsg));
	pMsg.IPos = pos;
	pMsg.Flag = flag;

	gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_ITEMUSESPECIALTIME
{
	PBMSG_HEAD h;	// C1:29
	BYTE Number;	// 3
	WORD Time;	// 4
};
// -----------------------------------------------------------------------------------------------
void GCItemUseSpecialTimeSend(int aIndex, unsigned char number, int time)
{
	if ( time > 65535 )
	{
		time = 65535;
	}

	PMSG_ITEMUSESPECIALTIME pMsg;

	C3HeadSet((LPBYTE)&pMsg, 0x29, sizeof(pMsg));
	pMsg.Number = number;
	pMsg.Time = time;

	gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_ITEMDUR
{
	PBMSG_HEAD h;	// C1:2A
	BYTE IPos;	// 3
	BYTE Dur;	// 4
	BYTE Flag;	// 5
};
// -----------------------------------------------------------------------------------------------
void GCItemDurSend(int aIndex, BYTE pos, BYTE dur, unsigned char flag)
{
	PMSG_ITEMDUR pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x2A, sizeof(pMsg));
	pMsg.IPos = pos;
	pMsg.Dur = dur;
	pMsg.Flag = flag;

	gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
void GCItemDurSend2(int aIndex, unsigned char pos, unsigned char dur, unsigned char flag)
{
	PMSG_ITEMDUR pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x2A, sizeof(pMsg));
	pMsg.IPos = pos;
	pMsg.Dur = dur;
	pMsg.Flag = flag;

	gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_WEATHER
{
	PBMSG_HEAD h;	// C1:0F
	BYTE Weather;	// 3
};
// -----------------------------------------------------------------------------------------------
void CGWeatherSend(int aIndex, BYTE weather)
{
	PMSG_WEATHER pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x0F, sizeof(pMsg));
	pMsg.Weather = weather;

	gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
void GCServerCmd(int aIndex, BYTE type, BYTE Cmd1, BYTE Cmd2)
{
	PMSG_SERVERCMD ServerCmd;

	C1SubHeadSet((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
	ServerCmd.CmdType = type;
	ServerCmd.X = Cmd1;
	ServerCmd.Y = Cmd2;

	gSendProto.DataSend(aIndex, (UCHAR*)&ServerCmd, ServerCmd.h.size);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_RESULT_MOVEDEVILSQUARE
{
	PBMSG_HEAD h;	// C1:90
	BYTE Result;	// 3
};
// -----------------------------------------------------------------------------------------------
void GCReqmoveDevilSquare(PMSG_REQ_MOVEDEVILSQUARE* lpMsg, int aIndex)
{
	if ( aIndex < 0 || aIndex > OBJMAX-1 )
	{
		CLog.LogAdd("return %s %d", __FILE__, __LINE__);
		return;
	}

	BYTE cSquareNumber = lpMsg->SquareNumber;
	BYTE cInvitationItemPos = lpMsg->InvitationItemPos - INVENTORY_BAG_START;

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type != 12 )
	{
		return;
	}

	PMSG_RESULT_MOVEDEVILSQUARE pResult;

	C1HeadSet((LPBYTE)&pResult, 0x90, sizeof(pResult));
	pResult.Result = 0;

	LPOBJ lpObj = &gObj[aIndex];

	CLog.LogAdd("[DevilSquare] [%s][%s] Request Move DevilSquare [%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);

	if ( gObj[aIndex].m_PK_Level >= 4 )
	{
		pResult.Result = 6;
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( MAIN_INVENTORY_RANGE(cInvitationItemPos) == FALSE )
	{
		pResult.Result = 1;
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		CLog.LogAdd("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);
		return;
	}

	if ( DS_LEVEL_RANGE(cSquareNumber) == FALSE )
	{
		pResult.Result = 1;
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		CLog.LogAdd("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);
		return;
	}

	if ( g_DevilSquare.GetState() != DevilSquare_OPEN )
	{
		pResult.Result = 2;
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		CLog.LogAdd("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);
		return;
	}

	if ( g_DevilSquare.m_DevilSquareGround[cSquareNumber].GetObjCount() >= MAX_DEVILSQUARE_USER )
	{
		pResult.Result = 5;
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		CLog.LogAdd("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);
		return;
	}

	CItem * sitem = &lpObj->pInventory[cInvitationItemPos];

	if ( sitem->m_Type != ITEMGET(14,19) && ( sitem->m_Type != ITEMGET(13,46) || sitem->m_Durability <= 0.0f ) )	// Devil's Invitation
	{
		pResult.Result = 1;
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		CLog.LogAdd("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);
		return;
	}

	int ItemLevel = sitem->m_Level;

	if ( sitem->m_Type == ITEMGET(13,46) )
	{
		ItemLevel = 10;
	}

	int level = lpObj->Level;

	if ( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_FIGHTER)
	{
		level = (level+1)/2*3;
	}

	if ( ItemLevel != 0 )
	{
		if ( ItemLevel != 10 )
		{
			if ( ItemLevel != (cSquareNumber+1) )
			{
				pResult.Result = 1;
				gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				CLog.LogAdd("[DevilSquare] [%s][%s] Move DevilSquare Invitation LevelError [%d][%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1, ItemLevel);
				return;
			}
		}
	}

	BOOL bEnterCheck = FALSE;
	WORD movegateindex = 61;	// #gate
	int iENTER_LEVEL = g_DevilSquare.GetUserLevelToEnter(aIndex, movegateindex);

	if ( DS_LEVEL_RANGE(iENTER_LEVEL) == FALSE )
	{
		CLog.LogAdd("[DevilSquare] [%s][%s] GetUserLevelToEnter() failed", lpObj->AccountID, lpObj->Name);
		return;
	}

	if ( ItemLevel == 10 )
	{
		cSquareNumber = iENTER_LEVEL;
	}

	if ( cSquareNumber != iENTER_LEVEL )
	{
		if ( cSquareNumber > iENTER_LEVEL )
		{
			bEnterCheck = 2;
		}
		else
		{
			bEnterCheck = 1;
		}
	}

	if ( bEnterCheck == 1 )
	{
		pResult.Result = 3;
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		CLog.LogAdd("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber, ItemLevel);
		return;
	}

	if ( bEnterCheck == 2 )
	{
		pResult.Result = 4;
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		CLog.LogAdd("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber, ItemLevel);
		return;
	}

	CLog.LogAdd("[DevilSquare] [%s][%s] Move DevilSquare success [%d][%d] RemoveItem[%s][%d][%d][%d]",
		lpObj->AccountID, lpObj->Name, lpObj->Level, cSquareNumber, lpObj->pInventory[cInvitationItemPos].GetName(),
		cInvitationItemPos, ItemLevel, lpObj->pInventory[cInvitationItemPos].m_Number);

	if ( sitem->m_Type == ITEMGET(14,19) || (sitem->m_Type == ITEMGET(13,46) && sitem->m_Durability == 1.0f) )
	{
		gObjInventoryDeleteItem(aIndex, cInvitationItemPos);
		GCInventoryItemDeleteSend(aIndex, cInvitationItemPos, 1);
		gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	}
	else if ( sitem->m_Type == ITEMGET(13,46) && sitem->m_Durability > 1.0f )
	{
		sitem->m_Durability -= 1.0f;
		GCItemDurSend2(lpObj->m_Index, cInvitationItemPos, sitem->m_Durability, 0);
	}

	lpObj->m_nEventExp = 0;
	lpObj->m_nEventScore = 0;
	lpObj->m_nEventMoney = 0;
	lpObj->m_bDevilSquareIndex = g_DevilSquare.GetDevilSquareIndex(movegateindex);
	lpObj->m_bDevilSquareAuth = true;
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.type = 0;
	lpObj->m_IfState.use = 0;

	gObjMoveGate(lpObj->m_Index, movegateindex);
	g_DevilSquare.m_DevilSquareGround[cSquareNumber].IncObjCount();
}
// -----------------------------------------------------------------------------------------------
struct PMSG_RESULT_REMAINTIME
{
	PBMSG_HEAD h;	// C1:91
	BYTE hEventType;	// 3
	BYTE RemainTime;	// 4
	BYTE EnteredUser;	// 5
	BYTE RemainTime_LOW;	// 6
};
// -----------------------------------------------------------------------------------------------
void GCReqEventRemainTime(PMSG_REQ_EVENT_REMAINTIME* lpMsg, int aIndex)
{
	PMSG_RESULT_REMAINTIME pResult;

	C1HeadSet((LPBYTE)&pResult, 0x91, sizeof(pResult));
	pResult.RemainTime = 0;

	switch ( lpMsg->hEventType )
	{
		// ---- Devil Square
		case 1:
			if ( gObj[aIndex].m_PK_Level >= 4 )
			{
				GCServerCmd(aIndex, 0x37, 0, 0);

				CLog.LogAdd("[PK User][DevilSquare] [%s][%s] Move Fail [PK Level:%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PK_Level);

				return;
			}

			pResult.hEventType = 1;

			if ( g_DevilSquare.GetState() == DevilSquare_CLOSE )
			{
				pResult.RemainTime = g_DevilSquare.GetRemainTime();
			}
			else if ( g_DevilSquare.GetState() == DevilSquare_OPEN )
			{
				pResult.RemainTime = 0;
			}
			else
			{
				pResult.RemainTime = g_DevilSquare.GetRemainTime() + g_DevilSquare.m_iCloseTime;
			}
			break;
		// ---- Blood Castle
		case 2:
			if ( gObj[aIndex].m_PK_Level >= 4 )
			{
				GCServerCmd(aIndex, 0x38, 0, 0);

				CLog.LogAdd("[PK User][BloodCastle] [%s][%s] Move Fail [PK Level:%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PK_Level);

				return;
			}

			pResult.hEventType = 2;

			if ( g_BloodCastle.GetCurrentState(lpMsg->btItemLevel-1) == 1 )
			{
				if ( g_BloodCastle.CheckCanEnter(lpMsg->btItemLevel-1) == true )
				{
					pResult.RemainTime = 0;
				}
				else
				{
					pResult.RemainTime = g_BloodCastle.GetRemainTime(lpMsg->btItemLevel-1);
				}
			}
			else
			{
				pResult.RemainTime = g_BloodCastle.GetRemainTime(lpMsg->btItemLevel-1);
			}
			break;

		case 4:
			if ( gObj[aIndex].m_PK_Level >= 4 )
			{
				GCServerCmd(aIndex, 0x39, 0, 0);

				CLog.LogAdd("[PK User][ChaosCastle] [%s][%s] Move Fail [PK Level:%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PK_Level);

				return;
			}

			int iENTER_LEVEL =  g_ChaosCastle.GetUserLevelToEnter(aIndex);

			if ( iENTER_LEVEL == -1 )
			{
				return;
			}

			pResult.hEventType = 4;

			if ( g_ChaosCastle.GetCurrentState(iENTER_LEVEL) == 1 )
			{
				if ( g_ChaosCastle.CheckCanEnter(iENTER_LEVEL) != false )
				{
					pResult.RemainTime = 0;
					pResult.RemainTime_LOW = 0;
					pResult.EnteredUser = g_ChaosCastle.GetCurEnteredUser(iENTER_LEVEL);
				}
				else
				{
					WORD wREMAIN_TIME = g_ChaosCastle.GetRemainTime(iENTER_LEVEL);
					pResult.RemainTime = SET_NUMBERH(wREMAIN_TIME);
					pResult.RemainTime_LOW = SET_NUMBERL(wREMAIN_TIME);
					pResult.EnteredUser = g_ChaosCastle.GetCurEnteredUser(iENTER_LEVEL);
				}
			}
			else
			{
				WORD wREMAIN_TIME = g_ChaosCastle.GetRemainTime(iENTER_LEVEL);
				pResult.RemainTime = SET_NUMBERH(wREMAIN_TIME);
				pResult.RemainTime_LOW = SET_NUMBERL(wREMAIN_TIME);
				pResult.EnteredUser = g_ChaosCastle.GetCurEnteredUser(iENTER_LEVEL);
			}
			break;
	}

	gSendProto.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}
// -----------------------------------------------------------------------------------------------
void AllSendMsg(LPBYTE Msg, int size)
{
	for ( int n=OBJ_STARTUSERINDEX;n<OBJMAX;n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				gSendProto.DataSend(n, Msg, size);
			}
		}
	}
}
// -----------------------------------------------------------------------------------------------
void AllSendSameMapMsg(UCHAR * Msg, int size, BYTE mapnumber)
{
	for ( int n=0;n<OBJMAX;n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				if ( gObj[n].MapNumber == mapnumber )
				{
					gSendProto.DataSend(n, Msg, size);
				}
			}
		}
	}
}
// -----------------------------------------------------------------------------------------------
struct PMSG_PING
{
	PBMSG_HEAD h;	// C1:71
};
// -----------------------------------------------------------------------------------------------
void GCSendPing(int aIndex)
{
	PMSG_PING pMsgPing;

	C1HeadSet((LPBYTE)&pMsgPing, 0x71, sizeof(pMsgPing));
	gObj[aIndex].iPingTime = GetTickCount();

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsgPing, pMsgPing.h.size);
}
// -----------------------------------------------------------------------------------------------
void GCPingSendRecv(PMSG_PING_RESULT* aRecv, int aIndex)
{
	int PingTime = GetTickCount() - gObj[aIndex].iPingTime;
	CLog.LogAdd("[%s][%s] Ping = %d ms", gObj[aIndex].AccountID, gObj[aIndex].Name, PingTime);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_REQ_REGISTER_EVENTCHIP
{
	PBMSG_HEAD h;	// C1:02
	int iINDEX;	// 4
	BYTE Pos;	// 8
	char szUID[11];	// 9
};
// -----------------------------------------------------------------------------------------------
struct PMSG_REQ_REGISTER_STONES
{
	PBMSG_HEAD h;	// C1:06
	int iINDEX;	// 4
	BYTE iPosition;	// 8
	char szUID[11];	// 9
};
// -----------------------------------------------------------------------------------------------
void GCRegEventChipRecv(PMSG_REGEVENTCHIP* lpMsg, int aIndex)
{
	if ( !EVENCHIP_TYPE_RANGE(lpMsg->Type) )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->m_IfState.type == 1 )
	{
		CLog.LogAdd("[EventChip] [%s][%s] Attempted ItemCopy using Trade Window", lpObj->AccountID, lpObj->Name);
		return;
	}

	if ( lpObj->m_IfState.type == 7 )
	{
		CLog.LogAdd("[EventChip] [%s][%s] Attempted ItemCopy using ChaosBox Window", lpObj->AccountID, lpObj->Name);
		return;
	}

	if ( lpObj->UseEventServer != FALSE )
	{
		PMSG_REGEVENTCHIP_RESULT Result;

		C1HeadSet((LPBYTE)&Result, 0x95, sizeof(Result));
		Result.ChipCount = -1;
		Result.Type = lpMsg->Type;

		CLog.LogAdd("[EventChip] [%s][%s] Not Found EventChip (RegEventchip) #1 %d", lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);

		gSendProto.DataSend(aIndex, (LPBYTE)&Result, Result.h.size);

		return;
	}

	int Pos = lpMsg->ChipPos + INVENTORY_BAG_START;
	CItem * sitem = &lpObj->pInventory[Pos];

	if ( !sitem->IsItem() )
	{
		PMSG_REGEVENTCHIP_RESULT Result;

		C1HeadSet((LPBYTE)&Result, 0x95, sizeof(Result));
		Result.ChipCount = -1;
		Result.Type = lpMsg->Type;

		CLog.LogAdd("[EventChip] [%s][%s] Not Found EventChip (RegEventchip) #2 %d", lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);

		gSendProto.DataSend(aIndex, (LPBYTE)&Result, Result.h.size);

		return;
	}

	lpObj->UseEventServer = TRUE;

	switch ( lpMsg->Type )
	{
		case 0x00:
			if ( sitem->m_Type == ITEMGET(14,21) && sitem->m_Level == 0 ) // Rena
			{
				PMSG_REQ_REGISTER_EVENTCHIP pMsg;

				C1HeadSet((LPBYTE)&pMsg, 0x02, sizeof(pMsg));
				pMsg.iINDEX = aIndex;
				pMsg.Pos = Pos;
				strcpy(pMsg.szUID, lpObj->AccountID);

				DataSendEventChip((PCHAR)&pMsg, sizeof(pMsg));
			}
			else
			{
				PMSG_REGEVENTCHIP_RESULT Result;

				C1HeadSet((LPBYTE)&Result, 0x95, sizeof(Result));
				Result.ChipCount = -1;
				Result.Type = 0x00;

				CLog.LogAdd("[EventChip] [%s][%s] Not Found EventChip (RegEventchip) #3 %d", lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);

				gSendProto.DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
				lpObj->UseEventServer = FALSE;
			}
			break;
		case 0x01:
			if ( sitem->m_Type == ITEMGET(14,21) && sitem->m_Level == 1 ) // Stone
			{
				PMSG_REQ_REGISTER_STONES pMsg;

				C1HeadSet((LPBYTE)&pMsg, 0x06, sizeof(pMsg));
				pMsg.iINDEX = aIndex;
				pMsg.iPosition = Pos;
				strcpy(pMsg.szUID, lpObj->AccountID);

				DataSendEventChip((PCHAR)&pMsg, sizeof(pMsg));

				CLog.LogAdd("[Stone] [%s][%s] Register Stone (Stone Pos:%d, Serial:%d)", lpObj->AccountID, lpObj->Name, lpMsg->ChipPos, sitem->m_Number);
			}
			else
			{
				PMSG_REGEVENTCHIP_RESULT Result;

				C1HeadSet((LPBYTE)&Result, 0x95, sizeof(Result));
				Result.ChipCount = -1;
				Result.Type = 0x01;

				CLog.LogAdd("[Stone] [%s][%s] Not Found EventChip (Stone Pos: %d)", lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);

				gSendProto.DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
				lpObj->UseEventServer = FALSE;
			}
			break;
		default:
			lpObj->UseEventServer = FALSE;
			break;
	}
}
// -----------------------------------------------------------------------------------------------
struct PMSG_REQ_REGISTER_MUTONUM
{
	PBMSG_HEAD h;	// C1:03
	int iINDEX;	// 4
	char szUID[11];	// 8
};
// -----------------------------------------------------------------------------------------------
void GCGetMutoNumRecv(PMSG_GETMUTONUMBER* lpMsg, int aIndex)
{
	if ( gObj[aIndex].MutoNumber != 0 )
	{
		char msg[255];
		wsprintf(msg, "ÀÌ¹Ì ·ç°¡µåÀÇ ¼ýÀÚ°¡ ÀÖ½À´Ï´Ù");
		GCServerMsgStringSend(msg, aIndex, 1);
		return;
	}

	if ( gObj[aIndex].UseEventServer != FALSE )
		return;

	gObj[aIndex].UseEventServer = TRUE;

	if ( !gObjFind10EventChip(aIndex) )
	{
		PMSG_GETMUTONUMBER_RESULT Result;

		C1HeadSet((LPBYTE)&Result, 0x96, sizeof(Result));
		Result.MutoNum[0] = -1;
		Result.MutoNum[1] = 0;
		Result.MutoNum[2] = 0;

		gSendProto.DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
		gObj[aIndex].UseEventServer = FALSE;

		return;
	}

	PMSG_REQ_REGISTER_MUTONUM pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x03, sizeof(pMsg));
	pMsg.iINDEX = aIndex;
	strcpy(pMsg.szUID, gObj[aIndex].AccountID);

	DataSendEventChip((PCHAR)&pMsg, sizeof(pMsg));

	CLog.LogAdd("[EventChip] [%s][%s] Request MutoNumber",
		gObj[aIndex].AccountID, gObj[aIndex].Name);
}
// -----------------------------------------------------------------------------------------------
void GCUseEndEventChipRescv(int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		CLog.LogAddC(2, "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 9 )
	{
		gObj[aIndex].m_IfState.state = 0;
		gObj[aIndex].m_IfState.type = 0;
		gObj[aIndex].m_IfState.use = 0;
	}
}
// -----------------------------------------------------------------------------------------------
struct PMSG_REQ_RESET_EVENTCHIP
{
	PBMSG_HEAD h;	// C1:[04:Rena] [09:Stone]
	int iINDEX;	// 4
	char szUID[11];	// 8
};
// -----------------------------------------------------------------------------------------------
void GCUseRenaChangeZenRecv(PMSG_EXCHANGE_EVENTCHIP* lpMsg, int aIndex)
{
	if ( gObj[aIndex].UseEventServer )
		return;

	gObj[aIndex].UseEventServer = TRUE;

	PMSG_REQ_RESET_EVENTCHIP pMsg;

	if ( lpMsg->btType == 1 )	// Stone?
		C1HeadSet((LPBYTE)&pMsg, 0x09, sizeof(pMsg));
	else
		C1HeadSet((LPBYTE)&pMsg, 0x04, sizeof(pMsg));

	pMsg.iINDEX = aIndex;
	strcpy(pMsg.szUID, gObj[aIndex].AccountID);

	DataSendEventChip((PCHAR)&pMsg, sizeof(pMsg));

	if ( lpMsg->btType == 0x01 )
		CLog.LogAdd("[EventChip] [%s][%s] Request Change Stones", gObj[aIndex].AccountID, gObj[aIndex].Name);
	else
		CLog.LogAdd("[EventChip] [%s][%s] Request Change Rena", gObj[aIndex].AccountID, gObj[aIndex].Name);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_SEND_QEUSTINFO
{
	PBMSG_HEAD h;	// C1:A0
	BYTE Count;	// 3
	BYTE State[50];	// 4
};
// -----------------------------------------------------------------------------------------------
void CGRequestQuestInfo(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->m_SendQuestInfo != 0 )
		return;

	int questcount = g_QuestInfo.GetQeustCount();
	int foundquest = 0;

	PMSG_SEND_QEUSTINFO pMsg;
	LPQUEST_INFO lpQuestInfo;

	memcpy(pMsg.State, lpObj->m_Quest, sizeof(pMsg.State));

	int i = 0;
	for ( i=0;i<MAX_QUEST_INFO;i++)
	{
		lpQuestInfo = g_QuestInfo.GetQuestInfo(i);

		if ( lpQuestInfo )
		{
			foundquest++;

			if ( foundquest == questcount )
			{
				break;
			}
		}
	}

	int iSize = i/4+5;

	C1HeadSet((LPBYTE)&pMsg, 0xA0, iSize);
	pMsg.Count = i;
	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, iSize);
	lpObj->m_SendQuestInfo = true;
}
// -----------------------------------------------------------------------------------------------
void GCSendQuestInfo(int aIndex, int QuestIndex)
{
	CGRequestQuestInfo(aIndex);
	LPOBJ lpObj = &gObj[aIndex];
	PMSG_SETQUEST pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0xA1, sizeof(pMsg));
	pMsg.QuestIndex = QuestIndex;
	pMsg.State = g_QuestInfo.GetQuestStateBYTE(lpObj, QuestIndex);


	// QUEST3rd
	if( lpObj->m_Quest[30] > 10)
	{
		lpObj->m_Quest[30] = 0;
	}
	if( lpObj->m_Quest[31] > 10 )
	{
		lpObj->m_Quest[31] = 0;
	}
	if( lpObj->m_Quest[32] > 10 )
	{
		lpObj->m_Quest[32] = 0;
	}
	if( lpObj->m_Quest[34] > 1 )
	{
		lpObj->m_Quest[34] = 0;
	}

	if(QuestIndex == 5 || QuestIndex == 6)
	{
	    BYTE QuestCount[0x31] = {0xC1,0x31,0xA4,0x01,0x02,0x03,0x04,0x05,
		
								 0x99,0x01,0x00,0x00,0x00,0x00,0x00,0x00,    
                                 0x9A,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
								 0x9B,0x01,0x00,0x00,0x00,0x00,0x00,0x00,   
                                 0x9C,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
								 
								 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

		QuestCount[12] = lpObj->m_Quest[30];
		QuestCount[20] = lpObj->m_Quest[31];
		QuestCount[28] = lpObj->m_Quest[32];
		QuestCount[36] = lpObj->m_Quest[34];

		gSendProto.DataSend(aIndex,QuestCount,QuestCount[1]);
	}

	if ( pMsg.State != 0 )
		gSendProto.DataSend(aIndex, (UCHAR*)&pMsg, sizeof(pMsg));
}
// -----------------------------------------------------------------------------------------------
struct PMSG_SETQUEST_RESULT
{
	BYTE c;
	BYTE size;
	BYTE headcode;
	BYTE QuestIndex;
	BYTE Result;
	BYTE State;
};
// -----------------------------------------------------------------------------------------------
void CGSetQuestState(PMSG_SETQUEST* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	PMSG_SETQUEST_RESULT pMsg;

	pMsg.c = 0xC1;
	pMsg.size = 0x06;
	pMsg.headcode = 0xA2;
	pMsg.QuestIndex = lpMsg->QuestIndex;
	pMsg.Result = g_QuestInfo.SetQuestState(lpObj, lpMsg->QuestIndex, lpMsg->State);
	pMsg.State = g_QuestInfo.GetQuestStateBYTE(lpObj, lpMsg->QuestIndex);

	gSendProto.DataSend(aIndex, (BYTE*)&pMsg, pMsg.size);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_SETQUEST_PRIZE
{
	PBMSG_HEAD h;	// C1:A3
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE Type;	// 5
	BYTE Count;	// 6
};
// -----------------------------------------------------------------------------------------------
void GCSendQuestPrize(int aIndex, BYTE Type, BYTE Count)
{
	CGRequestQuestInfo(aIndex);
	LPOBJ lpObj = &gObj[aIndex];

	PMSG_SETQUEST_PRIZE pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0xA3, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);
	pMsg.Type = Type;
	pMsg.Count = Count;

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
	gSendProto.VPMsgSend(lpObj, (LPBYTE)&pMsg, sizeof(pMsg));
}
// -----------------------------------------------------------------------------------------------
void CGCloseWindow(int aIndex)
{
	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type )
	{
		if ( gObj[aIndex].m_IfState.type == 7 )
		{
			return;
		}

		if ( gObj[aIndex].m_IfState.type == 10 && gObj[aIndex].TargetShopNumber == 229 )	// Marlon
		{
			gQeustNpcTeleport.TalkRefDel();
			gObj[aIndex].TargetShopNumber = -1;
		}

		if ( gObj[aIndex].m_IfState.type == 3 )
		{
			gObj[aIndex].TargetShopNumber = -1;
		}

		if ( gObj[aIndex].m_IfState.type == 1 )
		{
			CGTradeCancelButtonRecv(aIndex);
		}

		if ( gObj[aIndex].m_IfState.type == 6 )
		{
			CGWarehouseUseEnd(aIndex);
		}

		if ( gObj[aIndex].m_IfState.type == 13 )
		{
			gChaosBox.ChaosBoxInit(&gObj[aIndex]);
			gObjInventoryCommit(aIndex);
		}

		gObj[aIndex].m_IfState.state = 0;
		gObj[aIndex].m_IfState.type = 0;
		gObj[aIndex].m_IfState.use = 0;
	}
}
// -----------------------------------------------------------------------------------------------
void CGRequestEventEnterCount(PMSG_REQ_CL_ENTERCOUNT* lpMsg, int aIndex)
{
	if ( !lpMsg )
	{
		return;
	}

	switch ( lpMsg->btEventType )
	{
		case 0x02:
			EGReqBloodCastleEnterCount(aIndex);
			break;
	}
}
// -----------------------------------------------------------------------------------------------
struct PMSG_REQ_2ANIV_SERIAL
{
	PBMSG_HEAD h;	// C1:08
	int iINDEX;	// 8
	char szUID[MAX_ACCOUNT_LEN+1];	// 8
	char SERIAL1[5];	// 13
	char SERIAL2[5];	// 18
	char SERIAL3[5];	// 1D
	int iMEMB_GUID;	// 24
};
// -----------------------------------------------------------------------------------------------
void CGRequestLottoRegister(PMSG_REQ_2ANV_LOTTO_EVENT* lpMsg, int aIndex)
{
	PMSG_REQ_2ANIV_SERIAL pMsg;

	C1HeadSet((LPBYTE)&pMsg, 0x08, sizeof(pMsg));

	if ( gObj[aIndex].UseEventServer )
	{
		return;
	}

	gObj[aIndex].UseEventServer = TRUE;
	pMsg.iINDEX = aIndex;
	pMsg.iMEMB_GUID = gObj[aIndex].DBNumber;
	memcpy(pMsg.szUID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);
	pMsg.szUID[MAX_ACCOUNT_LEN] = 0;
	memcpy(pMsg.SERIAL1, lpMsg->SERIAL1, 4);
	pMsg.SERIAL1[4] = 0;
	memcpy(pMsg.SERIAL2, lpMsg->SERIAL2, 4);
	pMsg.SERIAL2[4] = 0;
	memcpy(pMsg.SERIAL3, lpMsg->SERIAL3, 4);
	pMsg.SERIAL3[4] = 0;

	DataSendEventChip((PCHAR)&pMsg, sizeof(pMsg));

	CLog.LogAdd("[Mu_2Anv_Event] [%s][%s] Register Lotto Number (Serial: %s-%s-%s)", gObj[aIndex].AccountID, gObj[aIndex].Name, pMsg.SERIAL1, pMsg.SERIAL2, pMsg.SERIAL3);
}
// -----------------------------------------------------------------------------------------------
void GCPacketCheckSumRecv(PMSG_PACKETCHECKSUM * aRecv, int aIndex)
{
	gPacketCheckSum.AddCheckSum(aIndex, aRecv->funcindex, aRecv->CheckSum);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_GETITEMINFO_FOR_PARTY
{
	PBMSG_HEAD h;	// C1:47
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	UINT ItemInfo;	// 8
	BYTE Level;	// C
};
// -----------------------------------------------------------------------------------------------
void GCSendGetItemInfoForParty(int aIndex, CMapItem * lpItem)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->PartyNumber >= 0)
	{
		PMSG_GETITEMINFO_FOR_PARTY pMsg;

		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x47;
		pMsg.h.size = sizeof(pMsg);
		pMsg.NumberH = SET_NUMBERH(aIndex);
		pMsg.NumberL = SET_NUMBERL(aIndex);
		pMsg.ItemInfo = lpItem->m_Type & (ITEMGET(MAX_TYPE_ITEMS-1, MAX_SUBTYPE_ITEMS-1));

		if ( lpItem->m_Option1 )
			pMsg.ItemInfo |= 0x2000;

		if ( lpItem->m_Option2 )
			pMsg.ItemInfo |= 0x4000;

		if ( lpItem->m_Option3 )
			pMsg.ItemInfo |= 0x8000;

		if ( lpItem->m_NewOption )
			pMsg.ItemInfo |= 0x10000;

		if ( lpItem->m_SetOption )
			pMsg.ItemInfo |= 0x20000;

		if ( g_Item380System.Is380OptionItem( (lpItem != NULL)?((CItem *)&lpItem->m_Number):NULL) == TRUE )
			pMsg.ItemInfo |= 0x40000;

		if ( gItemSocket.IsSeason4Item(lpItem->m_Type) != 0 )
			pMsg.ItemInfo |= 0x80000;

		pMsg.Level = lpItem->m_Level;
		
		int partycount = gParty.GetPartyCount(lpObj->PartyNumber);

		if ( partycount >= 0 )
		{
			for ( int n=0;n<MAX_USER_IN_PARTY;n++)
			{
				int number = gParty.m_PartyInfo[lpObj->PartyNumber].Number[n];

				if ( number >= 0 )
				{
					gSendProto.DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
		}
	}
}
// -----------------------------------------------------------------------------------------------
struct PMSG_EFFECTINFO
{
	PBMSG_HEAD h;	// C1:48
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE btType;	// 5
};
// -----------------------------------------------------------------------------------------------
void GCSendEffectInfo(int aIndex, BYTE btType)
{
	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_EFFECTINFO pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x48;
	pMsg.h.size = sizeof(pMsg);
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	pMsg.btType = btType;

	if ( btType == 17 )
	{
		gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}

	gSendProto.VPMsgSend(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
void CGRequestPetItemCommand(PMSG_REQUEST_PET_ITEM_COMMAND * lpMsg, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		CLog.LogAdd("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpMsg->PetType )
	{
		return;
	}

	int iTargetIndex = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
	gDarkSpirit[aIndex].ChangeCommand(lpMsg->Command, iTargetIndex);
}
// -----------------------------------------------------------------------------------------------
void CGRequestPetItemInfo(PMSG_REQUEST_PET_ITEMINFO * lpMsg, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		CLog.LogAdd("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpMsg->PetType != 0 && lpMsg->PetType != 1 )
	{
		return;
	}
	
	PMSG_SEND_PET_ITEMINFO pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xA9, sizeof(pMsg));
	pMsg.PetType = lpMsg->PetType;
	pMsg.InvenType = lpMsg->InvenType;
	pMsg.nPos = lpMsg->nPos;

	if ( lpMsg->InvenType == 0 )	// Inventory
	{
		if ( !INVENTORY_RANGE(lpMsg->nPos))
		{
			CLog.LogAdd("[PetItem] [%s][%s] RequestPetItem Invalid Iventory nPos %d", lpObj->AccountID, lpObj->Name, lpMsg->nPos);
			return;
		}

		if ( lpObj->pInventory[lpMsg->nPos].IsItem() )
		{
			pMsg.Exp = lpObj->pInventory[lpMsg->nPos].m_PetItem_Exp;
			pMsg.Level = lpObj->pInventory[lpMsg->nPos].m_PetItem_Level;

			gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if ( lpMsg->InvenType == 1 )	// Warehouse
	{
		if ( lpObj->LoadWareHouseInfo != false )
		{
			if ( !WAREHOUSE_RANGE(lpMsg->nPos))
			{
				CLog.LogAdd("[PetItem] [%s][%s] RequestPetItem Invalid WareHouse nPos %d", lpObj->AccountID, lpObj->Name, lpMsg->nPos);
				return;
			}

			if ( lpObj->pWarehouse[lpMsg->nPos].IsItem() )
			{
				pMsg.Exp = lpObj->pWarehouse[lpMsg->nPos].m_PetItem_Exp;
				pMsg.Level = lpObj->pWarehouse[lpMsg->nPos].m_PetItem_Level;

				gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
			}
		}
	}
	else if ( lpMsg->InvenType == 2 )	// Trade
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 1 )
		{
			return;
		}

		if ( !TRADE_BOX_RANGE(lpMsg->nPos))
		{
			CLog.LogAdd("[PetItem] [%s][%s] RequestPetItem Invalid Trade nPos %d", lpObj->AccountID, lpObj->Name, lpMsg->nPos);
			return;
		}

		if ( lpObj->Trade[lpMsg->nPos].IsItem() )
		{
			pMsg.Exp = lpObj->Trade[lpMsg->nPos].m_PetItem_Exp;
			pMsg.Level = lpObj->Trade[lpMsg->nPos].m_PetItem_Level;

			gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if ( lpMsg->InvenType == 3 )	// Target Trade
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 1 )
		{
			return;
		}

		int iTargetIndex = gObj[aIndex].TargetNumber;

		if ( iTargetIndex < 0 )
		{
			return;
		}

		if ( !OBJMAX_RANGE(iTargetIndex) )
		{
			return;
		}

		if ( gObj[iTargetIndex].m_IfState.use < 1 || gObj[iTargetIndex].m_IfState.type != 1 )
		{
			return;
		}

		if ( !TRADE_BOX_RANGE(lpMsg->nPos))
		{
			CLog.LogAdd("[PetItem] [%s][%s] RequestPetItem Invalid TargetTrade nPos %d", lpObj->AccountID, lpObj->Name, lpMsg->nPos);
			return;
		}

		if ( gObj[iTargetIndex].Trade[lpMsg->nPos].IsItem() )
		{
			pMsg.Exp = gObj[iTargetIndex].Trade[lpMsg->nPos].m_PetItem_Exp;
			pMsg.Level = gObj[iTargetIndex].Trade[lpMsg->nPos].m_PetItem_Level;

			gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if ( lpMsg->InvenType == 4 )	// Chaos
	{
		if ( !CHAOS_BOX_RANGE(lpMsg->nPos) )
		{
			CLog.LogAdd("[PetItem] [%s][%s] RequestPetItem Invalid Chaos nPos %d", lpObj->AccountID, lpObj->Name, lpMsg->nPos);
			return;
		}

		if ( lpObj->pChaosBox[lpMsg->nPos].IsItem() )
		{
			pMsg.Exp = lpObj->pChaosBox[lpMsg->nPos].m_PetItem_Exp;
			pMsg.Level = lpObj->pChaosBox[lpMsg->nPos].m_PetItem_Level;

			gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if ( lpMsg->InvenType == 5 )	// Personal Shop
	{
		if ( !INVENTORY_RANGE(lpMsg->nPos) )
		{
			CLog.LogAdd("[PetItem] [%s][%s] RequestPetItem Invalid Iventory nPos %d", lpObj->AccountID, lpObj->Name, lpMsg->nPos);
			return;
		}

		if ( lpObj->m_iPShopDealerIndex != -1 )
		{
			if ( OBJMAX_RANGE(lpObj->m_iPShopDealerIndex))
			{
				if ( gObj[lpObj->m_iPShopDealerIndex].pInventory[lpMsg->nPos].IsItem() )
				{
					pMsg.Exp = gObj[lpObj->m_iPShopDealerIndex].pInventory[lpMsg->nPos].m_PetItem_Exp;
					pMsg.Level = gObj[lpObj->m_iPShopDealerIndex].pInventory[lpMsg->nPos].m_PetItem_Level;

					gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
				}
			}
		}
	}	
}
// -----------------------------------------------------------------------------------------------
struct PMSG_ANS_GUILDVIEWPORT
{
	PBMSG_HEAD h;	// C1:66
	int GuildNumber;	// 4
	BYTE btGuildType;	// 8
	BYTE UnionName[8];	// 9
	char GuildName[8];	// 11
	BYTE Mark[32];	// 19
};
// -----------------------------------------------------------------------------------------------
void GCGuildViewportInfo(PMSG_REQ_GUILDVIEWPORT * aRecv, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		CLog.LogAdd("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	DWORD dwGuildNumber = aRecv->GuildNumber;
	_GUILD_INFO_STRUCT * lpGuildInfo = Guild.SearchGuild_Number(dwGuildNumber);

	if ( lpGuildInfo )
	{
		PMSG_ANS_GUILDVIEWPORT pMsg;

		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x66;
		pMsg.h.size = sizeof(pMsg);
		pMsg.GuildNumber = lpGuildInfo->Number;
		pMsg.btGuildType = lpGuildInfo->btGuildType;

		TUnionInfo * pUnionInfo = UnionManager.SearchUnion(lpGuildInfo->iGuildUnion);

		if ( pUnionInfo )
			memcpy(pMsg.UnionName, pUnionInfo->m_szMasterGuild, MAX_GUILD_LEN);
		else
			pMsg.UnionName[0] = '\0';

		strcpy(pMsg.GuildName, lpGuildInfo->Name);
		memcpy(pMsg.Mark, lpGuildInfo->Mark, sizeof(pMsg.Mark));

		gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
	}
	else
	{
		CLog.LogAdd("¡Ú¡Ú¡Ù ±æµå Á¤º¸ Ã£À»¼ö ¾øÀ½. ÀÌ¸§ : [%s] ¹øÈ£ : %d", lpObj->Name, dwGuildNumber);
	}
}
// -----------------------------------------------------------------------------------------------
void CGGuildAssignStatus(PMSG_GUILD_ASSIGN_STATUS_REQ * aRecv, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		CLog.LogAdd("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_GUILD_ASSIGN_STATUS_RESULT pMsg={0};
	
	pMsg.h.set((LPBYTE)&pMsg, 0xE1, sizeof(pMsg));
	pMsg.btType = aRecv->btType;
	memcpy(pMsg.szTagetName, aRecv->szTagetName, sizeof(pMsg.szTagetName));

	if ( lpObj->GuildNumber <= 0 || lpObj->lpGuild == NULL )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_GUILD;
		gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}

	if ( gObj[aIndex].GuildStatus != G_MASTER )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}

	char szTargetName[MAX_ACCOUNT_LEN+1]={0};
	memcpy(szTargetName, aRecv->szTagetName, MAX_ACCOUNT_LEN);

	if ( !strcmp(gObj[aIndex].Name, szTargetName))
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}

	if ( lpObj->lpGuild == NULL )
		return;

	if ( aRecv->btType == 1 || aRecv->btType == 2 )
	{
		int iSubMasterCount = 0;
		int iBattleMasterCount = 0;

		for ( int n=0;n<MAX_USER_GUILD;n++)
		{
			if ( lpObj->lpGuild->Use[n] > 0 )
			{
				if ( lpObj->lpGuild->GuildStatus[n] == G_SUB_MASTER )
				{
					iSubMasterCount++;
				}
				else if ( lpObj->lpGuild->GuildStatus[n] == G_BATTLE_MASTER )
				{
					iBattleMasterCount++;
				}
			}
		}

		if ( aRecv->btGuildStatus == G_SUB_MASTER )
		{
			if ( iSubMasterCount == 0 )
			{
				GDGuildReqAssignStatus(aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus);
				return;
			}

			pMsg.btResult = GUILD_ANS_NOTEXIST_EXTRA_STATUS;
			gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
			return;
		}
		else if ( aRecv->btGuildStatus == G_BATTLE_MASTER )
		{
			if ( iBattleMasterCount < ((lpObj->Level / 200 ) +1) )
			{
				GDGuildReqAssignStatus(aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus);
				return;
			}

			pMsg.btResult = GUILD_ANS_NOTEXIST_EXTRA_STATUS;
			gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
			return;
		}
		else if ( aRecv->btGuildStatus == G_PERSON )
		{
			GDGuildReqAssignStatus(aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus);
			return;
		}
		else
		{
			return;
		}
	}
	
	if ( aRecv->btType == 3 )
	{
		GDGuildReqAssignStatus(aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus);
	}
}
// -----------------------------------------------------------------------------------------------
void CGGuildAssignType(PMSG_GUILD_ASSIGN_TYPE_REQ * aRecv, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		CLog.LogAdd("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_GUILD_ASSIGN_TYPE_RESULT pMsg={0};
	
	pMsg.h.set((LPBYTE)&pMsg, 0xE2, sizeof(pMsg));
	pMsg.btGuildType = aRecv->btGuildType;
	
	if ( lpObj->GuildNumber <= 0 || lpObj->lpGuild == NULL )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_GUILD;
		gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}

	if ( gObj[aIndex].GuildStatus != G_MASTER )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}
	
	GDGuildReqAssignType(aIndex, aRecv->btGuildType);
}
// -----------------------------------------------------------------------------------------------
void CGRelationShipReqJoinBreakOff(PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ * aRecv, int aIndex)
{
	if ( g_CastleSiegeSync.GetCastleState() >= 5 && g_CastleSiegeSync.GetCastleState() <= 7 )
	{
		MsgOutput(aIndex, lMsg.Get(MSGGET(6, 196)));
		return;
	}

	int iTargetUserIndex = MAKE_NUMBERW(aRecv->btTargetUserIndexH, aRecv->btTargetUserIndexL);

	if ( !OBJMAX_RANGE(aIndex) || !OBJMAX_RANGE(iTargetUserIndex))
		return;

	if ( !PacketCheckTime(&gObj[aIndex]))
		return;

	if ( gObjIsConnected(&gObj[iTargetUserIndex]) == FALSE )
	{
		GCResultSend(aIndex, 0x51, 3);
		return;
	}

	if ( !gObj[aIndex].lpGuild || !gObj[iTargetUserIndex].lpGuild )
	{
		GCResultSend(aIndex, 0x51, 5);
		return;
	}

	if ( gObj[aIndex].GuildStatus != G_MASTER && gObj[iTargetUserIndex].GuildStatus != G_MASTER)
	{
		GCResultSend(aIndex, 0x51, 4);
		return;
	}

	if ( gObj[aIndex].m_IfState.use > 0 )
	{
		GCResultSend(aIndex, 0x51, 6);
		return;
	}

	if ( gObj[iTargetUserIndex].m_IfState.use > 0 )
	{
		GCResultSend(aIndex, 0x51, 6);
		return;
	}

	PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS ErrMsg={0};

	C1HeadSet((LPBYTE)&ErrMsg, 0xE6, sizeof(ErrMsg));
	ErrMsg.btResult = 0;
	ErrMsg.btRequestType = aRecv->btRequestType;
	ErrMsg.btRelationShipType = aRecv->btRelationShipType;
	ErrMsg.btTargetUserIndexH = aRecv->btTargetUserIndexH;
	ErrMsg.btTargetUserIndexL = aRecv->btTargetUserIndexL;

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[iTargetUserIndex];

	if ( lpObj->GuildStatus != G_MASTER || lpTargetObj->GuildStatus != G_MASTER)
	{
		ErrMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		gSendProto.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
		return;
	}

	_GUILD_INFO_STRUCT * lpGuildInfo = gObj[aIndex].lpGuild;
	_GUILD_INFO_STRUCT * lpTargetGuildInfo = gObj[iTargetUserIndex].lpGuild;
	BYTE btRelationShip = gObjGetRelationShip(lpObj, lpTargetObj);

	if ( aRecv->btRequestType == 1 )
	{
		if ( aRecv->btRelationShipType == 1 )
		{
			if ( lpTargetGuildInfo->Count < MIN_GUILD_COUNT_FOR_CS )
			{
				ErrMsg.btResult = GUILD_ANS_CANNOT_BE_UNION_MASTER;
				gSendProto.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( lpTargetGuildInfo->iGuildUnion != 0 && lpTargetGuildInfo->iGuildUnion == lpTargetGuildInfo->Number )	// #error
			{
				if ( UnionManager.GetGuildRelationShipCount(lpTargetGuildInfo->iGuildUnion, 1) >= MAX_UNION_MEMBER  )
				{
					ErrMsg.btResult = GUILD_ANS_EXCEED_MAX_UNION_MEMBER;
					gSendProto.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
					return;
				}
			}
		
			char szCastleOwnerGuildName[MAX_GUILD_LEN+1]={0};
			memcpy(szCastleOwnerGuildName, g_CastleSiegeSync.GetCastleOwnerGuild(), MAX_GUILD_LEN);

			if ( !strcmp(lpGuildInfo->Name, szCastleOwnerGuildName))
			{
				ErrMsg.btResult = GUILD_ANS_EXIST_UNION;
				gSendProto.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( lpGuildInfo->iGuildUnion )
			{
				ErrMsg.btResult = GUILD_ANS_EXIST_UNION;
				gSendProto.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( btRelationShip == 2 )
			{
				ErrMsg.btResult = GUILD_ANS_EXIST_RELATIONSHIP_RIVAL;
				gSendProto.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( lpTargetGuildInfo->iGuildUnion && lpTargetGuildInfo->Number != lpTargetGuildInfo->iGuildUnion )	// #error???
			{
				ErrMsg.btResult = GUIDL_ANS_NOT_UNION_MASTER;
				gSendProto.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}
		}
		else if ( aRecv->btRelationShipType == 2 )
		{
			if ( lpGuildInfo->iGuildRival || lpTargetGuildInfo->iGuildRival )
			{
				ErrMsg.btResult = GUILD_ANS_EXIST_RIVAL;
				gSendProto.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( btRelationShip == 1 )
			{
				ErrMsg.btResult = GUILD_ANS_EXIST_RELATIONSHIP_UNION;
				gSendProto.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}
		}
	}
	else if ( aRecv->btRequestType == 2 )
	{
		if ( aRecv->btRelationShipType == 1 )
		{
			if ( lpGuildInfo->iGuildUnion == 0 )
			{
				ErrMsg.btResult = GUILD_ANS_NOTEXIST_UNION;
				gSendProto.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}
		}
		else if ( aRecv->btRelationShipType == 2 )
		{
			if ( !lpGuildInfo->iGuildRival || !lpTargetGuildInfo->iGuildRival )
			{
				ErrMsg.btResult = GUILD_ANS_NOTEXIST_UNION;
				gSendProto.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( lpGuildInfo->iGuildRival != lpTargetGuildInfo->Number || lpGuildInfo->Number != lpTargetGuildInfo->iGuildRival )
			{
				ErrMsg.btResult = GUILD_ANS_NOT_GUILD_RIVAL;
				gSendProto.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}
		}
	}		
	
	if ( aRecv->btRequestType == 2 )
	{
		if ( aRecv->btRelationShipType == 1 )
		{
			GDUnionBreakOff(aIndex, lpGuildInfo->iGuildUnion);
			return;
		}
	}

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 14;
	lpObj->m_IfState.state = 0;
	lpObj->m_InterfaceTime = GetTickCount();

	lpTargetObj->m_IfState.use = 1;
	lpTargetObj->m_IfState.type = 14;
	lpTargetObj->m_IfState.state = 0;
	lpTargetObj->m_InterfaceTime = GetTickCount();

	PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ pMsg={0};

	C1HeadSet((LPBYTE)&pMsg, 0xE5, sizeof(pMsg));
	pMsg.btTargetUserIndexL = SET_NUMBERL(aIndex);
	pMsg.btTargetUserIndexH = SET_NUMBERH(aIndex);
	pMsg.btRequestType = aRecv->btRequestType;
	pMsg.btRelationShipType = aRecv->btRelationShipType;
	
	gSendProto.DataSend(iTargetUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
void CGRelationShipAnsJoinBreakOff(PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS * aRecv, int aIndex)
{
	int iTargetUserIndex = MAKE_NUMBERW(aRecv->btTargetUserIndexH, aRecv->btTargetUserIndexL);

	if ( !OBJMAX_RANGE(aIndex) || !OBJMAX_RANGE(iTargetUserIndex))
	{
		return;
	}

	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		return;
	}

	if ( gObjIsConnected(&gObj[iTargetUserIndex]) == FALSE )
	{
		GCResultSend(aIndex, 0x51, 3);
		return;
	}

	if ( aRecv->btResult == 1 )
	{
		if ( aRecv->btRequestType == 1 )
		{
			GDRelationShipReqJoin(iTargetUserIndex, aIndex, aRecv->btRelationShipType);
		}
		else if ( aRecv->btRequestType == 2 )
		{
			GDRelationShipReqBreakOff(aIndex, iTargetUserIndex, aRecv->btRelationShipType);
		}
	}
	else
	{
		PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS ErrMsg ={0};

		C1HeadSet((LPBYTE)&ErrMsg, 0xE6, sizeof(ErrMsg));
		ErrMsg.btResult = 0;
		ErrMsg.btRequestType = aRecv->btRequestType;
		ErrMsg.btRelationShipType = aRecv->btRelationShipType;
		ErrMsg.btTargetUserIndexH = aRecv->btTargetUserIndexH;
		ErrMsg.btTargetUserIndexL = aRecv->btTargetUserIndexL;
		ErrMsg.btResult = GUILD_ANS_CANCLE_REQUEST;

		gSendProto.DataSend(iTargetUserIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
	}

	if ( gObj[aIndex].m_IfState.use )
	{
		if ( gObj[aIndex].m_IfState.type == 14 )
		{
			gObj[aIndex].m_IfState.use = 0;
		}
	}

	if ( gObj[iTargetUserIndex].m_IfState.use )
	{
		if ( gObj[iTargetUserIndex].m_IfState.type == 14 )
		{
			gObj[iTargetUserIndex].m_IfState.use = 0;
		}
	}
}
// -----------------------------------------------------------------------------------------------
void CGUnionList(PMSG_UNIONLIST_REQ * aRecv, int aIndex)
{
	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	if ( !PacketCheckTime(&gObj[aIndex]))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj == NULL )
	{
		return;
	}

	if ( gObjIsConnected(&gObj[aIndex]) == FALSE )
	{
		GCResultSend(aIndex, 0x51, 3);
		return;
	}

	if ( lpObj->lpGuild == NULL )
	{
		GCResultSend(aIndex, 0x51, 3);
		return;
	}

	if ( lpObj->lpGuild->iGuildUnion == 0 )
	{
		GCResultSend(aIndex, 0x51, 3);
		return;
	}

	GDUnionListSend(aIndex, lpObj->lpGuild->iGuildUnion);

}
// -----------------------------------------------------------------------------------------------
void CGRelationShipReqKickOutUnionMember(PMSG_KICKOUT_UNIONMEMBER_REQ* aRecv, int aIndex)
{
	if ( !OBJMAX_RANGE(aIndex))
	{
		return;
	}

	if ( ::PacketCheckTime(&gObj[aIndex]) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj == NULL )
	{
		return;
	}

	if ( gObjIsConnected(&gObj[aIndex]) == FALSE )
	{
		GCResultSend(aIndex, 0x51, 3);
		MsgOutput(aIndex, "¡Ú Terminated User.");
		return;
	}

	if ( lpObj->lpGuild == NULL )
	{
		GCResultSend(aIndex, 0x51, 3);
		MsgOutput(aIndex, "¡Ù Terminated Guild.");
		return;
	}

	PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS ErrMsg = {0};

	C1HeadSet((LPBYTE)&ErrMsg, 0xE6, sizeof(ErrMsg));
	ErrMsg.btResult = 0;
	ErrMsg.btRequestType = 2;
	ErrMsg.btRelationShipType = 1;
	ErrMsg.btTargetUserIndexH = 0;
	ErrMsg.btTargetUserIndexL = 0;


	if ( lpObj->lpGuild->iGuildUnion == 0 )
	{
		ErrMsg.btResult = GUILD_ANS_NOTEXIST_UNION;
		gSendProto.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
		return;
	}

	if ( lpObj->GuildStatus != GUILD_MASTER || lpObj->lpGuild->Number != lpObj->lpGuild->iGuildUnion)
	{
		ErrMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		gSendProto.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
		return;
	}

	char szUnionMemberGuildName[MAX_GUILD_LEN+1] = {0};
	memcpy(szUnionMemberGuildName, aRecv->szTargetGuildName, MAX_GUILD_LEN);

	if ( !strcmp(lpObj->lpGuild->Name, szUnionMemberGuildName))
	{
		ErrMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		gSendProto.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
		return;
	}

	GDRelationShipReqKickOutUnionMember(aIndex, szUnionMemberGuildName);
}
// -----------------------------------------------------------------------------------------------
void CGReqMapSvrAuth(PMSG_REQ_MAPSERVERAUTH * lpMsg, int iIndex)
{
	if ( lpMsg->btCliVersion[0] != szClientVersion[0] || lpMsg->btCliVersion[1] != szClientVersion[1] || lpMsg->btCliVersion[2] != szClientVersion[2] || lpMsg->btCliVersion[3] != szClientVersion[3] || lpMsg->btCliVersion[4] != szClientVersion[4] )
	{
		GCAnsMapSvrAuth(iIndex, 6);
		gNet.CloseClient(iIndex);
		return;
	}

	BYTE btSerial[17];

	btSerial[16]=0;
	memcpy(btSerial, lpMsg->btCliSerial, sizeof(lpMsg->btCliSerial));
	char id[MAX_ACCOUNT_LEN+1];
	id[MAX_ACCOUNT_LEN] = 0;
	memcpy(id, lpMsg->szAccountID, MAX_ACCOUNT_LEN);
	BuxConvert(id, MAX_ACCOUNT_LEN);

	if ( strcmp((char*)btSerial, szGameServerExeSerial) )
	{
		CLog.LogAddC(2, "error-L1: Serial error [%s] [%s]", id, btSerial);
		GCAnsMapSvrAuth(iIndex, 6);
		gNet.CloseClient(iIndex);
		return;
	}

	if ( bCanConnectMember == TRUE )
	{
		if ( ConMember.IsMember(id) == FALSE )
		{
			GCAnsMapSvrAuth(iIndex, 2);	
			return;
		}
	}

	LPOBJ lpObj = &gObj[iIndex];

	if ( PacketCheckTime(lpObj) == FALSE )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 217)), iIndex, id);
		gNet.CloseClient(iIndex);
		return;
	}

	if ( gObj[iIndex].Connected != PLAYER_CONNECTED )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 218)), iIndex, id);
		gNet.CloseClient(iIndex);
		return;
	}

	if ( gObj[iIndex].LoginMsgSnd != 0 )
	{
		if ( gObj[iIndex].LoginMsgSnd == 1 )
		{
			CLog.LogAdd(lMsg.Get(MSGGET(1, 219)), iIndex, id, gObj[iIndex].Ip_addr);
		}
		else
		{
			CLog.LogAdd("error : %d %s %d", gObj[iIndex].LoginMsgSnd, __FILE__, __LINE__);
		}

		return;
	}

	gObj[iIndex].CheckTick = lpMsg->iTickCount;
	gObj[iIndex].CheckTick2 = GetTickCount();
	gObj[iIndex].ConnectCheckTime = GetTickCount(); 
	gObj[iIndex].CheckSpeedHack = true;
	gObj[iIndex].ConnectCheckTime = GetTickCount();
	gObj[iIndex].LoginMsgSnd = 1;
	gObj[iIndex].LoginMsgCount++;
	gObj[iIndex].AccountID[MAX_ACCOUNT_LEN] = 0;
	memcpy(gObj[iIndex].AccountID, id, MAX_ACCOUNT_LEN);
	memset(gObj[iIndex].Name, 0, MAX_ACCOUNT_LEN);
	gObj[iIndex].m_cAccountItemBlock = 0;
	//GJReqMapSvrAuth(iIndex, gObj[iIndex].AccountID, lpMsg->szCharName, lpMsg->iJoinAuthCode1, lpMsg->iJoinAuthCode2, lpMsg->iJoinAuthCode3, lpMsg->iJoinAuthCode4);

	CLog.LogAdd("[MapServerMng] Map Server Join Send : [%s][%s](%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iIndex);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_ANS_MAPSERVERAUTH
{
	PBMSG_HEAD2 h;	// C1:B1:01
	BYTE iResult;	// 4
};
// -----------------------------------------------------------------------------------------------
void GCAnsMapSvrAuth(int iIndex, int iResult)
{
	if ( !OBJMAX_RANGE(iIndex) )
	{
		CLog.LogAddC(2,"[MapServerMng] Packet Error JG [0x7B] - Index out of bound : %d",iIndex);
		return;
	}

	PMSG_ANS_MAPSERVERAUTH pMsgResult;

	C1SubHeadSet((LPBYTE)&pMsgResult, 0xB1, 0x01, sizeof(pMsgResult));
	pMsgResult.iResult = iResult;

	gSendProto.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}
// -----------------------------------------------------------------------------------------------
void CGReqCastleSiegeState(PMSG_REQ_CASTLESIEGESTATE * lpMsg, int iIndex)
{
#if (GS_CASTLE)
	GS_GDReqOwnerGuildMaster(g_MapServerManager.GetMapSvrGroup(), iIndex);
#endif
}
// -----------------------------------------------------------------------------------------------
#if (GS_CASTLE)
struct PMSG_ANS_CASTLESIEGESTATE
{
  PBMSG_HEAD2 h;
  unsigned char btResult;
  char cCastleSiegeState;
  unsigned char btStartYearH;
  unsigned char btStartYearL;
  unsigned char btStartMonth;
  unsigned char btStartDay;
  unsigned char btStartHour;
  unsigned char btStartMinute;
  unsigned char btEndYearH;
  unsigned char btEndYearL;
  unsigned char btEndMonth;
  unsigned char btEndDay;
  unsigned char btEndHour;
  unsigned char btEndMinute;
  unsigned char btSiegeStartYearH;
  unsigned char btSiegeStartYearL;
  unsigned char btSiegeStartMonth;
  unsigned char btSiegeStartDay;
  unsigned char btSiegeStartHour;
  unsigned char btSiegeStartMinute;
  char cOwnerGuild[8];
  char cOwnerGuildMaster[10];
  char btStateLeftSec1;
  char btStateLeftSec2;
  char btStateLeftSec3;
  char btStateLeftSec4;
};
#endif
// -----------------------------------------------------------------------------------------------
void GCAnsCastleSiegeState(int iIndex, int iResult, LPSTR lpszGuildName, LPSTR lpszGuildMaster)
{
#if (GS_CASTLE)
	if( (lpszGuildName == NULL) || (lpszGuildMaster == NULL) )
		return;

	PMSG_ANS_CASTLESIEGESTATE pMsgResult;


	pMsgResult.h.set((LPBYTE)&pMsgResult.h, 0xB2, 0, sizeof(pMsgResult));

	pMsgResult.btResult = iResult;

	memcpy(pMsgResult.cOwnerGuild, lpszGuildName, sizeof(pMsgResult.cOwnerGuild));
	memcpy(pMsgResult.cOwnerGuildMaster, lpszGuildMaster, sizeof(pMsgResult.cOwnerGuildMaster));

	SYSTEMTIME tmStateStartDate = {0};
	SYSTEMTIME tmSiegeStartDate = {0};
	SYSTEMTIME tmStateEndDate = {0};

	int iCastleState = g_CastleSiege.GetCastleState();
	int bRETVAL = g_CastleSiege.GetCastleStateTerm(&tmStateStartDate, &tmStateEndDate);
	tmSiegeStartDate = g_CastleSiege.GetCastleLeftSiegeDate();

	if( bRETVAL == FALSE )
	{
		iCastleState = -1;
	}

	int iStateLeftSec = g_CastleSiege.GetCurRemainSec();

	// Create Packet
	pMsgResult.cCastleSiegeState = iCastleState;
	// Start Date
	pMsgResult.btStartYearH = SET_NUMBERH(tmStateStartDate.wYear);
	pMsgResult.btStartYearL = SET_NUMBERL(tmStateStartDate.wYear);
	pMsgResult.btStartMonth = tmStateStartDate.wMonth;
	pMsgResult.btStartDay   = tmStateStartDate.wDay;
	pMsgResult.btStartHour  = tmStateStartDate.wHour;
	pMsgResult.btStartMinute= tmStateStartDate.wMinute;
	// End Date
	pMsgResult.btEndYearH = SET_NUMBERH(tmStateEndDate.wYear);
	pMsgResult.btEndYearL = SET_NUMBERL(tmStateEndDate.wYear);
	pMsgResult.btEndMonth = tmStateEndDate.wMonth;
	pMsgResult.btEndDay   = tmStateEndDate.wDay;
	pMsgResult.btEndHour  = tmStateEndDate.wHour;
	pMsgResult.btEndMinute= tmStateEndDate.wMinute;
	// Siege Start Date
	pMsgResult.btSiegeStartYearH = SET_NUMBERH(tmStateStartDate.wYear);
	pMsgResult.btSiegeStartYearL = SET_NUMBERL(tmStateStartDate.wYear);
	pMsgResult.btSiegeStartMonth = tmStateStartDate.wMonth;
	pMsgResult.btSiegeStartDay   = tmStateStartDate.wDay;
	pMsgResult.btSiegeStartHour  = tmStateStartDate.wHour;
	pMsgResult.btSiegeStartMinute= tmStateStartDate.wMinute;
	// Time Left
	pMsgResult.btStateLeftSec1 = SET_NUMBERH(SET_NUMBERHW(iStateLeftSec));
	pMsgResult.btStateLeftSec2 = SET_NUMBERL(SET_NUMBERHW(iStateLeftSec));
	pMsgResult.btStateLeftSec3 = SET_NUMBERH(SET_NUMBERLW(iStateLeftSec));
	pMsgResult.btStateLeftSec4 = SET_NUMBERL(SET_NUMBERLW(iStateLeftSec));

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

#endif
}
// -----------------------------------------------------------------------------------------------
void CGReqRegCastleSiege(PMSG_REQ_REGCASTLESIEGE * lpMsg, int iIndex)
{
#if (GS_CASTLE)
	if( g_CastleSiege.GetCastleState() != 1 )
	{
		GCAnsRegCastleSiege(iIndex, 7, "");
		return;
	}
	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
	{
		GCAnsRegCastleSiege(iIndex, 6, "");
		return;
	}
	if( g_CastleSiege.CheckUnionGuildMaster(iIndex) == FALSE )
	{
		GCAnsRegCastleSiege(iIndex, 0, "");
	}
	else if( (g_CastleSiege.CheckCastleOwnerMember(iIndex) != FALSE) || 
		(g_CastleSiege.CheckCastleOwnerUnionMember(iIndex) != FALSE) )
	{
		GCAnsRegCastleSiege(iIndex, 3, "");
	}
	else
	{
		GS_GDReqRegAttackGuild(g_MapServerManager.GetMapSvrGroup(), iIndex);
	}
#endif
}
// -----------------------------------------------------------------------------------------------
#if (GS_CASTLE)
struct PMSG_ANS_REGCASTLESIEGE { // <size 0xd>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x8|*/ char szGuildName[8];
};
#endif
// -----------------------------------------------------------------------------------------------
void GCAnsRegCastleSiege(int iIndex, int iResult, LPSTR lpszGuildName)
{
#if (GS_CASTLE)
	PMSG_ANS_REGCASTLESIEGE pMsgResult;

	if( lpszGuildName == NULL )
		return;

	if( OBJMAX_RANGE(iIndex) == FALSE )
		return;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x01, sizeof(pMsgResult));

	pMsgResult.btResult = iResult;
	memcpy(&pMsgResult.szGuildName, lpszGuildName, sizeof(pMsgResult.szGuildName));

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

	if( ::gObjIsConnected(iIndex) )
	{
		::CLog.LogAdd("[CastleSiege] [%s][%s] Registered Castle Siege (GUILD:%s) - Result:%d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, iResult);
	}
#endif
}
// -----------------------------------------------------------------------------------------------
struct PMSG_ANS_GIVEUPCASTLESIEGE {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btIsGiveUp;
  /*<thisrel this+0x6>*/ /*|0x8|*/ char szGuildName[8];
};
// -----------------------------------------------------------------------------------------------
void GCAnsGiveUpCastleSiege(int iIndex, int iResult, int bGiveUp, int iMarkCount, LPSTR lpszGuildName)
{
#if (GS_CASTLE)
	PMSG_ANS_GIVEUPCASTLESIEGE	pMsgResult;
	if(lpszGuildName == 0)
		return;

	if(!OBJMAX_RANGE(iIndex))
		return;

	pMsgResult.h.set((LPBYTE)&pMsgResult,0xb2,0x02,sizeof(pMsgResult));
	
	pMsgResult.btResult = iResult;
	pMsgResult.btIsGiveUp = bGiveUp;
	memcpy(&pMsgResult.szGuildName,lpszGuildName,sizeof(pMsgResult.szGuildName));

	if((pMsgResult.btResult & 0xFF) == 1)
	{
		if(iMarkCount > 0)
		{
			gObj[iIndex].Money += (iMarkCount * 3000);

			if( gObj[iIndex].Money > MAX_ZEN )
			{
				gObj[iIndex].Money = MAX_ZEN;
			}
			GCMoneySend(iIndex,gObj[iIndex].Money);
		}
	}

	DataSend(iIndex,(LPBYTE)&pMsgResult,pMsgResult.h.size);

	if( gObjIsConnected(iIndex) )
	{
		CLog.LogAdd("[CastleSiege] [%s][%s] GiveUp Castle Siege (GUILD:%s) - Result:%d",
			gObj[iIndex].AccountID,gObj[iIndex].Name,gObj[iIndex].GuildName,iResult);
	}

#endif
}
// -----------------------------------------------------------------------------------------------
void CGReqGiveUpCastleSiege(PMSG_REQ_GIVEUPCASTLESIEGE * lpMsg, int iIndex)
{
#if (GS_CASTLE)
	if( (g_CastleSiege.GetCastleState() >= 1) && (g_CastleSiege.GetCastleState() <= 3) )
	{
		GCAnsGiveUpCastleSiege(iIndex, 3, 0, 0, "");

		return;
	}
	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
		return;
	
	if( g_CastleSiege.CheckUnionGuildMaster(iIndex) == FALSE )
		return;

	GS_GDReqGuildSetGiveUp(g_MapServerManager.GetMapSvrGroup(), iIndex, lpMsg->btGiveUp);
#endif
}
// -----------------------------------------------------------------------------------------------
void CGReqGuildRegInfo(PMSG_REQ_GUILDREGINFO * lpMsg, int iIndex)
{
#if (GS_CASTLE)

	if( lpMsg == NULL )
		return;

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
		return;

	GS_GDReqGuildMarkRegInfo(g_MapServerManager.GetMapSvrGroup(), iIndex);
#endif
}
// -----------------------------------------------------------------------------------------------
#if (GS_CASTLE)
struct PMSG_ANS_GUILDREGINFO {// <size 0x13>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x8|*/ char szGuildName[8];
  /*<thisrel this+0xd>*/ /*|0x1|*/ unsigned char btGuildMark1;
  /*<thisrel this+0xe>*/ /*|0x1|*/ unsigned char btGuildMark2;
  /*<thisrel this+0xf>*/ /*|0x1|*/ unsigned char btGuildMark3;
  /*<thisrel this+0x10>*/ /*|0x1|*/ unsigned char btGuildMark4;
  /*<thisrel this+0x11>*/ /*|0x1|*/ unsigned char btIsGiveUp;
  /*<thisrel this+0x12>*/ /*|0x1|*/ unsigned char btRegRank;
};
#endif
// -----------------------------------------------------------------------------------------------
void GCAnsGuildRegInfo(int iIndex, int iResult, CSP_ANS_GUILDREGINFO* lpMsgResult)
{
#if (GS_CASTLE)
	PMSG_ANS_GUILDREGINFO pMsgResult;

	if( lpMsgResult == NULL )
		return;

	if( OBJMAX_RANGE(iIndex) == FALSE )
		return;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 3, sizeof(pMsgResult));

	pMsgResult.btResult = iResult;
	pMsgResult.btGuildMark1 = SET_NUMBERH(SET_NUMBERLW(SET_NUMBERHW(lpMsgResult->iRegMarkCount)));
	pMsgResult.btGuildMark2 = SET_NUMBERL(SET_NUMBERLW(SET_NUMBERHW(lpMsgResult->iRegMarkCount)));
	pMsgResult.btGuildMark3 = SET_NUMBERH(SET_NUMBERLW(SET_NUMBERLW(lpMsgResult->iRegMarkCount)));
	pMsgResult.btGuildMark4 = SET_NUMBERH(SET_NUMBERLW(SET_NUMBERLW(lpMsgResult->iRegMarkCount)));
	pMsgResult.btIsGiveUp = lpMsgResult->bIsGiveUp;
	pMsgResult.btRegRank = lpMsgResult->btRegRank;
	memcpy(&pMsgResult.szGuildName, lpMsgResult->szGuildName, sizeof(pMsgResult.szGuildName));

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);


#endif
}
// -----------------------------------------------------------------------------------------------
#if (GS_CASTLE)
struct PMSG_ANS_REGGUILDMARK { // <size 0x11>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x8|*/ char szGuildName[8];
  /*<thisrel this+0xd>*/ /*|0x1|*/ unsigned char btGuildMark1;
  /*<thisrel this+0xe>*/ /*|0x1|*/ unsigned char btGuildMark2;
  /*<thisrel this+0xf>*/ /*|0x1|*/ unsigned char btGuildMark3;
  /*<thisrel this+0x10>*/ /*|0x1|*/ unsigned char btGuildMark4;
};
#endif
// -----------------------------------------------------------------------------------------------
void CGReqRegGuildMark(PMSG_REQ_REGGUILDMARK * lpMsg, int iIndex)
{
#if (GS_CASTLE)
	if( lpMsg == NULL )
		return;
	
	if( OBJMAX_RANGE(iIndex) == FALSE )
		return;
	
	if( gObj[iIndex].UseEventServer == TRUE )
		return;

	gObj[iIndex].UseEventServer = TRUE;

	if( g_CastleSiege.GetCastleState() != 3 )
	{
		gObj[iIndex].UseEventServer = FALSE;
		return;
	}
	if( strcmp( gObj[iIndex].GuildName, "") == 0)
	{
		gObj[iIndex].UseEventServer = FALSE;
		return;
	}
	
	if( CHECK_LIMIT(lpMsg->btItemPos, 63) == FALSE )
	{
		PMSG_ANS_REGGUILDMARK pMsgResult = {0};
		pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
		pMsgResult.btResult = 3;		
		DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
		gObj[iIndex].UseEventServer = FALSE;
	}
	else
	{
		int iItemPos = (lpMsg->btItemPos + 12);

		if( gObj[iIndex].pInventory[iItemPos].IsItem() == FALSE )
		{
			PMSG_ANS_REGGUILDMARK pMsgResult = {0};
			pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
			pMsgResult.btResult = 3;		
			DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
			gObj[iIndex].UseEventServer = FALSE;
		}
		else
		{
			if( gObj[iIndex].pInventory[iItemPos].m_Type != ITEMGET(14,21) ||
				gObj[iIndex].pInventory[iItemPos].m_Level != 3 || 
				gObj[iIndex].pInventory[iItemPos].m_Durability == 0.0 )
			{
				PMSG_ANS_REGGUILDMARK pMsgResult = {0};
				pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
				pMsgResult.btResult = 3;		
				DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
				gObj[iIndex].UseEventServer = FALSE;
			}
			else
			{
				GS_GDReqRegGuildMark(g_MapServerManager.GetMapSvrGroup(), iIndex, iItemPos);
			}
		}
	}
#endif
}
// -----------------------------------------------------------------------------------------------
void GCAnsRegGuildMark(int iIndex, int iResult, CSP_ANS_GUILDREGMARK* lpMsgResult)
{
#if (GS_CASTLE)
	PMSG_ANS_REGGUILDMARK pMsgResult;

	if( lpMsgResult == NULL )
		return;

	if( OBJMAX_RANGE(iIndex) == FALSE )
		return;

	if( gObj[iIndex].pInventory[lpMsgResult->iItemPos].IsItem() == FALSE )
	{
		pMsgResult = PMSG_ANS_REGGUILDMARK();

		pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
		pMsgResult.btResult = 3;
		DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

		gObj[iIndex].UseEventServer = 0;
	}
	else
	{
		if( ((gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Durability - 1.0 ) - gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Durability) > 0.0 ) // Prodito : Review
		{
			::GCItemDurSend(iIndex, lpMsgResult->iItemPos, gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Durability, 0);
		}
		else
		{
			gObjInventoryDeleteItem(iIndex, lpMsgResult->iItemPos);
			
			pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
			pMsgResult.btResult = iResult;
			pMsgResult.btGuildMark1 = SET_NUMBERH(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
			pMsgResult.btGuildMark2 = SET_NUMBERL(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
			pMsgResult.btGuildMark3 = SET_NUMBERH(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
			pMsgResult.btGuildMark4 = SET_NUMBERL(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
			memcpy(&pMsgResult.szGuildName, lpMsgResult->szGuildName, sizeof(pMsgResult.szGuildName));
			DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
			gObj[iIndex].UseEventServer = 0;
		}
	}
#endif
}
// -----------------------------------------------------------------------------------------------
void CGReqNpcBuy(PMSG_REQ_NPCBUY * lpMsg, int iIndex)
{
#if (GS_CASTLE)
	if( lpMsg == NULL )
		return;

	if( ::gObjIsConnected(iIndex) == FALSE )
		return;

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
		return;

	if( g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE
		|| (gObj[iIndex].GuildStatus != G_MASTER && gObj[iIndex].GuildStatus != G_SUB_MASTER) )
	{
		CLog.LogAddC(2, "[CastleSiege] CGReqNpcBuy() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else
	{
		BYTE btResult = 0;

		BOOL bRETVAL = g_CastleSiege.CheckAddDbNPC(iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, btResult);

		if( bRETVAL == FALSE )
		{
			GCAnsNpcBuy(iIndex, btResult, lpMsg->iNpcNumber, lpMsg->iNpcIndex);
			CLog.LogAddC(2, "[CastleSiege] CGReqNpcBuy() ERROR - CCastleSiege::CheckAddDbNPC() [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
		}
		else
		{
			CLog.LogAddC(2, "[CastleSiege] CGReqNpcBuy() OK - [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
		}
	}
#endif
}
// -----------------------------------------------------------------------------------------------
#if (GS_CASTLE)
struct PMSG_ANS_NPCBUY { // <size 0x10>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iNpcNumber;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iNpcIndex;
};
#endif
// -----------------------------------------------------------------------------------------------
void GCAnsNpcBuy(int iIndex, int iResult, int iNpcNumber, int iNpcIndex)
{
#if (GS_CASTLE)
	PMSG_ANS_NPCBUY pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x05, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.iNpcNumber = iNpcNumber;
	pMsgResult.iNpcIndex = iNpcIndex;
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}
// -----------------------------------------------------------------------------------------------
void CGReqNpcRepair(PMSG_REQ_NPCREPAIR * lpMsg, int iIndex)
{
#if (GS_CASTLE)
		if( lpMsg == NULL )
		return;

	if( ::gObjIsConnected(iIndex) == FALSE )
		return;

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
		return;

	if( (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE) || (gObj[iIndex].GuildStatus != G_MASTER) || (gObj[iIndex].GuildStatus != G_SUB_MASTER) )
	{
		CLog.LogAddC(2, "[CastleSiege] CGReqNpcRepair() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else
	{
		_CS_NPC_DATA pNpcData;

		BOOL bRETVAL = g_CastleSiege.GetNpcData(lpMsg->iNpcNumber, lpMsg->iNpcIndex,pNpcData);

		if( bRETVAL == FALSE )
		{
			GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
			CLog.LogAddC(2, "[CastleSiege] CGReqNpcRepair() ERROR - CL Request Fail [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->iNpcNumber, lpMsg->iNpcIndex);

			pNpcData.~_CS_NPC_DATA(); // ???
		}
		else
		{
			int iNpcIndex = lpMsg->iNpcIndex;

			if( gObjIsConnected(iNpcIndex) == FALSE )
			{
				GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
				CLog.LogAdd("[CastleSiege] CGReqNpcRepair() ERROR - Npc Alive Data Mismatch [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
					gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->iNpcNumber, lpMsg->iNpcIndex);
				pNpcData.~_CS_NPC_DATA(); // ???
			}
			else if( gObj[iNpcIndex].Live == 0 )
			{
				GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
				CLog.LogAddC(2, "[CastleSiege] CGReqNpcRepair() ERROR - Npc Alive Data Mismatch [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
					gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->iNpcNumber, lpMsg->iNpcIndex);
				pNpcData.~_CS_NPC_DATA(); // ???	
			}
			else
			{
				if( (gObj[iNpcIndex].Life == gObj[iNpcIndex].MaxLife) || (gObj[iNpcIndex].Life < gObj[iNpcIndex].MaxLife) )
				{
					gObj[iNpcIndex].Life = gObj[iNpcIndex].MaxLife;
					GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
					pNpcData.~_CS_NPC_DATA(); // ???
				}
				else
				{
					int iRepairCost = 0;

					switch( lpMsg->iNpcNumber )
					{
					case 277:
							iRepairCost	= ( gObj[iNpcIndex].MaxLife - gObj[iNpcIndex].Life) * 0.5 + ( gObj[iNpcIndex].m_btCsNpcDfLevel * 1000000 );
						break;
					case 283:
							iRepairCost	= ( gObj[iNpcIndex].MaxLife - gObj[iNpcIndex].Life) * 0.3 + ( gObj[iNpcIndex].m_btCsNpcDfLevel + gObj[iNpcIndex].m_btCsNpcRgLevel) * 1000000;
						break;
					default:
							GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
							pNpcData.~_CS_NPC_DATA(); // ???
						break;
					}

					if( gObj[iNpcIndex].Money < iRepairCost )
					{
						GCAnsNpcRepair(iIndex, 3, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
						CLog.LogAddC(2, "[CastleSiege] CGReqNpcRepair() ERROR - Money isn't enough [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
							gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->iNpcNumber, lpMsg->iNpcIndex);
						pNpcData.~_CS_NPC_DATA(); // ???
					}
					else
					{
						CLog.LogAddC(2, "[CastleSiege] CGReqNpcRepair() OK - [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",
							gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->iNpcNumber, lpMsg->iNpcIndex);		
						GS_GDReqCastleNpcRepair(g_MapServerManager.GetMapSvrGroup(), iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, iRepairCost);
						pNpcData.~_CS_NPC_DATA(); // ???
					}
				}
			}
		}
	}	
#endif
}
// -----------------------------------------------------------------------------------------------
#if (GS_CASTLE)
struct PMSG_ANS_NPCREPAIR { // <size 0x18>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iNpcNumber;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iNpcIndex;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iNpcHP;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iNpcMaxHP;
};
#endif
// -----------------------------------------------------------------------------------------------
void GCAnsNpcRepair(int iIndex, int iResult, int iNpcNumber, int iNpcIndex, int iNpcHP, int iNpcMaxHP)
{
#if (GS_CASTLE)
	PMSG_ANS_NPCREPAIR pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x06, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.iNpcNumber = iNpcNumber;
	pMsgResult.iNpcIndex = iNpcIndex;
	pMsgResult.iNpcHP = iNpcHP;
	pMsgResult.iNpcMaxHP = iNpcMaxHP;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}
// -----------------------------------------------------------------------------------------------
void CGReqNpcUpgrade(PMSG_REQ_NPCUPGRADE * lpMsg, int iIndex)
{
#if (GS_CASTLE)
// <local /*<regrel ebp-0x120>*/ /*|0x4|*/ int iGEMOFDEFEND_COUNT>
// <local /*<regrel ebp-0x124>*/ /*|0x4|*/ int x>
// <local /*<regrel ebp-0x128>*/ /*|0x4|*/ int bRETVAL>


	if( lpMsg == NULL )
		return;

	if( ::gObjIsConnected(iIndex) == FALSE )
		return;

	int iNEED_GEMOFDEFEND = 0; 
	int	iNEED_MONEY = 0;
	int	iCUR_UPLVL = 0;
	int	iNXT_UPLVL = 0; 
	int	iNXT_UPVALUE = 0;
	int	bENABLE_UPGRADE = FALSE;

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
		return;

	if( (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE) || (gObj[iIndex].GuildStatus != G_MASTER) )
	{
		CLog.LogAdd("[CastleSiege] CGReqNpcUpgrade() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else
	{
		switch( lpMsg->iNpcNumber )
		{
			case 277://Castle Gate
			{
				_CS_NPC_DATA m_NpcData;

				BOOL bResult = g_CastleSiege.GetNpcData(lpMsg->iNpcNumber, lpMsg->iNpcIndex, m_NpcData);

				if( bResult )
				{
					switch( lpMsg->iNpcUpType )
					{
						case 1:
						{
							iCUR_UPLVL = m_NpcData.m_iNPC_DF_LEVEL;
							iNXT_UPLVL = lpMsg->iNpcUpValue;

							if( (iCUR_UPLVL >= 1) && (iCUR_UPLVL <= 3) && ((iCUR_UPLVL+1) == iNXT_UPLVL) )
							{
								bENABLE_UPGRADE = TRUE;
								iNXT_UPVALUE = iNXT_UPLVL;
								iNEED_GEMOFDEFEND =  g_iNpcUpDfLevel_CGATE[(iNXT_UPLVL-1)][1];
								iNEED_MONEY = g_iNpcUpDfLevel_CGATE[(iNXT_UPLVL-1)][2];
							}
						}
						break;

						case 3:
						{
							iCUR_UPLVL = 0;

							for( int i = 0; i < 3; i++ )
							{
								if( m_NpcData.m_iNPC_MAXHP >= *g_iNpcUpMaxHP_CGATE[i] )
									iCUR_UPLVL = (i+1);
							}
							iNXT_UPLVL = 0;

							for( int i = 0; i < 3; i++ )
							{
								if( lpMsg->iNpcUpValue >= *g_iNpcUpMaxHP_CGATE[i] )
									iNXT_UPLVL = (i+1);
							}

							if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) )
							{
								bENABLE_UPGRADE = TRUE;
								iNXT_UPVALUE = g_iNpcUpMaxHP_CGATE[(iNXT_UPLVL-1)][0];
								iNEED_GEMOFDEFEND = g_iNpcUpMaxHP_CGATE[(iNXT_UPLVL-1)][1];
								iNEED_MONEY = g_iNpcUpMaxHP_CGATE[(iNXT_UPLVL-1)][2];
							}
							else
							{
								GCAnsNpcUpgrade(iIndex, 5, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
								return;
							}
						}
						break;
						default:
						{
							GCAnsNpcUpgrade(iIndex, 7, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
							return;	
						}
						break;
					}
				}
			}
			break;
			case 283://Statue
			{
				_CS_NPC_DATA m_NpcData;
				
				BOOL bResult = g_CastleSiege.GetNpcData(lpMsg->iNpcNumber, lpMsg->iNpcIndex, m_NpcData);

				if( bResult )
				{
					switch( lpMsg->iNpcUpType )
					{
						case 1:
							{
								iCUR_UPLVL = m_NpcData.m_iNPC_DF_LEVEL;
								iNXT_UPLVL = lpMsg->iNpcUpValue;

								if( (iCUR_UPLVL >= 1) && (iCUR_UPLVL <= 3) && ((iCUR_UPLVL+1) == iNXT_UPLVL) )
								{
									bENABLE_UPGRADE = TRUE;
									iNXT_UPVALUE = iNXT_UPLVL;
									iNEED_GEMOFDEFEND = g_iNpcUpDfLevel_CSTATUE[(iNXT_UPLVL-1)][1];
									iNEED_MONEY = g_iNpcUpDfLevel_CSTATUE[(iNXT_UPLVL-1)][2];
								}//if
							}//case1
							break;
						case 2:
							{
							}
							break;
						case 3:
							{
								iCUR_UPLVL = 0;

								for( int i = 0; i < 3; i++ )
								{
									if( m_NpcData.m_iNPC_MAXHP >= *g_iNpcUpMaxHP_CSTATUE[i] )
										iCUR_UPLVL = (i+1);
								}
								iNXT_UPLVL = 0;

								for( int i = 0; i < 3; i++ )
								{
									if( lpMsg->iNpcUpValue >= *g_iNpcUpMaxHP_CSTATUE[i] )
										iNXT_UPLVL = (i+1);
								}

								if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) )
								{
									bENABLE_UPGRADE = TRUE;
									iNXT_UPVALUE = g_iNpcUpMaxHP_CSTATUE[(iNXT_UPLVL-1)][0];
									iNEED_GEMOFDEFEND = g_iNpcUpMaxHP_CSTATUE[(iNXT_UPLVL-1)][1];
									iNEED_MONEY = g_iNpcUpMaxHP_CSTATUE[(iNXT_UPLVL-1)][2];
								}
								else
								{
									GCAnsNpcUpgrade(iIndex, 5, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
									return;
								}
							}
							break;
						default:
							break;
					}
				}
			}
			break;
		}
	}
#endif
}
// -----------------------------------------------------------------------------------------------
#if (GS_CASTLE)
struct PMSG_ANS_NPCUPGRADE { // <size 0x18>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iNpcNumber;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iNpcIndex;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iNpcUpType;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iNpcUpValue;
};

#endif
// -----------------------------------------------------------------------------------------------
void GCAnsNpcUpgrade(int iIndex, int iResult, int iNpcNumber, int iNpcIndex, int iNpcUpType, int iNpcUpValue)
{
#if (GS_CASTLE)
	PMSG_ANS_NPCUPGRADE pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x07, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.iNpcNumber = iNpcNumber;
	pMsgResult.iNpcIndex = iNpcIndex;
	pMsgResult.iNpcUpType = iNpcUpType;
	pMsgResult.iNpcUpValue = iNpcUpValue;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}
// -----------------------------------------------------------------------------------------------
void CGReqTaxMoneyInfo(PMSG_REQ_TAXMONEYINFO * lpMsg, int iIndex)
{
#if (GS_CASTLE)
	if( lpMsg == NULL )
		return;

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
		return;
	
	if( (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE) || (gObj[iIndex].GuildStatus != G_MASTER) )
	{
		CLog.LogAdd("[CastleSiege] CGReqTaxMoneyInfo() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else
	{
		GS_GDReqTaxInfo(g_MapServerManager.GetMapSvrGroup(), iIndex);
		CLog.LogAdd("[CastleSiege] CGReqTaxMoneyInfo() REQ OK - [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
#endif
}
// -----------------------------------------------------------------------------------------------
#if (GS_CASTLE)
struct PMSG_ANS_TAXMONEYINFO { // <size 0xf>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btTaxRateChaos;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btTaxRateStore;
  /*<thisrel this+0x7>*/ /*|0x1|*/ unsigned char btMoney1;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char btMoney2;
  /*<thisrel this+0x9>*/ /*|0x1|*/ unsigned char btMoney3;
  /*<thisrel this+0xa>*/ /*|0x1|*/ unsigned char btMoney4;
  /*<thisrel this+0xb>*/ /*|0x1|*/ unsigned char btMoney5;
  /*<thisrel this+0xc>*/ /*|0x1|*/ unsigned char btMoney6;
  /*<thisrel this+0xd>*/ /*|0x1|*/ unsigned char btMoney7;
  /*<thisrel this+0xe>*/ /*|0x1|*/ unsigned char btMoney8;
};
#endif
// -----------------------------------------------------------------------------------------------
void GCAnsTaxMoneyInfo(int iIndex, int iResult, BYTE btTaxRateChaos, BYTE btTaxRateStore, __int64 i64Money)
{
#if (GS_CASTLE)
	PMSG_ANS_TAXMONEYINFO pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x08, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btTaxRateChaos = btTaxRateChaos;
	pMsgResult.btTaxRateStore = btTaxRateStore;
	pMsgResult.btMoney1 = SET_NUMBERHW(i64Money);
	pMsgResult.btMoney2 = SET_NUMBERLW(i64Money);
	pMsgResult.btMoney3 = SET_NUMBERHW(i64Money);
	pMsgResult.btMoney4 = SET_NUMBERLW(i64Money);
	pMsgResult.btMoney5 = SET_NUMBERHW(i64Money);
	pMsgResult.btMoney6 = SET_NUMBERLW(i64Money);
	pMsgResult.btMoney7 = SET_NUMBERHW(i64Money);
	pMsgResult.btMoney8 = SET_NUMBERLW(i64Money);

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}
// -----------------------------------------------------------------------------------------------
void CGReqTaxRateChange(PMSG_REQ_TAXRATECHANGE * lpMsg, int iIndex)
{
#if (GS_CASTLE)
	if( lpMsg == NULL )
		return;

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
		return;

	if( (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE) || (gObj[iIndex].GuildStatus != G_MASTER) )
	{
		CLog.LogAdd("[CastleSiege] CGReqTaxRateChange() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else
	{
		int iMaxTaxRate = 0;

		switch( lpMsg->btTaxType )
		{
		case 1:
			iMaxTaxRate = 3;
			break;
		case 2:
			iMaxTaxRate = 3;
			break;
		case 3:
			iMaxTaxRate = 300000;
			break;
		}

		int iTaxRate = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->btTaxRate1, lpMsg->btTaxRate2), MAKE_NUMBERW(lpMsg->btTaxRate3, lpMsg->btTaxRate4)); 

		if( (iTaxRate < 0) || (iTaxRate > iMaxTaxRate) )
		{
			CLog.LogAdd("[CastleSiege] CGReqTaxRateChange() ERROR - Tax Rate Out of Range [%s][%s], Guild:(%s)(%d), TaxType:%d, TaxRate:%d",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->btTaxType, iTaxRate);
		}
		else
		{
			GS_GDReqTaxRateChange(g_MapServerManager.GetMapSvrGroup(), iIndex, lpMsg->btTaxType, iTaxRate);

			CLog.LogAdd("[CastleSiege] CGReqTaxRateChange() REQ OK - [%s][%s], Guild:(%s)(%d), TaxType:%d, TaxRate:%d",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, lpMsg->btTaxType, iTaxRate);
		}
	}
#endif
}
// -----------------------------------------------------------------------------------------------
#if (GS_CASTLE)
struct PMSG_ANS_TAXRATECHANGE {// <size 0xa>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btTaxType;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btTaxRate1;
  /*<thisrel this+0x7>*/ /*|0x1|*/ unsigned char btTaxRate2;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char btTaxRate3;
  /*<thisrel this+0x9>*/ /*|0x1|*/ unsigned char btTaxRate4;
};

#endif
// -----------------------------------------------------------------------------------------------
void GCAnsTaxRateChange(int iIndex, int iResult, BYTE btTaxType, int iTaxRate)
{
#if (GS_CASTLE)
	PMSG_ANS_TAXRATECHANGE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x09, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btTaxRate1 = SET_NUMBERHW(iTaxRate);
	pMsgResult.btTaxRate2 = SET_NUMBERLW(iTaxRate);
	pMsgResult.btTaxRate3 = SET_NUMBERHW(iTaxRate);
	pMsgResult.btTaxRate4 = SET_NUMBERLW(iTaxRate);
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}
// -----------------------------------------------------------------------------------------------
void CGReqMoneyDrawOut(PMSG_REQ_MONEYDRAWOUT * lpMsg, int iIndex)
{
#if (GS_CASTLE)
	if( lpMsg == NULL )
		return;

	if( strcmp(gObj[iIndex].GuildName, "") == 0 )
		return;
	
	if( (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE) || (gObj[iIndex].GuildStatus != G_MASTER) )
	{
		CLog.LogAdd("[CastleSiege] CGReqMoneyDrawOut() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else
	{
		#pragma message("[CGReqMoneyDrawOut][Line 15431] Need Check this")
		int iMoneyChange = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->btMoney3, lpMsg->btMoney4), MAKE_NUMBERW(lpMsg->btMoney1, lpMsg->btMoney2));

		if( iMoneyChange <= 0 )
		{
			GCAnsMoneyDrawOut(iIndex, 0, 0);
			CLog.LogAdd("[CastleSiege] CGReqMoneyDrawOut() ERROR - Req Money < 0 [%s][%s], Guild:(%s)(%d), Money:%d, TotMoney:%d",
				gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, iMoneyChange, gObj[iIndex].Money);
		}
		else
		{
			int bRETVAL = g_CastleSiege.CheckCastleHasMoney(iMoneyChange);

			if( bRETVAL == FALSE )
			{
				GCAnsMoneyDrawOut(iIndex, 0, 0);
				CLog.LogAdd("[CastleSiege] CGReqMoneyDrawOut() ERROR - Castle Money is Low [%s][%s], Guild:(%s)(%d), Money:%d, TotMoney:%d",
					gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, iMoneyChange, gObj[iIndex].Money);
			}
			else
			{
				if( (gObj[iIndex].Money + iMoneyChange) > MAX_ZEN )
				{
					GCAnsMoneyDrawOut(iIndex, 0, 0);
				}
				else
				{
					GS_GDReqCastleMoneyChange(g_MapServerManager.GetMapSvrGroup(), iIndex, iMoneyChange);
					CLog.LogAdd("[CastleSiege] CGReqMoneyDrawOut() REQ OK - [%s][%s], Guild:(%s)(%d), Money:%d, TotMoney:%d",
						gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, iMoneyChange, gObj[iIndex].Money);
				}
			}
		}
	}
#endif
}
// -----------------------------------------------------------------------------------------------
#if (GS_CASTLE)
struct PMSG_ANS_MONEYDRAWOUT {// <size 0xd>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btMoney1;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btMoney2;
  /*<thisrel this+0x7>*/ /*|0x1|*/ unsigned char btMoney3;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char btMoney4;
  /*<thisrel this+0x9>*/ /*|0x1|*/ unsigned char btMoney5;
  /*<thisrel this+0xa>*/ /*|0x1|*/ unsigned char btMoney6;
  /*<thisrel this+0xb>*/ /*|0x1|*/ unsigned char btMoney7;
  /*<thisrel this+0xc>*/ /*|0x1|*/ unsigned char btMoney8;
};

#endif
// -----------------------------------------------------------------------------------------------
void GCAnsMoneyDrawOut(int iIndex, int iResult, __int64 i64Money)
{
#if (GS_CASTLE)
	PMSG_ANS_MONEYDRAWOUT pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x10, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btMoney1 = SET_NUMBERH(SET_NUMBERHW(HI_DWORD(i64Money)));//100%
	pMsgResult.btMoney2 = SET_NUMBERL(SET_NUMBERHW(HI_DWORD(i64Money)));//100%
	pMsgResult.btMoney3 = SET_NUMBERH(SET_NUMBERLW(HI_DWORD(i64Money)));//100%
	pMsgResult.btMoney4 = SET_NUMBERL(SET_NUMBERLW(HI_DWORD(i64Money)));//100%
	pMsgResult.btMoney5 = SET_NUMBERH(SET_NUMBERHW(i64Money & 0xFFFFFFFF));//100%
	pMsgResult.btMoney6 = SET_NUMBERL(SET_NUMBERHW(i64Money & 0xFFFFFFFF));//100%
	pMsgResult.btMoney7 = SET_NUMBERH(SET_NUMBERLW(i64Money & 0xFFFFFFFF));//100%
	pMsgResult.btMoney8 = SET_NUMBERL(SET_NUMBERLW(i64Money & 0xFFFFFFFF));//100%
	
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}
// -----------------------------------------------------------------------------------------------
#if (GS_CASTLE)
struct PMSG_ANS_CSGATESTATE {// <size 0x7>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btIndex1;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btIndex2;
};

#endif
// -----------------------------------------------------------------------------------------------
void GCAnsCsGateState(int iIndex, int iResult, int iGateIndex)
{
#if (GS_CASTLE)
	PMSG_ANS_CSGATESTATE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x11, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btIndex1 = SET_NUMBERH(iGateIndex);
	pMsgResult.btIndex2 = SET_NUMBERL(iGateIndex);
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}
// -----------------------------------------------------------------------------------------------
void CGReqCsGateOperate(PMSG_REQ_CSGATEOPERATE * lpMsg, int iIndex)
{
#if (GS_CASTLE)
	if( strcmp(gObj[iIndex].GuildName, "") == 0 ) 
		return;

	if( (g_CastleSiege.GetCastleState() == 7) && (gObj[iIndex].m_btCsJoinSide != 1) )
	{
		CLog.LogAdd("[CastleSiege] CGReqCsGateOperate() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);
	}
	else if( (g_CastleSiege.CheckCastleOwnerMember(iIndex) == FALSE) && (g_CastleSiege.CheckCastleOwnerUnionMember(iIndex) == FALSE) )
	{
		CLog.LogAdd("[CastleSiege] CGReqCsGateOperate() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);	
	}
	else
	{
		int iGateIndex = MAKE_NUMBERW(lpMsg->btIndex1, lpMsg->btIndex2);

		if( g_CastleSiege.CheckCsGateAlive(iGateIndex) == TRUE ) 
		{
			int iOperateGate = g_CastleSiege.OperateGate(iGateIndex, gObj[iGateIndex].m_iCsNpcExistVal, lpMsg->btOperate);

			if( iOperateGate == 0 )
			{
				GCAnsCsGateOperate(iIndex, 0, -1, 0);
				CLog.LogAdd("[CastleSiege] CGReqCsGateOperate() ERROR - Operate Gate Failed [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);	
			}
			else
			{
				GCAnsCsGateOperate(iIndex, 1, iGateIndex, lpMsg->btOperate);
				g_CastleSiege.SendCsGateStateViewPort(iGateIndex, lpMsg->btOperate);
				CLog.LogAdd("[CastleSiege] CGReqCsGateOperate() OK - [%s][%s], Guild:(%s)(%d), DOOR:(%d)(X:%d,Y:%d)(STATUE:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus, gObj[iGateIndex].X, gObj[iGateIndex].Y, gObj[iGateIndex].m_btCsGateOpen);
			}
		}
		else
		{
			GCAnsCsGateOperate(iIndex, 2, -1, 0);
			CLog.LogAdd("[CastleSiege] CGReqCsGateOperate() ERROR - Gate Doesn't Exist [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);	
		}
	}
#endif
}
// -----------------------------------------------------------------------------------------------
#if (GS_CASTLE)
struct PMSG_ANS_CSGATEOPERATE {// <size 0x8>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btOperate;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btIndex1;
  /*<thisrel this+0x7>*/ /*|0x1|*/ unsigned char btIndex2;
};
#endif
// -----------------------------------------------------------------------------------------------
void GCAnsCsGateOperate(int iIndex, int iResult, int iGateIndex, int iGateOperate)
{
#if (GS_CASTLE)
	PMSG_ANS_CSGATEOPERATE pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x12, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btOperate = iGateOperate;
	pMsgResult.btIndex1 = SET_NUMBERH(iGateIndex);
	pMsgResult.btIndex2 = SET_NUMBERL(iGateIndex);
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}
// -----------------------------------------------------------------------------------------------
#if (GS_CASTLE)
struct PMSG_ANS_CSGATECURSTATE {// <size 0x7>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btOperate;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btIndex1;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btIndex2;
};

#endif
// -----------------------------------------------------------------------------------------------
void GCAnsCsGateCurState(int iIndex, int iGateIndex, int iGateOperate)
{
#if (GS_CASTLE)
	PMSG_ANS_CSGATECURSTATE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x13, sizeof(pMsgResult));
	pMsgResult.btOperate = iGateOperate;
	pMsgResult.btIndex1 = SET_NUMBERH(iGateIndex);
	pMsgResult.btIndex2 = SET_NUMBERL(iGateIndex);
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}
// -----------------------------------------------------------------------------------------------
#if (GS_CASTLE)
struct PMSG_ANS_NOTIFYSWITCHPROC { // <size 0x9>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btIndex1;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btIndex2;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btUserIndex1;
  /*<thisrel this+0x7>*/ /*|0x1|*/ unsigned char btUserIndex2;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char btSwitchState;
};
#endif
// -----------------------------------------------------------------------------------------------
void GCAnsCsAccessSwitchState(int iIndex, int iSwitchIndex, int iSwitchUserIndex, BYTE btSwitchState)
{
#if (GS_CASTLE)
	PMSG_ANS_NOTIFYSWITCHPROC pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x14, sizeof(pMsgResult));
	pMsgResult.btIndex1 = SET_NUMBERH(iSwitchIndex);
	pMsgResult.btIndex1 = SET_NUMBERL(iSwitchIndex);
	pMsgResult.btUserIndex1 = SET_NUMBERH(iSwitchUserIndex);
	pMsgResult.btUserIndex1 = SET_NUMBERL(iSwitchUserIndex);
	pMsgResult.btSwitchState = btSwitchState;
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}
// -----------------------------------------------------------------------------------------------
#if (GS_CASTLE)
struct PMSG_ANS_NOTIFYCROWNPROC {// <size 0xc>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btCrownState;
  /*<thisrel this+0x8>*/ /*|0x4|*/ unsigned long dwAccumulatedCrownAccessTime;
};
#endif
// -----------------------------------------------------------------------------------------------
void GCAnsCsAccessCrownState(int iIndex, BYTE btCrownState)
{
#if (GS_CASTLE)
	PMSG_ANS_NOTIFYCROWNPROC pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x15, sizeof(pMsgResult));
	
	pMsgResult.btCrownState = btCrownState;

	if(btCrownState == FALSE)
	{
		if(gObj[iIndex].m_iAccumulatedCrownAccessTime < 0 ||
			gObj[iIndex].m_iAccumulatedCrownAccessTime > 30000)
		{
			gObj[iIndex].m_iAccumulatedCrownAccessTime = 0;
		}

		CLog.LogAdd("[CastleSiege] [Reg. Attempt] Accumulated Crown AccessTime : %d [%s](%s)(%s)",gObj[iIndex].m_iAccumulatedCrownAccessTime,
				gObj[iIndex].GuildName,gObj[iIndex].AccountID,gObj[iIndex].Name);
	}

	if(btCrownState == TRUE)
	{
		CLog.LogAdd("[CastleSiege] [Reg. Success] Accumulated Crown AccessTime : %d [%s](%s)(%s)",gObj[iIndex].m_iAccumulatedCrownAccessTime,
				gObj[iIndex].GuildName,gObj[iIndex].AccountID,gObj[iIndex].Name);

		gObj[iIndex].m_iAccumulatedCrownAccessTime = 0;
	}

	if(btCrownState == 2)
	{
		gObj[iIndex].m_iAccumulatedCrownAccessTime += (GetTickCount() - g_CastleSiege.GetCrownAccessTickCount());
		int iTickCount = 5000;

		gObj[iIndex].m_iAccumulatedCrownAccessTime -= iTickCount;

		if(gObj[iIndex].m_iAccumulatedCrownAccessTime < 0 ||
			gObj[iIndex].m_iAccumulatedCrownAccessTime > 30000)
		{
			gObj[iIndex].m_iAccumulatedCrownAccessTime = 0;
		}

		CLog.LogAdd("[CastleSiege] [Reg. Attempt] Accumulated Crown AccessTime : %d [%s](%s)(%s)",gObj[iIndex].m_iAccumulatedCrownAccessTime,
				gObj[iIndex].GuildName,gObj[iIndex].AccountID,gObj[iIndex].Name);
	}

	pMsgResult.dwAccumulatedCrownAccessTime = gObj[iIndex].m_iAccumulatedCrownAccessTime;

	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}
// -----------------------------------------------------------------------------------------------
void GCAnsCsNotifyStart(int iIndex, BYTE btStartState)
{
#if (GS_CASTLE)
	PMSG_ANS_NOTIFYCSSTART pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x17, sizeof(pMsgResult));
	pMsgResult.btStartState = btStartState;
	DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
#endif
}
// -----------------------------------------------------------------------------------------------
void GCAnsCsNotifyProgress(int iIndex, BYTE btCastleSiegeState, LPCSTR lpszGuildName)
{
	if ( lpszGuildName == NULL )
		return;

	PMSG_ANS_NOTIFYCSPROGRESS pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x18, sizeof(pMsgResult));
	pMsgResult.btCastleSiegeState = btCastleSiegeState;
	memset(pMsgResult.szGuildName, 0, sizeof(pMsgResult.szGuildName));
	memcpy(pMsgResult.szGuildName, lpszGuildName, sizeof(pMsgResult.szGuildName));

	gSendProto.DataSend(iIndex, (UCHAR*)&pMsgResult, pMsgResult.h.size);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_ANS_MAPSVRTAXINFO
{
	PBMSG_HEAD2 h;	// C1:B2:1A
	BYTE btTaxType;	// 4
	BYTE btTaxRate;	// 5
};
// -----------------------------------------------------------------------------------------------
void GCAnsCsMapSvrTaxInfo(int iIndex, BYTE btTaxType, BYTE btTaxRate)
{
	PMSG_ANS_MAPSVRTAXINFO pMsgResult;
	
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1A, sizeof(pMsgResult));
	pMsgResult.btTaxType = btTaxType;
	pMsgResult.btTaxRate = btTaxRate;

	gSendProto.DataSend(iIndex, (UCHAR*)&pMsgResult, pMsgResult.h.size);

}
// -----------------------------------------------------------------------------------------------
void CGReqCsMiniMapData(PMSG_REQ_MINIMAPDATA * lpMsg, int iIndex)
{
#if (GS_CASTLE)
	if( gObjIsConnected(iIndex) == FALSE ) 
		return;

	if( (gObj[iIndex].m_btCsJoinSide > 0) || ( g_CastleSiege.CheckUnionGuildMaster(iIndex) == FALSE ) )
	{
		GCAnsCsMiniMapData(iIndex, 3);
	}
	else if( g_CastleSiege.GetCastleState() != 7 )
	{
		GCAnsCsMiniMapData(iIndex, 2);
	}
	else
	{
		g_CastleSiege.AddMiniMapDataReqUser(iIndex);
		GCAnsCsMiniMapData(iIndex, 1);
	}
#endif
}
// -----------------------------------------------------------------------------------------------
struct PMSG_ANS_MINIMAPDATA
{
	PBMSG_HEAD2 h;
	BYTE btResult;	// 4
};
// -----------------------------------------------------------------------------------------------
void GCAnsCsMiniMapData(int iIndex, BYTE btResult)
{
	PMSG_ANS_MINIMAPDATA pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1B, sizeof(pMsgResult));
	pMsgResult.btResult = btResult;

	gSendProto.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}
// -----------------------------------------------------------------------------------------------
void CGReqStopCsMiniMapData(PMSG_REQ_STOPMINIMAPDATA * lpMsg, int iIndex)
{
#if (GS_CASTLE)
	if( gObjIsConnected(iIndex) == FALSE ) 
		return;
	g_CastleSiege.DelMiniMapDataReqUser(iIndex);
#endif
}
// -----------------------------------------------------------------------------------------------
void CGReqCsSendCommand(PMSG_REQ_CSCOMMAND * lpMsg, int iIndex)
{
#if (GS_CASTLE)
	if( gObjIsConnected(iIndex) == FALSE ) 
		return;

	if( (gObj[iIndex].m_btCsJoinSide > 0) && ( g_CastleSiege.CheckUnionGuildMaster(iIndex) == TRUE ) &&	( g_CastleSiege.GetCastleState() == 7 ) )
	{
		GCAnsCsSendCommand(gObj[iIndex].m_btCsJoinSide, lpMsg->btTeam, lpMsg->btX, lpMsg->btY, lpMsg->btCommand);
	}
#endif
}
// -----------------------------------------------------------------------------------------------
void GCAnsCsSendCommand(int iCsJoinSize, BYTE btTeam, BYTE btX, BYTE btY, BYTE btCommand)
{
#if (GS_CASTLE)
	PMSG_ANS_CSCOMMAND pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1D, sizeof(pMsgResult));
	
	pMsgResult.btTeam = btTeam;
	pMsgResult.btX = btX;
	pMsgResult.btY = btY;
	pMsgResult.btCommand = btCommand;
	
	for( int iIndex = OBJ_STARTUSERINDEX; iIndex < OBJMAX; iIndex++ )
	{
		if( gObjIsConnected(iIndex) == FALSE ) continue;

		if( (gObj[iIndex].m_btCsJoinSide == iCsJoinSize) && (gObj[iIndex].MapNumber == MAP_INDEX_CASTLESIEGE) )
		{
			DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
		}
	}
#endif
}
// -----------------------------------------------------------------------------------------------
struct PMSG_ANS_CSLEFTTIMEALARM
{
	PBMSG_HEAD2 h;	// C1:B2:1E
	BYTE btHour;	// 4
	BYTE btMinute;	// 5
};
// -----------------------------------------------------------------------------------------------
void GCAnsCsLeftTimeAlarm(BYTE btHour, BYTE btMinute)
{
	PMSG_ANS_CSLEFTTIMEALARM pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1E, sizeof(pMsgResult));
	pMsgResult.btHour = btHour;
	pMsgResult.btMinute = btMinute;

	for (int iIndex = OBJ_STARTUSERINDEX;iIndex < OBJMAX;iIndex++)
	{
		if ( !gObjIsConnected(iIndex) )
		{
			continue;
		}

		gSendProto.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
	}

}
// -----------------------------------------------------------------------------------------------
void GCAnsSelfCsLeftTimeAlarm(int iIndex, BYTE btHour, BYTE btMinute)
{
	PMSG_ANS_CSLEFTTIMEALARM pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1E, sizeof(pMsgResult));
	pMsgResult.btHour = btHour;
	pMsgResult.btMinute = btMinute;

	if ( !gObjIsConnected(iIndex))
	{
		return;
	}

	gSendProto.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}
// -----------------------------------------------------------------------------------------------
void CGReqCsSetEnterHuntZone(PMSG_REQ_CSHUNTZONEENTER * lpMsg, int iIndex)
{
#if (GS_CASTLE)
	if( gObjIsConnected(iIndex) == FALSE )
		return;
	
	if( g_CastleSiege.CheckCastleOwnerMember(iIndex) == FALSE )
	{
		GCAnsCsSetEnterHuntZone(iIndex, 2, lpMsg->btHuntZoneEnter);
	}
	else if( g_CastleSiege.GetCastleState() == 7 )
	{
		GCAnsCsSetEnterHuntZone(iIndex, 0, lpMsg->btHuntZoneEnter);
	}
	else
	{
		g_CastleSiege.SetHuntZoneEnter(lpMsg->btHuntZoneEnter);
		GCAnsCsSetEnterHuntZone(iIndex, 1, lpMsg->btHuntZoneEnter);
	}

#endif
}
// -----------------------------------------------------------------------------------------------
struct PMSG_ANS_CSHUNTZONEENTER
{
	PBMSG_HEAD2 h;	// C1:B2:1F
	BYTE btResult;	// 4
	BYTE btHuntZoneEnter;	// 5
};
// -----------------------------------------------------------------------------------------------
void GCAnsCsSetEnterHuntZone(int iIndex, BYTE btResult, BYTE btEnterHuntZone)
{
	PMSG_ANS_CSHUNTZONEENTER pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1F, sizeof(pMsgResult));
	pMsgResult.btResult = btResult;
	pMsgResult.btHuntZoneEnter = btEnterHuntZone;

	gSendProto.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}
// -----------------------------------------------------------------------------------------------
void CGReqNpcDbList(PMSG_REQ_NPCDBLIST * lpMsg, int iIndex)
{
#if (GS_CASTLE)
	PMSG_ANS_NPCDBLIST pResult;

	if( lpMsg == NULL ) 
		return;
	if( strcmp(gObj[iIndex].GuildName, "") == 0 ) 
		return;
	
	if( g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].GuildName) == FALSE || ( (gObj[iIndex].GuildStatus != 128) && (gObj[iIndex].GuildStatus != 64) ) )
	{
		CLog.LogAddC(2, "[CastleSiege] CGReqNpcDbList() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].GuildName, gObj[iIndex].GuildStatus);

		pResult.h.set((LPBYTE)&pResult, 0xB3, sizeof(pResult));
		pResult.iCount = 0;
		pResult.btResult = 2;

		DataSend(iIndex, (LPBYTE)&pResult, sizeof(pResult));
	}
	else
	{
		switch( lpMsg->btMonsterCode )
		{
		case 1:
			g_CastleSiege.SendNpcStateList(iIndex, 277);
			break;
		case 2:
			g_CastleSiege.SendNpcStateList(iIndex, 283);
			break;
		}
	}
#endif
}
// -----------------------------------------------------------------------------------------------
void CGReqCsRegGuildList(PMSG_REQ_CSREGGUILDLIST * lpMsg, int iIndex)
{
#if (GS_CASTLE)
	if( lpMsg == NULL ) return;

	GS_GDReqAllGuildMarkRegInfo(g_MapServerManager.GetMapSvrGroup(), iIndex);
#endif
}
// -----------------------------------------------------------------------------------------------
#if (GS_CASTLE)

struct PMSG_ANS_CSATTKGUILDLIST { // <size 0xc>
  /*<thisrel this+0x0>*/ /*|0x4|*/ PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iCount;
};
#endif
// -----------------------------------------------------------------------------------------------
void CGReqCsAttkGuildList(PMSG_REQ_CSATTKGUILDLIST * lpMsg, int iIndex)
{
#if (GS_CASTLE)
	PMSG_CSATTKGUILDLIST* lpMsgSendBody;
	char cBUFFER[1022];
	PMSG_ANS_CSATTKGUILDLIST* lpMsgSend;
	int iCount;

	if( lpMsg == NULL )	return;
	
	lpMsgSend = (PMSG_ANS_CSATTKGUILDLIST*)(cBUFFER);
	lpMsgSendBody = (PMSG_CSATTKGUILDLIST*)(cBUFFER+sizeof(PMSG_ANS_CSATTKGUILDLIST));

	iCount = 0;

	lpMsgSend->btResult = g_CastleSiege.GetCsAttkGuildList(lpMsgSendBody, iCount);
	lpMsgSend->iCount = iCount;

	if( lpMsgSend->iCount < 0 )	 lpMsgSend->iCount = 0;
	if( lpMsgSend->iCount > 100 )lpMsgSend->iCount = 100;

	lpMsgSend->h.set((LPBYTE)lpMsgSend, 0xB5, ((lpMsgSend->iCount*sizeof(PMSG_CSATTKGUILDLIST))+sizeof(PMSG_ANS_CSATTKGUILDLIST)));
	
	DataSend(iIndex, (LPBYTE)lpMsgSend, ((lpMsgSend->iCount*sizeof(PMSG_CSATTKGUILDLIST))+sizeof(PMSG_ANS_CSATTKGUILDLIST)));
#endif
}
// -----------------------------------------------------------------------------------------------
struct PMSG_ANS_USEWEAPON
{
	PBMSG_HEAD2 h;	// C1:B7:01
	BYTE btResult;	// [1:Attacker] [2:Targets]	4
	BYTE btObjIndexH;	// 5
	BYTE btObjIndexL;	// 6
	BYTE btWeaponType;	// [1:Slingshot attack] [2:Slingshot defense]	7
	BYTE btPointX;	// 8
	BYTE btPointY;	// 9
};
// -----------------------------------------------------------------------------------------------
// Weapon Use Request
void CGReqWeaponUse(PMSG_REQ_USEWEAPON * aRecv, int iIndex)
{
	WORD wObjIndex = MAKE_NUMBERW(aRecv->btObjIndexH, aRecv->btObjIndexL);

	if ( !OBJMAX_RANGE(wObjIndex))
	{
		return;
	}

	if ( !OBJMAX_RANGE(iIndex))
	{
		return;
	}

	LPOBJ lpOwnerObj = &gObj[iIndex];
	LPOBJ lpWeaponObj = &gObj[wObjIndex];

	if ( lpOwnerObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpWeaponObj->Type != OBJ_NPC )
	{
		return;
	}

	PMSG_ANS_USEWEAPON pMsg = {0};

	C1SubHeadSet((LPBYTE)&pMsg, 0xB7, 0x01, sizeof(pMsg));
	pMsg.btResult = 1;

	if ( lpWeaponObj->Class == 221 )	// Slingshot attack
	{
		pMsg.btWeaponType = 1;
		pMsg.btObjIndexH = SET_NUMBERH(wObjIndex);
		pMsg.btObjIndexL = SET_NUMBERL(wObjIndex);
		g_CsNPC_Weapon.GetTargetPointXY(lpWeaponObj->Class, aRecv->btTargetIndex-1, pMsg.btPointX, pMsg.btPointY, TRUE);
	}
	else if( lpWeaponObj->Class == 222 )	// Slingshot defense
	{
		pMsg.btWeaponType = 2;
		pMsg.btObjIndexH = SET_NUMBERH(wObjIndex);
		pMsg.btObjIndexL = SET_NUMBERL(wObjIndex);
		g_CsNPC_Weapon.GetTargetPointXY(lpWeaponObj->Class, aRecv->btTargetIndex-1, pMsg.btPointX, pMsg.btPointY, TRUE);
	}

	gSendProto.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));

	pMsg.btResult = 2;

	for ( int n=0;n<MAX_VIEWPORT;n++)
	{
		if ( lpWeaponObj->VpPlayer2[n].type == OBJ_USER )
		{
			if ( lpWeaponObj->VpPlayer2[n].state )
			{
				if ( iIndex != lpWeaponObj->VpPlayer2[n].number )
				{
					gSendProto.DataSend(lpWeaponObj->VpPlayer2[n].number, (LPBYTE)&pMsg, sizeof(pMsg));
				}
			}
		}
	}

	gObjNotifyUseWeaponV1(lpOwnerObj, lpWeaponObj, pMsg.btPointX, pMsg.btPointY);
	g_CsNPC_Weapon.SetWeaponCalDamageInfo(wObjIndex, pMsg.btPointX, pMsg.btPointY, 10000);
}
// -----------------------------------------------------------------------------------------------
// Weapon Damage Value Request
void CGReqWeaponDamageValue(PMSG_REQ_WEAPON_DAMAGE_VALUE * aRecv, int iIndex)
{
	if ( !OBJMAX_RANGE(iIndex) )
		return;

	BYTE btTargetX = 0;
	BYTE btTargetY = 0;
	WORD wObjIndex = MAKE_NUMBERW(aRecv->btObjIndexH, aRecv->btObjIndexL);

	if ( g_CsNPC_Weapon.GetWeaponCalDamageInfo(wObjIndex, btTargetX, btTargetY) == TRUE )
	{
		gObjNotifyUseWeaponDamage(&gObj[wObjIndex], btTargetX, btTargetY);
	}
	else
	{
		MsgOutput(iIndex, lMsg.Get(MSGGET(6, 112)));
	}
}
// -----------------------------------------------------------------------------------------------
struct PMSG_ANS_GUILDMARK_OF_CASTLEOWNER
{
	PBMSG_HEAD2 h;	// C1:B9:02
	BYTE GuildMarkOfCastleOwner[32];	// 4
};
// -----------------------------------------------------------------------------------------------
void CGReqGuildMarkOfCastleOwner(PMSG_REQ_GUILDMARK_OF_CASTLEOWNER * aRecv, int iIndex)
{
	if ( !OBJMAX_RANGE(iIndex) )
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];
	char szGuildName[MAX_GUILD_LEN+1] = {0};

	memcpy(szGuildName, g_CastleSiegeSync.GetCastleOwnerGuild(), MAX_GUILD_LEN);

	if ( szGuildName )
	{
		_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild(szGuildName);

		if ( lpGuild != NULL )
		{
			PMSG_ANS_GUILDMARK_OF_CASTLEOWNER pMsg = {0};

			C1SubHeadSet((LPBYTE)&pMsg, 0xB9, 0x02, sizeof(pMsg));
			memcpy(pMsg.GuildMarkOfCastleOwner, lpGuild->Mark, sizeof(pMsg.GuildMarkOfCastleOwner));

			gSendProto.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
}
// -----------------------------------------------------------------------------------------------
// Lahap Jewel Mix Request
void CGReqJewelMix(PMSG_REQ_JEWEL_MIX * lpMsg, int iIndex)
{
	if ( lpMsg == NULL )
		return;

	LPOBJ lpObj = &gObj[iIndex];

	if( lpObj->m_IfState.use == 1 && lpObj->m_IfState.type == 1)
	{
		CLog.LogAdd("[ANTI-HACK][Lahap NPC] [Mix Jewel] (%s)(%s)", lpObj->AccountID, lpObj->Name);
		return;
	}

	CJewelMixSystem::MixJewel(iIndex, lpMsg->btJewelType, lpMsg->btJewelMix);
}
// -----------------------------------------------------------------------------------------------
struct PMSG_ANS_JEWEL_UNMIX
{
	PBMSG_HEAD2 h;	// C1:BC:[00:01]
	BYTE btResult;	// 4
};
// -----------------------------------------------------------------------------------------------
// Lahap Jewel Mix Answer
void GCAnsJewelMix(int iIndex, int iResult)
{
	PMSG_ANS_JEWEL_UNMIX pMsg;

	LPOBJ lpObj = &gObj[iIndex];

	if( lpObj->m_IfState.use == 1 && lpObj->m_IfState.type == 1)
	{
		CLog.LogAdd("[ANTI-HACK][Lahap NPC] [Jewel MIX] (%s)(%s)", lpObj->AccountID, lpObj->Name);
		return;
	}

	C1SubHeadSet((LPBYTE)&pMsg, 0xBC, 0x00, sizeof(pMsg));
	pMsg.btResult = iResult;

	gSendProto.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}
// -----------------------------------------------------------------------------------------------
// Lahap Jewel Unmix Request
void CGReqJewelUnMix(PMSG_REQ_JEWEL_UNMIX * lpMsg, int iIndex)
{
	if ( lpMsg == NULL )
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if( lpObj->m_IfState.use == 1 && lpObj->m_IfState.type == 1)
	{
		CLog.LogAdd("[ANTI-HACK][Lahap NPC] [Mix Jewel] (%s)(%s)", lpObj->AccountID, lpObj->Name);
		return;
	}
	CJewelMixSystem::UnMixJewel(iIndex, lpMsg->btJewelType, lpMsg->btJewelLevel, lpMsg->btJewelPos);
}
// -----------------------------------------------------------------------------------------------
// Lahap Jewel Unmix Answer
void GCAnsJewelUnMix(int iIndex, int iResult)
{
	PMSG_ANS_JEWEL_UNMIX pMsg;

	C1SubHeadSet((LPBYTE)&pMsg, 0xBC, 0x01, sizeof(pMsg));
	pMsg.btResult = iResult;

	gSendProto.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}
// -----------------------------------------------------------------------------------------------
void CGReqCrywolfInfo(PMSG_REQ_CRYWOLF_INFO* lpMsg, int iIndex)
{
	if ( !OBJMAX_RANGE(iIndex))
		return;

	LPOBJ lpObj = &gObj[iIndex];
	GCAnsCrywolfInfo(iIndex, g_CrywolfSync.GetOccupationState(), g_CrywolfSync.GetCrywolfState());
}
// -----------------------------------------------------------------------------------------------
void GCAnsCrywolfInfo(int iIndex, BYTE btOccupationState, BYTE btCrywolfState)
{
	PMSG_ANS_CRYWOLF_INFO pMsg={0};

	C1SubHeadSet((LPBYTE)&pMsg, 0xBD, 0x00, sizeof(pMsg));
	pMsg.btOccupationState = btOccupationState;
	pMsg.btCrywolfState = btCrywolfState;
	
	gSendProto.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}
// -----------------------------------------------------------------------------------------------
struct PMSG_ANS_CRYWOLF_ALTAR_CONTRACT
{
	PBMSG_HEAD2 h;	// C1:BD:03
	BYTE btResult;	// 4
	BYTE btAltarState;	// 5
	BYTE btObjIndexH;	// 6
	BYTE btObjIndexL;	// 7
};
// -----------------------------------------------------------------------------------------------
void CGReqAlatrContract(PMSG_REQ_CRYWOLF_ALTAR_CONTRACT* lpMsg, int iIndex)
{
	int iAltarIndex = MAKE_NUMBERW(lpMsg->btObjIndexH, lpMsg->btObjIndexL);

	if ( !OBJMAX_RANGE(iIndex))
	{
		return;
	}

	if ( !OBJMAX_RANGE(iAltarIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpAltarObj = &gObj[iAltarIndex];

	if ( !CRYWOLF_ALTAR_CLASS_RANGE(lpAltarObj->Class))
	{
		return;
	}

	if ( !gObjIsConnected(lpObj))
	{
		return;
	}

	PMSG_ANS_CRYWOLF_ALTAR_CONTRACT pMsg={0};

	C1SubHeadSet((LPBYTE)&pMsg, 0xBD, 0x03, sizeof(pMsg));
	pMsg.btResult = 0;
	pMsg.btAltarState = g_CrywolfNPC_Altar.GetAltarState(lpAltarObj->Class);
	pMsg.btObjIndexH = lpMsg->btObjIndexH;
	pMsg.btObjIndexL = lpMsg->btObjIndexL;

	if ( pMsg.btAltarState == 0 && lpObj->Type == OBJ_USER && lpObj->Class == CLASS_ELF && lpObj->Level >= MIN_ELF_LEVEL_ALTAR )
	{
		if ( g_CrywolfNPC_Altar.SetAltarUserIndex(iAltarIndex, lpAltarObj->Class, iIndex) != FALSE )
		{
			pMsg.btResult = 1;
		}
	}
	else
	{
		MsgOutput(iIndex, lMsg.Get(MSGGET(13, 9)));	// #error It says 360 when real is 260	
	}

	gSendProto.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}
// -----------------------------------------------------------------------------------------------
struct PMSG_ANS_CRYWOLF_BENEFIT_PLUS_CHAOSRATE
{
	PBMSG_HEAD2 h;	// C1:BD:09
	BYTE btPlusChaosRate;	// 4
};
// -----------------------------------------------------------------------------------------------
void CGReqPlusChaosRate(PMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE* lpMsg, int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}

	PMSG_ANS_CRYWOLF_BENEFIT_PLUS_CHAOSRATE pMsg={0};

	C1SubHeadSet((LPBYTE)&pMsg, 0xBD, 0x09, sizeof(pMsg));
	pMsg.btPlusChaosRate = g_CrywolfSync.GetPlusChaosRate();

	gSendProto.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}
// -----------------------------------------------------------------------------------------------
void CGReqKanturuStateInfo(PMSG_REQ_KANTURU_STATE_INFO* lpMsg, int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
	{
		return;
	}
	g_KanturuEntranceNPC.NotifyEntranceInfo(iIndex);
}
// -----------------------------------------------------------------------------------------------
void GCReqEnterKanturuBossMap(PMSG_REQ_ENTER_KANTURU_BOSS_MAP* lpMsg, int iIndex)
{
	if ( OBJMAX_RANGE(iIndex) == FALSE )
		return;

	g_KanturuEntranceNPC.NotifyResultEnterKanturuBossMap(iIndex);
}
// -----------------------------------------------------------------------------------------------
#if(GS_CASTLE)

void GCSendObjectCreationState(int iObjectIndex)
{
	if( !OBJMAX_RANGE(iObjectIndex) )
		return;

	LPOBJ lpObj = &gObj[iObjectIndex];
	PMSG_NOTIFY_OBJECT_CREATION_STATE pMsg = {0};

	if(lpObj->Class == 278)
	{
		C1SubHeadSet((LPBYTE)&pMsg,0xb9,0x01,sizeof(PMSG_NOTIFY_OBJECT_CREATION_STATE));
		pMsg.btObjIndexH = SET_NUMBERH(iObjectIndex);
		pMsg.btObjIndexL = SET_NUMBERL(iObjectIndex);
		pMsg.btCreationState = lpObj->m_btCreationState;
		::gSendProto.VPMsgSend(lpObj,(LPBYTE)&pMsg,sizeof(PMSG_NOTIFY_OBJECT_CREATION_STATE));
	}
}

#endif
// -----------------------------------------------------------------------------------------------
void CGReqCastleHuntZoneEntrance(PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE* aRecv, int iIndex)
{
#if(GS_CASTLE)
	if( !OBJMAX_RANGE(iIndex) )
		return;
	
	LPOBJ lpObj = &gObj[iIndex];
	
	PMSG_ANS_MOVE_TO_CASTLE_HUNTZONE pMsg = {0};
	
	pMsg.h.set((LPBYTE)&pMsg, 0xB9, 0x05, sizeof(pMsg));
	
	pMsg.btResult = 0;
	
	int iEnterTaxMoney = 0;
	
	iEnterTaxMoney = g_CastleSiegeSync.GetTaxHuntZone(lpObj->m_Index, TRUE);
	
	if( iEnterTaxMoney < 0)
	{
		iEnterTaxMoney = 0;
	}
	
	BOOL bPermission = FALSE;
	
	if( g_CastleSiege.GetHuntZoneEnter() )
	{
		bPermission = TRUE;
	}
	if( lpObj->lpGuild )
	{
		if( g_CastleSiege.CheckCastleOwnerMember(lpObj->m_Index) )
		{
			bPermission = TRUE;
		}
		
		if( g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) )
		{
			bPermission = TRUE;
		}
	}
	else
	{
		if( g_CastleSiege.GetHuntZoneEnter() )
		{
			bPermission = TRUE;
		}
	}
	if( bPermission == TRUE )
	{
		if( lpObj->Money > iEnterTaxMoney )
		{
			if( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE )
			{
				if( gObjMoveGate(lpObj->m_Index, 0x5f) == TRUE )
				{
					
					pMsg.btResult = 1;
					
					int iOldMoney = lpObj->Money;
					
					lpObj->Money -= iEnterTaxMoney;
					
					g_CastleSiegeSync.AddTributeMoney(iEnterTaxMoney);
					
					if( lpObj->Money < 0)
					{
						lpObj->Money = 0;
					}
					
					GCMoneySend(lpObj->m_Index, lpObj->Money);
					
					CLog.LogAdd("[Castle HuntZone] [%s][%s] - Entrance TAX : %d - %d = %d",
						lpObj->AccountID, lpObj->Name, iOldMoney, iEnterTaxMoney, lpObj->Money);
				}
			}
		}
	}
	gSendProto.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
#endif
}
// -----------------------------------------------------------------------------------------------
void CGWereWolfQuarrelCheck(int aIndex)
{
	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER || gObj[aIndex].Connected <= PLAYER_LOGGED )
	{
		return;
	}

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type != 12 )
	{
		return;
	}

	/*if ( gPkLimitFree == FALSE )
	{
		if ( gObj[aIndex].m_PK_Level >= 6 )
		{
			PMSG_NOTICE pNotice;
			TNotice::MakeNoticeMsgEx(&pNotice, 1, lMsg.Get(MSGGET(4, 201)));
			gSendProto.DataSend(aIndex, (LPBYTE)&pNotice, pNotice.h.size);
			return;
		}
	}*/

	if ( gObj[aIndex].PartyNumber >= 0 )
	{
		PMSG_PARTYDELUSER pMsg;
		int pindex = gParty.GetIndex(gObj[aIndex].PartyNumber, aIndex, gObj[aIndex].DBNumber);

		if ( pindex >= 0 )
		{
			pMsg.Number = pindex;
			CGPartyDelUser(&pMsg, aIndex);
		}
	}

	if (gObj[aIndex].Money < 3000000)
	{
		return;
	}

	if (gObj[aIndex].m_Quest[1] == 0xF6 || gObj[aIndex].m_Quest[1] == 0xDA || gObj[aIndex].m_Quest[1] == 0xEA) //Anti-Hack
	{
		int X = gObj[aIndex].X;
		int Y = gObj[aIndex].Y;
		if(X >= 59 && Y >= 237 && X <= 64 && Y <= 241)
		{
			gObjMoveGate(aIndex, 240);
			gObj[aIndex].Money -= 3000000;
			GCMoneySend(gObj[aIndex].m_Index, gObj[aIndex].Money);
		}
		else
		{
			//CLog.LogAdd("[Anti-HACK][WereWolf Quarrel] [%s][%s] User out of range",gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
	}
	else
	{
		//CLog.LogAdd("[Anti-Hack][WereWolf Quarrel] [%s][%s] User try to Enter Barracks without Season3 Quest State",gObj[aIndex].AccountID, gObj[aIndex].Name);
	}

}
// -----------------------------------------------------------------------------------------------
void CGGateKeeperCheck(int aIndex)
{
	if ( OBJMAX_RANGE(aIndex) == FALSE )
	{
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER || gObj[aIndex].Connected <= PLAYER_LOGGED )
	{
		return;
	}

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type != 12 )
	{
		return;
	}

	/*if ( gPkLimitFree == FALSE )
	{
		if ( gObj[aIndex].m_PK_Level >= 6 )
		{
			PMSG_NOTICE pNotice;
			TNotice::MakeNoticeMsgEx(&pNotice, 1, lMsg.Get(MSGGET(4, 201)));
			gSendProto.DataSend(aIndex, (LPBYTE)&pNotice, pNotice.h.size);
			return;
		}
	}*/

	if ( gObj[aIndex].PartyNumber >= 0 )
	{
		PMSG_PARTYDELUSER pMsg;
		int pindex = gParty.GetIndex(gObj[aIndex].PartyNumber, aIndex, gObj[aIndex].DBNumber);

		if ( pindex >= 0 )
		{
			pMsg.Number = pindex;
			CGPartyDelUser(&pMsg, aIndex);
		}
	}

	if (gObj[aIndex].m_Quest[1] == 0xDA) //Anti-Hack
	{
		int X = gObj[aIndex].X;
		int Y = gObj[aIndex].Y;
		if(X >= 119 && Y >= 166 && X <= 121 && Y <= 169) //My Range Anti-Hack
		{
			gObjMoveGate(aIndex, 241); //Refuge of Balgass Gate
		}
		else
		{
			//CLog.LogAdd("[Anti-Hack][GateKeeper] [%s][%s] User out of range",gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
	}
	else
	{
		//CLog.LogAdd("[Anti-Hack][GateKeeper] [%s][%s] User try to Enter Refugee without Season3 Quest State",gObj[aIndex].AccountID, gObj[aIndex].Name);
	}
}
// -----------------------------------------------------------------------------------------------
void DSCheckPJAccount(int aIndex)
{
	PMSG_CHARCHECK_RESULT pMsg;

	C1SubHeadSet((LPBYTE)&pMsg, 0xF3, 0x15, sizeof(pMsg));
	memcpy(pMsg.szName, gObj[aIndex].Name, MAX_ACCOUNT_LEN);
	pMsg.bIsFromOtherAccount = FALSE;

	gSendProto.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------
void NewSkillPacket(int aIndex, LPBYTE aRecv)
{
	BYTE SaveProtocol1[4];
	// -------
	SaveProtocol1[0] = aRecv[5];
	SaveProtocol1[1] = aRecv[6];
	SaveProtocol1[2] = aRecv[3];
	SaveProtocol1[3] = aRecv[4];
	aRecv[3] = SaveProtocol1[0];	
	aRecv[4] = SaveProtocol1[1];
	aRecv[5] = SaveProtocol1[2];
	aRecv[6] = SaveProtocol1[3];
	// -------
}