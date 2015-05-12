#include "GInclude.h"
#include "DSProtocol.h"
#include "Main.h"
#include "TDbManager.h"
#include "TDSManager.h"

//-----------------------------------------------------------------------------------------------------------
// # leo123 : (FRIST REWRITE SETUP) : done till GJSetCharacterInfo
//-----------------------------------------------------------------------------------------------------------

void DSProtocolCore(CDSManager * thisManager, int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize)
{
	g_Log.SetStatusText(g_Colors.White(), g_Colors.GreenYellow(), "%s ON_GO", MAIN_PROJECT_NAME);
	// ----
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
	// ----
	g_Log.SetStatusText(g_Colors.Black(), g_Colors.LightCoral() , "%s STATE_WAIT", MAIN_PROJECT_NAME);
}
//-----------------------------------------------------------------------------------------------------------

void DataServerLogin(CDSManager * thisManager, int aIndex, SDHP_SERVERINFO * lpMsg)
{
	SDHP_RESULT pResult = {0};
	// ----
	pResult.h.set(0x00, sizeof(pResult));
	pResult.Result = 1;
	// ----
	if(g_DbManager.ExecFormat("SELECT ItemCount FROM GameServerInfo WHERE Number=0") == true)
	{
		g_DbManager.Fetch();
		// ----
		pResult.ItemCount = g_DbManager.GetInt("ItemCount");
	}
	g_DbManager.Clear();
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void JGPGetCharList(CDSManager * thisManager, int aIndex, SDHP_GETCHARLIST * aRecv)
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
	g_DbManager.ExecFormat("SELECT Id FROM AccountCharacter WHERE Id='%s'", szAccountID);
	// ----
	if(g_DbManager.Fetch() == SQL_NO_DATA)
	{
		g_DbManager.Clear();
		// ----
		g_DbManager.ExecFormat("INSERT INTO AccountCharacter (Id) VALUES ('%s')", szAccountID);
		// ----
		g_DbManager.Fetch();
	}
	// ----
	g_DbManager.Clear();
	// ----
	g_DbManager.ExecFormat("SELECT * FROM AccountCharacter WHERE Id='%s'", szAccountID);
	// ----
	g_DbManager.Fetch();
	// ----
	pCount->MoveCnt = (BYTE)g_DbManager.GetInt("MoveCnt");
	// ----
	g_DbManager.GetStr("GameID1", CharName[0]);
	g_DbManager.GetStr("GameID2", CharName[1]);
	g_DbManager.GetStr("GameID3", CharName[2]);
	g_DbManager.GetStr("GameID4", CharName[3]);
	g_DbManager.GetStr("GameID5", CharName[4]);
	// ----
	g_DbManager.Clear();
	// ----
	for(int i = 0 ; i < 5 ; i++)
	{
		if(CharName[i][0] != 0)
		{
			if(strlen(CharName[i]) >= 4)
			{
				if(g_DbManager.ExecFormat("SELECT cLevel, Class, CtlCode, DbVersion FROM vCharacterPreview WHERE Name='%s'", CharName[i]) == true)
				{
					char szTemp[200];
					pCL = (SDHP_CHARLIST *)(cBUFFER + lOfs);
					// ----
					g_DbManager.Fetch();
					pCL->Level = (WORD)g_DbManager.GetInt("cLevel");
					pCL->Class = (BYTE)g_DbManager.GetInt("Class");
					// ----
					if (pCL->Level >= 220 && pCount->Magumsa == 0)
					{
						pCount->Magumsa = 1;
					}
					// ----
					if (pCL->Level >= 250 && (pCount->Magumsa == 0 || pCount->Magumsa == 1))
					{
						pCount->Magumsa = 2;
					}
					// ----
					pCL->CtlCode = (BYTE)g_DbManager.GetInt("CtlCode");
					pCL->DbVersion = (BYTE)g_DbManager.GetInt("DbVersion");
					// ----
					g_DbManager.Clear();
					// ----
					wsprintf(szTemp, "SELECT Inventory FROM Character WHERE Name='%s'", CharName[i]);
					// ----
					g_DbManager.GetAsBinary(szTemp, btInvetory);
					g_DbManager.Clear();
					// ----
					if (pCL->DbVersion == 3)
					{
						memset(pCL->dbInventory, -1, sizeof(pCL->dbInventory));
						// ----
						for(int i=0;i<12;i++)
						{
							if ( btInvetory[0+i*16] == 0xFF && (btInvetory[7+i*16] & 0x80 ) == 0x80 && (btInvetory[10+i*16] & 0xF0) == 0xF0 )
							{
								pCL->dbInventory[i*4]	= -1;
								pCL->dbInventory[i*4+1] = -1;
								pCL->dbInventory[i*4+2] = -1;
								pCL->dbInventory[i*4+3] = -1;
							}
							else
							{
								pCL->dbInventory[i*4]	 = (btInvetory[0+i*16]);	// 0..7 bits of Item
								pCL->dbInventory[i*4+1]  = (btInvetory[1+i*16]);	// Get Level of Item
								pCL->dbInventory[i*4+2]  = (btInvetory[7+i*16]);	// 8 bit     of Item
								pCL->dbInventory[i*4+3]  = (btInvetory[9+i*16]);	// 9..12 bit of Item
							}
						}
					}
					// ----
					g_DbManager.ExecFormat("SELECT G_Status FROM GuildMember WHERE Name='%s'", CharName[i]);
					// ----
					if(g_DbManager.Fetch() != SQL_NO_DATA)
					{
						pCL->btGuildStatus = (BYTE)g_DbManager.GetInt("G_Status");
					}
					else
					{
						pCL->btGuildStatus = 0xFF;
					}
					// ----
					//g_DbManager.Clear(); // #shadow added
					// ----
					pCL->Index = i;
					lstrcpy(pCL->Name, CharName[i]);
					// ----
					iCharCount++;
					lOfs+=sizeof(SDHP_CHARLIST);
				}
				// ----
				g_DbManager.Clear();
			}
		}
	}
	// ----
	PHeadSetW((LPBYTE)pCount, 0x01, sizeof(SDHP_CHARLISTCOUNT) + sizeof(SDHP_CHARLIST) * iCharCount);
	pCount->Count = iCharCount;
	// ----
	thisManager->DataSend(aIndex, (LPBYTE)cBUFFER, sizeof(SDHP_CHARLISTCOUNT) + sizeof(SDHP_CHARLIST) * iCharCount);
}
//-----------------------------------------------------------------------------------------------------------

void JGCharacterCreateRequest(CDSManager * thisManager, int aIndex, SDHP_CREATECHAR * aRecv)
{
	SDHP_CREATECHARRESULT pResult	= {0};
	char szAccountID[11]			= {0};
	char szName[11]					= {0};
	TCHAR CharName[5][11]			= {0};
	int iIndex						= 0;
	// ----
	pResult.ClassSkin				= aRecv->ClassSkin;
	pResult.Number					= aRecv->Number;
	// ----
	memcpy(pResult.AccountId, aRecv->AccountId, 10);
	memcpy(szAccountID, aRecv->AccountId, 10);
	memcpy(szName, aRecv->Name, 10);
	// ----
	if(g_DbManager.ExecFormat("SELECT * FROM AccountCharacter WHERE Id='%s'", szAccountID) == true)
	{
		g_DbManager.Fetch();
		// ----
		g_DbManager.GetStr("GameID1", CharName[0]);
		g_DbManager.GetStr("GameID2", CharName[1]);
		g_DbManager.GetStr("GameID3", CharName[2]);
		g_DbManager.GetStr("GameID4", CharName[3]);
		g_DbManager.GetStr("GameID5", CharName[4]);
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
			g_DbManager.Clear();
			// ----
			if ( (pResult.ClassSkin >> 4) < 0 || (pResult.ClassSkin >> 4) >  5 /*MAX_TYPE_PLAYER*/-1 )
			{
				pResult.Result = 0;
			}
			// ----
			g_DbManager.ExecFormat("WZ_CreateCharacter '%s', '%s', '%d'",szAccountID, szName,
				pResult.ClassSkin);
			// ----
			g_DbManager.Fetch();
			// ----
			pResult.Result = g_DbManager.GetInt(0);
			// ----
			g_DbManager.Clear();
			// ----
			if ( pResult.Result == 1 )
			{
				g_DbManager.ExecFormat("UPDATE AccountCharacter SET GameID%d='%s' WHERE Id='%s'",
					iIndex+1, szName, szAccountID);
				// ----
				g_DbManager.Fetch();
				g_DbManager.Clear();
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
	g_DbManager.Clear();
	// ----
	pResult.h.set(0x04, sizeof(pResult));
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void JGCharDelRequest(CDSManager * thisManager, int aIndex, SDHP_CHARDELETE * aRecv)
{
	SDHP_CHARDELETERESULT pResult	= {0};
	char szAccountID[11]			= {0};
	char szName[11]					= {0};
	TCHAR CharName[5][11]			= {0};
	int iIndex						= 0;
	// ----
	pResult.h.set(0x05, sizeof(pResult));
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
		if ( g_DbManager.ExecFormat("DELETE FROM Character WHERE AccountID='%s' AND Name='%s'",
				szAccountID, szName) == TRUE )
		{
			g_DbManager.Fetch();
			g_DbManager.Clear();
			// ----
			g_DbManager.ExecFormat("SELECT * FROM AccountCharacter WHERE Id='%s'", szAccountID);
			g_DbManager.Fetch();
			// ----
			g_DbManager.GetStr("GameID1", CharName[0]);
			g_DbManager.GetStr("GameID2", CharName[1]);
			g_DbManager.GetStr("GameID3", CharName[2]);
			g_DbManager.GetStr("GameID4", CharName[3]);
			g_DbManager.GetStr("GameID5", CharName[4]);
			// ----
			for(iIndex = 0 ; iIndex < 5 ; iIndex++)
			{
				if ( lstrcmp(CharName[iIndex], szName) == 0)
				{
					break;
				}
			}
			// ----
			g_DbManager.Clear();
			// ----
			g_DbManager.ExecFormat("UPDATE AccountCharacter SET GameID%d=NULL WHERE Id='%s'", iIndex+1, szAccountID);
			g_DbManager.Fetch();
			g_DbManager.Clear();
			// ----
		}
		else
		{
			g_DbManager.Fetch();
			g_DbManager.Clear();
			// ----
			pResult.Result = 0;
		}
	}
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void JGGetCharacterInfo(CDSManager * thisManager, int aIndex, SDHP_DBCHARINFOREQUEST * aRecv)
{
	SDHP_DBCHAR_INFORESULT pResult		= {0};
	char szAccountID[11]				= {0};
	char szName[11]						= {0};
	char szTemp[256]					= {0};
	// ----
	BYTE btTemp2[256]					= {0};
	BYTE btTemp[INVENTORY_BINARY_SIZE]	= {0};
	// ----
	PHeadSetW((LPBYTE)&pResult, 0x06, sizeof(pResult));
	// ----
	BuxConvert(aRecv->Name, 10);
	// ----
	memcpy(szAccountID, aRecv->AccountID, 10);
	memcpy(szName, aRecv->Name, 10);
	memcpy(pResult.AccountID, aRecv->AccountID, 10);
	memcpy(pResult.Name, aRecv->Name, 10);
	// ----
	pResult.Number = aRecv->Number;
	pResult.result = 0;
	// ----
	g_DbManager.ExecFormat("SELECT cLevel, Class, LevelUpPoint, Experience, Strength, Dexterity, Vitality, Energy, Money, Life, MaxLife, Mana, MaxMana, MapNumber, MapPosX, MapPosY, MapDir, PkCount, PkLevel, PkTime, CtlCode, DbVersion, Leadership, ChatLimitTime, FruitPoint FROM Character WHERE Name='%s'", szName);
	// ----
	if ( g_DbManager.Fetch() != SQL_NO_DATA ) 
	{
		pResult.Level			= (short)g_DbManager.GetInt("cLevel"); 
		pResult.Class			= (BYTE)g_DbManager.GetInt("Class");
		pResult.LevelUpPoint	= g_DbManager.GetInt("LevelUpPoint");
		pResult.Exp				= g_DbManager.GetInt("Experience");
		pResult.Str				= (short)g_DbManager.GetInt("Strength"); 
		pResult.Dex				= (short)g_DbManager.GetInt("Dexterity");
		pResult.Vit				= (short)g_DbManager.GetInt("Vitality");
		pResult.Energy			= (short)g_DbManager.GetInt("Energy");
		pResult.Money			= g_DbManager.GetInt("Money");
		pResult.Life			= (WORD)g_DbManager.GetFloat("Life");
		pResult.MaxLife			= (WORD)g_DbManager.GetFloat("MaxLife");
		pResult.Mana			= (WORD)g_DbManager.GetFloat("Mana");
		pResult.MaxMana			= (WORD)g_DbManager.GetFloat("MaxMana");
		pResult.MapNumber		= (BYTE)g_DbManager.GetInt("MapNumber");
		pResult.MapX			= (BYTE)g_DbManager.GetInt("MapPosX");
		pResult.MapY			= (BYTE)g_DbManager.GetInt("MapPosY");
		pResult.Dir				= (BYTE)g_DbManager.GetInt("MapDir");
		pResult.PkCount			= g_DbManager.GetInt("PkCount");
		pResult.PkLevel			= g_DbManager.GetInt("PkLevel");
		pResult.PkTime			= g_DbManager.GetInt("PkTime");
		pResult.CtlCode			= (BYTE)g_DbManager.GetInt("CtlCode");
		pResult.DbVersion		= (BYTE)g_DbManager.GetInt("DbVersion");
		pResult.Leadership		= (WORD)g_DbManager.GetInt("Leadership");
		pResult.ChatLitmitTime	= (WORD)g_DbManager.GetInt("ChatLimitTime");
		pResult.iFruitPoint		= g_DbManager.GetInt("FruitPoint");
		// ----
		g_DbManager.Clear();
		// ----
		wsprintf(szTemp, "SELECT Inventory from Character where Name='%s'", szName);
		// ----
		if (g_DbManager.GetAsBinary(szTemp, btTemp) == -1)
		{
			memset(btTemp, -1, sizeof(btTemp));
		}
		// ----
		memcpy(pResult.dbInventory, btTemp, sizeof(pResult.dbInventory));
		memset(btTemp, 0, sizeof(btTemp));
		// ----
		g_DbManager.Clear();
		// ----
		wsprintf(szTemp, "SELECT MagicList from Character where Name='%s'", szName);
		// ----
		if (g_DbManager.GetAsBinary(szTemp, btTemp) == -1) 
		{
			memset(btTemp, -1, sizeof(btTemp));
		}
		memcpy(pResult.dbMagicList, btTemp, sizeof(pResult.dbMagicList));
		memset(btTemp, 0, sizeof(btTemp));
		// ----
		g_DbManager.Clear();
		// ----
		wsprintf(szTemp, "SELECT Quest from Character where Name='%s'", szName);
		// ----
		if ( g_DbManager.GetAsBinary(szTemp, btTemp) == -1 ) 
		{
			memset(btTemp, -1, sizeof(btTemp));
		}
		// ----
		memcpy(pResult.dbQuest, btTemp, sizeof(pResult.dbQuest));
		memset(btTemp, 0, sizeof(btTemp));
		// ----
		g_DbManager.Clear();
		// ----
		pResult.result = 1;
		// ----
		g_DbManager.ExecFormat("UPDATE AccountCharacter SET GameIDC='%s' WHERE Id='%s'", szName, szAccountID);
		g_DbManager.Fetch();
		g_DbManager.Clear();
	}
	// ----
	g_DbManager.Clear();
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, sizeof(pResult));
	// ----
	if ( pResult.result == 1 )
	{
		SDHP_SKILLKEYDATA_SEND pSkillData={0};
		// ----
		pSkillData.h.set(0x60, sizeof(pSkillData));
		pSkillData.aIndex = aRecv->Number;
		// ----
		g_DbManager.ExecFormat("SELECT * FROM OptionData WHERE Name='%s'", szName);
		// ----
		if (g_DbManager.Fetch() != SQL_NO_DATA )
		{
			memcpy(pSkillData.Name, aRecv->Name, 10);
			// ----
			pSkillData.GameOption = (BYTE)g_DbManager.GetInt("GameOption");
			pSkillData.QkeyDefine = (BYTE)g_DbManager.GetInt("QKey");
			pSkillData.WkeyDefine = (BYTE)g_DbManager.GetInt("WKey");
			pSkillData.EkeyDefine = (BYTE)g_DbManager.GetInt("EKey");
			pSkillData.RkeyDefine = (BYTE)g_DbManager.GetInt("RKey");
			pSkillData.ChatWindow = (BYTE)g_DbManager.GetInt("ChatWindow");
			// ----
			g_DbManager.Clear();
			// ----
			wsprintf(szTemp, "SELECT SkillKey from OptionData where Name='%s'", szName);
			// ----
			g_DbManager.GetAsBinary(szTemp, btTemp2);
			// ----
			memcpy(pSkillData.SkillKeyBuffer, btTemp2, sizeof(pSkillData.SkillKeyBuffer));
		}
		// ----
		g_DbManager.Clear();
		// ----
		thisManager->DataSend(aIndex, (unsigned char*)&pSkillData, sizeof(pSkillData));
	}
}
//-----------------------------------------------------------------------------------------------------------

void GJSetCharacterInfo(CDSManager * thisManager, int aIndex, SDHP_DBCHAR_INFOSAVE * aRecv)
{
	char szName[11]		= {0};
	char szTemp[128]	= {0};
	// ----
	memcpy(szName, aRecv->Name, 10);
	// ----
	g_DbManager.ExecFormat("UPDATE Character SET cLevel=%d,Class=%d,LevelUpPoint=%d, Experience=%d, Strength=%d, Dexterity=%d, Vitality=%d, Energy=%d, Money=%d, Life=%d, MaxLife=%d, Mana=%d, MaxMana=%d, MapNumber=%d, MapPosX=%d, MapPosY=%d, MapDir=%d, PkCount=%d, PkLevel=%d, PkTime=%d, DbVersion=3, Leadership=%d, ChatLimitTime=%d, FruitPoint=%d WHERE Name = '%s'",
		aRecv->Level, aRecv->Class, aRecv->LevelUpPoint, aRecv->Exp, aRecv->Str, aRecv->Dex, aRecv->Vit, aRecv->Energy, aRecv->Money, aRecv->Life, aRecv->MaxLife, aRecv->Mana, aRecv->MaxMana, aRecv->MapNumber, aRecv->MapX, aRecv->MapY, aRecv->Dir, aRecv->PkCount, aRecv->PkLevel, aRecv->PkTime, aRecv->Leadership, aRecv->ChatLitmitTime, aRecv->iFruitPoint, szName);
	g_DbManager.Fetch();
	g_DbManager.Clear();
	// ----
	wsprintf(szTemp, "UPDATE Character SET Inventory=? WHERE Name='%s'", szName);
	g_DbManager.SetAsBinary(szTemp, aRecv->dbInventory, sizeof(aRecv->dbInventory));
	g_DbManager.Fetch();
	g_DbManager.Clear();
	// ----
	wsprintf(szTemp, "UPDATE Character SET MagicList=? WHERE Name='%s'", szName);
	g_DbManager.SetAsBinary(szTemp, aRecv->dbMagicList, sizeof(aRecv->dbMagicList));
	g_DbManager.Fetch();
	g_DbManager.Clear();
	// ----
	wsprintf(szTemp, "UPDATE Character SET Quest=? WHERE Name='%s'", szName);
	g_DbManager.SetAsBinary(szTemp, aRecv->dbQuest, sizeof(aRecv->dbQuest));
	g_DbManager.Fetch();
	g_DbManager.Clear();
	// ----
	if (aRecv->CharInfoSave == 1)
	{
		g_DbManager.ExecFormat("INSERT INTO T_CurCharName (Name) VALUES('%s')", szName);
		g_DbManager.Fetch();
		g_DbManager.Clear();
	}
}
//-----------------------------------------------------------------------------------------------------------

void DGGetWarehouseList(CDSManager * thisManager, int aIndex, SDHP_GETWAREHOUSEDB * aRecv)
{
	SDHP_GETWAREHOUSEDB_SAVE pResult	= {0};
	SDHP_GETWAREHOUSEDB_RESULT pMsg		= {0};
	char szAccountID[11]				= {0};
	char szTemp[128]					= {0};
	// ----
	PHeadSetW((LPBYTE)&pResult, 0x08, sizeof(pResult));
	pMsg.h.set(0x10, sizeof(pMsg));
	// ----
	memcpy(szAccountID, aRecv->AccountID, 10);
	memcpy(pResult.AccountID, szAccountID, 10);
	memcpy(pMsg.AccountID, szAccountID, 10);
	// ----
	pMsg.aIndex	   = aRecv->aIndex;
	pResult.aIndex = aRecv->aIndex;
	// ----
	bool bRet = g_DbManager.ExecFormat("SELECT AccountID FROM warehouse WHERE AccountID='%s'", szAccountID);
	// ----
	if ( g_DbManager.Fetch() == SQL_NO_DATA || bRet == false )
	{
		g_DbManager.Clear();
		g_DbManager.ExecFormat("INSERT INTO warehouse (AccountID, Money, EndUseDate, DbVersion) VALUES ('%s',0, getdate(), 3)", szAccountID);
		g_DbManager.Fetch();
		g_DbManager.Clear();
		// ----
		thisManager->DataSend(aIndex, (unsigned char*)&pMsg, pMsg.h.size);
		// ----
		return;
	}
	// ----
	g_DbManager.Clear();
	// ----
	g_DbManager.ExecFormat("SELECT Money, DbVersion, pw FROM warehouse WHERE AccountID='%s'", szAccountID);
	g_DbManager.Fetch();
	// ----
	pResult.Money		= g_DbManager.GetInt("Money");
	pResult.DbVersion	= g_DbManager.GetInt("DbVersion");
	pResult.pw			= g_DbManager.GetInt("pw");
	// ----
	g_DbManager.Clear();
	// ----
	wsprintf(szTemp, "SELECT Items FROM warehouse WHERE AccountID='%s'", szAccountID);
	// ----
	if ( g_DbManager.GetAsBinary(szTemp, pResult.dbItems) < 0 )
	{
		memset(pResult.dbItems, -1, sizeof(pResult.dbItems));
	}
	// ----
	g_DbManager.Clear();
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, sizeof(pResult));
}
//-----------------------------------------------------------------------------------------------------------

void GDSetWarehouseList(CDSManager * thisManager, int aIndex, SDHP_GETWAREHOUSEDB_SAVE * aRecv)
{
	char szAccountID[11]	= {0};
	char szTemp[128]		= {0};
	// ----
	memcpy(szAccountID, aRecv->AccountID, 10);
	// ----
	g_DbManager.ExecFormat("UPDATE warehouse SET Money=%d, pw=%d, DbVersion=3 WHERE AccountID='%s'",
		aRecv->Money, aRecv->pw, szAccountID);
	g_DbManager.Fetch();
	g_DbManager.Clear();
	// ----
	wsprintf(szTemp, "UPDATE warehouse SET Items=? WHERE AccountID='%s'", szAccountID);
	// ----
	g_DbManager.SetAsBinary(szTemp, aRecv->dbItems, sizeof(aRecv->dbItems));
	g_DbManager.Fetch();
	g_DbManager.Clear();
}
//-----------------------------------------------------------------------------------------------------------

void DGGetQuestMonsterKill(CDSManager * thisManager, int aIndex, SDHP_GETQUESTMONSTER_KILL* aRecv)
{
	SDHP_DBQUEST_KILLSAVE pResult	= {0};
	char szName[11]					= {0};
	bool bRet						= false;
	// ----
	pResult.h.set(0x14, sizeof(pResult));
	// ----
	memcpy(szName, aRecv->Name, 10);
	memcpy(pResult.Name, szName, 10);
	// ----
	pResult.aIndex = aRecv->aIndex;
	// ----
	bRet = g_DbManager.ExecFormat("EXEC WZ_QMK_InfoLoad '%s'", aRecv->Name);
	// ----
	if (bRet == true)
	{
		if (g_DbManager.Fetch() != SQL_NO_DATA)
		{
			// # Get Values from Database and store in the struct
			pResult.QuestDBIndex			= g_DbManager.GetInt("QUEST_INDEX");
			pResult.QuestMonsterIndex1		= g_DbManager.GetInt("MON_INDEX_1");
			pResult.QuestMonsterKillCount1	= g_DbManager.GetInt("KILL_COUNT_1");
			pResult.QuestMonsterIndex2		= g_DbManager.GetInt("MON_INDEX_2");
			pResult.QuestMonsterKillCount2	= g_DbManager.GetInt("KILL_COUNT_2");
			pResult.QuestMonsterIndex3		= g_DbManager.GetInt("MON_INDEX_3");
			pResult.QuestMonsterKillCount3	= g_DbManager.GetInt("KILL_COUNT_3");
			pResult.QuestMonsterIndex4		= g_DbManager.GetInt("MON_INDEX_4");
			pResult.QuestMonsterKillCount4	= g_DbManager.GetInt("KILL_COUNT_4");
			pResult.QuestMonsterIndex5		= g_DbManager.GetInt("MON_INDEX_5");
			pResult.QuestMonsterKillCount5	= g_DbManager.GetInt("KILL_COUNT_5");
		}
	}
	// ----
	g_DbManager.Clear();
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GDQuestMonsterKillSave(CDSManager * thisManager, int aIndex, SDHP_DBQUEST_KILLSAVE* aRecv)
{
	g_DbManager.ExecFormat("EXEC WZ_QMK_InfoSave '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", 
	// ----
	aRecv->Name, aRecv->QuestDBIndex,
		aRecv->QuestMonsterIndex1, aRecv->QuestMonsterKillCount1,
		aRecv->QuestMonsterIndex2, aRecv->QuestMonsterKillCount2,
		aRecv->QuestMonsterIndex3, aRecv->QuestMonsterKillCount3,
		aRecv->QuestMonsterIndex4, aRecv->QuestMonsterKillCount4,
		aRecv->QuestMonsterIndex5, aRecv->QuestMonsterKillCount5);
	// ----
	g_DbManager.Fetch();
	g_DbManager.Clear();
}
//-----------------------------------------------------------------------------------------------------------

void GDUserItemSave(CDSManager * thisManager, int aIndex, SDHP_DBCHAR_ITEMSAVE * aRecv)
{
	char szName[11]		= {0};
	char szTemp[128]	= {0};
	memcpy(szName, aRecv->Name, 10);
	// ----
	g_DbManager.ExecFormat("UPDATE Character SET DbVersion=3 WHERE Name = '%s'", szName);
	g_DbManager.Fetch();
	g_DbManager.Clear();
	// ----
	wsprintf(szTemp, "UPDATE Character SET Inventory=? WHERE Name='%s'", szName);
	// ----
	g_DbManager.SetAsBinary(szTemp, aRecv->dbInventory, sizeof(aRecv->dbInventory));
	g_DbManager.Fetch();
	g_DbManager.Clear();
}
//-----------------------------------------------------------------------------------------------------------

void GDSetWarehouseMoney(CDSManager * thisManager, int aIndex, SDHP_WAREHOUSEMONEY_SAVE * aRecv)
{
	char szAccountID[11]	= {0};
	// ----
	memcpy(szAccountID, aRecv->AccountID, 10);
	// ----
	g_DbManager.ExecFormat("UPDATE warehouse SET Money=%d WHERE AccountID='%s'", aRecv->Money, szAccountID);
	g_DbManager.Fetch();
	g_DbManager.Clear();
}
//-----------------------------------------------------------------------------------------------------------

void ItemSerialCreateRecv(CDSManager * thisManager, int aIndex, SDHP_ITEMCREATE * aRecv)
{
	SDHP_ITEMCREATERECV pResult = {0};
	int					Number	= 0;
	// ----
	pResult.h.set(0x52, sizeof(pResult));
	// ----
	g_DbManager.ExecFormat("EXEC WZ_GetItemSerial");
	g_DbManager.Fetch();
	// ----
	Number				= g_DbManager.GetInt(0);
	// ----
	g_DbManager.Clear();
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
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void PetItemSerialCreateRecv(CDSManager * thisManager, int aIndex, SDHP_ITEMCREATE * aRecv)
{
	SDHP_ITEMCREATERECV pResult = {0};
	int					Number	= 0;
	// ----
	pResult.h.set(0x52, sizeof(pResult));
	// ----
	g_DbManager.ExecFormat("EXEC WZ_GetItemSerial");
	g_DbManager.Fetch();
	// ----
	Number = g_DbManager.GetInt(0);
	// ----
	g_DbManager.Clear();
	// ----
	g_DbManager.ExecFormat("INSERT INTO T_PetItem_Info (ItemSerial, Pet_Level, Pet_Exp) VALUES (%d, %d, %d)",
		Number, 1, 0);
	g_DbManager.Fetch();
	g_DbManager.Clear();
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
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void DGRecvPetItemInfo(CDSManager * thisManager, int aIndex, SDHP_REQUEST_PETITEM_INFO * aRecv)
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
		g_DbManager.ExecFormat("SELECT Pet_Level, Pet_Exp FROM T_PetItem_Info WHERE ItemSerial=%d",
			pRequestPetInfo->nSerial);
		// ----
		if (g_DbManager.Fetch() == SQL_NO_DATA)
		{
			g_DbManager.Clear();
			// ----
			g_DbManager.ExecFormat("INSERT INTO T_PetItem_Info (ItemSerial, Pet_Level, Pet_Exp) VALUES (%d, %d, %d)",
				pRequestPetInfo->nSerial, 1, 0);
			g_DbManager.Fetch();
			g_DbManager.Clear();
			// ----
			g_DbManager.ExecFormat("SELECT Pet_Level, Pet_Exp FROM T_PetItem_Info WHERE ItemSerial=%d",
				pRequestPetInfo->nSerial);
			g_DbManager.Fetch();
		}
		// ----
		pRecvPetInfo->Level		= g_DbManager.GetInt("Pet_Level");
		pRecvPetInfo->Exp		= g_DbManager.GetInt("Pet_Level");
		pRecvPetInfo->nPos		= pRequestPetInfo->nPos;
		pRecvPetInfo->nSerial	= pRequestPetInfo->nSerial;
		// ----
		g_DbManager.Clear();
		pRecvPetInfoCount->nCount++;
		// ----
		lOfs1 += sizeof(Request_PetItem_Info);
		lOfs2 += sizeof(Recv_PetItem_Info);
	}
	// ----
	PHeadSetW((LPBYTE)pRecvPetInfoCount, 0x56, sizeof(SDHP_RECV_PETITEM_INFO) + pRecvPetInfoCount->nCount * sizeof(Recv_PetItem_Info));
	// ----
	pRecvPetInfoCount->InvenType	= aRecv->nCount;
	pRecvPetInfoCount->Number		= aRecv->Number;
	// ----
	memcpy(pRecvPetInfoCount->AccountID, szAccountID, 10);
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)cBUFFER, sizeof(SDHP_RECV_PETITEM_INFO) + pRecvPetInfoCount->nCount * sizeof(Recv_PetItem_Info));
}
//-----------------------------------------------------------------------------------------------------------

void GDSavePetItemInfo(CDSManager * thisManager, int aIndex, SDHP_SAVE_PETITEM_INFO * aRecv)
{
	int lOfs = sizeof(SDHP_SAVE_PETITEM_INFO);
	// ----
	for(int i = 0 ; i < aRecv->nCount ; i++)
	{
		Save_PetItem_Info * pSavePetInfo = (Save_PetItem_Info *)((LPBYTE)aRecv + lOfs);
		// ----
		g_DbManager.ExecFormat("UPDATE T_PetItem_Info SET Pet_Level=%d, Pet_Exp=%d WHERE ItemSerial=%d",
			pSavePetInfo->Level, pSavePetInfo->Exp, pSavePetInfo->nSerial);
		g_DbManager.Fetch();
		g_DbManager.Clear();
	}
}
//-----------------------------------------------------------------------------------------------------------

void DGOptionDataRecv(CDSManager * thisManager, int aIndex, SDHP_SKILLKEYDATA * aRecv)
{
	char szName[11] =	{0};
	char szTemp[100]=	{0};
	// ----
	memcpy(szName, aRecv->Name, 10);
	// ----
	g_DbManager.ExecFormat("SELECT Name FROM OptionData WHERE Name='%s'", szName);
	// ----
	if (g_DbManager.Fetch() == SQL_NO_DATA)
	{
		g_DbManager.Clear();
		g_DbManager.ExecFormat("INSERT INTO OptionData (Name, GameOption, Qkey, Wkey, Ekey, Rkey, ChatWindow) VALUES ('%s', %d, %d, %d, %d, %d, %d)",
			szName, aRecv->GameOption, aRecv->QkeyDefine, aRecv->WkeyDefine, aRecv->EkeyDefine, aRecv->RkeyDefine, aRecv->ChatWindow);
		g_DbManager.Fetch();
		g_DbManager.Clear();
	}
	else
	{
		g_DbManager.Clear();
		g_DbManager.ExecFormat("UPDATE OptionData SET GameOption=%d, Qkey=%d, Wkey=%d, Ekey=%d, Rkey=%d, ChatWindow=%d WHERE Name='%s'",
			aRecv->GameOption, aRecv->QkeyDefine, aRecv->WkeyDefine, aRecv->EkeyDefine, aRecv->RkeyDefine, aRecv->ChatWindow, szName);
		g_DbManager.Fetch();
		g_DbManager.Clear();
	}
	// ----
	wsprintf(szTemp, "UPDATE OptionData SET SkillKey=? where Name='%s'", szName);
	// ----
	g_DbManager.SetAsBinary(szTemp, aRecv->SkillKeyBuffer, sizeof(aRecv->SkillKeyBuffer));
	g_DbManager.Fetch();
	g_DbManager.Clear();
}
//-----------------------------------------------------------------------------------------------------------

void DGMoveOtherServer(CDSManager * thisManager, int aIndex, SDHP_CHARACTER_TRANSFER * aRecv)
{
	g_Log.LogAdd(g_Colors.Beige(), "[DSProtocol][DGMoveOtherServer] :: Error Character Transfer is not longer avaible");
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsOwnerGuildMaster(CDSManager * thisManager, int aIndex, CSP_REQ_OWNERGUILDMASTER * aRecv)
{
	CSP_ANS_OWNERGUILDMASTER pResult = {0};
	bool bRet						 = false;
	// ----
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x01, sizeof(pResult));
	// ----
	pResult.iIndex		= aRecv->iIndex;
	pResult.wMapSvrNum	= aRecv->wMapSvrNum;
	pResult.iResult		= 0;
	// ----
	bRet = g_DbManager.ExecFormat("EXEC WZ_CS_GetOwnerGuildMaster %d",aRecv->wMapSvrNum);
	// ----
	if ((bRet == true) && (g_DbManager.Fetch() != SQL_NO_DATA))
	{
		pResult.iResult = g_DbManager.GetInt("QueryResult");
		// ----
		g_DbManager.GetStr("OwnerGuild", pResult.szCastleOwnGuild);
		g_DbManager.GetStr("OwnerGuildMaster", pResult.szCastleOwnGuildMaster);
	}
	// ----
	g_DbManager.Clear();
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCastleNpcBuy(CDSManager * thisManager, int aIndex, CSP_REQ_NPCBUY * aRecv)
{
	CSP_ANS_NPCBUY pResult	= {0};
	bool bRet				= false;
	// ----
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x03, sizeof(pResult));
	// ----
	pResult.iBuyCost	= aRecv->iBuyCost;
	pResult.iIndex		= aRecv->iIndex;
	pResult.iNpcIndex	= aRecv->iNpcIndex;
	pResult.iNpcNumber	= aRecv->iNpcNumber;
	pResult.wMapSvrNum	= aRecv->wMapSvrNum;
	pResult.iResult		= 0;
	// ----
	bRet = g_DbManager.ExecFormat("EXEC WZ_CS_ReqNpcBuy %d,%d,%d,%d,%d,%d,%d,%d,%d,%d",aRecv->wMapSvrNum,aRecv->iNpcNumber,aRecv->iNpcIndex,aRecv->iNpcDfLevel,aRecv->iNpcRgLevel,aRecv->iNpcMaxHp,aRecv->iNpcHp,aRecv->btNpcX,aRecv->btNpcY,aRecv->btNpcDIR);
	// ----
	if ((bRet == true) && (g_DbManager.Fetch() != SQL_NO_DATA))
	{
		pResult.iResult = g_DbManager.GetInt("QueryResult");
	}
	// ----
	g_DbManager.Clear();
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCastleNpcRepair(CDSManager * thisManager, int aIndex, CSP_REQ_NPCREPAIR * aRecv)
{
	CSP_ANS_NPCREPAIR pResult	= {0};
	bool bRet					= false;
	// ----
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x04, sizeof(pResult));
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
	bRet = g_DbManager.ExecFormat("EXEC WZ_CS_ReqNpcRepair %d,%d,%d",aRecv->wMapSvrNum,aRecv->iNpcNumber,aRecv->iNpcIndex);
	// ----
	if ((bRet == true) && (g_DbManager.Fetch() != SQL_NO_DATA))
	{
		pResult.iResult		= g_DbManager.GetInt("QueryResult");
		pResult.iNpcHpl		= g_DbManager.GetInt("NPC_HP");
		pResult.iNpcMaxHp	= g_DbManager.GetInt("NPC_MAXHP");
	}
	// ----
	g_DbManager.Clear();
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCastleNpcUpgrade(CDSManager * thisManager, int aIndex, CSP_REQ_NPCUPGRADE * aRecv)
{
	CSP_ANS_NPCUPGRADE pResult	= {0};
	bool bRet					= false;
	// ----
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x05, sizeof(pResult));
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
	bRet = g_DbManager.ExecFormat("EXEC WZ_CS_ReqNpcUpgrade %d,%d,%d,%d,%d",aRecv->wMapSvrNum,aRecv->iNpcNumber,aRecv->iNpcIndex,aRecv->iNpcUpType,aRecv->iNpcUpValue);
	// ----
	if ((bRet == true) && (g_DbManager.Fetch() != SQL_NO_DATA))
	{
		pResult.iResult = g_DbManager.GetInt("QueryResult");
	}
	// ----
	g_DbManager.Clear();
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsTaxInfo(CDSManager * thisManager, int aIndex, CSP_REQ_TAXINFO * aRecv)
{
	CSP_ANS_TAXINFO pResult	= {0};
	bool bRet				= false;
	// ----
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x06, sizeof(pResult));
	// ----
	pResult.iIndex			= aRecv->iIndex;
	pResult.wMapSvrNum		= aRecv->wMapSvrNum;
	pResult.i64CastleMoney	= 0;
	pResult.iResult			= 0;
	pResult.iTaxHuntZone	= 0;
	pResult.iTaxRateChaos	= 0;
	pResult.iTaxRateStore	= 0;
	// ----
	bRet = g_DbManager.ExecFormat("EXEC WZ_CS_GetCastleTaxInfo %d",aRecv->wMapSvrNum);
	// ----
	if ((bRet == true) && (g_DbManager.Fetch() != SQL_NO_DATA))
	{
		pResult.iResult			= 1;
		pResult.i64CastleMoney	= g_DbManager.GetInt64("MONEY");
		pResult.iTaxRateChaos	= g_DbManager.GetInt("TAX_RATE_CHAOS");
		pResult.iTaxRateStore	= g_DbManager.GetInt("TAX_RATE_STORE");
		pResult.iTaxHuntZone	= g_DbManager.GetInt("TAX_HUNT_ZONE");
	}
	// ----
	g_DbManager.Clear();
	// ----
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsTaxRateChange(CDSManager * thisManager, int aIndex, CSP_REQ_TAXRATECHANGE * aRecv)
{
	CSP_ANS_TAXRATECHANGE pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x07, sizeof(pResult));
	
	pResult.iIndex = aRecv->iIndex;
	pResult.iResult = 0;
	pResult.iTaxKind = aRecv->iTaxKind;
	pResult.iTaxRate = aRecv->iTaxRate;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_ModifyTaxRate %d,%d,%d",aRecv->wMapSvrNum,aRecv->iTaxKind,aRecv->iTaxRate);
	if (bRet == TRUE )
	{
		if(g_DbManager.Fetch() != SQL_NO_DATA)
		{			
			pResult.iResult = g_DbManager.GetInt("QueryResult");
			pResult.iTaxKind = g_DbManager.GetInt("TaxKind");
			pResult.iTaxRate = g_DbManager.GetInt("TaxRate");
		}
	}
	g_DbManager.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCastleMoneyChange(CDSManager * thisManager, int aIndex, CSP_REQ_MONEYCHANGE * aRecv)
{
	CSP_ANS_MONEYCHANGE pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x08, sizeof(pResult));
	
	pResult.i64CastleMoney = 0;
	pResult.iIndex = aRecv->iIndex;
	pResult.iMoneyChanged = aRecv->iMoneyChanged;
	pResult.iResult = 0;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_ModifyMoney %d,%d",aRecv->wMapSvrNum,aRecv->iMoneyChanged);
	if (bRet == TRUE )
	{
		if(g_DbManager.Fetch() != SQL_NO_DATA)
		{			
			pResult.iResult = g_DbManager.GetInt("QueryResult");
			pResult.i64CastleMoney = g_DbManager.GetInt64("MONEY");
		}
	}
	g_DbManager.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsSiegeDateChange(CDSManager * thisManager, int aIndex, CSP_REQ_SDEDCHANGE * aRecv)
{
	CSP_ANS_SDEDCHANGE pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x09, sizeof(pResult));
	
	pResult.btEndDay = aRecv->btEndDay;
	pResult.btEndMonth = aRecv->btEndMonth;
	pResult.btStartDay = aRecv->btStartDay;
	pResult.btStartMonth = aRecv->btStartMonth;
	pResult.iIndex = aRecv->iIndex;
	pResult.iResult = 0;
	pResult.wEndYear = aRecv->wEndYear;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	pResult.wStartYear = aRecv->wStartYear;
	
	BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_ModifyCastleSchedule %d, '%d-%d-%d 00:00:00', '%d-%d-%d 00:00:00'",aRecv->wMapSvrNum,aRecv->btStartDay,aRecv->btStartMonth,aRecv->wStartYear,aRecv->btEndDay,aRecv->btEndMonth,aRecv->wEndYear);
	if (bRet == TRUE )
	{
		if(g_DbManager.Fetch() != SQL_NO_DATA)
		{			
			pResult.iResult = g_DbManager.GetInt("QueryResult");
		}
	}
	g_DbManager.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsGuildMarkRegInfo(CDSManager * thisManager, int aIndex, CSP_REQ_GUILDREGINFO * aRecv)
{
	CSP_ANS_GUILDREGINFO pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x0A, sizeof(pResult));
	
	pResult.bIsGiveUp = 0;
	pResult.btRegRank = 0;
	pResult.iIndex = aRecv->iIndex;
	pResult.iRegMarkCount = 0;
	pResult.iResult = 0;
	memcpy(pResult.szGuildName,aRecv->szGuildName,8);
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_GetGuildMarkRegInfo %d, '%s'",aRecv->wMapSvrNum,aRecv->szGuildName);
	if (bRet == TRUE )
	{
		if(g_DbManager.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = 1;
			pResult.iRegMarkCount = g_DbManager.GetInt("REG_MARKS");
			pResult.btRegRank = (BYTE) g_DbManager.GetInt("SEQ_NUM");
			pResult.bIsGiveUp = g_DbManager.GetInt("IS_GIVEUP");
		}
	}
	g_DbManager.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsSiegeEndedChange(CDSManager * thisManager, int aIndex, CSP_REQ_SIEGEENDCHANGE * aRecv)
{
	CSP_ANS_SIEGEENDCHANGE pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x0B, sizeof(pResult));
	
	pResult.bIsSiegeEnded = aRecv->bIsSiegeEnded;
	pResult.iResult = 0;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_ModifySiegeEnd %d, %d",aRecv->wMapSvrNum,aRecv->bIsSiegeEnded);
	if (bRet == TRUE )
	{
		if(g_DbManager.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = g_DbManager.GetInt("QueryResult");
		}
	}
	g_DbManager.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCastleOwnerChange(CDSManager * thisManager, int aIndex, CSP_REQ_CASTLEOWNERCHANGE * aRecv)
{
	CSP_ANS_CASTLEOWNERCHANGE pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x0C, sizeof(pResult));
	
	pResult.bIsCastleOccupied = aRecv->bIsCastleOccupied;
	pResult.iResult = 0;
	memcpy(pResult.szOwnerGuildName,aRecv->szOwnerGuildName,8);
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_ModifyCastleOwnerInfo %d, %d, '%s'",aRecv->wMapSvrNum,aRecv->bIsCastleOccupied,aRecv->szOwnerGuildName);
	if (bRet == TRUE )
	{
		if(g_DbManager.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = g_DbManager.GetInt("QueryResult");
		}
	}
	g_DbManager.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsRegAttackGuild(CDSManager * thisManager, int aIndex, CSP_REQ_REGATTACKGUILD * aRecv)
{
	CSP_ANS_REGATTACKGUILD pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x0D, sizeof(pResult));
	
	pResult.iIndex = aRecv->iIndex;
	pResult.iResult = 0;
	memcpy(pResult.szEnemyGuildName,aRecv->szEnemyGuildName,8);
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_ReqRegAttackGuild %d, '%s'",aRecv->wMapSvrNum,aRecv->szEnemyGuildName);
	if (bRet == TRUE )
	{
		if(g_DbManager.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = g_DbManager.GetInt("QueryResult");
		}
	}
	g_DbManager.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsRestartCastleState(CDSManager * thisManager, int aIndex, CSP_REQ_CASTLESIEGEEND * aRecv)
{
	CSP_ANS_CASTLESIEGEEND pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x0E, sizeof(pResult));
	
	pResult.iResult = 0;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_ResetCastleSiege %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if(g_DbManager.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = g_DbManager.GetInt("QueryResult");
		}
	}
	g_DbManager.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsMapSvrMsgMultiCast(CDSManager * thisManager, int aIndex, CSP_REQ_MAPSVRMULTICAST * aRecv)
{
	CSP_ANS_MAPSVRMULTICAST pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x0F, sizeof(pResult));
	
	memcpy(pResult.szMsgText,aRecv->szMsgText,128);
	pResult.wMapSvrNum = aRecv->wMapSvrNum;



	thisManager->DataSendAll((unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsRegGuildMark(CDSManager * thisManager, int aIndex, CSP_REQ_GUILDREGMARK * aRecv)
{
	CSP_ANS_GUILDREGMARK pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x10, sizeof(pResult));
	
	pResult.iIndex = aRecv->iIndex;
	pResult.iItemPos = aRecv->iItemPos;
	pResult.iRegMarkCount = 0;
	pResult.iResult = 0;
	memcpy(pResult.szGuildName,aRecv->szGuildName,8);
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_ReqRegGuildMark %d, '%s'",aRecv->wMapSvrNum,aRecv->szGuildName);
	if (bRet == TRUE )
	{
		if(g_DbManager.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = g_DbManager.GetInt("QueryResult");
			pResult.iRegMarkCount = g_DbManager.GetInt("REG_MARKS");
		}
	}
	g_DbManager.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsGuildMarkReset(CDSManager * thisManager, int aIndex, CSP_REQ_GUILDRESETMARK * aRecv)
{
	CSP_ANS_GUILDRESETMARK pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x11, sizeof(pResult));
	
	pResult.iIndex = aRecv->iIndex;
	pResult.iRegMarkCount = 0;
	pResult.iResult = 0;
	memcpy(pResult.szGuildName,aRecv->szGuildName,8);
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_ModifyGuildMarkReset %d, '%s'",aRecv->wMapSvrNum,aRecv->szGuildName);
	if (bRet == TRUE )
	{
		if( g_DbManager.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = g_DbManager.GetInt("QueryResult");
			pResult.iRegMarkCount = g_DbManager.GetInt("DEL_MARKS");
		}
	}
	g_DbManager.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsGuildSetGiveUp(CDSManager * thisManager, int aIndex, CSP_REQ_GUILDSETGIVEUP * aRecv)
{
	CSP_ANS_GUILDSETGIVEUP pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x12, sizeof(pResult));
	
	pResult.bIsGiveUp = aRecv->bIsGiveUp;
	pResult.iIndex = aRecv->iIndex;
	pResult.iRegMarkCount = 0;
	pResult.iResult = 0;
	memcpy(pResult.szGuildName,aRecv->szGuildName,8);
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_ModifyGuildGiveUp %d, '%s', %d",aRecv->wMapSvrNum,aRecv->szGuildName,aRecv->bIsGiveUp);
	if (bRet == TRUE )
	{
		if( g_DbManager.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = g_DbManager.GetInt("QueryResult");
			pResult.iRegMarkCount = g_DbManager.GetInt("DEL_MARKS");
		}
	}
	g_DbManager.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsNpcRemove(CDSManager * thisManager, int aIndex, CSP_REQ_NPCREMOVE * aRecv)
{
	CSP_ANS_NPCREMOVE pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x16, sizeof(pResult));
	
	pResult.iNpcIndex = aRecv->iNpcIndex;
	pResult.iNpcNumber = aRecv->iNpcNumber;
	pResult.iResult = 0;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_ReqNpcRemove %d, %d, %d",aRecv->wMapSvrNum,aRecv->iNpcNumber,aRecv->iNpcIndex);
	if (bRet == TRUE )
	{
		if( g_DbManager.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = g_DbManager.GetInt("QueryResult");
		}
	}
	g_DbManager.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCastleStateSync(CDSManager * thisManager, int aIndex, CSP_REQ_CASTLESTATESYNC * aRecv)
{
	CSP_ANS_CASTLESTATESYNC pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x17, sizeof(pResult));
	
	pResult.iCastleState = aRecv->iCastleState;
	pResult.iTaxHuntZone = aRecv->iTaxHuntZone;
	pResult.iTaxRateChaos = aRecv->iTaxRateChaos;
	pResult.iTaxRateStore = aRecv->iTaxRateStore;
	memcpy(pResult.szOwnerGuildName,aRecv->szOwnerGuildName,8);
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	thisManager->DataSendAll((unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCastleTributeMoney(CDSManager * thisManager, int aIndex, CSP_REQ_CASTLETRIBUTEMONEY * aRecv)
{
	CSP_ANS_CASTLETRIBUTEMONEY pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x18, sizeof(pResult));
	
	pResult.iResult = 0;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = g_DbManager.ExecFormat("EXEC EXEC WZ_CS_ModifyMoney %d, %d",aRecv->wMapSvrNum,aRecv->iCastleTributeMoney);
	if (bRet == TRUE )
	{
		if( g_DbManager.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = g_DbManager.GetInt("QueryResult");
		}
	}
	g_DbManager.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsResetCastleTaxInfo(CDSManager * thisManager, int aIndex, CSP_REQ_RESETCASTLETAXINFO * aRecv)
{
	CSP_ANS_RESETCASTLETAXINFO pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x19, sizeof(pResult));
	
	pResult.iResult = 0;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_ResetCastleTaxInfo %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if( g_DbManager.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = g_DbManager.GetInt("QueryResult");
		}
	}
	g_DbManager.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsResetSiegeGuildInfo(CDSManager * thisManager, int aIndex, CSP_REQ_RESETSIEGEGUILDINFO * aRecv)
{
	CSP_ANS_RESETSIEGEGUILDINFO pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x1A, sizeof(pResult));
	
	pResult.iResult = 0;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_ResetSiegeGuildInfo %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if( g_DbManager.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = g_DbManager.GetInt("QueryResult");
		}
	}
	g_DbManager.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsResetRegSiegeInfo(CDSManager * thisManager, int aIndex, CSP_REQ_RESETREGSIEGEINFO * aRecv)
{
	CSP_ANS_RESETREGSIEGEINFO pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0x80,0x1B, sizeof(pResult));
	
	pResult.iResult = 0;
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	
	BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_ResetRegSiegeInfo %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if( g_DbManager.Fetch() != SQL_NO_DATA)
		{	
			pResult.iResult = g_DbManager.GetInt("QueryResult");
		}
	}
	g_DbManager.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCastleInitData(CDSManager * thisManager, int aIndex, CSP_REQ_CSINITDATA * aRecv)
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

	bRet = g_DbManager.ExecFormat("EXEC WZ_CS_GetCastleTotalInfo %d,%d",aRecv->wMapSvrNum,aRecv->iCastleEventCycle);
	if (bRet == TRUE )
	{
		if( g_DbManager.Fetch() != SQL_NO_DATA)
		{	
			pResult->iResult = 1;
			pResult->btEndDay = g_DbManager.GetInt("EDAY");
			pResult->btEndMonth = g_DbManager.GetInt("EMONTH");
			pResult->btIsCastleOccupied = g_DbManager.GetInt("CASLE_OCCUPY");
			pResult->btIsSiegeEnded = g_DbManager.GetInt("SIEGE_ENDED");
			pResult->btIsSiegeGuildList = g_DbManager.GetInt("SIEGE_GUILDLIST_SETTED");
			pResult->btStartDay = g_DbManager.GetInt("SDAY");
			pResult->btStartMonth = g_DbManager.GetInt("SMONTH");
			pResult->i64CastleMoney = g_DbManager.GetInt64("MONEY");
			pResult->iFirstCreate = g_DbManager.GetInt("FIRST_CREATE");
			pResult->iTaxHuntZone = g_DbManager.GetInt("TAX_RATE_ZONE");
			pResult->iTaxRateChaos = g_DbManager.GetInt("TAX_RATE_CHAOS");
			pResult->iTaxRateStore = g_DbManager.GetInt("TAX_RATE_STORE");
			g_DbManager.GetStr("OWNER_GUILD", pResult->szCastleOwnGuild);
			pResult->wEndYear = g_DbManager.GetInt("EYEAR");
			pResult->wStartYear = g_DbManager.GetInt("SYEAR");
			g_DbManager.Clear();

			bRet = g_DbManager.ExecFormat("EXEC WZ_CS_GetCastleNpcInfo %d",aRecv->wMapSvrNum);
			if (bRet == TRUE )
			{
				if( g_DbManager.Fetch() != SQL_NO_DATA)
				{
					do
					{
						NPCInfo = (CSP_CSINITDATA *)(cBUFFER + lOfs);
						NPCInfo->iNpcNumber = g_DbManager.GetInt("NPC_NUMBER");
						NPCInfo->iNpcIndex = g_DbManager.GetInt("NPC_INDEX");
						NPCInfo->iNpcDfLevel = g_DbManager.GetInt("NPC_DF_LEVEL");
						NPCInfo->iNpcRgLevel = g_DbManager.GetInt("NPC_RG_LEVEL");
						NPCInfo->iNpcMaxHp = g_DbManager.GetInt("NPC_MAXHP");
						NPCInfo->iNpcHp = g_DbManager.GetInt("NPC_HP");
						NPCInfo->btNpcX = g_DbManager.GetInt("NPC_X");
						NPCInfo->btNpcY = g_DbManager.GetInt("NPC_Y");
						NPCInfo->btNpcDIR = g_DbManager.GetInt("NPC_DIR");
						lOfs+=sizeof(CSP_CSINITDATA);
						Counter++;
					}while(Counter < 200 && g_DbManager.Fetch() != SQL_NO_DATA);
				}
			}
		}
	}
	g_DbManager.Clear();
	PHeadSetW((LPBYTE)pResult, 0x81, sizeof(CSP_ANS_CSINITDATA) + sizeof(CSP_CSINITDATA) * Counter);
	pResult->iCount = Counter;
	thisManager->DataSend(aIndex, (unsigned char*)cBUFFER, sizeof(CSP_ANS_CSINITDATA) + sizeof(CSP_CSINITDATA) * Counter);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsAllGuildMarkRegInfo(CDSManager * thisManager, int aIndex, CSP_REQ_ALLGUILDREGINFO * aRecv)
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

	BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_GetAllGuildMarkRegInfo %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if( g_DbManager.Fetch() != SQL_NO_DATA)
		{
			do
			{
				GuildInfo = (CSP_GUILDREGINFO *)(cBUFFER + lOfs);
				char tmp[8]={0};
				GuildInfo->bIsGiveUp = g_DbManager.GetInt("IS_GUIVEUP");
				GuildInfo->btRegRank = (BYTE) g_DbManager.GetInt("SEQ_NUM");
				GuildInfo->iRegMarkCount = g_DbManager.GetInt("REG_MARKS");
				g_DbManager.GetStr("REG_SIEGE_GUILD", tmp);
				memcpy(GuildInfo->szGuildName,tmp,8);
				Counter++;
				lOfs+=sizeof(CSP_GUILDREGINFO);
			}while(Counter < 100 && g_DbManager.Fetch() != SQL_NO_DATA);
		}
	}
	g_DbManager.Clear();
	PHeadSetW((LPBYTE)pResult, 0x83, sizeof(CSP_ANS_ALLGUILDREGINFO) + sizeof(CSP_GUILDREGINFO) * Counter);
	pResult->iCount = Counter;
	thisManager->DataSend(aIndex, (unsigned char*)cBUFFER, sizeof(CSP_ANS_ALLGUILDREGINFO) + sizeof(CSP_GUILDREGINFO) * Counter);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsFirstCreateNPC(CDSManager * thisManager, int aIndex, CSP_REQ_NPCSAVEDATA * aRecv)
{
	int Counter = 0;
	BOOL bRet = FALSE;
	CSP_ANS_NPCSAVEDATA pResult;
	pResult.h.set(0x84, sizeof(pResult));
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	pResult.iResult = 0;

	int lOfs=sizeof(CSP_REQ_NPCSAVEDATA);

	bRet = g_DbManager.ExecFormat("DELETE FROM MuCastle_NPC WHERE MAP_SVR_GROUP = %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if(aRecv->iCount > 0)
		{
			while(Counter<aRecv->iCount)
			{
				g_DbManager.Clear();
				CSP_NPCSAVEDATA * NPCInfo = (CSP_NPCSAVEDATA*)((LPBYTE)aRecv + lOfs);
				bRet = g_DbManager.ExecFormat("EXEC WZ_CS_ReqNpcBuy %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",aRecv->wMapSvrNum,NPCInfo->iNpcNumber,NPCInfo->iNpcIndex,NPCInfo->iNpcDfLevel,NPCInfo->iNpcRgLevel,NPCInfo->iNpcMaxHp,NPCInfo->iNpcHp,NPCInfo->btNpcX,NPCInfo->btNpcY,NPCInfo->btNpcDIR);
				if (bRet != TRUE )
				{
					g_DbManager.Clear();
					break;
				}
				Counter++;
				lOfs+=sizeof(CSP_NPCSAVEDATA);
			}
			pResult.iResult = 1;
		}
	}
	g_DbManager.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult,sizeof(pResult));
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCalcRegGuildList(CDSManager * thisManager, int aIndex, CSP_REQ_CALCREGGUILDLIST * aRecv)
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

	BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_GetCalcRegGuildList %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if( g_DbManager.Fetch() != SQL_NO_DATA)
		{
			do
			{
				GuildInfo = (CSP_CALCREGGUILDLIST *)(cBUFFER + lOfs);
				char tmp[8]={0};
				GuildInfo->iGuildMasterLevel = g_DbManager.GetInt("GM_LEVEL");
				GuildInfo->iGuildMemberCount = g_DbManager.GetInt("GUILD_MEMBER");
				GuildInfo->iRegMarkCount = g_DbManager.GetInt("REG_MARKS");
				GuildInfo->iSeqNum = g_DbManager.GetInt("SEQ_NUM");
				g_DbManager.GetStr("REG_SIEGE_GUILD", tmp);
				memcpy(GuildInfo->szGuildName,tmp,8);
				Counter++;
				lOfs+=sizeof(CSP_CALCREGGUILDLIST);
			}while(Counter < 100 && g_DbManager.Fetch() != SQL_NO_DATA);
		}
	}
	g_DbManager.Clear();
	PHeadSetW((LPBYTE)pResult, 0x85, sizeof(CSP_ANS_CALCREGGUILDLIST) + sizeof(CSP_CALCREGGUILDLIST) * Counter);
	pResult->iCount = Counter;
	thisManager->DataSend(aIndex, (unsigned char*)cBUFFER, sizeof(CSP_ANS_CALCREGGUILDLIST) + sizeof(CSP_CALCREGGUILDLIST) * Counter);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCsGulidUnionInfo(CDSManager * thisManager, int aIndex, CSP_REQ_CSGUILDUNIONINFO * aRecv)
{
	int Counter = 0;
	int GuildCount = 0;
	char cBUFFER[10000]={0};

	CSP_ANS_CSGUILDUNIONINFO * pResult = (CSP_ANS_CSGUILDUNIONINFO *)(cBUFFER);
	CSP_CSGUILDUNIONINFO * GuildInfo = (CSP_CSGUILDUNIONINFO *)(cBUFFER + sizeof(CSP_ANS_CSGUILDUNIONINFO));

	//PHeadSetB((LPBYTE)pResult, 0x86, sizeof(pResult));
	pResult->h.set(0x86, sizeof(pResult));
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
			BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_GetCsGuildUnionInfo '%s'",GuildRecvInfo->szGuildName);
			
			if(( g_DbManager.Fetch() != SQL_NO_DATA) && (bRet != TRUE ))
			{
				g_DbManager.Clear();
				break;
			}
			
			do
			{
				GuildInfo = (CSP_CSGUILDUNIONINFO *)(cBUFFER + ROfs);
				char tmp[8]={0};
				g_DbManager.GetStr("GUILD_NAME", tmp);
				memcpy(GuildInfo->szGuildName,tmp,8);
				GuildInfo->iCsGuildID = GuildCount;
				GuildCount++;
				ROfs+=sizeof(CSP_CALCREGGUILDLIST);
			}while(GuildCount < 100 && g_DbManager.Fetch() != SQL_NO_DATA);
			g_DbManager.Clear();

			Counter++;
			lOfs+=sizeof(CSP_CSGUILDUNIONINFO);
		}
		pResult->iResult = 1;
	}
	PHeadSetW((LPBYTE)pResult, 0x86, sizeof(CSP_ANS_CSGUILDUNIONINFO) + sizeof(CSP_CSGUILDUNIONINFO) * GuildCount);
	pResult->iCount = Counter;
	thisManager->DataSend(aIndex, (unsigned char*)cBUFFER, sizeof(CSP_ANS_CSGUILDUNIONINFO) + sizeof(CSP_CSGUILDUNIONINFO) * GuildCount);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCsSaveTotalGuildInfo(CDSManager * thisManager, int aIndex, CSP_REQ_CSSAVETOTALGUILDINFO * aRecv)
{
	int Counter = 0;
	CSP_ANS_CSSAVETOTALGUILDINFO pResult;
	pResult.h.set(0x87, sizeof(pResult));
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	pResult.iResult = 0;

	int lOfs=sizeof(CSP_REQ_CSSAVETOTALGUILDINFO);

	if(aRecv->iCount > 0)
	{
		while(Counter<aRecv->iCount)
		{
			CSP_CSSAVETOTALGUILDINFO * GuildInfo = (CSP_CSSAVETOTALGUILDINFO*)((LPBYTE)aRecv + lOfs);
			BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_SetSiegeGuildInfo %d, '%s', %d, %d",aRecv->wMapSvrNum,GuildInfo->szGuildName,GuildInfo->iCsGuildID,GuildInfo->iGuildInvolved);
			if (bRet != TRUE )
			{
				g_DbManager.Clear();
				break;
			}
			g_DbManager.Clear();
			Counter++;
			lOfs+=sizeof(CSP_CSSAVETOTALGUILDINFO);
		}
		pResult.iResult = 1;
	}
	thisManager->DataSend(aIndex, (unsigned char*)&pResult,sizeof(pResult));
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCsLoadTotalGuildInfo(CDSManager * thisManager, int aIndex, CSP_REQ_CSLOADTOTALGUILDINFO * aRecv)
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

	BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CS_GetSiegeGuildInfo %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if( g_DbManager.Fetch() != SQL_NO_DATA)
		{
			do
			{
				GuildInfo = (CSP_CSLOADTOTALGUILDINFO *)(cBUFFER + lOfs);
				char tmp[8]={0};
				GuildInfo->iCsGuildID = g_DbManager.GetInt("GUILD_ID");
				GuildInfo->iGuildInvolved = g_DbManager.GetInt("GUILD_INVOLVED");
				g_DbManager.GetStr("GUILD_NAME", tmp);
				memcpy(GuildInfo->szGuildName,tmp,8);
				Counter++;
				lOfs+=sizeof(CSP_CSLOADTOTALGUILDINFO);
			}while(Counter < 100 && g_DbManager.Fetch() != SQL_NO_DATA);
		}
	}
	g_DbManager.Clear();
	PHeadSetW((LPBYTE)pResult, 0x88, sizeof(CSP_ANS_CSLOADTOTALGUILDINFO) + sizeof(CSP_CSLOADTOTALGUILDINFO) * Counter);
	pResult->iCount = Counter;
	thisManager->DataSend(aIndex, (unsigned char*)cBUFFER, sizeof(CSP_ANS_CSLOADTOTALGUILDINFO) + sizeof(CSP_CSLOADTOTALGUILDINFO) * Counter);
}
//-----------------------------------------------------------------------------------------------------------

void GS_DGAnsCastleNpcUpdate(CDSManager * thisManager, int aIndex, CSP_REQ_NPCUPDATEDATA * aRecv)
{
	int Counter = 0;
	CSP_ANS_NPCUPDATEDATA pResult;
	pResult.h.set(0x89, sizeof(pResult));
	pResult.wMapSvrNum = aRecv->wMapSvrNum;
	pResult.iResult = 0;
	BOOL bRet = FALSE;

	int lOfs=sizeof(CSP_REQ_NPCUPDATEDATA);

	bRet = g_DbManager.ExecFormat("DELETE FROM MuCastle_NPC WHERE MAP_SVR_GROUP = %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if(aRecv->iCount > 0)
		{
			while(Counter<aRecv->iCount)
			{
				g_DbManager.Clear();
				CSP_NPCUPDATEDATA * NPCInfo = (CSP_NPCUPDATEDATA*)((LPBYTE)aRecv + lOfs);
				bRet = g_DbManager.ExecFormat("EXEC WZ_CS_ReqNpcUpdate %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",aRecv->wMapSvrNum,NPCInfo->iNpcNumber,NPCInfo->iNpcIndex,NPCInfo->iNpcDfLevel,NPCInfo->iNpcRgLevel,NPCInfo->iNpcMaxHp,NPCInfo->iNpcHp,NPCInfo->btNpcX,NPCInfo->btNpcY,NPCInfo->btNpcDIR);
				if (bRet != TRUE )
				{
					g_DbManager.Clear();
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

void DGAnsCrywolfSync(CDSManager * thisManager, int aIndex, CWP_REQ_CRYWOLFSYNC * aRecv)
{
	CWP_ANS_CRYWOLFSYNC pResult;
	pResult.h.set(0xB0, sizeof(pResult));
	pResult.iCrywolfState = aRecv->iCrywolfState;
	pResult.iOccupationState = aRecv->iOccupationState;
	pResult.btResult = aRecv->iOccupationState;	//REVEER

	thisManager->DataSendAll((unsigned char*)&pResult,sizeof(pResult));
}
//-----------------------------------------------------------------------------------------------------------

void DGAnsCrywolfInfoLoad(CDSManager * thisManager, int aIndex, CWP_REQ_CRYWOLFINFOLOAD * aRecv)
{
	CWP_ANS_CRYWOLFINFOLOAD pResult;
	pResult.h.set(0xB1, sizeof(pResult));
	
	pResult.btResult = 0;
	pResult.iCrywolfState = 0;
	pResult.iOccupationState = 0;
	
	BOOL bRet = g_DbManager.ExecFormat("EXEC WZ_CW_InfoLoad %d",aRecv->wMapSvrNum);
	if (bRet == TRUE )
	{
		if( g_DbManager.Fetch() != SQL_NO_DATA)
		{	
			pResult.btResult = 1;
			pResult.iCrywolfState = g_DbManager.GetInt("CRYWOLF_STATE");
			pResult.iOccupationState = g_DbManager.GetInt("CRYWOLF_OCCUFY");
		}
	}
	g_DbManager.Clear();
	thisManager->DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------