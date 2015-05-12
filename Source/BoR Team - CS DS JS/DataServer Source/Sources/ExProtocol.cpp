/*#include "GInclude.h"
#include "ExProtocol.h"
#include "Main.h"
#include "TDbManager.h"
#include "TExManager.h"
//-----------------------------------------------------------------------------------------------------------

void ExProtocolCore(TExManager * thisManager, int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize)
{
	g_Log.SetStatusText(g_Colors.White(), g_Colors.GreenYellow(), "%s ON_GO", MAIN_PROJECT_NAME);
	// ----
	switch(HeadCode)
	{
		case 0x00:
		{
			ExDataServerLogin(thisManager, aIndex, (SDHP_SERVERINFO *)aRecv);
		}
		break;

		case 0x02:
		{
			GDCharCloseRecv(thisManager, aIndex, (SDHP_USERCLOSE *)aRecv);
		}
		break;

		case 0x30:
		{
			GDGuildCreateSend(thisManager, aIndex, (SDHP_GUILDCREATE *)aRecv);
		}
		break;

		case 0x31:
		{
			GDGuildDestroyRecv(thisManager, aIndex, (SDHP_GUILDDESTROY *)aRecv);
		}
		break;

		case 0x32:
		{
			GDGuildMemberAdd(thisManager, aIndex, (SDHP_GUILDMEMBERADD *)aRecv);
		}
		break;

		case 0x33:
		{
			GDGuildMemberDel(thisManager, aIndex, (SDHP_GUILDMEMBERDEL *)aRecv);
		}
		break;

		/*case 0x34:	Packet Sended to Update Guilds - Not Called by any function of GS, so here is useless
		break;*/
		/*
		case 0x35:
		{
			DGGuildMemberInfoRequest(thisManager, aIndex, (SDHP_GUILDMEMBER_INFO_REQUEST *)aRecv);
		}
		break;

		case 0x37:
		{
			DGGuildScoreUpdate(thisManager, aIndex, (SDHP_GUILDSCOREUPDATE *)aRecv);
		}
		break;

		case 0x38:
		{
			GDGuildNoticeSave(thisManager, aIndex, (SDHP_GUILDNOTICE *)aRecv);
		}
		break;

		case 0xE1:
		{
			GDGuildReqAssignStatus(thisManager, aIndex, (EXSDHP_GUILD_ASSIGN_STATUS_REQ *)aRecv);
		}
		break;

		case 0xE2:
		{
			GDGuildReqAssignType(thisManager, aIndex, (EXSDHP_GUILD_ASSIGN_TYPE_REQ *)aRecv);
		}
		break;
	}
	// ----
	g_Log.SetStatusText(g_Colors.White(), g_Colors.Yellow() , "%s STATE_WAIT", MAIN_PROJECT_NAME);
}
//-----------------------------------------------------------------------------------------------------------

void ExDataServerLogin(TExManager * thisManager, int aIndex, SDHP_SERVERINFO * lpMsg)
{
	SDHP_RESULT pResult = {0};
	// ----
	pResult.h.set(0x00, sizeof(pResult));
	pResult.Result = 0;
	// ----
	switch(lpMsg->Type)
	{
		case 1: // GameServer
		{
			if(g_ServerManager.AddServer(aIndex, lpMsg->ServerCode, lpMsg->ServerName, lpMsg->Port) == true)
			{
				g_Log.LogAdd(g_Colors.BlueViolet(), "[ExProtocol][ExDataServerLogin] :: GS(%s) Index(%d) Port(%d) connected", lpMsg->ServerName,
				lpMsg->ServerCode, lpMsg->Port);
				// ----
				pMsg.Result = 1;
			}
		} 
		break;

		case 2: // ChatServer
		{
			g_Log.LogAdd(0x000000, "[ExDataServerLogin][ExDataServerLogin] :: Chat Server Connected");
		}
		break; 
	}
	// ----
	thisManager->DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GDCharCloseRecv(TExManager * thisManager, int aIndex, SDHP_USERCLOSE * aRecv)
{
	//SDHP_USERCLOSE pResult;
}
//-----------------------------------------------------------------------------------------------------------

void GDGuildCreateSend(TExManager * thisManager, int aIndex, SDHP_GUILDCREATE * aRecv)
{
	SDHP_GUILDCREATE_RESULT	pResult;
	char					szMaster[11]	= {0};
	char					szGuild[9]		= {0};
	// ----
	pResult.h.set(0x30, sizeof(pResult));
	pResult.Flag		= 1;
	pResult.Result		= 0;
	pResult.NumberH		= aRecv->NumberH;
	pResult.NumberL		= aRecv->NumberL;
	pResult.btGuildType	= aRecv->btGuildType;
	// ----
	memcpy(pResult.GuildName, aRecv->GuildName, 9);
	memcpy(pResult.Master, aRecv->Master, 11);
	// ----
	if(lstrlen(aRecv->GuildName) < 4)
	{
		pResult.Result	= 2;
		pResult.Flag	= 1;
		// ----
		thisManager->DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		// ----
		return;
	}
	// ----
	if(lstrlen(aRecv->Master) < 3)
	{
		pResult.Result	= 2;
		pResult.Flag	= 1;
		// ----
		thisManager->DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		// ----
		return;
	}
	// ----
	memcpy(szMaster, aRecv->Master, 10);
	memcpy(szGuild, aRecv->GuildName, 8);
	// ----
	if(SpaceSyntexCheck(szMaster) != FALSE)
	{
		if(SQLSyntexCheck(szMaster) != FALSE)
		{
			if(g_bCanCreateDeleteGuild != false)
			{
				g_DbManager.ExecFormat("SELECT Name FROM GuildMember WHERE Name='%s'", szMaster);
				// ----
				// Check if Exist
				if(g_DbManager.Fetch() == SQL_NO_DATA)
				{
					g_DbManager.Clear();
					// ----
					if(g_DbManager.ExecFormat("WZ_GuildCreate '%s', '%s'", szGuild, szMaster) == TRUE)
					{
						if(g_DbManager.Fetch() != SQL_NO_DATA)
						{
							if(g_DbManager.GetInt(0) == 0)
							{
								g_DbManager.Clear();
								char szMsg[10000];
								wsprintf(szMsg,"UPDATE Guild SET G_Mark=? WHERE G_Name='%s'", szMaster);
								g_DbManager.SetAsBinary(szMsg, aRecv->Mark, sizeof(aRecv->Mark));
								g_DbManager.Fetch();
							}
						}
					}
					// ----
					g_DbManager.Clear();
					// ----
					g_DbManager.ExecFormat("SELECT Number FROM Guild WHERE G_Name='%s'", szMaster);
					g_DbManager.Fetch();
					pResult.GuildNumber = g_DbManager.GetInt("Number");
					memcpy(pResult.Mark, aRecv->Mark, sizeof(pResult.Mark));
					// ----
					tagGUILD_INFO_STRUCT pGuildInfo;
					// ----
					pGuildInfo.m_iNumber = pResult.GuildNumber;
					pGuildInfo.m_btGuildType = aRecv->btGuildType;
					memcpy(pGuildInfo.m_szGuildName, aRecv->GuildName, 10);
					pGuildInfo.m_btConnected[0] = MuLoginFindUser(szMaster);
					pGuildInfo.m_btGuildStatus[0] = 0x80;
					// ----
					#pragma message(" [ERROR] !!! : LACKING to be Completed")
					for(int i=1;i<MAX_MEMBER_GUILD;i++)
					{
						pGuildInfo.m_btConnected[i] = -1;
						pGuildInfo.m_btGuildStatus[i] = 0x00;
					}
					// ----
					g_mapGuildManager[szGuild] = pGuildInfo;
					g_DbManager.Clear();
					// ----
					g_DbManager.ExecFormat("UPDATE Guild SET G_Union=%d WHERE Number=%d", 0, pResult.GuildNumber);
					g_DbManager.ExecFormat("UPDATE GuildMember SET G_Status	=%d WHERE Name='%s'", 0x80, szMaster);
					g_DbManager.ExecFormat("UPDATE Guild SET G_Type=%d WHERE G_Name='%s'", aRecv->btGuildType, szGuild);
					pResult.Result = 1;
					// ----
					for(int i=0;i<MAX_OBJECT;i++)
					{
						if((m_Server[i].m_Index != -1) && (m_Server[i].m_State >= 3))
						{
							pResult.Flag = (i == aIndex) ? 1 : 0;
							thisManager->DataSend(i, (LPBYTE)&pResult, pResult.h.size);
						}
					}
					// ----
					#pragma message(" [ERROR] !!! : Send Also the GuildMemberInfo")
					// ----
					return;
				}
				else
				{
					pResult.Result = 3;	// Is Member of a Guild
				}
			}
		}
		else
		{
			pResult.Result	= 5;
			pResult.Flag	= 1;
		}
	}
	else
	{
		pResult.Result		= 4;
	}
	// ----
	thisManager->DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}
//-----------------------------------------------------------------------------------------------------------

void GDGuildDestroyRecv(TExManager * thisManager, int aIndex, SDHP_GUILDDESTROY * aRecv)
{
	SDHP_GUILDDESTROY_RESULT pResult;
	
	pResult.h.set(0x31, sizeof(pResult));
	memcpy(pResult.GuildName, aRecv->GuildName, 8);
	memcpy(pResult.Master, aRecv->Master, 10);
	char szGuildMaster[11]={0};
	char szGuildName[9]={0};
	memcpy(szGuildMaster, aRecv->Master, 10);
	memcpy(szGuildName, aRecv->GuildName, 8);
	pResult.NumberH = aRecv->NumberH;
	pResult.NumberL = aRecv->NumberL;
	pResult.Flag = 0;	
	#pragma message(" [WARNING] !!! : pResult.Flag is Set 1 when is on Server")
	

	if(lstrlen(szGuildMaster) < 4)
		pResult.Result =	3;

	if(lstrlen(szGuildName) < 2)
		pResult.Result = 3;

	if(g_bCanCreateDeleteGuild == false)
		pResult.Result = 3;

	if(pResult.Result != 3)
	{
		BOOL bRet = FALSE;

		g_DbManager.ExecFormat("WZ_CS_CheckSiegeGuildList '%s'", szGuildName);
		if(g_DbManager.Fetch() != SQL_NO_DATA)
			bRet = g_DbManager.GetInt(0);
		
		g_DbManager.Clear();

		// Can't Delete if is Registered on Castle Siege
		if(bRet == TRUE)
			pResult.Result = 3;

		if(pResult.Result != 3)
		{
			g_DbManager.ExecFormat("DELETE FROM GuildMember WHERE G_Name='%s'", szGuildName);
			g_DbManager.Clear();
			g_DbManager.ExecFormat("DELETE FROM Guild WHERE G_Name='%s'", szGuildName);
			g_DbManager.Clear();
			pResult.Result = 4;
		}
	}
	#pragma message(" [ERROR] !!! : Still Lacking the Other Removal for Union and Rival")
	
	for(int i=0;i<MAX_OBJECT;i++)
	{
		if(m_Server[i].m_Index != -1 &&
			 m_Server[i].m_State >= 3)
		{
			pResult.Flag = (i == aIndex) ? 1 : 0;
			thisManager->DataSend(i, (LPBYTE)&pResult, pResult.h.size);
		}
	}	
}
//-----------------------------------------------------------------------------------------------------------

void GDGuildMemberAdd(TExManager * thisManager, int aIndex, SDHP_GUILDMEMBERADD * aRecv)
{
	SDHP_GUILDMEMBERADD_RESULT pResult;

	pResult.h.set(0x32, sizeof(pResult));
	memcpy(pResult.GuildName, aRecv->GuildName, 9);
	pResult.NumberH = aRecv->NumberH;
	pResult.NumberL = aRecv->NumberL;
	memcpy(pResult.MemberID, aRecv->MemberID, 11);
	char szGuildMember[11]={0};
	char szGuildName[9]={0};
	memcpy(szGuildMember, aRecv->MemberID, 10);
	memcpy(szGuildName, aRecv->GuildName, 8);
	pResult.pServer = m_Server[aIndex].m_ServerCode;
	pResult.Result = 0;

	if(lstrlen(szGuildName) < 3)
	{
		pResult.Result = 0;
		thisManager->DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if(lstrlen(szGuildMember) < 4)
	{
		pResult.Result = 0;
		thisManager->DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	// Check if Member Exist
	if(g_DbManager.ExecFormat("SELECT Name FROM GuildMember WHERE Name='%s'", szGuildMember) == TRUE)
	{
		if(g_DbManager.Fetch() != SQL_NO_DATA)
		{
			g_DbManager.Clear();
			pResult.Result = 4;
			thisManager->DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}
	g_DbManager.Clear();
	
	// Add Member
	if(g_DbManager.ExecFormat("INSERT INTO GuildMember (Name, G_Name) VALUES ('%s', '%s')", szGuildMember, szGuildName) == FALSE)
	{
		g_DbManager.Clear();
		pResult.Result = 4;
		thisManager->DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	g_DbManager.Clear();

	// Add Guild Member into Memory
	pResult.Result = 1;
	#pragma message(" [ERROR] !!! : Lacking this Procedure")

	// Send Data to All Servers
	for(int i=0;i<MAX_OBJECT;i++)
	{
		if(m_Server[i].m_Index != -1 &&
			 m_Server[i].m_State >= 3)
		{
			pResult.Flag = (i == aIndex) ? 1 : 0;
			thisManager->DataSend(i, (LPBYTE)&pResult, pResult.h.size);
		}
	}
}
//-----------------------------------------------------------------------------------------------------------

void GDGuildMemberDel(TExManager * thisManager, int aIndex, SDHP_GUILDMEMBERDEL * aRecv)
{
	SDHP_GUILDMEMBERDEL_RESULT pResult;

	pResult.h.set(0x33, sizeof(pResult));
	memcpy(pResult.GuildName, aRecv->GuildName, 9);
	pResult.NumberH = aRecv->NumberH;
	pResult.NumberL = aRecv->NumberL;
	memcpy(pResult.MemberID, aRecv->MemberID, 11);
	pResult.Result = 0;
	char szGuildMember[11]={0};
	char szGuildName[9]={0};
	memcpy(szGuildMember, aRecv->MemberID, 10);
	memcpy(szGuildName, aRecv->GuildName, 8);
	
	if(lstrlen(szGuildName) < 3)
	{
		pResult.Result = 3;
		thisManager->DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if(lstrlen(szGuildMember) < 4)
	{
		pResult.Result = 3;
		thisManager->DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	// Delete FROM Guild
	if(g_DbManager.ExecFormat("DELETE FROM GuildMember WHERE Name='%s'", szGuildMember) == FALSE)
	{
		g_DbManager.Clear();
		pResult.Result = 3;
		thisManager->DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	g_DbManager.Clear();

	// Send Data to All Servers
	pResult.Result = 1;
	for(int i=0;i<MAX_OBJECT;i++)
	{
		if(m_Server[i].m_Index != -1 &&
			 m_Server[i].m_State >= 3)
		{
			pResult.Flag = (i == aIndex) ? 1 : 0;
			thisManager->DataSend(i, (LPBYTE)&pResult, pResult.h.size);
		}
	}
}
//-----------------------------------------------------------------------------------------------------------

void DGGuildMemberInfoRequest(TExManager * thisManager, int aIndex, SDHP_GUILDMEMBER_INFO_REQUEST * aRecv)
{
	char cBUFFER[2048]={0};
	int iLoginUserIndex = -1;
	SDHP_GUILDALL_COUNT * pListCount = (SDHP_GUILDALL_COUNT *)cBUFFER;
	int lOfs = sizeof(SDHP_GUILDALL_COUNT);
	SDHP_GUILDALL * pList;
	SDHP_GUILDMEMBER_INFO pResult={0};

	pResult.h.set(0x35, sizeof(pResult));
	memcpy(pResult.MemberID, aRecv->MemberID, 10);
	char szGuildMember[11]={0};
	memcpy(szGuildMember, aRecv->MemberID, 10);
	int iIndex = MuLoginFindUser(szGuildMember); 
	if (iIndex == -1)
		pResult.pServer = -1;
	else
		pResult.pServer = g_MuLoginUserData[iIndex].m_ServerCode;



	if(lstrlen(szGuildMember) < 4)
		return;

	// Get Guild Member Info
	if(g_DbManager.ExecFormat("SELECT G_Name, G_Level, G_Status FROM GuildMember WHERE Name='%s'", szGuildMember) != TRUE)
	{
		g_DbManager.Clear();
		return;
	}

	if(g_DbManager.Fetch() == SQL_NO_DATA)
	{
		g_DbManager.Clear();
		return;
	}
	
	g_DbManager.GetStr("G_Name", pResult.GuildName);
	pListCount->btGuildType = g_DbManager.GetInt("G_Type");
	pResult.btGuildType = pListCount->btGuildType;
	pResult.btGuildStatus = g_DbManager.GetInt("G_Status");
	g_DbManager.Clear();

	//std::map<std::string, tagGUILD_INFO_STRUCT, strCmp>::iterator _Iter = g_mapGuildManager.find(pResult.GuildName);

	//if(_Iter == g_mapGuildManager.end())
	//	return;

	tagGUILD_INFO_STRUCT pGuildInfo;// = _Iter->second;

	// Send GuildMasterList
	if(true)
	{
		pListCount->Count = 0;
		memcpy(pListCount->GuildName, pResult.GuildName, 9);
		pListCount->iGuildRival = pGuildInfo.m_iRivalGuild;
		pListCount->iGuildUnion = pGuildInfo.m_iUnionGuild;
		memcpy(pListCount->Mark, pGuildInfo.m_Mark, sizeof(pGuildInfo.m_Mark));
		memcpy(pListCount->Master, pGuildInfo.m_szGuildMaster, 8);
		pListCount->Number = pGuildInfo.m_iNumber;
		pListCount->score = pGuildInfo.m_Score;

		if(pGuildInfo.m_iRivalGuild != 0)
		{
			g_DbManager.ExecFormat("SELECT G_Name FROM Guild WHERE G_Rival=%d", pGuildInfo.m_iRivalGuild);
			
			if(g_DbManager.Fetch() != SQL_NO_DATA)
			{
				g_DbManager.GetStr("G_Name", pListCount->szGuildRivalName);
			}

			g_DbManager.Clear();
		}

		for(int i=0;i<MAX_MEMBER_GUILD;i++)
		{
			if(pGuildInfo.m_szGuildMember[i][0] == '\0')
				continue;

			pList = (SDHP_GUILDALL *)((LPBYTE)cBUFFER + lOfs);
			memcpy(pList->MemberID, pGuildInfo.m_szGuildMember[i], 8);
			pList->btGuildStatus = pGuildInfo.m_btGuildStatus[i];
			iLoginUserIndex = MuLoginFindUser(pList->MemberID);

			if(iLoginUserIndex != -1)
				pList->pServer = g_MuLoginUserData[iLoginUserIndex].m_ServerCode;
			else
				pList->pServer = -1;
		
			lOfs+=sizeof(SDHP_GUILDALL);
			pListCount->Count++;
		}

		for(int i=0;i<MAX_OBJECT;i++)
		{
			if(m_Server[i].m_Index != -1 &&
				 m_Server[i].m_State >= 3)
			{
				thisManager->DataSend(i, (LPBYTE)cBUFFER, sizeof(SDHP_GUILDALL_COUNT) + pListCount->Count * sizeof(SDHP_GUILDALL));
			}
		}
	}

	// Send RelationShipList
	if(true)
	{
		EXSDHP_UNION_RELATIONSHIP_LIST pRelationShipList={0};

		PHeadSetW((LPBYTE)&pRelationShipList, 0xE7, sizeof(pRelationShipList));
		pRelationShipList.btRelationShipType = 0;
		pRelationShipList.btRelationShipMemberCount = 0;

		// For Union RelationShip

		// For Rival RelationShip
	}

	for(int i=0;i<MAX_OBJECT;i++)
	{
		if(m_Server[i].m_Index != -1 &&
			 m_Server[i].m_State >= 3)
		{
			thisManager->DataSend(i, (LPBYTE)&pResult, pResult.h.size);
		}
	}
}
//-----------------------------------------------------------------------------------------------------------

void DGGuildScoreUpdate(TExManager * thisManager, int aIndex, SDHP_GUILDSCOREUPDATE * aRecv)
{
	SDHP_GUILDSCOREUPDATE pResult;

	pResult.h.set(0x37, sizeof(pResult));
	memcpy(pResult.GuildName, aRecv->GuildName, 9);
	char szGuildName[11]={0};
	memcpy(szGuildName, aRecv->GuildName, 10);

	// Update Score	and send Packet
	if(g_DbManager.ExecFormat("UPDATE Guild SET G_Score=%d WHERE G_Name='%s'", aRecv->Score, szGuildName) == TRUE)
	{
		pResult.Score = aRecv->Score;		
		for(int i=0;i<MAX_OBJECT;i++)
		{
			if(m_Server[i].m_Index != -1 &&
				 m_Server[i].m_State >= 3)
			{
				thisManager->DataSend(i, (LPBYTE)&pResult, pResult.h.size);
			}
		}
	}
}
//-----------------------------------------------------------------------------------------------------------

void GDGuildNoticeSave(TExManager * thisManager, int aIndex, SDHP_GUILDNOTICE * aRecv)
{
	SDHP_GUILDNOTICE pResult;

	pResult.h.set(0x38, sizeof(pResult));
	memcpy(pResult.GuildName, aRecv->GuildName, 9);
	memcpy(pResult.szGuildNotice, aRecv->szGuildNotice, sizeof(pResult.szGuildNotice));

	char szMsg[256];
	wsprintf("UPDATE Guild SET G_Notice=? WHERE G_Name='%s'", pResult.GuildName);
	g_DbManager.SetAsBinary(szMsg, (LPBYTE)pResult.szGuildNotice, sizeof(pResult.szGuildNotice));
	g_DbManager.Fetch();
	g_DbManager.Clear();

	for(int i=0;i<MAX_OBJECT;i++)
	{
		if(m_Server[i].m_Index != -1 &&
			 m_Server[i].m_State >= 3)
		{
			thisManager->DataSend(i, (LPBYTE)&pResult, pResult.h.size);
		}
	}
}
//-----------------------------------------------------------------------------------------------------------

void GDGuildReqAssignStatus(TExManager * thisManager, int aIndex, EXSDHP_GUILD_ASSIGN_STATUS_REQ * aRecv)
{
	EXSDHP_GUILD_ASSIGN_STATUS_RESULT pResult;

	pResult.h.set(0xE1, sizeof(pResult));
	pResult.btGuildStatus = aRecv->btGuildStatus;
	pResult.btType = aRecv->btType;
	memcpy(pResult.szGuildName, aRecv->szGuildName, 9);
	memcpy(pResult.szTargetName, aRecv->szTargetName, 11);
	pResult.wUserIndex = aRecv->wUserIndex;
	pResult.btResult = 0;
	
	if(g_DbManager.ExecFormat("UPDATE GuildMember SET G_Status=%d WHERE Name='%s'",
		pResult.btGuildStatus, pResult.szTargetName) == TRUE)
		pResult.btResult = 1;

	// Send Packet
	for(int i=0;i<MAX_OBJECT;i++)
	{
		if(m_Server[i].m_Index != -1 &&
			 m_Server[i].m_State >= 3)
		{
			pResult.btFlag = (i == aIndex) ? 1 : 0;
			thisManager->DataSend(i, (LPBYTE)&pResult, pResult.h.size);
		}
	}
}
//-----------------------------------------------------------------------------------------------------------

void GDGuildReqAssignType(TExManager * thisManager, int aIndex, EXSDHP_GUILD_ASSIGN_TYPE_REQ * aRecv)
{
	EXSDHP_GUILD_ASSIGN_TYPE_RESULT pResult;

	pResult.h.set(0xE2, sizeof(pResult));
	pResult.btGuildType = aRecv->btGuildType;
	memcpy(pResult.szGuildName, aRecv->szGuildName, 9);
	pResult.wUserIndex = aRecv->wUserIndex;
	pResult.btResult = 0;
	
	if(g_DbManager.ExecFormat("UPDATE GuildMember SET G_Type=%d WHERE G_Name='%s'",
		pResult.btGuildType, pResult.szGuildName) == TRUE)
		pResult.btResult = 1;

	// Send Packet
	for(int i=0;i<MAX_OBJECT;i++)
	{
		if(m_Server[i].m_Index != -1 &&
			 m_Server[i].m_State >= 3)
		{
			pResult.btFlag = (i == aIndex) ? 1 : 0;
			thisManager->DataSend(i, (LPBYTE)&pResult, pResult.h.size);
		}
	}
}
//-----------------------------------------------------------------------------------------------------------*/