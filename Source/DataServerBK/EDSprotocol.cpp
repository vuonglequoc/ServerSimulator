// EDSprotocol.cpp: implementation of the EDSprotocol class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EDSprotocol.h"

#include "giocp.h"
#include "LogProc.h"
#include "winutil.h"
#include "Sprotocol.h"
#include "Query.h"

CQuery g_MuOnlineExDB;


#pragma pack(1)

struct tagGUILD_INFO_STRUCT
{
	tagGUILD_INFO_STRUCT()
	{
		m_iNumber = -1;
		memset(m_szGuildName, 0, sizeof(m_szGuildName));
		memset(m_szGuildMaster, 0, sizeof(m_szGuildMaster));
		m_btGuildType = -1;
		memset(m_szGuildMember, 0, sizeof(m_szGuildMember));
		memset(m_btGuildStatus, 0, sizeof(m_btGuildStatus));
		memset(m_btConnected, 0, sizeof(m_btConnected));
		memset(m_Mark, 0, sizeof(m_Mark));
		m_iRivalGuild = 0;
		m_iUnionGuild = 0;
		m_Score = 0;
	}

	int m_iNumber;
	char m_szGuildName[8];
	char m_szGuildMaster[10];
	BYTE m_btGuildType;
	char m_szGuildMember[MAX_MEMBER_GUILD][10];
	BYTE m_btGuildStatus[MAX_MEMBER_GUILD];
	BYTE m_btConnected[MAX_MEMBER_GUILD];
	int m_iRivalGuild;
	int m_iUnionGuild;
	BYTE m_Mark[32];
	int m_Score;
};


struct strCmp
{
	bool operator()( const std::string s1, const std::string s2 ) const 
	{
		return strcmp( s1.data(), s2.data() ) < 0;
	}
};

#pragma pack()


struct tagUNION_DATA
{
	std::vector<int>	m_vecUnionMember;
	std::vector<int>	m_vecRivalMember;
};


BOOL g_bCanCreateDeleteGuild = (BOOL)GetPrivateProfileInt("CreateDeleteGuild", "CONFIG", 0, ".\\eDataServer.ini");
std::map<std::string, tagGUILD_INFO_STRUCT, strCmp>	g_mapGuildManager;
std::map<int, tagUNION_DATA>	g_mapUnionManager;


BOOL InitExDataServer()
{
	DWORD dwStartTickCount = GetTickCount();

	g_mapGuildManager.clear();
	
	CQuery TempQueryGuild;
	int iMemberIndex = 0;
	TempQueryGuild.Connect(g_MuOnlineDNS, g_UserID, g_Password);
	CQuery TempQueryGuildMember;
	TempQueryGuildMember.Connect(g_MuOnlineDNS, g_UserID, g_Password);
	
	// Check if Guild Data Exists
	if ( TempQueryGuild.ExecQuery("SELECT * FROM Guild") == FALSE )
	{
		LogAddC(2, "[MuOnlineDB] Guild Info is missing");
		g_MuOnlineExDB.Close();
		return FALSE;
	}

	// Get Guild Data and put it in Memory
	while ( TempQueryGuild.Fetch() != SQL_NO_DATA )
	{
		tagGUILD_INFO_STRUCT pGuildInfo;
		char szGuildName[9]={0};
		char szGuildMaster[11]={0};

		char szMsg[128];
		pGuildInfo .m_iNumber = TempQueryGuild.GetAsInteger("Number");
		TempQueryGuild.GetAsString("G_Name", szGuildName);
		memcpy(pGuildInfo.m_szGuildName, szGuildName, 8);
		TempQueryGuild.GetAsString("G_Master", szGuildMaster);
		memcpy(pGuildInfo.m_szGuildMaster, szGuildMaster, 10);
		TempQueryGuild.GetAsString("G_Notice", NULL);
		TempQueryGuild.GetAsInteger("G_Score");
		wsprintf(szMsg, "SELECT G_Mark FROM Guild WHERE G_Name'%s'", szGuildName);
//		TempQueryGuild.GetAsBinary(szMsg);
		TempQueryGuild.GetAsInteger("G_Rival");
		TempQueryGuild.GetAsInteger("G_Union");
		TempQueryGuild.GetAsInteger("G_Type");
		// #error Add Data to Mem

		TempQueryGuildMember.ExecQuery("SELECT * FROM GuildMember WHERE G_Name = '%s'", szGuildName);

		// Get Guild Members Data
		iMemberIndex = 0;
		while ( TempQueryGuildMember.Fetch() != SQL_NO_DATA )
		{
			char szGuildMember[11]={0};

			TempQueryGuildMember.GetAsInteger("G_Status");
//			TempQueryGuildMember.GetAsString("G_Name");
//			TempQueryGuildMember.GetAsString("Name");
			iMemberIndex++;
		}
		TempQueryGuildMember.Close();
	}

	TempQueryGuild.Close();
	TempQueryGuildMember.Close();
	TempQueryGuild.Disconnect();
	TempQueryGuildMember.Disconnect();

	LogAddC(4, "[MuOnlineDB] GuildData Loaded in %d msec", GetTickCount() - dwStartTickCount);

	return TRUE;
}

void InitMuOnlineExDB()
{
	if ( g_MuOnlineExDB.Connect(g_MuOnlineDNS, g_UserID, g_Password) == TRUE )
	{
		LogAddC(4, "[MuOnlineExDB] Connection Successfull!");
	}
	else
	{
		LogAddC(2, "[MuOnlineExDB] Error On Connection!");
	}
}

void EDSProtocolCore(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize)
{
	if(g_UseExDataServer == TRUE)
	{
#if (TRACE_PACKET == 1 )
	LogAddHeadHex("EX_DATA_SERVER", aRecv, iSize);
#endif
	g_ServerInfoDisplayer.CheckEXDSConnection(TRUE);
	switch ( HeadCode )
	{
		case 0x00:
			ExDataServerLogin(aIndex, (SDHP_SERVERINFO *)aRecv);
			break;
		case 0x02:
			GDCharCloseRecv(aIndex, (SDHP_USERCLOSE *) aRecv);
			break;
		case 0x30:
			GDGuildCreateSend(aIndex, (SDHP_GUILDCREATE *)aRecv);
			break;
		case 0x31:
			GDGuildDestroyRecv(aIndex, (SDHP_GUILDDESTROY *) aRecv);
			break;
		case 0x32:
			GDGuildMemberAdd(aIndex, (SDHP_GUILDMEMBERADD *) aRecv);
			break;
		case 0x33:
			GDGuildMemberDel(aIndex, (SDHP_GUILDMEMBERDEL *) aRecv);
			break;
		/*case 0x34:	Packet Sended to Update Guilds - Not Called by any function of GS, so here is useless
			break;*/
		case 0x35:
			DGGuildMemberInfoRequest(aIndex, (SDHP_GUILDMEMBER_INFO_REQUEST *) aRecv);
			break;
		case 0x37:
			DGGuildScoreUpdate(aIndex, (SDHP_GUILDSCOREUPDATE *) aRecv);
			break;
		case 0x38:
			GDGuildNoticeSave(aIndex, (SDHP_GUILDNOTICE *) aRecv);
			break;
		case 0xE1:
			GDGuildReqAssignStatus(aIndex, (EXSDHP_GUILD_ASSIGN_STATUS_REQ *) aRecv);
			break;
		case 0xE2:
			GDGuildReqAssignType(aIndex, (EXSDHP_GUILD_ASSIGN_TYPE_REQ *) aRecv);
			break;
	}
	g_ServerInfoDisplayer.CheckEXDSConnection(FALSE);
	}
}



void ExDataServerLogin(int aIndex, SDHP_SERVERINFO * lpMsg)
{
	SDHP_RESULT pResult = {0};

	PHeadSetB((LPBYTE)&pResult, 0x00, sizeof(pResult));
	pResult.Result = 1;

	// Case GameServer
	if ( lpMsg->Type == 1 )
	{
		//if ( MuLoginFindServer(lpMsg->ServerCode) != -1 )
		if ( MuLoginFindServer(lpMsg->ServerCode) != -1 )
		{
			g_Server[aIndex].m_State = SS_GUILD;
		}
		else
		{
			LogAddC(2, "[MuOnlineDB] GameServer Not Connected");
			pResult.Result = 0;
		}
	}
	// Case ChatServer
	else if ( lpMsg->Type == 2 )
	{
		LogAddC(2, "[ChatServer] Chat Server Connected");
	}

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}




void GDCharCloseRecv(int aIndex, SDHP_USERCLOSE * aRecv)
{
	//SDHP_USERCLOSE pResult;
}






void GDGuildCreateSend(int aIndex, SDHP_GUILDCREATE * aRecv)
{
	SDHP_GUILDCREATE_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x30, sizeof(pResult));
	pResult.Flag = 1;
	pResult.Result = 0;
	memcpy(pResult.GuildName, aRecv->GuildName, 9);
	memcpy(pResult.Master, aRecv->Master, 11);
	pResult.NumberH = aRecv->NumberH;
	pResult.NumberL = aRecv->NumberL;
	pResult.btGuildType = aRecv->btGuildType;
	
	if ( lstrlen(aRecv->GuildName) < 4 )
	{
		pResult.Result = 2;
		pResult.Flag = 1;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lstrlen(aRecv->Master) < 3 )
	{
		pResult.Result = 2;
		pResult.Flag = 1;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	char szMaster[11]={0};
	char szGuild[9]={0};
	memcpy(szMaster, aRecv->Master, 10);
	memcpy(szGuild, aRecv->GuildName, 8);

	if ( SpaceSyntexCheck(szMaster) != FALSE )
	{
		if ( SQLSyntexCheck(szMaster) != FALSE )
		{
			if ( g_bCanCreateDeleteGuild != false )
			{
				g_MuOnlineExDB.ExecQuery("SELECT Name FROM GuildMember WHERE Name='%s'",
					szMaster);
		
				// Check if Exist
				if ( g_MuOnlineExDB.Fetch() == SQL_NO_DATA )
				{
					g_MuOnlineExDB.Close();

					if ( g_MuOnlineExDB.ExecQuery("WZ_GuildCreate '%s', '%s'", szGuild, szMaster) == TRUE )
					{
						if ( g_MuOnlineExDB.Fetch() != SQL_NO_DATA )
						{
							if ( g_MuOnlineExDB.GetResult(0) == 0 )
							{
								g_MuOnlineExDB.Close();
								char szMsg[128];
								wsprintf("UPDATE Guild SET G_Mark=? WHERE G_Name='%s'", szMaster);
								g_MuOnlineExDB.SetAsBinary(szMsg, aRecv->Mark, sizeof(aRecv->Mark));
							}
						}
					}

					g_MuOnlineExDB.Close();

					g_MuOnlineExDB.ExecQuery("SELECT Number FROM Guild WHERE G_Name='%s'", szMaster);
					g_MuOnlineExDB.Fetch();
					pResult.GuildNumber = g_MuOnlineExDB.GetAsInteger("Number");
					memcpy(pResult.Mark, aRecv->Mark, sizeof(pResult.Mark));

					tagGUILD_INFO_STRUCT pGuildInfo;

					pGuildInfo.m_iNumber = pResult.GuildNumber;
					pGuildInfo.m_btGuildType = aRecv->btGuildType;
					memcpy(pGuildInfo.m_szGuildName, aRecv->GuildName, 10);
//					pGuildInfo.m_iLoginIndex[0] = MuLoginFindUser(szMaster);
//					pGuildInfo.m_GuildStatus[0] = 0x80;

// #error LACKING to be Completed
					for(int i=1;i<MAX_MEMBER_GUILD;i++)
					{
//						pGuildInfo.m_iLoginIndex[i] = -1;
//						pGuildInfo.m_GuildStatus[i] = 0x00;
					}
			
					g_mapGuildManager[szGuild] = pGuildInfo;
					g_MuOnlineExDB.Close();

					g_MuOnlineExDB.ExecQuery("UPDATE Guild SET G_Union=%d WHERE Number=%d", 0, pResult.GuildNumber);
					g_MuOnlineExDB.ExecQuery("UPDATE GuildMember SET G_Status	=%d WHERE Name='%s'", 0x80, szMaster);
					g_MuOnlineExDB.ExecQuery("UPDATE Guild SET G_Type=%d WHERE G_Name='%s'", aRecv->btGuildType, szGuild);
					pResult.Result = 1;

					for(int i=0;i<g_dwMaxServerGroups;i++)
					{
						if ( g_Server[i].m_Index != -1 &&
							 g_Server[i].m_State >= SS_GUILD )
						{
							pResult.Flag = (i == aIndex) ? 1 : 0;
							DataSend(i, (LPBYTE)&pResult, pResult.h.size);
						}
					}

//#error Send Also the GuildMemberInfo
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
			pResult.Result = 5;
			pResult.Flag = 1;
		}
	}
	else
	{
		pResult.Result = 4;
	}

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}



void GDGuildDestroyRecv(int aIndex, SDHP_GUILDDESTROY * aRecv)
{
	SDHP_GUILDDESTROY_RESULT pResult;
	
	PHeadSetB((LPBYTE)&pResult, 0x31, sizeof(pResult));
	memcpy(pResult.GuildName, aRecv->GuildName, 8);
	memcpy(pResult.Master, aRecv->Master, 10);
	char szGuildMaster[11]={0};
	char szGuildName[9]={0};
	memcpy(szGuildMaster, aRecv->Master, 10);
	memcpy(szGuildName, aRecv->GuildName, 8);
	pResult.NumberH = aRecv->NumberH;
	pResult.NumberL = aRecv->NumberL;
	pResult.Flag = 0;	// #warning is Set 1 when is on Server

	if ( lstrlen(szGuildMaster) < 4 )
		pResult.Result =	3;

	if ( lstrlen(szGuildName) < 2 )
		pResult.Result = 3;

	if ( g_bCanCreateDeleteGuild == false )
		pResult.Result = 3;

	if ( pResult.Result != 3 )
	{
		BOOL bRet = FALSE;

		g_MuOnlineExDB.ExecQuery("WZ_CS_CheckSiegeGuildList '%s'", szGuildName);
		if ( g_MuOnlineExDB.Fetch() != SQL_NO_DATA )
			bRet = g_MuOnlineExDB.GetResult(0);
		
		g_MuOnlineExDB.Close();

		// Can't Delete if is Registered on Castle Siege
		if ( bRet == TRUE )
			pResult.Result = 3;

		if ( pResult.Result != 3 )
		{
			g_MuOnlineExDB.ExecQuery("DELETE FROM GuildMember WHERE G_Name='%s'", szGuildName);
			g_MuOnlineExDB.Close();
			g_MuOnlineExDB.ExecQuery("DELETE FROM Guild WHERE G_Name='%s'", szGuildName);
			g_MuOnlineExDB.Close();
			pResult.Result = 4;
		}
	}
// #error Still Lacking the Other Removal for Union and Rival
	
	for(int i=0;i<g_dwMaxServerGroups;i++)
	{
		if ( g_Server[i].m_Index != -1 &&
			 g_Server[i].m_State >= SS_GUILD )
		{
			pResult.Flag = (i == aIndex) ? 1 : 0;
			DataSend(i, (LPBYTE)&pResult, pResult.h.size);
		}
	}	
}




void GDGuildMemberAdd(int aIndex, SDHP_GUILDMEMBERADD * aRecv)
{
	SDHP_GUILDMEMBERADD_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x32, sizeof(pResult));
	memcpy(pResult.GuildName, aRecv->GuildName, 9);
	pResult.NumberH = aRecv->NumberH;
	pResult.NumberL = aRecv->NumberL;
	memcpy(pResult.MemberID, aRecv->MemberID, 11);
	char szGuildMember[11]={0};
	char szGuildName[9]={0};
	memcpy(szGuildMember, aRecv->MemberID, 10);
	memcpy(szGuildName, aRecv->GuildName, 8);
	pResult.pServer = g_Server[aIndex].m_ServerCode;
	pResult.Result = 0;

	if ( lstrlen(szGuildName) < 3 )
	{
		pResult.Result = 0;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lstrlen(szGuildMember) < 4 )
	{
		pResult.Result = 0;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	// Check if Member Exist
	if ( g_MuOnlineExDB.ExecQuery("SELECT Name FROM GuildMember WHERE Name='%s'", szGuildMember) == TRUE )
	{
		if ( g_MuOnlineExDB.Fetch() != SQL_NO_DATA )
		{
			g_MuOnlineExDB.Close();
			pResult.Result = 4;
			DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}
	g_MuOnlineExDB.Close();
	
	// Add Member
	if ( g_MuOnlineExDB.ExecQuery("INSERT INTO GuildMember (Name, G_Name) VALUES ('%s', '%s')", szGuildMember, szGuildName) == FALSE )
	{
		g_MuOnlineExDB.Close();
		pResult.Result = 4;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	g_MuOnlineExDB.Close();

	// Add Guild Member into Memory
	pResult.Result = 1;
	// #error LAcking this Procedure

	// Send Data to All Servers
	for(int i=0;i<g_dwMaxServerGroups;i++)
	{
		if ( g_Server[i].m_Index != -1 &&
			 g_Server[i].m_State >= SS_GUILD )
		{
			pResult.Flag = (i == aIndex) ? 1 : 0;
			DataSend(i, (LPBYTE)&pResult, pResult.h.size);
		}
	}
}




void GDGuildMemberDel(int aIndex, SDHP_GUILDMEMBERDEL * aRecv)
{
	SDHP_GUILDMEMBERDEL_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x33, sizeof(pResult));
	memcpy(pResult.GuildName, aRecv->GuildName, 9);
	pResult.NumberH = aRecv->NumberH;
	pResult.NumberL = aRecv->NumberL;
	memcpy(pResult.MemberID, aRecv->MemberID, 11);
	pResult.Result = 0;
	char szGuildMember[11]={0};
	char szGuildName[9]={0};
	memcpy(szGuildMember, aRecv->MemberID, 10);
	memcpy(szGuildName, aRecv->GuildName, 8);
	
	if ( lstrlen(szGuildName) < 3 )
	{
		pResult.Result = 3;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lstrlen(szGuildMember) < 4 )
	{
		pResult.Result = 3;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	// Delete FROM Guild
	if ( g_MuOnlineExDB.ExecQuery("DELETE FROM GuildMember WHERE Name='%s'", szGuildMember) == FALSE)
	{
		g_MuOnlineExDB.Close();
		pResult.Result = 3;
		DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	g_MuOnlineExDB.Close();

	// Send Data to All Servers
	pResult.Result = 1;
	for(int i=0;i<g_dwMaxServerGroups;i++)
	{
		if ( g_Server[i].m_Index != -1 &&
			 g_Server[i].m_State >= SS_GUILD )
		{
			pResult.Flag = (i == aIndex) ? 1 : 0;
			DataSend(i, (LPBYTE)&pResult, pResult.h.size);
		}
	}
}




void DGGuildMemberInfoRequest(int aIndex, SDHP_GUILDMEMBER_INFO_REQUEST * aRecv)
{
	char cBUFFER[2048]={0};
	int iLoginUserIndex = -1;
	SDHP_GUILDALL_COUNT * pListCount = (SDHP_GUILDALL_COUNT *)cBUFFER;
	int lOfs = sizeof(SDHP_GUILDALL_COUNT);
	SDHP_GUILDALL * pList;
	SDHP_GUILDMEMBER_INFO pResult={0};

	PHeadSetB((LPBYTE)&pResult, 0x35, sizeof(pResult));
	memcpy(pResult.MemberID, aRecv->MemberID, 10);
	char szGuildMember[11]={0};
	memcpy(szGuildMember, aRecv->MemberID, 10);
	int iIndex = MuLoginFindUser(szGuildMember);
	if (iIndex == -1 )
		pResult.pServer = -1;
	else
		pResult.pServer = g_MuLoginUserData[iIndex].m_ServerCode;



	if ( lstrlen(szGuildMember) < 4 )
		return;

	// Get Guild Member Info
	if ( g_MuOnlineExDB.ExecQuery("SELECT G_Name, G_Level, G_Status FROM GuildMember WHERE Name='%s'", szGuildMember) != TRUE )
	{
		g_MuOnlineExDB.Close();
		return;
	}

	if ( g_MuOnlineExDB.Fetch() == SQL_NO_DATA )
	{
		g_MuOnlineExDB.Close();
		return;
	}
	
	g_MuOnlineExDB.GetAsString("G_Name", pResult.GuildName);
	pListCount->btGuildType = g_MuOnlineExDB.GetAsInteger("G_Type");
	pResult.btGuildType = pListCount->btGuildType;
	pResult.btGuildStatus = g_MuOnlineExDB.GetAsInteger("G_Status");
	g_MuOnlineExDB.Close();

	std::map<std::string, tagGUILD_INFO_STRUCT, strCmp>::iterator _Iter = g_mapGuildManager.find(pResult.GuildName);

	if ( _Iter == g_mapGuildManager.end() )
		return;

	tagGUILD_INFO_STRUCT pGuildInfo = _Iter->second;

	// Send GuildMasterList
	if ( true )
	{
		pListCount->Count = 0;
		memcpy(pListCount->GuildName, pResult.GuildName, 9);
		pListCount->iGuildRival = pGuildInfo.m_iRivalGuild;
		pListCount->iGuildUnion = pGuildInfo.m_iUnionGuild;
		memcpy(pListCount->Mark, pGuildInfo.m_Mark, sizeof(pGuildInfo.m_Mark));
		memcpy(pListCount->Master, pGuildInfo.m_szGuildMaster, 8);
		pListCount->Number = pGuildInfo.m_iNumber;
		pListCount->score = pGuildInfo.m_Score;

		if ( pGuildInfo.m_iRivalGuild != 0 )
		{
			g_MuOnlineExDB.ExecQuery("SELECT G_Name FROM Guild WHERE G_Rival=%d", pGuildInfo.m_iRivalGuild);
			
			if ( g_MuOnlineExDB.Fetch() != SQL_NO_DATA )
			{
				g_MuOnlineExDB.GetAsString("G_Name", pListCount->szGuildRivalName);
			}

			g_MuOnlineExDB.Close();
		}

		for(int i=0;i<MAX_MEMBER_GUILD;i++)
		{
			if ( pGuildInfo.m_szGuildMember[i][0] == '\0' )
				continue;

			pList = (SDHP_GUILDALL *)((LPBYTE)cBUFFER + lOfs);
			memcpy(pList->MemberID, pGuildInfo.m_szGuildMember[i], 8);
			pList->btGuildStatus = pGuildInfo.m_btGuildStatus[i];
			iLoginUserIndex = MuLoginFindUser(pList->MemberID);

			if ( iLoginUserIndex != -1 )
				pList->pServer = g_MuLoginUserData[iLoginUserIndex].m_ServerCode;
			else
				pList->pServer = -1;
		
			lOfs+=sizeof(SDHP_GUILDALL);
			pListCount->Count++;
		}

		for(int i=0;i<g_dwMaxServerGroups;i++)
		{
			if ( g_Server[i].m_Index != -1 &&
				 g_Server[i].m_State >= SS_GUILD )
			{
				DataSend(i, (LPBYTE)cBUFFER, sizeof(SDHP_GUILDALL_COUNT) + pListCount->Count * sizeof(SDHP_GUILDALL));
			}
		}
	}

	// Send RelationShipList
	if ( true )
	{
		EXSDHP_UNION_RELATIONSHIP_LIST pRelationShipList={0};

		PHeadSetW((LPBYTE)&pRelationShipList, 0xE7, sizeof(pRelationShipList));
		pRelationShipList.btRelationShipType = 0;
		pRelationShipList.btRelationShipMemberCount = 0;

		// For Union RelationShip

		// For Rival RelationShip
	}

	for(int i=0;i<g_dwMaxServerGroups;i++)
	{
		if ( g_Server[i].m_Index != -1 &&
			 g_Server[i].m_State >= SS_GUILD )
		{
			DataSend(i, (LPBYTE)&pResult, pResult.h.size);
		}
	}
}
	
	


void DGGuildScoreUpdate(int aIndex, SDHP_GUILDSCOREUPDATE * aRecv)
{
	SDHP_GUILDSCOREUPDATE pResult;

	PHeadSetB((LPBYTE)&pResult, 0x37, sizeof(pResult));
	memcpy(pResult.GuildName, aRecv->GuildName, 9);
	char szGuildName[11]={0};
	memcpy(szGuildName, aRecv->GuildName, 10);

	// Update Score	and send Packet
	if ( g_MuOnlineExDB.ExecQuery("UPDATE Guild SET G_Score=%d WHERE G_Name='%s'", aRecv->Score, szGuildName) == TRUE )
	{
		pResult.Score = aRecv->Score;		
		for(int i=0;i<g_dwMaxServerGroups;i++)
		{
			if ( g_Server[i].m_Index != -1 &&
				 g_Server[i].m_State >= SS_GUILD )
			{
				DataSend(i, (LPBYTE)&pResult, pResult.h.size);
			}
		}
	}
}




void GDGuildNoticeSave(int aIndex, SDHP_GUILDNOTICE * aRecv)
{
	SDHP_GUILDNOTICE pResult;

	PHeadSetB((LPBYTE)&pResult, 0x38, sizeof(pResult));
	memcpy(pResult.GuildName, aRecv->GuildName, 9);
	memcpy(pResult.szGuildNotice, aRecv->szGuildNotice, sizeof(pResult.szGuildNotice));

	char szMsg[256];
	wsprintf("UPDATE Guild SET G_Notice=? WHERE G_Name='%s'", pResult.GuildName);
	g_MuOnlineExDB.SetAsBinary(szMsg, (LPBYTE)pResult.szGuildNotice, sizeof(pResult.szGuildNotice));
	g_MuOnlineExDB.Close();

	for(int i=0;i<g_dwMaxServerGroups;i++)
	{
		if ( g_Server[i].m_Index != -1 &&
			 g_Server[i].m_State >= SS_GUILD )
		{
			DataSend(i, (LPBYTE)&pResult, pResult.h.size);
		}
	}
}



void GDGuildReqAssignStatus(int aIndex, EXSDHP_GUILD_ASSIGN_STATUS_REQ * aRecv)
{
	EXSDHP_GUILD_ASSIGN_STATUS_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0xE1, sizeof(pResult));
	pResult.btGuildStatus = aRecv->btGuildStatus;
	pResult.btType = aRecv->btType;
	memcpy(pResult.szGuildName, aRecv->szGuildName, 9);
	memcpy(pResult.szTargetName, aRecv->szTargetName, 11);
	pResult.wUserIndex = aRecv->wUserIndex;
	pResult.btResult = 0;
	
	if ( g_MuOnlineExDB.ExecQuery("UPDATE GuildMember SET G_Status=%d WHERE Name='%s'",
		pResult.btGuildStatus, pResult.szTargetName) == TRUE )
		pResult.btResult = 1;

	// Send Packet
	for(int i=0;i<g_dwMaxServerGroups;i++)
	{
		if ( g_Server[i].m_Index != -1 &&
			 g_Server[i].m_State >= SS_GUILD )
		{
			pResult.btFlag = (i == aIndex) ? 1 : 0;
			DataSend(i, (LPBYTE)&pResult, pResult.h.size);
		}
	}
}



void GDGuildReqAssignType(int aIndex, EXSDHP_GUILD_ASSIGN_TYPE_REQ * aRecv)
{
	EXSDHP_GUILD_ASSIGN_TYPE_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0xE2, sizeof(pResult));
	pResult.btGuildType = aRecv->btGuildType;
	memcpy(pResult.szGuildName, aRecv->szGuildName, 9);
	pResult.wUserIndex = aRecv->wUserIndex;
	pResult.btResult = 0;
	
	if ( g_MuOnlineExDB.ExecQuery("UPDATE GuildMember SET G_Type=%d WHERE G_Name='%s'",
		pResult.btGuildType, pResult.szGuildName) == TRUE )
		pResult.btResult = 1;

	// Send Packet
	for(int i=0;i<g_dwMaxServerGroups;i++)
	{
		if ( g_Server[i].m_Index != -1 &&
			 g_Server[i].m_State >= SS_GUILD )
		{
			pResult.btFlag = (i == aIndex) ? 1 : 0;
			DataSend(i, (LPBYTE)&pResult, pResult.h.size);
		}
	}
}


