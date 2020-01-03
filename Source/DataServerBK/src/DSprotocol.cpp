// DSprotocol.cpp: implementation of the DSprotocol class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DSprotocol.h"

#include "giocp.h"
#include "LogProc.h"
#include "winutil.h"
#include "Query.h"

CQuery g_MuOnlineDB;


void InitMuOnlineDB()
{
	if ( g_MuOnlineDB.Connect(g_MuOnlineDNS, g_UserID, g_Password) == TRUE )
	{
		LogAddC(4, "[MuOnlineDB] Connection Successfull!");
	}
	else
	{
		LogAddC(2, "[MuOnlineDB] Error On Connection!");
	}
}


void DSProtocolCore(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize)
{
	//if(g_UseDataServer1 == TRUE | g_UseDataServer2 == TRUE)
	if(g_UseDataServer1 == TRUE || g_UseDataServer2 == TRUE) // VuongLQ
	{
	g_ServerInfoDisplayer.CheckDSConnection(TRUE);
#if (TRACE_PACKET == 1 )
	LogAddHeadHex("DATA_SERVER", aRecv, iSize);
#endif

	switch ( HeadCode )
	{
		case 0x00:
			DataServerLogin(aIndex, (SDHP_SERVERINFO *)aRecv);
			break;
		case 0x01:
			JGPGetCharList(aIndex, (SDHP_GETCHARLIST *)aRecv);
			break;
		case 0x04:
			JGCharacterCreateRequest(aIndex, (SDHP_CREATECHAR *)aRecv);
			break;
		case 0x05:
			JGCharDelRequest(aIndex, (SDHP_CHARDELETE *)aRecv);
			break;
		case 0x06:
			JGGetCharacterInfo(aIndex, (SDHP_DBCHARINFOREQUEST *)aRecv);
			break;
		case 0x07:
			GJSetCharacterInfo(aIndex, (SDHP_DBCHAR_INFOSAVE *)aRecv);
			break;
		case 0x08:
			DGGetWarehouseList(aIndex, (SDHP_GETWAREHOUSEDB *) aRecv);
			break;
		case 0x09:
			GDSetWarehouseList(aIndex, (SDHP_GETWAREHOUSEDB_SAVE *) aRecv);
			break;
		case 0x11:
			GDUserItemSave(aIndex, (SDHP_DBCHAR_ITEMSAVE *) aRecv);
			break;
		/*case 0x20:
			Packet that SET the ItemCount from GameServerInfo - Not Sended by GSq	
			break;*/
		/*case 0x2D:
			Packet that GET via GetPropertyInfo() SP - Not Referenced - Send PAcket 0x2D - Not Referenced
			break;*/
		case 0x52:
			ItemSerialCreateRecv(aIndex, (SDHP_ITEMCREATE *) aRecv);
			break;
		/*case 0x53:
			Packet that is sended by gs but not found in DS
			break;*/
		/*case 0x54:
			Packet that is for CItemPropertyExt
			break;*/
		case 0x55:
			PetItemSerialCreateRecv(aIndex, (SDHP_ITEMCREATE *) aRecv);
			break;
		case 0x56:
			DGRecvPetItemInfo(aIndex, (SDHP_REQUEST_PETITEM_INFO *) aRecv);
			break;
		case 0x57:
			GDSavePetItemInfo(aIndex, (SDHP_SAVE_PETITEM_INFO *) aRecv);
			break;
		case 0x60:
			DGOptionDataRecv(aIndex, (SDHP_SKILLKEYDATA *) aRecv);
			break;
		case 0xCF:
			DGMoveOtherServer(aIndex, (SDHP_CHARACTER_TRANSFER *) aRecv);
			break;
	}

	g_ServerInfoDisplayer.CheckDSConnection(FALSE);
	}
}



void DataServerLogin(int aIndex, SDHP_SERVERINFO * lpMsg)
{
	SDHP_RESULT pResult = {0};

	PHeadSetB((LPBYTE)&pResult, 0x00, sizeof(pResult));
	pResult.Result = 1;

	if ( g_MuOnlineDB.ExecQuery("SELECT ItemCount FROM GameServerInfo WHERE Number=0") == TRUE )
	{
		g_MuOnlineDB.Fetch();
		pResult.ItemCount = g_MuOnlineDB.GetAsInteger("ItemCount");
	}
	
	g_MuOnlineDB.Close();

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}



void JGPGetCharList(int aIndex, SDHP_GETCHARLIST * aRecv)
{
	char cBUFFER[10000]={0};

	SDHP_CHARLISTCOUNT * pCount = (SDHP_CHARLISTCOUNT *)(cBUFFER);
	SDHP_CHARLIST * pCL = (SDHP_CHARLIST *)(cBUFFER + sizeof(SDHP_CHARLISTCOUNT));

	char szAccountID[11]={0};
	memcpy(szAccountID, aRecv->Id, 10);

	pCount->Number = aRecv->Number;
	lstrcpy(pCount->AccountId, szAccountID);

	g_MuOnlineDB.ExecQuery("SELECT Id FROM AccountCharacter WHERE Id='%s'", szAccountID);

	if ( g_MuOnlineDB.Fetch() == SQL_NO_DATA )
	{
		g_MuOnlineDB.Close();
		g_MuOnlineDB.ExecQuery("INSERT INTO AccountCharacter (Id) VALUES ('%s')", szAccountID);
		g_MuOnlineDB.Fetch();
	}
	
	g_MuOnlineDB.Close();


	g_MuOnlineDB.ExecQuery("SELECT * FROM AccountCharacter WHERE Id='%s'", szAccountID);
		g_MuOnlineDB.Fetch();
		TCHAR CharName[5][11]={0};
		pCount->MoveCnt = (BYTE)g_MuOnlineDB.GetAsInteger("MoveCnt");
		g_MuOnlineDB.GetAsString("GameID1", CharName[0]);
		g_MuOnlineDB.GetAsString("GameID2", CharName[1]);
		g_MuOnlineDB.GetAsString("GameID3", CharName[2]);
		g_MuOnlineDB.GetAsString("GameID4", CharName[3]);
		g_MuOnlineDB.GetAsString("GameID5", CharName[4]);
	g_MuOnlineDB.Close();

	int iCharCount=0;
	int lOfs=sizeof(SDHP_CHARLISTCOUNT);

	for(int i=0;i<5;i++)
	{
		if ( CharName[i][0] != 0 )
		{
			if ( lstrlen(CharName[i]) >= 4 )
			{
				if ( g_MuOnlineDB.ExecQuery("SELECT cLevel, Class, CtlCode, DbVersion FROM vCharacterPreview WHERE Name='%s'", CharName[i]) == TRUE )
				{
					char szTemp[200];
					pCL = (SDHP_CHARLIST *)(cBUFFER + lOfs);

					g_MuOnlineDB.Fetch();
					pCL->Level = (WORD)g_MuOnlineDB.GetAsInteger("cLevel");
					pCL->Class = (BYTE)g_MuOnlineDB.GetAsInteger("Class");

					if ( pCL->Class >= 220 && pCount->Magumsa == 0 )
						pCount->Magumsa = 1;
					
					if ( pCL->Class >= 250 && (pCount->Magumsa == 0 || pCount->Magumsa == 1) )
						pCount->Magumsa = 2;

					pCL->CtlCode = (BYTE)g_MuOnlineDB.GetAsInteger("CtlCode");
					pCL->DbVersion = (BYTE)g_MuOnlineDB.GetAsInteger("DbVersion");
					g_MuOnlineDB.Close();

					BYTE btInvetory[INVENTORY_BINARY_SIZE];
					wsprintf(szTemp, "SELECT Inventory FROM Character WHERE Name='%s'", CharName[i]);
					
					if ( g_MuOnlineDB.GetAsBinary(szTemp, btInvetory) == -1 )
						memset(btInvetory, -1, sizeof(btInvetory));

					g_MuOnlineDB.Close();

					if ( pCL->DbVersion == 3 )
					{
						memset(pCL->dbInventory, -1, sizeof(pCL[iCharCount].dbInventory));

						for(int i=0;i<12;i++)
						{
							if ( btInvetory[0+i*16] == 0xFF && (btInvetory[7+i*16] & 0x80 ) == 0x80 && (btInvetory[10+i*16] & 0xF0) == 0xF0 )
							{
								pCL->dbInventory[i*4] = -1;
								pCL->dbInventory[i*4+1] = -1;
								pCL->dbInventory[i*4+2] = -1;
								pCL->dbInventory[i*4+3] = -1;
							}
							else
							{
								pCL->dbInventory[i*4]  = (btInvetory[0+i*16]);	// 0..7 bits of Item
								pCL->dbInventory[i*4+1]  = (btInvetory[1+i*16]);	// Get Level of Item
								pCL->dbInventory[i*4+2]  = (btInvetory[7+i*16]);	// 8 bit     of Item
								pCL->dbInventory[i*4+3]  = (btInvetory[10+i*16]);	// 9..12 bit of Item
							}
						}
					}

					g_MuOnlineDB.ExecQuery("SELECT G_Status FROM GuildMember WHERE Name='%s'", CharName[i]);
					g_MuOnlineDB.Fetch();
					pCL->btGuildStatus = (BYTE)g_MuOnlineDB.GetAsInteger("G_Status");
					pCL->Index = i;
					lstrcpy(pCL[iCharCount].Name, CharName[i]);
					g_MuOnlineDB.Close();
					iCharCount++;
					lOfs+=sizeof(SDHP_CHARLIST);
				}

				g_MuOnlineDB.Close();
			}
		}
	}

	PHeadSetW((LPBYTE)pCount, 0x01, sizeof(SDHP_CHARLISTCOUNT) + sizeof(SDHP_CHARLIST) * iCharCount);
	pCount->Count = iCharCount;
	
	DataSend(aIndex, (LPBYTE)cBUFFER, sizeof(SDHP_CHARLISTCOUNT) + sizeof(SDHP_CHARLIST) * iCharCount);
}


void JGCharacterCreateRequest(int aIndex, SDHP_CREATECHAR * aRecv)
{
	SDHP_CREATECHARRESULT pResult={0};
	PHeadSetB((LPBYTE)&pResult, 0x04, sizeof(pResult));
	memcpy(pResult.AccountId, aRecv->AccountId, 10);
	pResult.ClassSkin = aRecv->ClassSkin;
	pResult.Number = aRecv->Number;
	int iIndex=0;

	char szAccountID[11]={0};
	char szName[11]={0};
	memcpy(szAccountID, aRecv->AccountId, 10);
	memcpy(szName, aRecv->Name, 10);

	g_MuOnlineDB.ExecQuery("SELECT * FROM AccountCharacter WHERE Id='%s'", szAccountID);
		g_MuOnlineDB.Fetch();
		TCHAR CharName[5][11]={0};
		g_MuOnlineDB.GetAsString("GameID1", CharName[0]);
		g_MuOnlineDB.GetAsString("GameID2", CharName[1]);
		g_MuOnlineDB.GetAsString("GameID3", CharName[2]);
		g_MuOnlineDB.GetAsString("GameID4", CharName[3]);
		g_MuOnlineDB.GetAsString("GameID5", CharName[4]);

		
		for(iIndex=0;iIndex<5;iIndex++)
		{
			if ( CharName[iIndex][0] == 0 )
				break;
		}

		if ( iIndex == 5 )
			pResult.Result = 2;

		memcpy(pResult.Name, aRecv->Name, 10);
		pResult.Pos = iIndex;
	g_MuOnlineDB.Close();

	if ( (pResult.ClassSkin >> 4) < 0 || (pResult.ClassSkin >> 4) > MAX_TYPE_PLAYER-1 )
		pResult.Result = 0;

	g_MuOnlineDB.ExecQuery("WZ_CreateCharacter '%s', '%s', '%d'",
		szAccountID, szName, pResult.ClassSkin);
	g_MuOnlineDB.Fetch();
	pResult.Result = g_MuOnlineDB.GetResult(0);
	g_MuOnlineDB.Close();
	
	

	if ( pResult.Result == 1 )
	{
		g_MuOnlineDB.ExecQuery("UPDATE AccountCharacter SET GameID%d='%s' WHERE Id='%s'",
			iIndex+1, szName, szAccountID);
		g_MuOnlineDB.Fetch();
		g_MuOnlineDB.Close();
	}

	//pResult.Equipment
	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}





void JGCharDelRequest(int aIndex, SDHP_CHARDELETE * aRecv)
{
	SDHP_CHARDELETERESULT pResult={0};

	PHeadSetB((LPBYTE)&pResult, 0x05, sizeof(pResult));
	memcpy(pResult.AccountID, aRecv->AccountID, 10);
	pResult.Number = aRecv->Number;
	pResult.Result = 1;

	char szAccountID[11]={0};
	char szName[11]={0};
	memcpy(szAccountID, aRecv->AccountID, 10);
	memcpy(szName, aRecv->Name, 10);

	if ( lstrlen(aRecv->AccountID) < 1 )
		pResult.Result = 0;

	if ( lstrlen(aRecv->Name) < 1 )
		pResult.Result = 0;

	if ( pResult.Result == 1 )
	{
		if ( g_MuOnlineDB.ExecQuery("DELETE FROM Character WHERE AccountID='%s' AND Name='%s'",
				szAccountID, szName) == TRUE )
		{
			g_MuOnlineDB.Fetch();
			g_MuOnlineDB.Close();

			g_MuOnlineDB.ExecQuery("SELECT * FROM AccountCharacter WHERE Id='%s'", szAccountID);
			g_MuOnlineDB.Fetch();
			TCHAR CharName[5][11]={0};
			g_MuOnlineDB.GetAsString("GameID1", CharName[0]);
			g_MuOnlineDB.GetAsString("GameID2", CharName[1]);
			g_MuOnlineDB.GetAsString("GameID3", CharName[2]);
			g_MuOnlineDB.GetAsString("GameID4", CharName[3]);
			g_MuOnlineDB.GetAsString("GameID5", CharName[4]);
			int iIndex;
			for(iIndex=0;iIndex<5;iIndex++)
			{
				if ( lstrcmp(CharName[iIndex], szName) == 0 )
				{
					break;
				}
			}
			g_MuOnlineDB.Close();

			g_MuOnlineDB.ExecQuery("UPDATE AccountCharacter SET GameID%d=NULL WHERE Id='%s'", iIndex+1, szAccountID);
			g_MuOnlineDB.Fetch();
			g_MuOnlineDB.Close();
		}
		else
		{
			g_MuOnlineDB.Fetch();
			g_MuOnlineDB.Close();
			pResult.Result = 0;
		}
	}

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}



void JGGetCharacterInfo(int aIndex, SDHP_DBCHARINFOREQUEST * aRecv)
{
	SDHP_DBCHAR_INFORESULT pResult ={0};

	PHeadSetW((LPBYTE)&pResult, 0x06, sizeof(pResult));
	char szAccountID[11]={0};
	char szName[11]={0};
	BuxConvert(aRecv->Name, 10);
	memcpy(szAccountID, aRecv->AccountID, 10);
	memcpy(szName, aRecv->Name, 10);
	memcpy(pResult.AccountID, aRecv->AccountID, 10);
	memcpy(pResult.Name, aRecv->Name, 10);
	pResult.Number = aRecv->Number;
	//pResult.AccountCtlCode
	pResult.result = 0;
	
	g_MuOnlineDB.ExecQuery("SELECT cLevel, Class, LevelUpPoint, Experience, Strength, Dexterity, Vitality, Energy, Money, Life, MaxLife, Mana, MaxMana, MapNumber, MapPosX, MapPosY, MapDir, PkCount, PkLevel, PkTime, CtlCode, DbVersion, Leadership, ChatLimitTime, FruitPoint FROM Character WHERE Name='%s'", szName);

	if ( g_MuOnlineDB.Fetch() != SQL_NO_DATA ) 
	{
		
		pResult.Level = (short)g_MuOnlineDB.GetAsInteger("cLevel"); 
		pResult.Class = (BYTE)g_MuOnlineDB.GetAsInteger("Class");
		pResult.LevelUpPoint = g_MuOnlineDB.GetAsInteger("LevelUpPoint");
		pResult.Exp = g_MuOnlineDB.GetAsInteger("Experience");
		pResult.Str = (short)g_MuOnlineDB.GetAsInteger("Strength"); 
		pResult.Dex = (short)g_MuOnlineDB.GetAsInteger("Dexterity");
		pResult.Vit = (short)g_MuOnlineDB.GetAsInteger("Vitality");
		pResult.Energy = (short)g_MuOnlineDB.GetAsInteger("Energy");
		pResult.Money = g_MuOnlineDB.GetAsInteger("Money");
		pResult.Life = (WORD)g_MuOnlineDB.GetAsFloat("Life");
		pResult.MaxLife = (WORD)g_MuOnlineDB.GetAsFloat("MaxLife");
		pResult.Mana = (WORD)g_MuOnlineDB.GetAsFloat("Mana");
		pResult.MaxMana = (WORD)g_MuOnlineDB.GetAsFloat("MaxMana");
		pResult.MapNumber= (BYTE)g_MuOnlineDB.GetAsInteger("MapNumber");
		pResult.MapX = (BYTE)g_MuOnlineDB.GetAsInteger("MapX");
		pResult.MapY = (BYTE)g_MuOnlineDB.GetAsInteger("MapY");
		pResult.Dir = (BYTE)g_MuOnlineDB.GetAsInteger("MapDir");
		pResult.PkCount = g_MuOnlineDB.GetAsInteger("PkCount");
		pResult.PkLevel = g_MuOnlineDB.GetAsInteger("PkLevel");
		pResult.PkTime = g_MuOnlineDB.GetAsInteger("PkTime");
		pResult.CtlCode = (BYTE)g_MuOnlineDB.GetAsInteger("CtlCode");
		pResult.DbVersion = (BYTE)g_MuOnlineDB.GetAsInteger("DbVersion");
		pResult.Leadership = (WORD)g_MuOnlineDB.GetAsInteger("Leadership");
		pResult.ChatLitmitTime = (WORD)g_MuOnlineDB.GetAsInteger("ChatLimitTime");
		pResult.iFruitPoint  = g_MuOnlineDB.GetAsInteger("FruitPoint");
		g_MuOnlineDB.Close();

		char szTemp[256];
		BYTE btTemp[INVENTORY_BINARY_SIZE]={0};

		wsprintf(szTemp, "SELECT Inventory from Character where Name='%s'", szName);
		if ( g_MuOnlineDB.GetAsBinary(szTemp, btTemp) == -1 )
			memset(btTemp, -1, sizeof(btTemp));
		memcpy(pResult.dbInventory, btTemp, sizeof(pResult.dbInventory));
		memset(btTemp, 0, sizeof(btTemp));
		g_MuOnlineDB.Close();

		wsprintf(szTemp, "SELECT MagicList from Character where Name='%s'", szName);
		if ( g_MuOnlineDB.GetAsBinary(szTemp, btTemp) == -1 ) 
			memset(btTemp, -1, sizeof(btTemp));
		memcpy(pResult.dbMagicList, btTemp, sizeof(pResult.dbMagicList));
		memset(btTemp, 0, sizeof(btTemp));
		g_MuOnlineDB.Close();

		wsprintf(szTemp, "SELECT Quest from Character where Name='%s'", szName);
		if ( g_MuOnlineDB.GetAsBinary(szTemp, btTemp) == -1 ) 
			memset(btTemp, -1, sizeof(btTemp));
		memcpy(pResult.dbQuest, btTemp, sizeof(pResult.dbQuest));
		memset(btTemp, 0, sizeof(btTemp));
		g_MuOnlineDB.Close();

		pResult.result = 1;


		g_MuOnlineDB.ExecQuery("UPDATE AccountCharacter SET GameIDC='%s' WHERE Id='%s'", szName, szAccountID);
		g_MuOnlineDB.Fetch();
		g_MuOnlineDB.Close();
	}

	g_MuOnlineDB.Close();

	DataSend(aIndex, (LPBYTE)&pResult, sizeof(pResult));

	if ( pResult.result == 1 )
	{
		SDHP_SKILLKEYDATA_SEND pSkillData={0};

		PHeadSetB((LPBYTE)&pSkillData, 0x60, sizeof(pSkillData));
		pSkillData.aIndex = aRecv->Number;

		g_MuOnlineDB.ExecQuery("SELECT * FROM OptionData WHERE Name='%s'", szName);

		if (g_MuOnlineDB.Fetch() != SQL_NO_DATA )
		{
			memcpy(pSkillData.Name, aRecv->Name, 10);
			pSkillData.GameOption = (BYTE)g_MuOnlineDB.GetAsInteger("GameOption");
			pSkillData.QkeyDefine = (BYTE)g_MuOnlineDB.GetAsInteger("QKey");
			pSkillData.WkeyDefine = (BYTE)g_MuOnlineDB.GetAsInteger("WKey");
			pSkillData.EkeyDefine = (BYTE)g_MuOnlineDB.GetAsInteger("EKey");
			pSkillData.ChatWindow = (BYTE)g_MuOnlineDB.GetAsInteger("ChatWindow");
			g_MuOnlineDB.Close();

			BYTE btTemp[sizeof(pSkillData.SkillKeyBuffer)]={0};
			char szTemp[256];
			wsprintf(szTemp, "SELECT SkillKey from OptionData where Name='%s'", szName);
			g_MuOnlineDB.GetAsBinary(szTemp, btTemp);
			memcpy(pSkillData.SkillKeyBuffer, btTemp, sizeof(pSkillData.SkillKeyBuffer));
		}

		g_MuOnlineDB.Close();

		DataSend(aIndex, (LPBYTE)&pSkillData, sizeof(pSkillData));
	}
}


void GJSetCharacterInfo(int aIndex, SDHP_DBCHAR_INFOSAVE * aRecv)
{
	char szName[11]={0};
	memcpy(szName, aRecv->Name, 10);
	
	g_MuOnlineDB.ExecQuery("UPDATE Character SET cLevel=%d,Class=%d,LevelUpPoint=%d, Experience=%d, Strength=%d, Dexterity=%d, Vitality=%d, Energy=%d, Money=%d, Life=%f, MaxLife=%f, Mana=%f, MaxMana=%f, MapNumber=%d, MapPosX=%d, MapPosY=%d, MapDir=%d, PkCount=%d, PkLevel=%d, PkTime=%d, DbVersion=3, Leadership=%d, ChatLimitTime=%d, FruitPoint=%d WHERE Name = '%s'",
		aRecv->Level, aRecv->Class, aRecv->LevelUpPoint, aRecv->Exp, aRecv->Str, aRecv->Dex, aRecv->Vit, aRecv->Energy, aRecv->Money, aRecv->Life, aRecv->MaxLife, aRecv->Mana, aRecv->MaxMana, aRecv->MapNumber, aRecv->MapX, aRecv->MapY, aRecv->Dir, aRecv->PkCount, aRecv->PkLevel, aRecv->PkTime, aRecv->Leadership, aRecv->ChatLitmitTime, aRecv->iFruitPoint, szName);
	g_MuOnlineDB.Fetch();
	g_MuOnlineDB.Close();

	char szTemp[128];
	wsprintf(szTemp, "UPDATE Character SET Inventory=? WHERE Name='%s'", szName);
	g_MuOnlineDB.SetAsBinary(szTemp, aRecv->dbInventory, sizeof(aRecv->dbInventory));
	g_MuOnlineDB.Fetch();
	g_MuOnlineDB.Close();

	wsprintf(szTemp, "UPDATE Character SET MagicList=? WHERE Name='%s'", szName);
	g_MuOnlineDB.SetAsBinary(szTemp, aRecv->dbMagicList, sizeof(aRecv->dbMagicList));
	g_MuOnlineDB.Fetch();
	g_MuOnlineDB.Close();

	wsprintf(szTemp, "UPDATE Character SET Quest=? WHERE Name='%s'", szName);
	g_MuOnlineDB.SetAsBinary(szTemp, aRecv->dbQuest, sizeof(aRecv->dbQuest));
	g_MuOnlineDB.Fetch();
	g_MuOnlineDB.Close();

	if ( aRecv->CharInfoSave == 1 )
	{
		g_MuOnlineDB.ExecQuery("INSERT INTO T_CurCharName (Name) VALUES('%s')", szName);
		g_MuOnlineDB.Fetch();
		g_MuOnlineDB.Close();
	}
}



void DGGetWarehouseList(int aIndex, SDHP_GETWAREHOUSEDB * aRecv)
{
	char szAccountID[11]={0};
	memcpy(szAccountID, aRecv->AccountID, 10);


	SDHP_GETWAREHOUSEDB_SAVE pResult;

	PHeadSetW((LPBYTE)&pResult, 0x08, sizeof(pResult));
	memcpy(pResult.AccountID, szAccountID, 10);
	pResult.aIndex = aRecv->aIndex;
	
	BOOL bRet = g_MuOnlineDB.ExecQuery("SELECT AccountID FROM warehouse WHERE AccountID='%s'", szAccountID);
	
	if ( g_MuOnlineDB.Fetch() == SQL_NO_DATA || bRet == FALSE )
	{
		g_MuOnlineDB.Close();
		g_MuOnlineDB.ExecQuery("INSERT INTO warehouse (AccountID, Money, EndUseDate, DbVersion) VALUES ('%s',0, getdate(), 3)", szAccountID);
		g_MuOnlineDB.Fetch();
		g_MuOnlineDB.Close();

		SDHP_GETWAREHOUSEDB_RESULT pMsg;

		PHeadSetB((LPBYTE)&pMsg, 0x10, sizeof(pMsg));
		pMsg.aIndex = aRecv->aIndex;
		memcpy(pMsg.AccountID, szAccountID, 10);

		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	g_MuOnlineDB.Close();

	g_MuOnlineDB.ExecQuery("SELECT Money, DbVersion, pw FROM warehouse WHERE AccountID='%s'", szAccountID);
	g_MuOnlineDB.Fetch();
	pResult.Money = g_MuOnlineDB.GetAsInteger("Money");
	pResult.DbVersion = g_MuOnlineDB.GetAsInteger("DbVersion");
	pResult.pw = g_MuOnlineDB.GetAsInteger("pw");
	g_MuOnlineDB.Close();

	char szTemp[128];
	wsprintf(szTemp, "SELECT Items FROM warehouse WHERE AccountID='%s'", szAccountID);
	
	if ( g_MuOnlineDB.GetAsBinary(szTemp, pResult.dbItems) < 0 )
		memset(pResult.dbItems, -1, sizeof(pResult.dbItems));

	g_MuOnlineDB.Close();

	DataSend(aIndex, (LPBYTE)&pResult, sizeof(pResult));
}



void GDSetWarehouseList(int aIndex, SDHP_GETWAREHOUSEDB_SAVE * aRecv)
{
	char szAccountID[11]={0};
	memcpy(szAccountID, aRecv->AccountID, 10);

	g_MuOnlineDB.ExecQuery("UPDATE warehouse SET Money=%d, pw=%d, DbVersion=3 WHERE AccountID='%s'",
		aRecv->Money, aRecv->pw, szAccountID);
	g_MuOnlineDB.Fetch();
	g_MuOnlineDB.Close();

	char szTemp[128];
	wsprintf(szTemp, "UPDATE warehouse SET Items=? WHERE AccountID='%s'", szAccountID);
	g_MuOnlineDB.SetAsBinary(szTemp, aRecv->dbItems, sizeof(aRecv->dbItems));
	g_MuOnlineDB.Close();
}


void GDUserItemSave(int aIndex, SDHP_DBCHAR_ITEMSAVE * aRecv)
{
	char szName[11]={0};
	memcpy(szName, aRecv->Name, 10);
	
	g_MuOnlineDB.ExecQuery("UPDATE Character SET DbVersion=3 WHERE Name = '%s'", szName);
	g_MuOnlineDB.Fetch();
	g_MuOnlineDB.Close();

	char szTemp[128];
	wsprintf(szTemp, "UPDATE Character SET Inventory=? WHERE Name='%s'", szName);
	g_MuOnlineDB.SetAsBinary(szTemp, aRecv->dbInventory, sizeof(aRecv->dbInventory));
	g_MuOnlineDB.Close();
}



void GDSetWarehouseMoney(int aIndex, SDHP_WAREHOUSEMONEY_SAVE * aRecv)
{
	char szAccountID[11]={0};
	memcpy(szAccountID, aRecv->AccountID, 10);
	
	g_MuOnlineDB.ExecQuery("UPDATE warehouse SET Money=%d WHERE AccountID='%s'", aRecv->Money, szAccountID);
	g_MuOnlineDB.Fetch();
	g_MuOnlineDB.Close();
}

void ItemSerialCreateRecv(int aIndex, SDHP_ITEMCREATE * aRecv)
{
	SDHP_ITEMCREATERECV pResult;

	PHeadSetB((LPBYTE)&pResult, 0x52, sizeof(pResult));
	pResult.aIndex = aRecv->aIndex;
	pResult.Dur = aRecv->Dur;
	pResult.Level = aRecv->Level;
	pResult.lootindex = aRecv->lootindex;
	g_MuOnlineDB.ExecQuery("EXEC WZ_GetItemSerial");
	g_MuOnlineDB.Fetch();
	pResult.m_Number = g_MuOnlineDB.GetResult(0);
	g_MuOnlineDB.Close();
	pResult.MapNumber = aRecv->MapNumber;
	pResult.NewOption = aRecv->NewOption;
	pResult.Op1 = aRecv->Op1;
	pResult.Op2 = aRecv->Op2;
	pResult.Op3 = aRecv->Op3;
	pResult.SetOption = aRecv->SetOption;
	pResult.Type = aRecv->Type;
	pResult.x = aRecv->x;
	pResult.y = aRecv->y;

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}



void PetItemSerialCreateRecv(int aIndex, SDHP_ITEMCREATE * aRecv)
{
	SDHP_ITEMCREATERECV pResult;

	PHeadSetB((LPBYTE)&pResult, 0x52, sizeof(pResult));
	pResult.aIndex = aRecv->aIndex;
	pResult.Dur = aRecv->Dur;
	pResult.Level = aRecv->Level;
	pResult.lootindex = aRecv->lootindex;
	g_MuOnlineDB.ExecQuery("EXEC WZ_GetItemSerial");
	g_MuOnlineDB.Fetch();
	pResult.m_Number = g_MuOnlineDB.GetResult(0);
	g_MuOnlineDB.Close();
	g_MuOnlineDB.ExecQuery("INSERT INTO T_PetItem_Info (ItemSerial, Pet_Level, Pet_Exp) VALUES (%d, %d, %d)",
		pResult.m_Number, 1, 0);
	g_MuOnlineDB.Fetch();
	g_MuOnlineDB.Close();
	pResult.MapNumber = aRecv->MapNumber;
	pResult.NewOption = aRecv->NewOption;
	pResult.Op1 = aRecv->Op1;
	pResult.Op2 = aRecv->Op2;
	pResult.Op3 = aRecv->Op3;
	pResult.SetOption = aRecv->SetOption;
	pResult.Type = aRecv->Type;
	pResult.x = aRecv->x;
	pResult.y = aRecv->y;

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}



void DGRecvPetItemInfo(int aIndex, SDHP_REQUEST_PETITEM_INFO * aRecv)
{
	char szAccountID[11]={0};
	memcpy(szAccountID, aRecv->AccountID, 10);
	int lOfs1 = sizeof(SDHP_REQUEST_PETITEM_INFO);
	int lOfs2 = sizeof(SDHP_RECV_PETITEM_INFO);
	char cBUFFER[1000]={0};
	SDHP_RECV_PETITEM_INFO * pRecvPetInfoCount = (SDHP_RECV_PETITEM_INFO *)(cBUFFER);
	Request_PetItem_Info * pRequestPetInfo;
	Recv_PetItem_Info * pRecvPetInfo;

	for(int i=0;i<aRecv->nCount;i++)
	{
		pRequestPetInfo = (Request_PetItem_Info *)((LPBYTE)aRecv + lOfs1);
		pRecvPetInfo = (Recv_PetItem_Info *)((LPBYTE)cBUFFER + lOfs2);

		g_MuOnlineDB.ExecQuery("SELECT Pet_Level, Pet_Exp FROM T_PetItem_Info WHERE ItemSerial=%d",
			pRequestPetInfo->nSerial);
		
		if ( g_MuOnlineDB.Fetch() == SQL_NO_DATA )
		{
			g_MuOnlineDB.Close();
			g_MuOnlineDB.ExecQuery("INSERT INTO T_PetItem_Info (ItemSerial, Pet_Level, Pet_Exp) VALUES (%d, %d, %d)",
				pRequestPetInfo->nSerial, 1, 0);
			g_MuOnlineDB.Fetch();
			g_MuOnlineDB.Close();

			g_MuOnlineDB.ExecQuery("SELECT Pet_Level, Pet_Exp FROM T_PetItem_Info WHERE ItemSerial=%d",
				pRequestPetInfo->nSerial);
			g_MuOnlineDB.Fetch();
		}

		pRecvPetInfo->Level = g_MuOnlineDB.GetAsInteger("Pet_Level");
		pRecvPetInfo->Exp = g_MuOnlineDB.GetAsInteger("Pet_Level");
		pRecvPetInfo->nPos = pRequestPetInfo->nPos;
		pRecvPetInfo->nSerial = pRequestPetInfo->nSerial;
	
		g_MuOnlineDB.Close();
		pRecvPetInfoCount->nCount++;

		lOfs1+=sizeof(Request_PetItem_Info);
		lOfs2+=sizeof(Recv_PetItem_Info);
	}

	PHeadSetW((LPBYTE)pRecvPetInfoCount, 0x56, sizeof(SDHP_RECV_PETITEM_INFO) + pRecvPetInfoCount->nCount * sizeof(Recv_PetItem_Info));
	pRecvPetInfoCount->InvenType = aRecv->nCount;
	pRecvPetInfoCount->Number = aRecv->Number;
	memcpy(pRecvPetInfoCount->AccountID, szAccountID, 10);

	DataSend(aIndex, (LPBYTE)cBUFFER, sizeof(SDHP_RECV_PETITEM_INFO) + pRecvPetInfoCount->nCount * sizeof(Recv_PetItem_Info));
}


void GDSavePetItemInfo(int aIndex, SDHP_SAVE_PETITEM_INFO * aRecv)
{
	int lOfs = sizeof(SDHP_SAVE_PETITEM_INFO);

	for(int i=0;i<aRecv->nCount;i++)
	{
		Save_PetItem_Info * pSavePetInfo = (Save_PetItem_Info *)((LPBYTE)aRecv + lOfs);

		g_MuOnlineDB.ExecQuery("UPDATE T_PetItem_Info SET Pet_Level=%d, Pet_Exp=%d WHERE ItemSerial=%d",
			pSavePetInfo->Level, pSavePetInfo->Exp, pSavePetInfo->nSerial);
		g_MuOnlineDB.Fetch();
		g_MuOnlineDB.Close();
	}
}


void DGOptionDataRecv(int aIndex, SDHP_SKILLKEYDATA * aRecv)
{
	char szName[11]={0};
	memcpy(szName, aRecv->Name, 10);

	g_MuOnlineDB.ExecQuery("SELECT Name FROM OptionData WHERE Name='%s'", szName);

	if ( g_MuOnlineDB.Fetch() == SQL_NO_DATA )
	{
		g_MuOnlineDB.Close();
		g_MuOnlineDB.ExecQuery("INSERT INTO OptionData (Name, GameOption, Qkey, Wkey, Ekey, ChatWindow) VALUES ('%s', %d, %d, %d, %d, %d)",
			szName, aRecv->GameOption, aRecv->QkeyDefine, aRecv->WkeyDefine, aRecv->EkeyDefine, aRecv->ChatWindow);
		g_MuOnlineDB.Fetch();
		g_MuOnlineDB.Close();
	}
	else
	{
		g_MuOnlineDB.Close();
		g_MuOnlineDB.ExecQuery("UPDATE OptionData SET GameOption=%d, Qkey=%d, Wkey=%d, Ekey=%d, ChatWindow=%d WHERE Name='%s'",
			aRecv->GameOption, aRecv->QkeyDefine, aRecv->WkeyDefine, aRecv->EkeyDefine, aRecv->ChatWindow, szName);
		g_MuOnlineDB.Fetch();
		g_MuOnlineDB.Close();
	}

	char szTemp[100];
	wsprintf(szTemp, "UPDATE OptionData SET SkillKey=? where Name='%s'", szName);
	g_MuOnlineDB.SetAsBinary(szTemp, aRecv->SkillKeyBuffer, sizeof(aRecv->SkillKeyBuffer));
	g_MuOnlineDB.Close();
}


void DGMoveOtherServer(int aIndex, SDHP_CHARACTER_TRANSFER * aRecv)
{
	LogAddC(2, "[MuOnlineDB] Error Character Transfer is not longer avaible");
}

//////////////////////////////////////////






/*void DataServerProtocolCore(BYTE protoNum, unsigned char* aRecv, int aLen)
{

	#ifdef TRACE_PROTOCOL
		LogAddHeadHex("DATA_SERVER", aRecv, aLen);
	#endif

	switch ( protoNum )
	{
		case 0x00:
			DataServerLoginResult((SDHP_RESULT *)aRecv);
			break;

		case 0x01:
			JGPGetCharList(aRecv);
			break;

		case 0x04:
			JGCharacterCreateRequest((SDHP_CREATECHARRESULT *)aRecv);
			break;

		case 0x05:
			JGCharDelRequest((SDHP_CHARDELETERESULT *)aRecv);
			break;

		case 0x06:
			JGGetCharacterInfo((SDHP_DBCHAR_INFORESULT *)aRecv);
			break;

		case 0x08:
			DGGetWarehouseList((SDHP_GETWAREHOUSEDB_SAVE *)aRecv);
			break;

		case 0x10:
			GDGetWarehouseNoItem((SDHP_GETWAREHOUSEDB_RESULT *)aRecv);
			break;

		case 0x52:
			ItemSerialCreateRecv((SDHP_ITEMCREATERECV *)aRecv);
			break;

		case 0x56:
			DGRecvPetItemInfo(aRecv);
			break;

		case 0x60:
			DGOptionDataRecv((SDHP_SKILLKEYDATA_SEND *)aRecv);
			break;

		case 0x80:
			{
				PMSG_DEFAULT2 * lpDef1 = (PMSG_DEFAULT2 *)aRecv;

				switch ( lpDef1->subcode )
				{
					case 0x00:
						GS_DGAnsCastleTotalInfo(aRecv);
						break;

					case 0x01:
						GS_DGAnsOwnerGuildMaster(aRecv);
						break;

					case 0x03:
						GS_DGAnsCastleNpcBuy(aRecv);
						break;

					case 0x04:
						GS_DGAnsCastleNpcRepair(aRecv);
						break;

					case 0x05:
						GS_DGAnsCastleNpcUpgrade(aRecv);
						break;

					case 0x06:
						GS_DGAnsTaxInfo(aRecv);
						break;

					case 0x07:
						GS_DGAnsTaxRateChange(aRecv);
						break;

					case 0x08:
						GS_DGAnsCastleMoneyChange(aRecv);
						break;

					case 0x09:
						GS_DGAnsSiegeDateChange(aRecv);
						break;

					case 0x0A:
						GS_DGAnsGuildMarkRegInfo(aRecv);
						break;

					case 0x0B:
						GS_DGAnsSiegeEndedChange(aRecv);
						break;

					case 0x0C:
						GS_DGAnsCastleOwnerChange(aRecv);
						break;

					case 0x0D:
						GS_DGAnsRegAttackGuild(aRecv);
						break;

					case 0x0E:
						GS_DGAnsRestartCastleState(aRecv);
						break;

					case 0x0F:
						GS_DGAnsMapSvrMsgMultiCast(aRecv);
						break;

					case 0x10:
						GS_DGAnsRegGuildMark(aRecv);
						break;

					case 0x11:
						GS_DGAnsGuildMarkReset(aRecv);
						break;

					case 0x12:
						GS_DGAnsGuildSetGiveUp(aRecv);
						break;

					case 0x16:
						GS_DGAnsNpcRemove(aRecv);
						break;

					case 0x17:
						GS_DGAnsCastleStateSync(aRecv);
						break;

					case 0x18:
						GS_DGAnsCastleTributeMoney(aRecv);
						break;

					case 0x19:
						GS_DGAnsResetCastleTaxInfo(aRecv);
						break;

					case 0x1A:
						GS_DGAnsResetSiegeGuildInfo(aRecv);
						break;

					case 0x1B:
						GS_DGAnsResetRegSiegeInfo(aRecv);
						break;
				}
			}
			break;

		case 0x81:
			GS_DGAnsCastleInitData(aRecv);
			break;

		case 0x82:
			GS_DGAnsCastleNpcInfo(aRecv);
			break;

		case 0x83:
			GS_DGAnsAllGuildMarkRegInfo(aRecv);
			break;

		case 0x84:
			GS_DGAnsFirstCreateNPC(aRecv);
			break;

		case 0x85:
			GS_DGAnsCalcREgGuildList(aRecv);
			break;

		case 0x86:
			GS_DGAnsCsGulidUnionInfo(aRecv);
			break;

		case 0x87:
			GS_DGAnsCsSaveTotalGuildInfo(aRecv);
			break;

		case 0x88:
			GS_DGAnsCsLoadTotalGuildInfo(aRecv);
			break;

		case 0x89:
			GS_DGAnsCastleNpcUpdate(aRecv);
			break;

		case 0xB0:
			DGAnsCrywolfSync(aRecv);
			break;

		case 0xB1:
			DGAnsCrywolfInfoLoad(aRecv);
			break;

		case 0xB2:
			DGAnsCrywolfInfoSave(aRecv);
			break;

		case 0xCF:
			DGMoveOtherServer((SDHP_CHARACTER_TRANSFER_RESULT *)aRecv);
			break;

		case 0xD0:
			{
				PMSG_DEFAULT2 * lpMsg = (PMSG_DEFAULT2 *)aRecv;

				switch ( lpMsg->subcode )
				{
					case 0x02:
						g_CashItemPeriodSystem.DGAnsPeriodItemInquiry((PMSG_ANS_PERIODITEM_INQUIRY *)aRecv);
						break;

					case 0x04:
						g_CashItemPeriodSystem.DGAnsPeriodItemInsert((PMSG_ANS_PERIODITEM_INSERT *)aRecv);
						break;

					case 0x06:
						g_CashItemPeriodSystem.DGAnsPeriodItemList((PMSG_ANS_PERIODITEM_LIST *)aRecv);
						break;

					case 0x08:
						g_CashItemPeriodSystem.DGAnsPeriodItemUpdate((PMSG_ANS_PERIODITEM_UPDATE *)aRecv);
						break;
				}
			}
			break;

		case 0xFF:	// Test Packet
			{
				PMSG_TEST * pMsg = (PMSG_TEST *)aRecv;

				LogAddC(2, "error-L2DataServer RECV : %d (%d)", aLen, pMsg->count);
			}
			break;
	}
}*/





