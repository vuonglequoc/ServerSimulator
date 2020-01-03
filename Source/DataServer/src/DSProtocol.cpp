#include "StdAfx.H"
#include "LogProc.H"
#include "Database.H"


void DSProtocolCore(DServerManager * thisManager, int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize)
{
	switch (HeadCode)
	{
		case 0x00:
		{
			DataServerLogin(thisManager, aIndex, (SDHP_SERVERINFO *)aRecv);
		}
		break;

		case 0x01:
		{		
			JGPGetCharList(thisManager, aIndex, (SDHP_GETCHARLIST *)aRecv);
		}
		break;

		case 0x04:
		{
			JGCharacterCreateRequest(thisManager, aIndex, (SDHP_CREATECHAR *)aRecv);
		}
		break;

		case 0x05:
		{
			JGCharDelRequest(thisManager, aIndex, (SDHP_CHARDELETE *)aRecv);
		}
		break;

		case 0x06:
		{
			JGGetCharacterInfo(thisManager, aIndex, (SDHP_DBCHARINFOREQUEST *)aRecv);
		}
		break;

		case 0x07:
		{
			GJSetCharacterInfo(thisManager, aIndex, (SDHP_DBCHAR_INFOSAVE *)aRecv);
		}
		break;

		case 0x8:
		{
			DGGetWarehouseList(thisManager, aIndex, (SDHP_GETWAREHOUSEDB *)aRecv);
		}
		break;		

		case 0x09:
		{
			GDSetWarehouseList(thisManager, aIndex, (SDHP_GETWAREHOUSEDB_SAVE *)aRecv);
		}
		break;

		case 0x11:
		{
			GDUserItemSave(thisManager, aIndex, (SDHP_DBCHAR_ITEMSAVE *)aRecv);
		}
		break;

		case 0x12:
		{
			GDSetWarehouseMoney(thisManager, aIndex, (SDHP_WAREHOUSEMONEY_SAVE *)aRecv);
		}
		break;

		case 0x13:
		{
			DGGetQuestMonsterKill(thisManager, aIndex, (SDHP_GETQUESTMONSTER_KILL*) aRecv);
			
		}
		break;


		case 0x14:
		{
			GDQuestMonsterKillSave(thisManager, aIndex, (SDHP_DBQUEST_KILLSAVE*) aRecv);
		}
		break;

		case 0x52:
		{
			ItemSerialCreateRecv(thisManager, aIndex, (SDHP_ITEMCREATE *)aRecv);
		}
		break;

		case 0x55:
		{
			PetItemSerialCreateRecv(thisManager, aIndex, (SDHP_ITEMCREATE *)aRecv);
		}
		break;

		case 0x56:
		{
			DGRecvPetItemInfo(thisManager, aIndex, (SDHP_REQUEST_PETITEM_INFO *)aRecv);
		}
		break;

		case 0x57:
		{
			GDSavePetItemInfo(thisManager, aIndex, (SDHP_SAVE_PETITEM_INFO *)aRecv);
		}
		break;

		case 0x60:
		{
			DGOptionDataRecv(thisManager, aIndex, (SDHP_SKILLKEYDATA *)aRecv);
		}
		break;

		case 0x80:
		{
			PMSG_DEFAULT2 * lpDef1 = (PMSG_DEFAULT2 *)aRecv;
			// ----
			switch(lpDef1->subcode)
			{
			case 0x01:
				GS_DGAnsOwnerGuildMaster(thisManager, aIndex, (CSP_REQ_OWNERGUILDMASTER*)aRecv);
				break;
			case 0x03:
				GS_DGAnsCastleNpcBuy(thisManager, aIndex, (CSP_REQ_NPCBUY *)aRecv);
				break;
			case 0x04:
				GS_DGAnsCastleNpcRepair(thisManager, aIndex, (CSP_REQ_NPCREPAIR*)aRecv);
				break;
			case 0x05:
				GS_DGAnsCastleNpcUpgrade(thisManager, aIndex, (CSP_REQ_NPCUPGRADE*)aRecv);
				break;
			case 0x06:
				GS_DGAnsTaxInfo(thisManager, aIndex, (CSP_REQ_TAXINFO*)aRecv);
				break;
			case 0x07:
				GS_DGAnsTaxRateChange(thisManager, aIndex, (CSP_REQ_TAXRATECHANGE*)aRecv);
				break;
			case 0x08:
				GS_DGAnsCastleMoneyChange(thisManager, aIndex, (CSP_REQ_MONEYCHANGE*)aRecv);
				break;
			case 0x09:
				GS_DGAnsSiegeDateChange(thisManager, aIndex, (CSP_REQ_SDEDCHANGE*)aRecv);
				break;
			case 0x0A:
				GS_DGAnsGuildMarkRegInfo(thisManager, aIndex, (CSP_REQ_GUILDREGINFO*)aRecv);
				break;
			case 0x0B:
				GS_DGAnsSiegeEndedChange(thisManager, aIndex, (CSP_REQ_SIEGEENDCHANGE*)aRecv);
				break;
			case 0x0C:
				GS_DGAnsCastleOwnerChange(thisManager, aIndex, (CSP_REQ_CASTLEOWNERCHANGE*)aRecv);
				break;
			case 0x0D:
				GS_DGAnsRegAttackGuild(thisManager, aIndex, (CSP_REQ_REGATTACKGUILD*)aRecv);
				break;
			case 0x0E:
				GS_DGAnsRestartCastleState(thisManager, aIndex, (CSP_REQ_CASTLESIEGEEND*)aRecv);
				break;
			case 0x0F:
				GS_DGAnsMapSvrMsgMultiCast(thisManager, aIndex, (CSP_REQ_MAPSVRMULTICAST*)aRecv);
				break;
			case 0x10:
				GS_DGAnsRegGuildMark(thisManager, aIndex, (CSP_REQ_GUILDREGMARK*)aRecv);
				break;
			case 0x11:
				GS_DGAnsGuildMarkReset(thisManager, aIndex, (CSP_REQ_GUILDRESETMARK*)aRecv);
				break;
			case 0x12:
				GS_DGAnsGuildSetGiveUp(thisManager, aIndex, (CSP_REQ_GUILDSETGIVEUP*)aRecv);
				break;
			case 0x16:
				GS_DGAnsNpcRemove(thisManager, aIndex, (CSP_REQ_NPCREMOVE*)aRecv);
				break;
			case 0x17:
				GS_DGAnsCastleStateSync(thisManager, aIndex, (CSP_REQ_CASTLESTATESYNC*)aRecv);
				break;
			case 0x18:
				GS_DGAnsCastleTributeMoney(thisManager, aIndex, (CSP_REQ_CASTLETRIBUTEMONEY *)aRecv);
				break;
			case 0x19:
				GS_DGAnsResetCastleTaxInfo(thisManager, aIndex, (CSP_REQ_RESETCASTLETAXINFO*)aRecv);
				break;
			case 0x1A:
				GS_DGAnsResetSiegeGuildInfo(thisManager, aIndex, (CSP_REQ_RESETSIEGEGUILDINFO*)aRecv);
				break;
			case 0x1B:
				GS_DGAnsResetRegSiegeInfo(thisManager, aIndex, (CSP_REQ_RESETREGSIEGEINFO *)aRecv);
				break;
			}
		}
		break;

		case 0x81:
		{
			GS_DGAnsCastleInitData(thisManager, aIndex, (CSP_REQ_CSINITDATA *)aRecv);
		}
		break;

		case 0x83:
		{
			GS_DGAnsAllGuildMarkRegInfo(thisManager, aIndex, (CSP_REQ_ALLGUILDREGINFO*)aRecv);
		}
		break;

		case 0x84:
		{
			GS_DGAnsFirstCreateNPC(thisManager, aIndex, (CSP_REQ_NPCSAVEDATA*)aRecv);
		}
		break;

		case 0x85:
		{
			GS_DGAnsCalcRegGuildList(thisManager, aIndex, (CSP_REQ_CALCREGGUILDLIST*)aRecv);
		}
		break;

		case 0x86:
		{
			GS_DGAnsCsGulidUnionInfo(thisManager, aIndex, (CSP_REQ_CSGUILDUNIONINFO *)aRecv);
		}
		break;

		case 0x87:
		{
			GS_DGAnsCsSaveTotalGuildInfo(thisManager, aIndex, (CSP_REQ_CSSAVETOTALGUILDINFO*)aRecv);
		}
		break;

		case 0x88:
		{
			GS_DGAnsCsLoadTotalGuildInfo(thisManager, aIndex, (CSP_REQ_CSLOADTOTALGUILDINFO*)aRecv);
		}
		break;

		case 0x89:
		{
			GS_DGAnsCastleNpcUpdate(thisManager, aIndex, (CSP_REQ_NPCUPDATEDATA*)aRecv);
		}
		break;

		case 0xB0:
		{
			DGAnsCrywolfSync(thisManager, aIndex, (CWP_REQ_CRYWOLFSYNC*)aRecv);
		}
		break;

		case 0xB1:
		{
			DGAnsCrywolfInfoLoad(thisManager, aIndex, (CWP_REQ_CRYWOLFINFOLOAD*)aRecv);
		}
		break;

		case 0xCF:
		{
			DGMoveOtherServer(thisManager, aIndex, (SDHP_CHARACTER_TRANSFER *)aRecv);
		}
		break;
	}
}
//-----------------------------------------------------------------------------------------------------------
void DataServerLogin(DServerManager * thisManager, int aIndex, SDHP_SERVERINFO * lpMsg)
{
	SDHP_RESULT pResult = {0};
	// ----
	pResult.h.Set(0x00, sizeof(pResult));
	pResult.Result = 1;
	// ----
	if(gDataBase.ExecFormat("SELECT ItemCount FROM GameServerInfo WHERE Number=0") == true)
	{
		gDataBase.Fetch();
		// ----
		pResult.ItemCount = gDataBase.GetInt("ItemCount");
	}
	gDataBase.Clear();
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------
void JGPGetCharList(DServerManager * thisManager, int aIndex, SDHP_GETCHARLIST * aRecv)
{
	char cBUFFER[10000]		= {0};
	char szAccountID[11]	= {0};
	TCHAR CharName[5][11]	= {0};
	int iCharCount			= 0;
	int lOfs				= sizeof(SDHP_CHARLISTCOUNT);
	// ----
	BYTE btInvetory[INVENTORY_BINARY_SIZE];
	// ----
	SDHP_CHARLISTCOUNT * pCount = (SDHP_CHARLISTCOUNT *)(cBUFFER);
	SDHP_CHARLIST * pCL = (SDHP_CHARLIST *)(cBUFFER + sizeof(SDHP_CHARLISTCOUNT));
	// ----
	memcpy(szAccountID, aRecv->Id, 10);
	// ----
	pCount->Number = aRecv->Number;
	// ----
	lstrcpy(pCount->AccountId, szAccountID);
	// ----
	gDataBase.ExecFormat("SELECT Id FROM AccountCharacter WHERE Id='%s'", szAccountID);
	// ----
	if(gDataBase.Fetch() == SQL_NO_DATA)
	{
		gDataBase.Clear();
		// ----
		gDataBase.ExecFormat("INSERT INTO AccountCharacter (Id) VALUES ('%s')", szAccountID);
		// ----
		gDataBase.Fetch();
	}
	// ----
	gDataBase.Clear();
	// ----
	gDataBase.ExecFormat("SELECT * FROM AccountCharacter WHERE Id='%s'", szAccountID);
	// ----
	gDataBase.Fetch();
	// ----
	pCount->MoveCnt = (BYTE)gDataBase.GetInt("MoveCnt");
	// ----
	gDataBase.GetStr("GameID1", CharName[0]);
	gDataBase.GetStr("GameID2", CharName[1]);
	gDataBase.GetStr("GameID3", CharName[2]);
	gDataBase.GetStr("GameID4", CharName[3]);
	gDataBase.GetStr("GameID5", CharName[4]);
	// ----
	gDataBase.Clear();
	// ----
	for(int i=0; i<5; i++)
	{
		if ( CharName[i][0] != 0 )
		{
			if ( lstrlen(CharName[i]) >= 4 )
			{
				if(gDataBase.ExecFormat("SELECT cLevel, Class, CtlCode, DbVersion FROM Character WHERE Name='%s'", CharName[i]) == true)
				{
					char szTemp[200];
					pCL = (SDHP_CHARLIST *)(cBUFFER + lOfs);
					// ----
					gDataBase.Fetch();
					pCL->Level = (WORD)gDataBase.GetInt("cLevel");
					pCL->Class = (BYTE)gDataBase.GetInt("Class");
					// ----
					if (pCL->Level >= 150 && pCount->Magumsa == 0)
					{
						pCount->Magumsa = 1;
					}
					if (pCL->Level >= 220 && (pCount->Magumsa == 0 || pCount->Magumsa == 1 ))
					{
						pCount->Magumsa = 2;
					}
					// ----
					if (pCL->Level >= 250 && (pCount->Magumsa == 0 || pCount->Magumsa == 1 || pCount->Magumsa == 2))
					{
						pCount->Magumsa = 3;
					}
					// ----
					pCL->CtlCode = (BYTE)gDataBase.GetInt("CtlCode");
					pCL->DbVersion = (BYTE)gDataBase.GetInt("DbVersion");
					// ----
					gDataBase.Clear();
					// ----
					sprintf(szTemp, "SELECT Inventory FROM Character WHERE Name='%s'", CharName[i]);
					// ----
					gDataBase.GetAsBinary(szTemp, btInvetory);
					gDataBase.Clear();
					// ----
					memset(pCL->dbInventory, -1, sizeof(pCL->dbInventory));
					// ----
					for(int x=0;x<12;x++)
					{
						if ( btInvetory[0+x*16] == 0xFF && (btInvetory[7+x*16] & 0x80 ) == 0x80 && (btInvetory[10+x*16] & 0xF0) == 0xF0 )
						{
							pCL->dbInventory[x*4]	= -1;
							pCL->dbInventory[x*4+1] = -1;
							pCL->dbInventory[x*4+2] = -1;
							pCL->dbInventory[x*4+3] = -1;
						}
						else
						{
							pCL->dbInventory[x*4]	 = (btInvetory[0+x*16]);	// 0..7 bits of Item
							pCL->dbInventory[x*4+1]  = (btInvetory[1+x*16]);	// Get Level of Item
							pCL->dbInventory[x*4+2]  = (btInvetory[7+x*16]);	// 8 bit     of Item
							pCL->dbInventory[x*4+3]  = (btInvetory[9+x*16]);	// 9..12 bit of Item
						}
					}
					// ----
					gDataBase.ExecFormat("SELECT G_Status FROM GuildMember WHERE Name='%s'", CharName[i]);
					// ----
					if(gDataBase.Fetch() != SQL_NO_DATA)
					{
						pCL->btGuildStatus = (BYTE)gDataBase.GetInt("G_Status");
					}
					else
					{
						pCL->btGuildStatus = 0xFF;
					}
					// ----
					//gDataBase.Clear();
					// ----
					pCL->Index = i;
					lstrcpy(pCL->Name, CharName[i]);
					// ----
					iCharCount++;
					lOfs+=sizeof(SDHP_CHARLIST);
				}
				// ----
				gDataBase.Clear();
			}
		}
	}
	// ----
	gFunc.PHeadSetW((LPBYTE)pCount, 0x01, sizeof(SDHP_CHARLISTCOUNT) + sizeof(SDHP_CHARLIST) * iCharCount);
	pCount->Count = iCharCount;
	// ----
	thisManager->DataSend(aIndex, (LPBYTE)cBUFFER, sizeof(SDHP_CHARLISTCOUNT) + sizeof(SDHP_CHARLIST) * iCharCount);
}
//-----------------------------------------------------------------------------------------------------------
void JGCharacterCreateRequest(DServerManager * thisManager, int aIndex, SDHP_CREATECHAR * aRecv)
{
	SDHP_CREATECHARRESULT pResult	= {0};
	char szAccountID[11]			= {0};
	char szName[11]					= {0};
	TCHAR CharName[5][11]			= {0};
	BYTE btInvetory[INVENTORY_BINARY_SIZE];
	char szTemp[200];
//	char Temp[200];
	BYTE QTemp[600] = {0};
	int iIndex						= 0;
	BYTE OptionData;
	// ----
	pResult.ClassSkin				= aRecv->ClassSkin;
	pResult.Number					= aRecv->Number;
	// ----
	memcpy(pResult.AccountId, aRecv->AccountId, 10);
	memcpy(szAccountID, aRecv->AccountId, 10);
	memcpy(szName, aRecv->Name, 10);
	// ----
	if(gDataBase.ExecFormat("SELECT * FROM AccountCharacter WHERE Id='%s'", szAccountID) == true)
	{
		gDataBase.Fetch();
		// ----
		gDataBase.GetStr("GameID1", CharName[0]);
		gDataBase.GetStr("GameID2", CharName[1]);
		gDataBase.GetStr("GameID3", CharName[2]);
		gDataBase.GetStr("GameID4", CharName[3]);
		gDataBase.GetStr("GameID5", CharName[4]);
		// ----
		for(iIndex = 0 ; iIndex < 5 ; iIndex++)
		{
			if(CharName[iIndex][0] == 0)
			{
				break;
			}
		}
		// ----
		if ( iIndex < 5 )
		{
			memcpy(pResult.Name, aRecv->Name, 10);
			// ----
			pResult.Pos = iIndex;
			// ----
			gDataBase.Clear();
			// ----
			if ( (pResult.ClassSkin >> 4) < 0 || (pResult.ClassSkin >> 4) >  7 /*MAX_TYPE_PLAYER*/-1 )
			{
				pResult.Result = 0;
			}
			// ----
			gDataBase.ExecFormat("WZ_CreateCharacter '%s', '%s', '%d'",szAccountID, szName, pResult.ClassSkin);
			// ----
			gDataBase.Fetch();
			// ----
			pResult.Result = gDataBase.GetInt(0);
			// ----
			gDataBase.Clear();
			// ----
			if ( pResult.Result == 1 )
			{
				gDataBase.ExecFormat("UPDATE AccountCharacter Set GameID%d='%s' WHERE Id='%s'", iIndex+1, szName, szAccountID);
				// ----
				gDataBase.Fetch();
				gDataBase.Clear();
				// ----
				gDataBase.ExecFormat("INSERT INTO GenSystem (Name,GenFamily,GenLevel,GenRanking,GenContribution,GenReward) VALUES ('%s',0,0,0,0,0)",szName);
				// ----
				gDataBase.Fetch();
				gDataBase.Clear();
				// ----
				gDataBase.ExecFormat("INSERT INTO T_MasterLevelSystem (Name,MasterLevel,MasterExp,MasterNextExp,MasterPoints) VALUES ('%s',0,0,0,0)",szName);
				// ----
				gDataBase.Fetch();
				gDataBase.Clear();
				// ----
				gDataBase.ExecFormat("UPDATE Character Set ExpandedInventory=%d WHERE Name='%s'",0,szName);
				// ----
				gDataBase.Fetch();
				gDataBase.Clear();
				// ----
				gDataBase.ExecFormat("SELECT cLevel FROM Character WHERE Name='%s'", szName);
				// ----
				gDataBase.Fetch();
				// ----
				pResult.Level = gDataBase.GetInt("cLevel");
				// ----
				gDataBase.Clear();
				// ----
				gDataBase.ExecFormat("INSERT INTO NewQuestSystem (Name,QuestInfo,TutorialReg,MercenaryReg) VALUES ('%s',0,0,0)",szName);
				// ----
				gDataBase.Fetch();
				gDataBase.Clear();
				// ----
				sprintf(szTemp, "SELECT Inventory FROM DefaultClassType WHERE Class=%d", pResult.ClassSkin);
				// ----
				gDataBase.GetAsBinary(szTemp, btInvetory);
				gDataBase.Clear();
				// ----
				memset(pResult.Equipment, -1, sizeof(pResult.Equipment));
				// ----
				for(int i=0;i<2;i++)
				{
					if ( btInvetory[0+i*16] == 0xFF )
					{
						pResult.Equipment[i*12+0] = -1;
						pResult.Equipment[i*12+1] = -1;
						pResult.Equipment[i*12+2] = -1;
						pResult.Equipment[i*12+3] = -1;
						pResult.Equipment[i*12+4] = -1;
						pResult.Equipment[i*12+5] = -1;
						pResult.Equipment[i*12+6] = -1;
						pResult.Equipment[i*12+7] = -1;
						pResult.Equipment[i*12+8] = -1;
						pResult.Equipment[i*12+9] = -1;
						pResult.Equipment[i*12+10] = -1;
						pResult.Equipment[i*12+11] = -1;
					}
					else
					{
						OptionData = (btInvetory[1+(i*16)]);
						pResult.Equipment[i*12+0] = (btInvetory[0+(i*16)]);	// Item
						pResult.Equipment[i*12+1] = ((OptionData)>>3) & 15;	// Level
						pResult.Equipment[i*12+2] = (btInvetory[2+(i*16)]);	// Durability
						pResult.Equipment[i*12+3] = -1;
						pResult.Equipment[i*12+4] = -1;
						pResult.Equipment[i*12+5] = -1;
						pResult.Equipment[i*12+6] = -1;
						pResult.Equipment[i*12+7] = -1;
						pResult.Equipment[i*12+8] = -1;
						pResult.Equipment[i*12+9] = -1;
						pResult.Equipment[i*12+10] = -1;
						pResult.Equipment[i*12+11] = -1;
					}
				}
				// ----
			}
		}
		else
		{
			pResult.Result = 2;
		}
	}
	else
	{
		pResult.Result = 2;
	}
	gDataBase.Clear();
	// ----
	pResult.h.Set(0x04, sizeof(pResult));
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------
void JGCharDelRequest(DServerManager * thisManager, int aIndex, SDHP_CHARDELETE * aRecv)
{
	SDHP_CHARDELETERESULT pResult	= {0};
	char szAccountID[11]			= {0};
	char szName[11]					= {0};
	TCHAR CharName[5][11]			= {0};
	int iIndex						= 0;
	// ----
	pResult.h.Set(0x05, sizeof(pResult));
	// ----
	pResult.Number = aRecv->Number;
	pResult.Result = 1;	
	// ----
	memcpy(pResult.AccountID, aRecv->AccountID, 10);
	memcpy(szAccountID, aRecv->AccountID, 10);
	memcpy(szName, aRecv->Name, 10);
	// ----
	if ( lstrlen(aRecv->AccountID) < 1 )
	{
		pResult.Result = 0;
	}
	// ----
	if ( lstrlen(aRecv->Name) < 1 )
	{
		pResult.Result = 0;
	}
	// ----
	if ( pResult.Result == 1 )
	{
		if ( gDataBase.ExecFormat("DELETE FROM Character WHERE AccountID='%s' AND Name='%s'", szAccountID, szName) == TRUE )
		{
			gDataBase.Fetch();
			gDataBase.Clear();
			// ----
			gDataBase.ExecFormat("SELECT * FROM AccountCharacter WHERE Id='%s'", szAccountID);
			gDataBase.Fetch();
			// ----
			gDataBase.GetStr("GameID1", CharName[0]);
			gDataBase.GetStr("GameID2", CharName[1]);
			gDataBase.GetStr("GameID3", CharName[2]);
			gDataBase.GetStr("GameID4", CharName[3]);
			gDataBase.GetStr("GameID5", CharName[4]);
			// ----
			for(iIndex = 0 ; iIndex < 5 ; iIndex++)
			{
				if ( lstrcmp(CharName[iIndex], szName) == 0)
				{
					break;
				}
			}
			// ----
			gDataBase.Clear();
			// ----
			gDataBase.ExecFormat("UPDATE AccountCharacter Set GameID%d=NULL WHERE Id='%s'", iIndex+1, szAccountID);
			gDataBase.Fetch();
			gDataBase.Clear();
			// ----
			gDataBase.ExecFormat("DELETE FROM GenSystem WHERE Name='%s'", szName);
			// ----
			gDataBase.Fetch();
			gDataBase.Clear();
			// ----
			gDataBase.ExecFormat("DELETE FROM T_MasterLevelSystem WHERE Name='%s'", szName);
			// ----
			gDataBase.Fetch();
			gDataBase.Clear();
			// ----
			gDataBase.ExecFormat("DELETE FROM NewQuestSystem WHERE Name='%s'", szName);
			// ----
			gDataBase.Fetch();
			gDataBase.Clear();
		}
		else
		{
			gDataBase.Fetch();
			gDataBase.Clear();
			// ----
			pResult.Result = 0;
		}
		
	}
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------
void JGGetCharacterInfo(DServerManager * thisManager, int aIndex, SDHP_DBCHARINFOREQUEST * aRecv)
{
	SDHP_DBCHAR_INFORESULT pResult		= {0};
	char szAccountID[11]				= {0};
	char szName[11]						= {0};
	char szTemp[256]					= {0};
	// ----
	BYTE btTemp2[256]					= {0};
	BYTE btTemp[INVENTORY_BINARY_SIZE]	= {0};
	BYTE QTemp[600];
	// ----
	gFunc.PHeadSetW((LPBYTE)&pResult, 0x06, sizeof(pResult));
	// ----
	gFunc.BuxConvert(aRecv->Name, 10);
	// ----
	memcpy(szAccountID, aRecv->AccountID, 10);
	memcpy(szName, aRecv->Name, 10);
	memcpy(pResult.AccountID, aRecv->AccountID, 10);
	memcpy(pResult.Name, aRecv->Name, 10);
	// ----
	pResult.Number = aRecv->Number;
	pResult.result = 0;
	// ----
	gDataBase.ExecFormat("SELECT cLevel, Class, LevelUpPoint, Experience, Strength, Dexterity, Vitality, Energy, Money, Life, MaxLife, Mana, MaxMana, MapNumber, MapPosX, MapPosY, MapDir, PkCount, PkLevel, PkTime, CtlCode, DbVersion, Leadership, ChatLimitTime, FruitPoint, ExpandedInventory FROM Character WHERE Name='%s'", szName);
	// ----
	if ( gDataBase.Fetch() != SQL_NO_DATA ) 
	{
		pResult.Level			= (short)gDataBase.GetInt("cLevel"); 
		pResult.Class			= (BYTE)gDataBase.GetInt("Class");
		pResult.LevelUpPoint	= gDataBase.GetInt("LevelUpPoint");
		pResult.Exp				= gDataBase.GetInt("Experience");
		pResult.Str				= (short)gDataBase.GetInt("Strength"); 
		pResult.Dex				= (short)gDataBase.GetInt("Dexterity");
		pResult.Vit				= (short)gDataBase.GetInt("Vitality");
		pResult.Energy			= (short)gDataBase.GetInt("Energy");
		pResult.Money			= gDataBase.GetInt("Money");
		pResult.Life			= (WORD)gDataBase.GetFloat("Life");
		pResult.MaxLife			= (WORD)gDataBase.GetFloat("MaxLife");
		pResult.Mana			= (WORD)gDataBase.GetFloat("Mana");
		pResult.MaxMana			= (WORD)gDataBase.GetFloat("MaxMana");
		pResult.MapNumber		= (BYTE)gDataBase.GetInt("MapNumber");
		pResult.MapX			= (BYTE)gDataBase.GetInt("MapPosX");
		pResult.MapY			= (BYTE)gDataBase.GetInt("MapPosY");
		pResult.Dir				= (BYTE)gDataBase.GetInt("MapDir");
		pResult.PkCount			= gDataBase.GetInt("PkCount");
		pResult.PkLevel			= gDataBase.GetInt("PkLevel");
		pResult.PkTime			= gDataBase.GetInt("PkTime");
		pResult.CtlCode			= (BYTE)gDataBase.GetInt("CtlCode");
		pResult.DbVersion		= (BYTE)gDataBase.GetInt("DbVersion");
		pResult.Leadership		= (WORD)gDataBase.GetInt("Leadership");
		pResult.ChatLitmitTime	= (WORD)gDataBase.GetInt("ChatLimitTime");
		pResult.iFruitPoint		= gDataBase.GetInt("FruitPoint");
		pResult.ExpandedInventory = (BYTE)gDataBase.GetInt("ExpandedInventory");
		// ----
		gDataBase.Clear();
		// ----
		sprintf(szTemp, "SELECT Inventory from Character where Name='%s'", szName);
		// ----
		if (gDataBase.GetAsBinary(szTemp, btTemp) == -1)
		{
			memset(btTemp, -1, sizeof(btTemp));
		}
		// ----
		memcpy(pResult.dbInventory, btTemp, sizeof(pResult.dbInventory));
		memset(btTemp, 0, sizeof(btTemp));
		// ----
		gDataBase.Clear();
		// ----
		sprintf(szTemp, "SELECT MagicList from Character where Name='%s'", szName);
		// ----
		if (gDataBase.GetAsBinary(szTemp, btTemp) == -1) 
		{
			memset(btTemp, -1, sizeof(btTemp));
		}
		memcpy(pResult.dbMagicList, btTemp, sizeof(pResult.dbMagicList));
		memset(btTemp, 0, sizeof(btTemp));
		// ----
		gDataBase.Clear();
		// ----
		sprintf(szTemp, "SELECT Quest from Character where Name='%s'", szName);
		// ----
		if ( gDataBase.GetAsBinary(szTemp, btTemp) == -1 ) 
		{
			memset(btTemp, -1, sizeof(btTemp));
		}
		// ----
		memcpy(pResult.dbQuest, btTemp, sizeof(pResult.dbQuest));
		memset(btTemp, 0, sizeof(btTemp));
		// ----
		gDataBase.Clear();
		// ----
		pResult.result = 1;
		// ----
		gDataBase.ExecFormat("UPDATE AccountCharacter Set GameIDC='%s' WHERE Id='%s'", szName, szAccountID);
		gDataBase.Fetch();
		gDataBase.Clear();
	}
	// ----
	gDataBase.Clear();
	// ----
	gDataBase.ExecFormat("SELECT GenFamily, GenLevel, GenRanking, GenContribution, GenReward FROM GenSystem WHERE Name='%s'", szName);
	// ----
	if ( gDataBase.Fetch() != SQL_NO_DATA ) 
	{
		pResult.GenFamily		= (BYTE)gDataBase.GetInt("GenFamily"); 
		pResult.GenLevel		= (BYTE)gDataBase.GetInt("GenLevel");
		pResult.GenRanking		= gDataBase.GetInt("GenRanking");
		pResult.GenContribution	= gDataBase.GetInt("GenContribution");
		pResult.GenReward		= (BYTE)gDataBase.GetInt("GenReward"); 
		// ----
		gDataBase.Clear();
		// ----
		pResult.result = 1;
	}
	// ----
	gDataBase.Clear();
	/// ----
	sprintf(szTemp, "SELECT QuestInfo from NewQuestSystem where Name='%s'", szName);
	// ----
	if (gDataBase.GetAsBinary(szTemp, QTemp) == -1)
	{
		memset(QTemp, -1, sizeof(QTemp));
	}
	// ----
	memcpy(pResult.Season5Quest, QTemp, sizeof(pResult.Season5Quest));
	memset(QTemp, 0, sizeof(QTemp));
	// ----
	gDataBase.Clear();
	// ----
	gDataBase.ExecFormat("SELECT TutorialReg, MercenaryReg FROM NewQuestSystem WHERE Name='%s'", szName);
	// ----
	if ( gDataBase.Fetch() != SQL_NO_DATA ) 
	{
		pResult.TutorialReg		= (BYTE)gDataBase.GetInt("TutorialReg"); 
		pResult.MercenaryReg	= (BYTE)gDataBase.GetInt("MercenaryReg");
		// ----
		gDataBase.Clear();
		// ----
		pResult.result = 1;
	}
	// ----
	gDataBase.ExecFormat("SELECT MasterLevel, MasterExp, MasterNextExp, MasterPoints FROM T_MasterLevelSystem WHERE Name='%s'", szName);
	// ----
	if ( gDataBase.Fetch() != SQL_NO_DATA ) 
	{
		pResult.MasterLevel		= gDataBase.GetInt("MasterLevel"); 
		pResult.MasterExp		= gDataBase.GetInt("MasterExp");
		pResult.MasterNextExp	= gDataBase.GetInt("MasterNextExp");
		pResult.MasterPoints	= gDataBase.GetInt("MasterPoints");
		// ----
		gDataBase.Clear();
		// ----
		pResult.result = 1;
	}
	// ----
	gDataBase.Clear();
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, sizeof(pResult));
	// ----
	if ( pResult.result == 1 )
	{
		SDHP_SKILLKEYDATA_SEND pSkillData={0};
		// ----
		pSkillData.h.Set(0x60, sizeof(pSkillData));
		pSkillData.aIndex = aRecv->Number;
		// ----
		gDataBase.ExecFormat("SELECT * FROM OptionData WHERE Name='%s'", szName);
		// ----
		if (gDataBase.Fetch() != SQL_NO_DATA )
		{
			memcpy(pSkillData.Name, aRecv->Name, 10);
			// ----
			pSkillData.GameOption = (BYTE)gDataBase.GetInt("GameOption");
			pSkillData.QkeyDefine = (BYTE)gDataBase.GetInt("QKey");
			pSkillData.WkeyDefine = (BYTE)gDataBase.GetInt("WKey");
			pSkillData.EkeyDefine = (BYTE)gDataBase.GetInt("EKey");
			pSkillData.RkeyDefine = (BYTE)gDataBase.GetInt("RKey");
			pSkillData.ChatWindow = (BYTE)gDataBase.GetInt("ChatWindow");
			// ----
			gDataBase.Clear();
			// ----
			sprintf(szTemp, "SELECT SkillKey from OptionData where Name='%s'", szName);
			// ----
			gDataBase.GetAsBinary(szTemp, btTemp2);
			// ----
			memcpy(pSkillData.SkillKeyBuffer, btTemp2, sizeof(pSkillData.SkillKeyBuffer));
		}
		// ----
		gDataBase.Clear();
		// ----
		thisManager->DataSend(aIndex, (unsigned char*)&pSkillData, sizeof(pSkillData));
	}
}
//-----------------------------------------------------------------------------------------------------------
void GJSetCharacterInfo(DServerManager * thisManager, int aIndex, SDHP_DBCHAR_INFOSAVE * aRecv)
{
	char szName[11]		= {0};
	char szTemp[128]	= {0};
	// ----
	memcpy(szName, aRecv->Name, 10);
	// ----
	gDataBase.ExecFormat("UPDATE Character Set cLevel=%d,Class=%d,LevelUpPoint=%d, Experience=%d, Strength=%d, Dexterity=%d, Vitality=%d, Energy=%d, Money=%d, Life=%d, MaxLife=%d, Mana=%d, MaxMana=%d, MapNumber=%d, MapPosX=%d, MapPosY=%d, MapDir=%d, PkCount=%d, PkLevel=%d, PkTime=%d, DbVersion=3, Leadership=%d, ChatLimitTime=%d, FruitPoint=%d, ExpandedInventory=%d WHERE Name = '%s'",
		aRecv->Level, aRecv->Class, aRecv->LevelUpPoint, aRecv->Exp, aRecv->Str, aRecv->Dex, aRecv->Vit, aRecv->Energy, aRecv->Money, aRecv->Life, aRecv->MaxLife, aRecv->Mana, aRecv->MaxMana, aRecv->MapNumber, aRecv->MapX, aRecv->MapY, aRecv->Dir, aRecv->PkCount, aRecv->PkLevel, aRecv->PkTime, aRecv->Leadership, aRecv->ChatLitmitTime, aRecv->iFruitPoint,aRecv->ExpandedInventory, szName);
	gDataBase.Fetch();
	gDataBase.Clear();
	// ----
	sprintf(szTemp, "UPDATE Character Set Inventory=? WHERE Name='%s'", szName);
	gDataBase.SetAsBinary(szTemp, aRecv->dbInventory, sizeof(aRecv->dbInventory));
	gDataBase.Fetch();
	gDataBase.Clear();
	// ----
	sprintf(szTemp, "UPDATE Character Set MagicList=? WHERE Name='%s'", szName);
	gDataBase.SetAsBinary(szTemp, aRecv->dbMagicList, sizeof(aRecv->dbMagicList));
	gDataBase.Fetch();
	gDataBase.Clear();
	// ----
	sprintf(szTemp, "UPDATE Character Set Quest=? WHERE Name='%s'", szName);
	gDataBase.SetAsBinary(szTemp, aRecv->dbQuest, sizeof(aRecv->dbQuest));
	gDataBase.Fetch();
	gDataBase.Clear();
	// ----
	gDataBase.ExecFormat("UPDATE GenSystem Set GenFamily=%d,GenLevel=%d,GenRanking=%d, GenContribution=%d, GenReward=%d WHERE Name = '%s'",
		aRecv->GenFamily, aRecv->GenLevel, aRecv->GenRanking, aRecv->GenContribution, aRecv->GenReward, szName);
	gDataBase.Fetch();
	gDataBase.Clear();
	// ----
	gDataBase.ExecFormat("UPDATE T_MasterLevelSystem Set MasterLevel=%d,MasterExp=%d,MasterNextExp=%d, MasterPoints=%d WHERE Name = '%s'",
		aRecv->MasterLevel, aRecv->MasterExp, aRecv->MasterNextExp, aRecv->MasterPoints, szName);
	gDataBase.Fetch();
	gDataBase.Clear();
	// ----
	gDataBase.ExecFormat("UPDATE NewQuestSystem Set TutorialReg=%d,MercenaryReg=%d WHERE Name = '%s'",
		aRecv->TutorialReg, aRecv->MercenaryReg, szName);
	gDataBase.Fetch();
	gDataBase.Clear();
	// ----
	sprintf(szTemp, "UPDATE NewQuestSystem Set QuestInfo=? WHERE Name='%s'", szName);
	gDataBase.SetAsBinary(szTemp, aRecv->NewQuest, sizeof(aRecv->NewQuest));
	gDataBase.Fetch();
	gDataBase.Clear();
	// ----
	if (aRecv->CharInfoSave == 1)
	{
		gDataBase.ExecFormat("INSERT INTO T_CurCharName (Name) VALUES('%s')", szName);
		gDataBase.Fetch();
		gDataBase.Clear();
	}
}
//-----------------------------------------------------------------------------------------------------------
void DGGetWarehouseList(DServerManager * thisManager, int aIndex, SDHP_GETWAREHOUSEDB * aRecv)
{
	SDHP_GETWAREHOUSEDB_SAVE pResult	= {0};
	SDHP_GETWAREHOUSEDB_RESULT pMsg		= {0};
	char szAccountID[11]				= {0};
	char szTemp[128]					= {0};
	// ----
	gFunc.PHeadSetW((LPBYTE)&pResult, 0x08, sizeof(pResult));
	pMsg.h.Set(0x10, sizeof(pMsg));
	// ----
	memcpy(szAccountID, aRecv->AccountID, 10);
	memcpy(pResult.AccountID, szAccountID, 10);
	memcpy(pMsg.AccountID, szAccountID, 10);
	// ----
	pMsg.aIndex	   = aRecv->aIndex;
	pResult.aIndex = aRecv->aIndex;
	// ----
	bool bRet = gDataBase.ExecFormat("SELECT AccountID FROM warehouse WHERE AccountID='%s'", szAccountID);
	// ----
	if ( gDataBase.Fetch() == SQL_NO_DATA || bRet == false )
	{
		gDataBase.Clear();
		gDataBase.ExecFormat("INSERT INTO warehouse (AccountID, Money, EndUseDate, DbVersion) VALUES ('%s',0, getdate(), 3)", szAccountID);
		gDataBase.Fetch();
		gDataBase.Clear();
		// ----
		thisManager->DataSend(aIndex, (unsigned char*)&pMsg, pMsg.h.Size);
		// ----
		return;
	}
	// ----
	gDataBase.Clear();
	// ----
	gDataBase.ExecFormat("SELECT Money, DbVersion, pw FROM warehouse WHERE AccountID='%s'", szAccountID);
	gDataBase.Fetch();
	// ----
	pResult.Money		= gDataBase.GetInt("Money");
	pResult.DbVersion	= gDataBase.GetInt("DbVersion");
	pResult.pw			= gDataBase.GetInt("pw");
	// ----
	gDataBase.Clear();
	// ----
	sprintf(szTemp, "SELECT Items FROM warehouse WHERE AccountID='%s'", szAccountID);
	// ----
	if ( gDataBase.GetAsBinary(szTemp, pResult.dbItems) < 0 )
	{
		memset(pResult.dbItems, -1, sizeof(pResult.dbItems));
	}
	// ----
	gDataBase.Clear();
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, sizeof(pResult));
}
//-----------------------------------------------------------------------------------------------------------
void GDSetWarehouseList(DServerManager * thisManager, int aIndex, SDHP_GETWAREHOUSEDB_SAVE * aRecv)
{
	char szAccountID[11]	= {0};
	char szTemp[128]		= {0};
	// ----
	memcpy(szAccountID, aRecv->AccountID, 10);
	// ----
	gDataBase.ExecFormat("UPDATE warehouse Set Money=%d, pw=%d, DbVersion=3 WHERE AccountID='%s'",
		aRecv->Money, aRecv->pw, szAccountID);
	gDataBase.Fetch();
	gDataBase.Clear();
	// ----
	sprintf(szTemp, "UPDATE warehouse Set Items=? WHERE AccountID='%s'", szAccountID);
	// ----
	gDataBase.SetAsBinary(szTemp, aRecv->dbItems, sizeof(aRecv->dbItems));
	gDataBase.Fetch();
	gDataBase.Clear();
}
//-----------------------------------------------------------------------------------------------------------
void DGGetQuestMonsterKill(DServerManager * thisManager, int aIndex, SDHP_GETQUESTMONSTER_KILL* aRecv)
{
	SDHP_DBQUEST_KILLSAVE pResult	= {0};
	char szName[11]					= {0};
	bool bRet						= false;
	// ----
	pResult.h.Set(0x14, sizeof(pResult));
	// ----
	memcpy(szName, aRecv->Name, 10);
	memcpy(pResult.Name, szName, 10);
	// ----
	pResult.aIndex = aRecv->aIndex;
	// ----
	bRet = gDataBase.ExecFormat("EXEC WZ_QMK_InfoLoad '%s'", aRecv->Name);
	// ----
	if (bRet == true)
	{
		if (gDataBase.Fetch() != SQL_NO_DATA)
		{
			// # Get Values from Database and store in the struct
			pResult.QuestDBIndex			= gDataBase.GetInt("QUEST_INDEX");
			pResult.QuestMonsterIndex1		= gDataBase.GetInt("MON_INDEX_1");
			pResult.QuestMonsterKillCount1	= gDataBase.GetInt("KILL_COUNT_1");
			pResult.QuestMonsterIndex2		= gDataBase.GetInt("MON_INDEX_2");
			pResult.QuestMonsterKillCount2	= gDataBase.GetInt("KILL_COUNT_2");
			pResult.QuestMonsterIndex3		= gDataBase.GetInt("MON_INDEX_3");
			pResult.QuestMonsterKillCount3	= gDataBase.GetInt("KILL_COUNT_3");
			pResult.QuestMonsterIndex4		= gDataBase.GetInt("MON_INDEX_4");
			pResult.QuestMonsterKillCount4	= gDataBase.GetInt("KILL_COUNT_4");
			pResult.QuestMonsterIndex5		= gDataBase.GetInt("MON_INDEX_5");
			pResult.QuestMonsterKillCount5	= gDataBase.GetInt("KILL_COUNT_5");
		}
	}
	// ----
	gDataBase.Clear();
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------
void GDQuestMonsterKillSave(DServerManager * thisManager, int aIndex, SDHP_DBQUEST_KILLSAVE* aRecv)
{
	gDataBase.ExecFormat("EXEC WZ_QMK_InfoSave '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", 
	// ----
	aRecv->Name, aRecv->QuestDBIndex,
		aRecv->QuestMonsterIndex1, aRecv->QuestMonsterKillCount1,
		aRecv->QuestMonsterIndex2, aRecv->QuestMonsterKillCount2,
		aRecv->QuestMonsterIndex3, aRecv->QuestMonsterKillCount3,
		aRecv->QuestMonsterIndex4, aRecv->QuestMonsterKillCount4,
		aRecv->QuestMonsterIndex5, aRecv->QuestMonsterKillCount5);
	// ----
	gDataBase.Fetch();
	gDataBase.Clear();
}
//-----------------------------------------------------------------------------------------------------------

void GDUserItemSave(DServerManager * thisManager, int aIndex, SDHP_DBCHAR_ITEMSAVE * aRecv)
{
	char szName[11]		= {0};
	char szTemp[128]	= {0};
	memcpy(szName, aRecv->Name, 10);
	// ----
	gDataBase.ExecFormat("UPDATE Character Set DbVersion=3 WHERE Name = '%s'", szName);
	gDataBase.Fetch();
	gDataBase.Clear();
	// ----
	sprintf(szTemp, "UPDATE Character Set Inventory=? WHERE Name='%s'", szName);
	// ----
	gDataBase.SetAsBinary(szTemp, aRecv->dbInventory, sizeof(aRecv->dbInventory));
	gDataBase.Fetch();
	gDataBase.Clear();
}
//-----------------------------------------------------------------------------------------------------------

void GDSetWarehouseMoney(DServerManager * thisManager, int aIndex, SDHP_WAREHOUSEMONEY_SAVE * aRecv)
{
	char szAccountID[11]	= {0};
	// ----
	memcpy(szAccountID, aRecv->AccountID, 10);
	// ----
	gDataBase.ExecFormat("UPDATE warehouse Set Money=%d WHERE AccountID='%s'", aRecv->Money, szAccountID);
	gDataBase.Fetch();
	gDataBase.Clear();
}
//-----------------------------------------------------------------------------------------------------------

void ItemSerialCreateRecv(DServerManager * thisManager, int aIndex, SDHP_ITEMCREATE * aRecv)
{
	SDHP_ITEMCREATERECV pResult = {0};
	int					Number	= 0;
	// ----
	pResult.h.Set(0x52, sizeof(pResult));
	// ----
	gDataBase.ExecFormat("EXEC WZ_GetItemSerial");
	gDataBase.Fetch();
	// ----
	Number				= gDataBase.GetInt(0);
	// ----
	gDataBase.Clear();
	// ----
	pResult.m_Number	= Number;
	pResult.aIndex		= aRecv->aIndex;
	pResult.Dur			= aRecv->Dur;
	pResult.Level		= aRecv->Level;
	pResult.lootindex	= aRecv->lootindex;
	pResult.MapNumber	= aRecv->MapNumber;
	pResult.NewOption	= aRecv->NewOption;
	pResult.Op1			= aRecv->Op1;
	pResult.Op2			= aRecv->Op2;
	pResult.Op3			= aRecv->Op3;
	pResult.SetOption	= aRecv->SetOption;
	pResult.Type		= aRecv->Type;
	pResult.x			= aRecv->x;
	pResult.y			= aRecv->y;
	pResult.Socket[0]	= aRecv->Socket[0];
	pResult.Socket[1]	= aRecv->Socket[1];
	pResult.Socket[2]	= aRecv->Socket[2];
	pResult.Socket[3]	= aRecv->Socket[3];
	pResult.Socket[4]	= aRecv->Socket[4];
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void PetItemSerialCreateRecv(DServerManager * thisManager, int aIndex, SDHP_ITEMCREATE * aRecv)
{
	SDHP_ITEMCREATERECV pResult = {0};
	int					Number	= 0;
	// ----
	pResult.h.Set(0x52, sizeof(pResult));
	// ----
	gDataBase.ExecFormat("EXEC WZ_GetItemSerial");
	gDataBase.Fetch();
	// ----
	Number = gDataBase.GetInt(0);
	// ----
	gDataBase.Clear();
	// ----
	gDataBase.ExecFormat("INSERT INTO T_PetItem_Info (ItemSerial, Pet_Level, Pet_Exp) VALUES (%d, %d, %d)",
		Number, 1, 0);
	gDataBase.Fetch();
	gDataBase.Clear();
	// ----
	pResult.m_Number	= Number;
	pResult.aIndex		= aRecv->aIndex;
	pResult.Dur			= aRecv->Dur;
	pResult.Level		= aRecv->Level;
	pResult.lootindex	= aRecv->lootindex;
	pResult.MapNumber	= aRecv->MapNumber;
	pResult.NewOption	= aRecv->NewOption;
	pResult.Op1			= aRecv->Op1;
	pResult.Op2			= aRecv->Op2;
	pResult.Op3			= aRecv->Op3;
	pResult.SetOption	= aRecv->SetOption;
	pResult.Type		= aRecv->Type;
	pResult.x			= aRecv->x;
	pResult.y			= aRecv->y;
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void DGRecvPetItemInfo(DServerManager * thisManager, int aIndex, SDHP_REQUEST_PETITEM_INFO * aRecv)
{
	int lOfs1				= sizeof(SDHP_REQUEST_PETITEM_INFO);
	int lOfs2				= sizeof(SDHP_RECV_PETITEM_INFO);
	// ----
	char szAccountID[11]	= {0};
	char cBUFFER[1000]		= {0};
	// ----
	Request_PetItem_Info	* pRequestPetInfo	= {0};
	Recv_PetItem_Info		* pRecvPetInfo		= {0};
	SDHP_RECV_PETITEM_INFO	* pRecvPetInfoCount = (SDHP_RECV_PETITEM_INFO *)(cBUFFER);
	// ----
	memcpy(szAccountID, aRecv->AccountID, 10);
	// ----
	for(int i = 0; i < aRecv->nCount ; i++)
	{
		pRequestPetInfo = (Request_PetItem_Info *)((LPBYTE)aRecv + lOfs1);
		pRecvPetInfo	= (Recv_PetItem_Info *   )((LPBYTE)cBUFFER + lOfs2);
		// ----
		gDataBase.ExecFormat("SELECT Pet_Level, Pet_Exp FROM T_PetItem_Info WHERE ItemSerial=%d",
			pRequestPetInfo->nSerial);
		// ----
		if (gDataBase.Fetch() == SQL_NO_DATA)
		{
			gDataBase.Clear();
			// ----
			gDataBase.ExecFormat("INSERT INTO T_PetItem_Info (ItemSerial, Pet_Level, Pet_Exp) VALUES (%d, %d, %d)",
				pRequestPetInfo->nSerial, 1, 0);
			gDataBase.Fetch();
			gDataBase.Clear();
			// ----
			gDataBase.ExecFormat("SELECT Pet_Level, Pet_Exp FROM T_PetItem_Info WHERE ItemSerial=%d",
				pRequestPetInfo->nSerial);
			gDataBase.Fetch();
		}
		// ----
		pRecvPetInfo->Level		= gDataBase.GetInt("Pet_Level");
		pRecvPetInfo->Exp		= gDataBase.GetInt("Pet_Level");
		pRecvPetInfo->nPos		= pRequestPetInfo->nPos;
		pRecvPetInfo->nSerial	= pRequestPetInfo->nSerial;
		// ----
		gDataBase.Clear();
		pRecvPetInfoCount->nCount++;
		// ----
		lOfs1 += sizeof(Request_PetItem_Info);
		lOfs2 += sizeof(Recv_PetItem_Info);
	}
	// ----
	gFunc.PHeadSetW((LPBYTE)pRecvPetInfoCount, 0x56, sizeof(SDHP_RECV_PETITEM_INFO) + pRecvPetInfoCount->nCount * sizeof(Recv_PetItem_Info));
	// ----
	pRecvPetInfoCount->InvenType	= aRecv->nCount;
	pRecvPetInfoCount->Number		= aRecv->Number;
	// ----
	memcpy(pRecvPetInfoCount->AccountID, szAccountID, 10);
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)cBUFFER, sizeof(SDHP_RECV_PETITEM_INFO) + pRecvPetInfoCount->nCount * sizeof(Recv_PetItem_Info));
}
//-----------------------------------------------------------------------------------------------------------

void GDSavePetItemInfo(DServerManager * thisManager, int aIndex, SDHP_SAVE_PETITEM_INFO * aRecv)
{
	int lOfs = sizeof(SDHP_SAVE_PETITEM_INFO);
	// ----
	for(int i = 0 ; i < aRecv->nCount ; i++)
	{
		Save_PetItem_Info * pSavePetInfo = (Save_PetItem_Info *)((LPBYTE)aRecv + lOfs);
		// ----
		gDataBase.ExecFormat("UPDATE T_PetItem_Info Set Pet_Level=%d, Pet_Exp=%d WHERE ItemSerial=%d",
			pSavePetInfo->Level, pSavePetInfo->Exp, pSavePetInfo->nSerial);
		gDataBase.Fetch();
		gDataBase.Clear();
	}
}
//-----------------------------------------------------------------------------------------------------------

void DGOptionDataRecv(DServerManager * thisManager, int aIndex, SDHP_SKILLKEYDATA * aRecv)
{
	char szName[11] =	{0};
	char szTemp[100]=	{0};
	// ----
	memcpy(szName, aRecv->Name, 10);
	// ----
	gDataBase.ExecFormat("SELECT Name FROM OptionData WHERE Name='%s'", szName);
	// ----
	if (gDataBase.Fetch() == SQL_NO_DATA)
	{
		gDataBase.Clear();
		gDataBase.ExecFormat("INSERT INTO OptionData (Name, GameOption, Qkey, Wkey, Ekey, Rkey, ChatWindow) VALUES ('%s', %d, %d, %d, %d, %d, %d)",
			szName, aRecv->GameOption, aRecv->QkeyDefine, aRecv->WkeyDefine, aRecv->EkeyDefine, aRecv->RkeyDefine, aRecv->ChatWindow);
		gDataBase.Fetch();
		gDataBase.Clear();
	}
	else
	{
		gDataBase.Clear();
		gDataBase.ExecFormat("UPDATE OptionData Set GameOption=%d, Qkey=%d, Wkey=%d, Ekey=%d, Rkey=%d, ChatWindow=%d WHERE Name='%s'",
			aRecv->GameOption, aRecv->QkeyDefine, aRecv->WkeyDefine, aRecv->EkeyDefine, aRecv->RkeyDefine, aRecv->ChatWindow, szName);
		gDataBase.Fetch();
		gDataBase.Clear();
	}
	// ----
	sprintf(szTemp, "UPDATE OptionData Set SkillKey=? where Name='%s'", szName);
	// ----
	gDataBase.SetAsBinary(szTemp, aRecv->SkillKeyBuffer, sizeof(aRecv->SkillKeyBuffer));
	gDataBase.Fetch();
	gDataBase.Clear();
}
//-----------------------------------------------------------------------------------------------------------

void DGMoveOtherServer(DServerManager * thisManager, int aIndex, SDHP_CHARACTER_TRANSFER * aRecv)
{
	//g_Log.LogAdd(g_Colors.Beige(), "[DSProtocol][DGMoveOtherServer] :: Error Character Transfer is not longer avaible");
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsOwnerGuildMaster(DServerManager * thisManager, int aIndex, CSP_REQ_OWNERGUILDMASTER * aRecv)
{
	CSP_ANS_OWNERGUILDMASTER pResult = {0};
	bool bRet						 = false;
	// ----
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x01, sizeof(pResult));
	// ----
	pResult.iIndex		= aRecv->iIndex;
	pResult.wMapSvrNum	= aRecv->wMapSvrNum;
	pResult.iResult		= 0;
	// ----
	bRet = gDataBase.ExecFormat("EXEC WZ_CS_GetOwnerGuildMaster %d",aRecv->wMapSvrNum);
	// ----
	if ((bRet == true) && (gDataBase.Fetch() != SQL_NO_DATA))
	{
		pResult.iResult = gDataBase.GetInt("QueryResult");
		// ----
		gDataBase.GetStr("OwnerGuild", pResult.szCastleOwnGuild);
		gDataBase.GetStr("OwnerGuildMaster", pResult.szCastleOwnGuildMaster);
	}
	// ----
	gDataBase.Clear();
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCastleNpcBuy(DServerManager * thisManager, int aIndex, CSP_REQ_NPCBUY * aRecv)
{
	CSP_ANS_NPCBUY pResult	= {0};
	bool bRet				= false;
	// ----
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x03, sizeof(pResult));
	// ----
	pResult.iBuyCost	= aRecv->iBuyCost;
	pResult.iIndex		= aRecv->iIndex;
	pResult.iNpcIndex	= aRecv->iNpcIndex;
	pResult.iNpcNumber	= aRecv->iNpcNumber;
	pResult.wMapSvrNum	= aRecv->wMapSvrNum;
	pResult.iResult		= 0;
	// ----
	bRet = gDataBase.ExecFormat("EXEC WZ_CS_ReqNpcBuy %d,%d,%d,%d,%d,%d,%d,%d,%d,%d",aRecv->wMapSvrNum,aRecv->iNpcNumber,aRecv->iNpcIndex,aRecv->iNpcDfLevel,aRecv->iNpcRgLevel,aRecv->iNpcMaxHp,aRecv->iNpcHp,aRecv->btNpcX,aRecv->btNpcY,aRecv->btNpcDIR);
	// ----
	if ((bRet == true) && (gDataBase.Fetch() != SQL_NO_DATA))
	{
		pResult.iResult = gDataBase.GetInt("QueryResult");
	}
	// ----
	gDataBase.Clear();
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCastleNpcRepair(DServerManager * thisManager, int aIndex, CSP_REQ_NPCREPAIR * aRecv)
{
	CSP_ANS_NPCREPAIR pResult	= {0};
	bool bRet					= false;
	// ----
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x04, sizeof(pResult));
	// ----
	pResult.iIndex		= aRecv->iIndex;
	pResult.iNpcIndex	= aRecv->iNpcIndex;
	pResult.iNpcNumber	= aRecv->iNpcNumber;
	pResult.iRepairCost = aRecv->iRepairCost;
	pResult.wMapSvrNum	= aRecv->wMapSvrNum;
	pResult.iResult		= 0;
	pResult.iNpcMaxHp	= 0;
	pResult.iNpcHpl		= 0;
	// ----
	bRet = gDataBase.ExecFormat("EXEC WZ_CS_ReqNpcRepair %d,%d,%d",aRecv->wMapSvrNum,aRecv->iNpcNumber,aRecv->iNpcIndex);
	// ----
	if ((bRet == true) && (gDataBase.Fetch() != SQL_NO_DATA))
	{
		pResult.iResult		= gDataBase.GetInt("QueryResult");
		pResult.iNpcHpl		= gDataBase.GetInt("NPC_HP");
		pResult.iNpcMaxHp	= gDataBase.GetInt("NPC_MAXHP");
	}
	// ----
	gDataBase.Clear();
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCastleNpcUpgrade(DServerManager * thisManager, int aIndex, CSP_REQ_NPCUPGRADE * aRecv)
{
	CSP_ANS_NPCUPGRADE pResult	= {0};
	bool bRet					= false;
	// ----
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x05, sizeof(pResult));
	// ----
	pResult.iIndex		= aRecv->iIndex;
	pResult.iNpcIndex	= aRecv->iNpcIndex;
	pResult.iNpcNumber	= aRecv->iNpcNumber;
	pResult.iNpcUpIndex = aRecv->iNpcUpIndex;
	pResult.iNpcUpType	= aRecv->iNpcUpType;
	pResult.iNpcUpValue = aRecv->iNpcUpValue;
	pResult.wMapSvrNum	= aRecv->wMapSvrNum;
	pResult.iResult		= 0;
	// ----
	bRet = gDataBase.ExecFormat("EXEC WZ_CS_ReqNpcUpgrade %d,%d,%d,%d,%d",aRecv->wMapSvrNum,aRecv->iNpcNumber,aRecv->iNpcIndex,aRecv->iNpcUpType,aRecv->iNpcUpValue);
	// ----
	if ((bRet == true) && (gDataBase.Fetch() != SQL_NO_DATA))
	{
		pResult.iResult = gDataBase.GetInt("QueryResult");
	}
	// ----
	gDataBase.Clear();
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsTaxInfo(DServerManager * thisManager, int aIndex, CSP_REQ_TAXINFO * aRecv)
{
	CSP_ANS_TAXINFO pResult	= {0};
	bool bRet				= false;
	// ----
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x06, sizeof(pResult));
	// ----
	pResult.iIndex			= aRecv->iIndex;
	pResult.wMapSvrNum		= aRecv->wMapSvrNum;
	pResult.i64CastleMoney	= 0;
	pResult.iResult			= 0;
	pResult.iTaxHuntZone	= 0;
	pResult.iTaxRateChaos	= 0;
	pResult.iTaxRateStore	= 0;
	// ----
	bRet = gDataBase.ExecFormat("EXEC WZ_CS_GetCastleTaxInfo %d",aRecv->wMapSvrNum);
	// ----
	if ((bRet == true) && (gDataBase.Fetch() != SQL_NO_DATA))
	{
		pResult.iResult			= 1;
		pResult.i64CastleMoney	= gDataBase.GetInt64("MONEY");
		pResult.iTaxRateChaos	= gDataBase.GetInt("TAX_RATE_CHAOS");
		pResult.iTaxRateStore	= gDataBase.GetInt("TAX_RATE_STORE");
		pResult.iTaxHuntZone	= gDataBase.GetInt("TAX_HUNT_ZONE");
	}
	// ----
	gDataBase.Clear();
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsTaxRateChange(DServerManager * thisManager, int aIndex, CSP_REQ_TAXRATECHANGE * aRecv)
{
	CSP_ANS_TAXRATECHANGE pResult;
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x07, sizeof(pResult));
	
	pResult.iIndex = aRecv->iIndex;
	pResult.iResult = 0;
	pResult.iTaxKind = aRecv->iTaxKind;
	pResult.iTaxRate = aRecv->iTaxRate;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_ModifyTaxRate %d,%d,%d",aRecv->wMapSvrNum,aRecv->iTaxKind,aRecv->iTaxRate);
	if (bRet == TRUE )
	{
		if(gDataBase.Fetch() != SQL_NO_DATA)
		{			
			pResult.iResult = gDataBase.GetInt("QueryResult");
			pResult.iTaxKind = gDataBase.GetInt("TaxKind");
			pResult.iTaxRate = gDataBase.GetInt("TaxRate");
		}
	}
	gDataBase.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCastleMoneyChange(DServerManager * thisManager, int aIndex, CSP_REQ_MONEYCHANGE * aRecv)
{
	CSP_ANS_MONEYCHANGE pResult;
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x08, sizeof(pResult));
	
	pResult.i64CastleMoney = 0;
	pResult.iIndex = aRecv->iIndex;
	pResult.iMoneyChanged = aRecv->iMoneyChanged;
	pResult.iResult = 0;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_ModifyMoney %d,%d",aRecv->wMapSvrNum,aRecv->iMoneyChanged);
	if (bRet == TRUE )
	{
		if(gDataBase.Fetch() != SQL_NO_DATA)
		{			
			pResult.iResult = gDataBase.GetInt("QueryResult");
			pResult.i64CastleMoney = gDataBase.GetInt64("MONEY");
		}
	}
	gDataBase.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsSiegeDateChange(DServerManager * thisManager, int aIndex, CSP_REQ_SDEDCHANGE * aRecv)
{
	CSP_ANS_SDEDCHANGE pResult;
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x09, sizeof(pResult));
	
	pResult.btEndDay = aRecv->btEndDay;
	pResult.btEndMonth = aRecv->btEndMonth;
	pResult.btStartDay = aRecv->btStartDay;
	pResult.btStartMonth = aRecv->btStartMonth;
	pResult.iIndex = aRecv->iIndex;
	pResult.iResult = 0;
	pResult.wEndYear = aRecv->wEndYear;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	pResult.wStartYear = aRecv->wStartYear;
	
	BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_ModifyCastleSchedule %d, '%d-%d-%d 00:00:00', '%d-%d-%d 00:00:00'",aRecv->wMapSvrNum,aRecv->btStartDay,aRecv->btStartMonth,aRecv->wStartYear,aRecv->btEndDay,aRecv->btEndMonth,aRecv->wEndYear);
	if (bRet == TRUE )
	{
		if(gDataBase.Fetch() != SQL_NO_DATA)
		{			
			pResult.iResult = gDataBase.GetInt("QueryResult");
		}
	}
	gDataBase.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsGuildMarkRegInfo(DServerManager * thisManager, int aIndex, CSP_REQ_GUILDREGINFO * aRecv)
{
	CSP_ANS_GUILDREGINFO pResult;
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x0A, sizeof(pResult));
	
	pResult.bIsGiveUp = 0;
	pResult.btRegRank = 0;
	pResult.iIndex = aRecv->iIndex;
	pResult.iRegMarkCount = 0;
	pResult.iResult = 0;
	memcpy(pResult.szGuildName,aRecv->szGuildName,8);
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_GetGuildMarkRegInfo %d, '%s'",aRecv->wMapSvrNum,aRecv->szGuildName);
	if (bRet == TRUE )
	{
		if(gDataBase.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = 1;
			pResult.iRegMarkCount = gDataBase.GetInt("REG_MARKS");
			pResult.btRegRank = (BYTE) gDataBase.GetInt("SEQ_NUM");
			pResult.bIsGiveUp = gDataBase.GetInt("IS_GIVEUP");
		}
	}
	gDataBase.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsSiegeEndedChange(DServerManager * thisManager, int aIndex, CSP_REQ_SIEGEENDCHANGE * aRecv)
{
	CSP_ANS_SIEGEENDCHANGE pResult;
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x0B, sizeof(pResult));
	
	pResult.bIsSiegeEnded = aRecv->bIsSiegeEnded;
	pResult.iResult = 0;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_ModifySiegeEnd %d, %d",aRecv->wMapSvrNum,aRecv->bIsSiegeEnded);
	if (bRet == TRUE )
	{
		if(gDataBase.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = gDataBase.GetInt("QueryResult");
		}
	}
	gDataBase.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCastleOwnerChange(DServerManager * thisManager, int aIndex, CSP_REQ_CASTLEOWNERCHANGE * aRecv)
{
	CSP_ANS_CASTLEOWNERCHANGE pResult;
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x0C, sizeof(pResult));
	
	pResult.bIsCastleOccupied = aRecv->bIsCastleOccupied;
	pResult.iResult = 0;
	memcpy(pResult.szOwnerGuildName,aRecv->szOwnerGuildName,8);
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_ModifyCastleOwnerInfo %d, %d, '%s'",aRecv->wMapSvrNum,aRecv->bIsCastleOccupied,aRecv->szOwnerGuildName);
	if (bRet == TRUE )
	{
		if(gDataBase.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = gDataBase.GetInt("QueryResult");
		}
	}
	gDataBase.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsRegAttackGuild(DServerManager * thisManager, int aIndex, CSP_REQ_REGATTACKGUILD * aRecv)
{
	CSP_ANS_REGATTACKGUILD pResult;
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x0D, sizeof(pResult));
	
	pResult.iIndex = aRecv->iIndex;
	pResult.iResult = 0;
	memcpy(pResult.szEnemyGuildName,aRecv->szEnemyGuildName,8);
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_ReqRegAttackGuild %d, '%s'",aRecv->wMapSvrNum,aRecv->szEnemyGuildName);
	if (bRet == TRUE )
	{
		if(gDataBase.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = gDataBase.GetInt("QueryResult");
		}
	}
	gDataBase.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsRestartCastleState(DServerManager * thisManager, int aIndex, CSP_REQ_CASTLESIEGEEND * aRecv)
{
	CSP_ANS_CASTLESIEGEEND pResult;
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x0E, sizeof(pResult));
	
	pResult.iResult = 0;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_ResetCastleSiege %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if(gDataBase.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = gDataBase.GetInt("QueryResult");
		}
	}
	gDataBase.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsMapSvrMsgMultiCast(DServerManager * thisManager, int aIndex, CSP_REQ_MAPSVRMULTICAST * aRecv)
{
	CSP_ANS_MAPSVRMULTICAST pResult;
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x0F, sizeof(pResult));
	
	memcpy(pResult.szMsgText,aRecv->szMsgText,128);
	pResult.wMapSvrNum = aRecv->wMapSvrNum;



	thisManager->DataSendAll((unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsRegGuildMark(DServerManager * thisManager, int aIndex, CSP_REQ_GUILDREGMARK * aRecv)
{
	CSP_ANS_GUILDREGMARK pResult;
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x10, sizeof(pResult));
	
	pResult.iIndex = aRecv->iIndex;
	pResult.iItemPos = aRecv->iItemPos;
	pResult.iRegMarkCount = 0;
	pResult.iResult = 0;
	memcpy(pResult.szGuildName,aRecv->szGuildName,8);
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_ReqRegGuildMark %d, '%s'",aRecv->wMapSvrNum,aRecv->szGuildName);
	if (bRet == TRUE )
	{
		if(gDataBase.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = gDataBase.GetInt("QueryResult");
			pResult.iRegMarkCount = gDataBase.GetInt("REG_MARKS");
		}
	}
	gDataBase.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsGuildMarkReset(DServerManager * thisManager, int aIndex, CSP_REQ_GUILDRESETMARK * aRecv)
{
	CSP_ANS_GUILDRESETMARK pResult;
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x11, sizeof(pResult));
	
	pResult.iIndex = aRecv->iIndex;
	pResult.iRegMarkCount = 0;
	pResult.iResult = 0;
	memcpy(pResult.szGuildName,aRecv->szGuildName,8);
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_ModifyGuildMarkReset %d, '%s'",aRecv->wMapSvrNum,aRecv->szGuildName);
	if (bRet == TRUE )
	{
		if( gDataBase.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = gDataBase.GetInt("QueryResult");
			pResult.iRegMarkCount = gDataBase.GetInt("DEL_MARKS");
		}
	}
	gDataBase.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsGuildSetGiveUp(DServerManager * thisManager, int aIndex, CSP_REQ_GUILDSETGIVEUP * aRecv)
{
	CSP_ANS_GUILDSETGIVEUP pResult;
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x12, sizeof(pResult));
	
	pResult.bIsGiveUp = aRecv->bIsGiveUp;
	pResult.iIndex = aRecv->iIndex;
	pResult.iRegMarkCount = 0;
	pResult.iResult = 0;
	memcpy(pResult.szGuildName,aRecv->szGuildName,8);
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_ModifyGuildGiveUp %d, '%s', %d",aRecv->wMapSvrNum,aRecv->szGuildName,aRecv->bIsGiveUp);
	if (bRet == TRUE )
	{
		if( gDataBase.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = gDataBase.GetInt("QueryResult");
			pResult.iRegMarkCount = gDataBase.GetInt("DEL_MARKS");
		}
	}
	gDataBase.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsNpcRemove(DServerManager * thisManager, int aIndex, CSP_REQ_NPCREMOVE * aRecv)
{
	CSP_ANS_NPCREMOVE pResult;
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x16, sizeof(pResult));
	
	pResult.iNpcIndex = aRecv->iNpcIndex;
	pResult.iNpcNumber = aRecv->iNpcNumber;
	pResult.iResult = 0;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_ReqNpcRemove %d, %d, %d",aRecv->wMapSvrNum,aRecv->iNpcNumber,aRecv->iNpcIndex);
	if (bRet == TRUE )
	{
		if( gDataBase.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = gDataBase.GetInt("QueryResult");
		}
	}
	gDataBase.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCastleStateSync(DServerManager * thisManager, int aIndex, CSP_REQ_CASTLESTATESYNC * aRecv)
{
	CSP_ANS_CASTLESTATESYNC pResult;
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x17, sizeof(pResult));
	
	pResult.iCastleState = aRecv->iCastleState;
	pResult.iTaxHuntZone = aRecv->iTaxHuntZone;
	pResult.iTaxRateChaos = aRecv->iTaxRateChaos;
	pResult.iTaxRateStore = aRecv->iTaxRateStore;
	memcpy(pResult.szOwnerGuildName,aRecv->szOwnerGuildName,8);
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	thisManager->DataSendAll((unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCastleTributeMoney(DServerManager * thisManager, int aIndex, CSP_REQ_CASTLETRIBUTEMONEY * aRecv)
{
	CSP_ANS_CASTLETRIBUTEMONEY pResult;
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x18, sizeof(pResult));
	
	pResult.iResult = 0;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = gDataBase.ExecFormat("EXEC EXEC WZ_CS_ModifyMoney %d, %d",aRecv->wMapSvrNum,aRecv->iCastleTributeMoney);
	if (bRet == TRUE )
	{
		if( gDataBase.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = gDataBase.GetInt("QueryResult");
		}
	}
	gDataBase.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsResetCastleTaxInfo(DServerManager * thisManager, int aIndex, CSP_REQ_RESETCASTLETAXINFO * aRecv)
{
	CSP_ANS_RESETCASTLETAXINFO pResult;
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x19, sizeof(pResult));
	
	pResult.iResult = 0;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_ResetCastleTaxInfo %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if( gDataBase.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = gDataBase.GetInt("QueryResult");
		}
	}
	gDataBase.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsResetSiegeGuildInfo(DServerManager * thisManager, int aIndex, CSP_REQ_RESETSIEGEGUILDINFO * aRecv)
{
	CSP_ANS_RESETSIEGEGUILDINFO pResult;
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x1A, sizeof(pResult));
	
	pResult.iResult = 0;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_ResetSiegeGuildInfo %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if( gDataBase.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = gDataBase.GetInt("QueryResult");
		}
	}
	gDataBase.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsResetRegSiegeInfo(DServerManager * thisManager, int aIndex, CSP_REQ_RESETREGSIEGEINFO * aRecv)
{
	CSP_ANS_RESETREGSIEGEINFO pResult;
	gFunc.PHeadSubSetB((LPBYTE)&pResult, 0x80,0x1B, sizeof(pResult));
	
	pResult.iResult = 0;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_ResetRegSiegeInfo %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if( gDataBase.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = gDataBase.GetInt("QueryResult");
		}
	}
	gDataBase.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCastleInitData(DServerManager * thisManager, int aIndex, CSP_REQ_CSINITDATA * aRecv)
{
	BOOL bRet = FALSE;
	int Counter = 0;

	
	char cBUFFER[10000]={0};

	CSP_ANS_CSINITDATA * pResult = (CSP_ANS_CSINITDATA *)(cBUFFER);
	CSP_CSINITDATA * NPCInfo = (CSP_CSINITDATA *)(cBUFFER + sizeof(CSP_ANS_CSINITDATA));
	
	pResult->btEndDay = 0;
	pResult->btEndMonth = 0;
	pResult->btIsCastleOccupied = 0;
	pResult->btIsSiegeEnded = 0;
	pResult->btIsSiegeGuildList = 0;
	pResult->btStartDay = 0;
	pResult->btStartMonth = 0;
	pResult->i64CastleMoney = 0;
	pResult->iCount = 0;
	pResult->iFirstCreate = 0;
	pResult->iResult  = 0;
	pResult->iTaxHuntZone = 0;
	pResult->iTaxRateChaos = 0;
	pResult->iTaxRateStore = 0;
	memset(pResult->szCastleOwnGuild,0,8);
	//memcpy(pResult->szCastleOwnGuild,0,8);
	pResult->wEndYear = 0;
	pResult->wMapSvrNum = aRecv->wMapSvrNum;
	pResult->wStartYear = 0;
	
	int lOfs=sizeof(CSP_ANS_CSINITDATA);

	bRet = gDataBase.ExecFormat("EXEC WZ_CS_GetCastleTotalInfo %d,%d",aRecv->wMapSvrNum,aRecv->iCastleEventCycle);
	if (bRet == TRUE )
	{
		if( gDataBase.Fetch() != SQL_NO_DATA)
		{	
			pResult->iResult = 1;
			pResult->btEndDay = gDataBase.GetInt("EDAY");
			pResult->btEndMonth = gDataBase.GetInt("EMONTH");
			pResult->btIsCastleOccupied = gDataBase.GetInt("CASLE_OCCUPY");
			pResult->btIsSiegeEnded = gDataBase.GetInt("SIEGE_ENDED");
			pResult->btIsSiegeGuildList = gDataBase.GetInt("SIEGE_GUILDLIST_SETTED");
			pResult->btStartDay = gDataBase.GetInt("SDAY");
			pResult->btStartMonth = gDataBase.GetInt("SMONTH");
			pResult->i64CastleMoney = gDataBase.GetInt64("MONEY");
			pResult->iFirstCreate = gDataBase.GetInt("FIRST_CREATE");
			pResult->iTaxHuntZone = gDataBase.GetInt("TAX_RATE_ZONE");
			pResult->iTaxRateChaos = gDataBase.GetInt("TAX_RATE_CHAOS");
			pResult->iTaxRateStore = gDataBase.GetInt("TAX_RATE_STORE");
			gDataBase.GetStr("OWNER_GUILD", pResult->szCastleOwnGuild);
			pResult->wEndYear = gDataBase.GetInt("EYEAR");
			pResult->wStartYear = gDataBase.GetInt("SYEAR");
			gDataBase.Clear();

			bRet = gDataBase.ExecFormat("EXEC WZ_CS_GetCastleNpcInfo %d",aRecv->wMapSvrNum);
			if (bRet == TRUE )
			{
				if( gDataBase.Fetch() != SQL_NO_DATA)
				{
					do
					{
						NPCInfo = (CSP_CSINITDATA *)(cBUFFER + lOfs);
						NPCInfo->iNpcNumber = gDataBase.GetInt("NPC_NUMBER");
						NPCInfo->iNpcIndex = gDataBase.GetInt("NPC_INDEX");
						NPCInfo->iNpcDfLevel = gDataBase.GetInt("NPC_DF_LEVEL");
						NPCInfo->iNpcRgLevel = gDataBase.GetInt("NPC_RG_LEVEL");
						NPCInfo->iNpcMaxHp = gDataBase.GetInt("NPC_MAXHP");
						NPCInfo->iNpcHp = gDataBase.GetInt("NPC_HP");
						NPCInfo->btNpcX = gDataBase.GetInt("NPC_X");
						NPCInfo->btNpcY = gDataBase.GetInt("NPC_Y");
						NPCInfo->btNpcDIR = gDataBase.GetInt("NPC_DIR");
						lOfs+=sizeof(CSP_CSINITDATA);
						Counter++;
					}while(Counter < 200 && gDataBase.Fetch() != SQL_NO_DATA);
				}
			}
		}
	}
	gDataBase.Clear();
	gFunc.PHeadSetW((LPBYTE)pResult, 0x81, sizeof(CSP_ANS_CSINITDATA) + sizeof(CSP_CSINITDATA) * Counter);
	pResult->iCount = Counter;
	thisManager->DataSend(aIndex, (unsigned char*)cBUFFER, sizeof(CSP_ANS_CSINITDATA) + sizeof(CSP_CSINITDATA) * Counter);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsAllGuildMarkRegInfo(DServerManager * thisManager, int aIndex, CSP_REQ_ALLGUILDREGINFO * aRecv)
{
	char cBUFFER[10000]={0};

	CSP_ANS_ALLGUILDREGINFO * pResult = (CSP_ANS_ALLGUILDREGINFO *)(cBUFFER);
	CSP_GUILDREGINFO * GuildInfo = (CSP_GUILDREGINFO *)(cBUFFER + sizeof(CSP_ANS_ALLGUILDREGINFO));
	int Counter = 0;

	pResult->iIndex = aRecv->iIndex;
	pResult->iResult = 1;
	pResult->wMapSvrNum = aRecv->wMapSvrNum;
	GuildInfo->iRegMarkCount = 0;
	GuildInfo->btRegRank = 0;
	GuildInfo->bIsGiveUp = 0;
	memset(GuildInfo->szGuildName,0,8);
	//memcpy(GuildInfo->szGuildName,0,8);
	
	int lOfs=sizeof(CSP_ANS_ALLGUILDREGINFO);

	BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_GetAllGuildMarkRegInfo %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if( gDataBase.Fetch() != SQL_NO_DATA)
		{
			do
			{
				GuildInfo = (CSP_GUILDREGINFO *)(cBUFFER + lOfs);
				char tmp[8]={0};
				GuildInfo->bIsGiveUp = gDataBase.GetInt("IS_GUIVEUP");
				GuildInfo->btRegRank = (BYTE) gDataBase.GetInt("SEQ_NUM");
				GuildInfo->iRegMarkCount = gDataBase.GetInt("REG_MARKS");
				gDataBase.GetStr("REG_SIEGE_GUILD", tmp);
				memcpy(GuildInfo->szGuildName,tmp,8);
				Counter++;
				lOfs+=sizeof(CSP_GUILDREGINFO);
			}while(Counter < 100 && gDataBase.Fetch() != SQL_NO_DATA);
		}
	}
	gDataBase.Clear();
	gFunc.PHeadSetW((LPBYTE)pResult, 0x83, sizeof(CSP_ANS_ALLGUILDREGINFO) + sizeof(CSP_GUILDREGINFO) * Counter);
	pResult->iCount = Counter;
	thisManager->DataSend(aIndex, (unsigned char*)cBUFFER, sizeof(CSP_ANS_ALLGUILDREGINFO) + sizeof(CSP_GUILDREGINFO) * Counter);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsFirstCreateNPC(DServerManager * thisManager, int aIndex, CSP_REQ_NPCSAVEDATA * aRecv)
{
	int Counter = 0;
	BOOL bRet = FALSE;
	CSP_ANS_NPCSAVEDATA pResult;
	pResult.h.Set(0x84, sizeof(pResult));
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	pResult.iResult = 0;

	int lOfs=sizeof(CSP_REQ_NPCSAVEDATA);

	bRet = gDataBase.ExecFormat("DELETE FROM MuCastle_NPC WHERE MAP_SVR_GROUP = %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if(aRecv->iCount > 0)
		{
			while(Counter<aRecv->iCount)
			{
				gDataBase.Clear();
				CSP_NPCSAVEDATA * NPCInfo = (CSP_NPCSAVEDATA*)((LPBYTE)aRecv + lOfs);
				bRet = gDataBase.ExecFormat("EXEC WZ_CS_ReqNpcBuy %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",aRecv->wMapSvrNum,NPCInfo->iNpcNumber,NPCInfo->iNpcIndex,NPCInfo->iNpcDfLevel,NPCInfo->iNpcRgLevel,NPCInfo->iNpcMaxHp,NPCInfo->iNpcHp,NPCInfo->btNpcX,NPCInfo->btNpcY,NPCInfo->btNpcDIR);
				if (bRet != TRUE )
				{
					gDataBase.Clear();
					break;
				}
				Counter++;
				lOfs+=sizeof(CSP_NPCSAVEDATA);
			}
			pResult.iResult = 1;
		}
	}
	gDataBase.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult,sizeof(pResult));
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCalcRegGuildList(DServerManager * thisManager, int aIndex, CSP_REQ_CALCREGGUILDLIST * aRecv)
{
	char cBUFFER[10000]={0};

	CSP_ANS_CALCREGGUILDLIST * pResult = (CSP_ANS_CALCREGGUILDLIST *)(cBUFFER);
	CSP_CALCREGGUILDLIST * GuildInfo = (CSP_CALCREGGUILDLIST *)(cBUFFER + sizeof(CSP_ANS_CALCREGGUILDLIST));
	int Counter = 0;

	pResult->iResult = 1;
	pResult->wMapSvrNum = aRecv->wMapSvrNum;
	GuildInfo->iGuildMasterLevel = 0;
	GuildInfo->iGuildMemberCount = 0;
	GuildInfo->iRegMarkCount = 0;
	GuildInfo->iSeqNum = 0;
	memset(GuildInfo->szGuildName,0,8);
	//memcpy(GuildInfo->szGuildName,0,8);
	
	int lOfs=sizeof(CSP_ANS_CALCREGGUILDLIST);

	BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_GetCalcRegGuildList %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if( gDataBase.Fetch() != SQL_NO_DATA)
		{
			do
			{
				GuildInfo = (CSP_CALCREGGUILDLIST *)(cBUFFER + lOfs);
				char tmp[8]={0};
				GuildInfo->iGuildMasterLevel = gDataBase.GetInt("GM_LEVEL");
				GuildInfo->iGuildMemberCount = gDataBase.GetInt("GUILD_MEMBER");
				GuildInfo->iRegMarkCount = gDataBase.GetInt("REG_MARKS");
				GuildInfo->iSeqNum = gDataBase.GetInt("SEQ_NUM");
				gDataBase.GetStr("REG_SIEGE_GUILD", tmp);
				memcpy(GuildInfo->szGuildName,tmp,8);
				Counter++;
				lOfs+=sizeof(CSP_CALCREGGUILDLIST);
			}while(Counter < 100 && gDataBase.Fetch() != SQL_NO_DATA);
		}
	}
	gDataBase.Clear();
	gFunc.PHeadSetW((LPBYTE)pResult, 0x85, sizeof(CSP_ANS_CALCREGGUILDLIST) + sizeof(CSP_CALCREGGUILDLIST) * Counter);
	pResult->iCount = Counter;
	thisManager->DataSend(aIndex, (unsigned char*)cBUFFER, sizeof(CSP_ANS_CALCREGGUILDLIST) + sizeof(CSP_CALCREGGUILDLIST) * Counter);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCsGulidUnionInfo(DServerManager * thisManager, int aIndex, CSP_REQ_CSGUILDUNIONINFO * aRecv)
{
	int Counter = 0;
	int GuildCount = 0;
	char cBUFFER[10000]={0};

	CSP_ANS_CSGUILDUNIONINFO * pResult = (CSP_ANS_CSGUILDUNIONINFO *)(cBUFFER);
	CSP_CSGUILDUNIONINFO * GuildInfo = (CSP_CSGUILDUNIONINFO *)(cBUFFER + sizeof(CSP_ANS_CSGUILDUNIONINFO));

	pResult->h.Set(0x86, sizeof(pResult));
	pResult->wMapSvrNum = aRecv->wMapSvrNum;
	pResult->iResult = 0;
	pResult->iCount = 0;

	int lOfs=sizeof(CSP_REQ_CSSAVETOTALGUILDINFO);
	int ROfs=lOfs;

	if(aRecv->iCount > 0)
	{
		while(Counter<aRecv->iCount)
		{
			CSP_CSGUILDUNIONINFO * GuildRecvInfo = (CSP_CSGUILDUNIONINFO*)((LPBYTE)aRecv + lOfs);
			BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_GetCsGuildUnionInfo '%s'",GuildRecvInfo->szGuildName);
			
			if(( gDataBase.Fetch() != SQL_NO_DATA) && (bRet != TRUE ))
			{
				gDataBase.Clear();
				break;
			}
			
			do
			{
				GuildInfo = (CSP_CSGUILDUNIONINFO *)(cBUFFER + ROfs);
				char tmp[8]={0};
				gDataBase.GetStr("GUILD_NAME", tmp);
				memcpy(GuildInfo->szGuildName,tmp,8);
				GuildInfo->iCsGuildID = GuildCount;
				GuildCount++;
				ROfs+=sizeof(CSP_CALCREGGUILDLIST);
			}while(GuildCount < 100 && gDataBase.Fetch() != SQL_NO_DATA);
			gDataBase.Clear();

			Counter++;
			lOfs+=sizeof(CSP_CSGUILDUNIONINFO);
		}
		pResult->iResult = 1;
	}
	gFunc.PHeadSetW((LPBYTE)pResult, 0x86, sizeof(CSP_ANS_CSGUILDUNIONINFO) + sizeof(CSP_CSGUILDUNIONINFO) * GuildCount);
	pResult->iCount = Counter;
	thisManager->DataSend(aIndex, (unsigned char*)cBUFFER, sizeof(CSP_ANS_CSGUILDUNIONINFO) + sizeof(CSP_CSGUILDUNIONINFO) * GuildCount);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCsSaveTotalGuildInfo(DServerManager * thisManager, int aIndex, CSP_REQ_CSSAVETOTALGUILDINFO * aRecv)
{
	int Counter = 0;
	CSP_ANS_CSSAVETOTALGUILDINFO pResult;
	pResult.h.Set(0x87, sizeof(pResult));
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	pResult.iResult = 0;

	int lOfs=sizeof(CSP_REQ_CSSAVETOTALGUILDINFO);

	if(aRecv->iCount > 0)
	{
		while(Counter<aRecv->iCount)
		{
			CSP_CSSAVETOTALGUILDINFO * GuildInfo = (CSP_CSSAVETOTALGUILDINFO*)((LPBYTE)aRecv + lOfs);
			BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_SetSiegeGuildInfo %d, '%s', %d, %d",aRecv->wMapSvrNum,GuildInfo->szGuildName,GuildInfo->iCsGuildID,GuildInfo->iGuildInvolved);
			if (bRet != TRUE )
			{
				gDataBase.Clear();
				break;
			}
			gDataBase.Clear();
			Counter++;
			lOfs+=sizeof(CSP_CSSAVETOTALGUILDINFO);
		}
		pResult.iResult = 1;
	}
	thisManager->DataSend(aIndex, (unsigned char*)&pResult,sizeof(pResult));
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCsLoadTotalGuildInfo(DServerManager * thisManager, int aIndex, CSP_REQ_CSLOADTOTALGUILDINFO * aRecv)
{
	char cBUFFER[10000]={0};

	CSP_ANS_CSLOADTOTALGUILDINFO * pResult = (CSP_ANS_CSLOADTOTALGUILDINFO *)(cBUFFER);
	CSP_CSLOADTOTALGUILDINFO * GuildInfo = (CSP_CSLOADTOTALGUILDINFO *)(cBUFFER + sizeof(CSP_ANS_CSLOADTOTALGUILDINFO));
	int Counter = 0;

	pResult->iResult = 1;
	pResult->wMapSvrNum = aRecv->wMapSvrNum;
	GuildInfo->iCsGuildID = 0;
	GuildInfo->iGuildInvolved = 0;
	memset(GuildInfo->szGuildName,0,8);
	//memcpy(GuildInfo->szGuildName,0,8);
	
	int lOfs=sizeof(CSP_ANS_CSLOADTOTALGUILDINFO);

	BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CS_GetSiegeGuildInfo %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if( gDataBase.Fetch() != SQL_NO_DATA)
		{
			do
			{
				GuildInfo = (CSP_CSLOADTOTALGUILDINFO *)(cBUFFER + lOfs);
				char tmp[8]={0};
				GuildInfo->iCsGuildID = gDataBase.GetInt("GUILD_ID");
				GuildInfo->iGuildInvolved = gDataBase.GetInt("GUILD_INVOLVED");
				gDataBase.GetStr("GUILD_NAME", tmp);
				memcpy(GuildInfo->szGuildName,tmp,8);
				Counter++;
				lOfs+=sizeof(CSP_CSLOADTOTALGUILDINFO);
			}while(Counter < 100 && gDataBase.Fetch() != SQL_NO_DATA);
		}
	}
	gDataBase.Clear();
	gFunc.PHeadSetW((LPBYTE)pResult, 0x88, sizeof(CSP_ANS_CSLOADTOTALGUILDINFO) + sizeof(CSP_CSLOADTOTALGUILDINFO) * Counter);
	pResult->iCount = Counter;
	thisManager->DataSend(aIndex, (unsigned char*)cBUFFER, sizeof(CSP_ANS_CSLOADTOTALGUILDINFO) + sizeof(CSP_CSLOADTOTALGUILDINFO) * Counter);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCastleNpcUpdate(DServerManager * thisManager, int aIndex, CSP_REQ_NPCUPDATEDATA * aRecv)
{
	int Counter = 0;
	CSP_ANS_NPCUPDATEDATA pResult;
	pResult.h.Set(0x89, sizeof(pResult));
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	pResult.iResult = 0;
	BOOL bRet = FALSE;

	int lOfs=sizeof(CSP_REQ_NPCUPDATEDATA);

	bRet = gDataBase.ExecFormat("DELETE FROM MuCastle_NPC WHERE MAP_SVR_GROUP = %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if(aRecv->iCount > 0)
		{
			while(Counter<aRecv->iCount)
			{
				gDataBase.Clear();
				CSP_NPCUPDATEDATA * NPCInfo = (CSP_NPCUPDATEDATA*)((LPBYTE)aRecv + lOfs);
				bRet = gDataBase.ExecFormat("EXEC WZ_CS_ReqNpcUpdate %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",aRecv->wMapSvrNum,NPCInfo->iNpcNumber,NPCInfo->iNpcIndex,NPCInfo->iNpcDfLevel,NPCInfo->iNpcRgLevel,NPCInfo->iNpcMaxHp,NPCInfo->iNpcHp,NPCInfo->btNpcX,NPCInfo->btNpcY,NPCInfo->btNpcDIR);
				if (bRet != TRUE )
				{
					gDataBase.Clear();
					break;
				}
				Counter++;
				lOfs+=sizeof(CSP_NPCUPDATEDATA);
			}
			pResult.iResult = 1;
		}
	}
	thisManager->DataSend(aIndex, (unsigned char*)&pResult,sizeof(pResult));
}
//-----------------------------------------------------------------------------------------------------------

void DGAnsCrywolfSync(DServerManager * thisManager, int aIndex, CWP_REQ_CRYWOLFSYNC * aRecv)
{
	CWP_ANS_CRYWOLFSYNC pResult;
	pResult.h.Set(0xB0, sizeof(pResult));
	pResult.iCrywolfState = aRecv->iCrywolfState;
	pResult.iOccupationState = aRecv->iOccupationState;
	pResult.btResult = aRecv->iOccupationState;	//REVEER

	thisManager->DataSendAll((unsigned char*)&pResult,sizeof(pResult));
}
//-----------------------------------------------------------------------------------------------------------

void DGAnsCrywolfInfoLoad(DServerManager * thisManager, int aIndex, CWP_REQ_CRYWOLFINFOLOAD * aRecv)
{
	CWP_ANS_CRYWOLFINFOLOAD pResult;
	pResult.h.Set(0xB1, sizeof(pResult));
	
	pResult.btResult = 0;
	pResult.iCrywolfState = 0;
	pResult.iOccupationState = 0;
	
	BOOL bRet = gDataBase.ExecFormat("EXEC WZ_CW_InfoLoad %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if( gDataBase.Fetch() != SQL_NO_DATA)
		{	
			pResult.btResult = 1;
			pResult.iCrywolfState = gDataBase.GetInt("CRYWOLF_STATE");
			pResult.iOccupationState = gDataBase.GetInt("CRYWOLF_OCCUFY");
		}
	}
	gDataBase.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.Size);
}
//-----------------------------------------------------------------------------------------------------------
