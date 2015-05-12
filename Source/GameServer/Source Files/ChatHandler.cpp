//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Chat & Commands Class
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------
#include "../Header Files/stdafx.h"
#include "../Header Files/User.H"
#include "../Header Files/GameMain.h"
#include "../Header Files/DSProtocol.h"
#include "../Header Files/Functions.h"
#include "../Header Files/MonsterAttr.h"
#include "../Header Files/Notice.H"
#include "../Header Files/Defines.h"
#include "../Header Files/ObjUseSkill.h"
#include "../Header Files/ObjCalCharacter.h"
#include "../Header Files/EDSProtocol.h"
#include "../Header Files/TUnionInfo.h"
#include "../Header Files/Party.h"
#include "../Header Files/TUnion.h"
#include "../Header Files/GMManager.h"
#include "../Header Files/SendHackLog.h"
#include "../Header Files/SkillAdditionInfo.h"
#include "../Header Files/MasterSkillSystem.h"
#include "../Header Files/Notice.H"
#include "../Header Files/LogProc.H"
#include "../Header Files/SendProto.H"


ChatHandlE		TChat;
COMMANDSTRUCT	CMDSTR;
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::GameChatProcess(PMSG_CHATDATA * lpChat, short aIndex)
{
	int	 Number;
	char PlayerID[MAX_ACCOUNT_LEN+1] = {0};
	int	 cLen = strlen(lpChat->chatmsg);
	int  szTargetNameCount = 0;
	// -----
	LPOBJ lpObj = &gObj[aIndex];
	CHAT_LOG_DATA pChatMsg;
	// -----
	if ( cLen < 1 || cLen > MAX_CHAT_LEN-1 ) return;
	// -----
	memcpy(PlayerID, lpObj->Name, MAX_ACCOUNT_LEN);
	memcpy(lpChat->chatid, PlayerID, MAX_ACCOUNT_LEN);
	// -----
	pChatMsg.h.c			= 0xC1;
	pChatMsg.h.headcode	= 0x02;
	// -----
	memcpy(pChatMsg.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN);
	memcpy(pChatMsg.Name, lpObj->Name, MAX_ACCOUNT_LEN);
	// -----
	pChatMsg.AccountID[MAX_ACCOUNT_LEN]		= 0;
	pChatMsg.Name[MAX_ACCOUNT_LEN]	= 0;
	// -----
	pChatMsg.wServer	= gGameServerCode;
	pChatMsg.btType		= 0xFF;
	// -----
	if ( (lpObj->Penalty&2) == 2 ) return;
	// -----
	if ( lpObj->ChatLimitTime > 0 )		// Blocking Chat...
	{
		SendMsg.MessageOut(lpObj->m_Index,  0x01, "[ChatBlock]: Remaining %d Minute(s).", lpObj->ChatLimitTime);
		return;
	}
	// -----
	if ( lpChat->chatmsg[0] == '!' )	//-- Admin & GameMasters Global Notice
	{
		if ( (lpObj->AuthorityCode&1) == 1 )
		{
			if ( CMDSTR.iMessageChatting[0] == TRUE )
			{
				gSendProto.DataSend(aIndex, (LPBYTE)lpChat, lpChat->h.size);
				// -----
				SendMsg.MessageOutAll(0x00, &lpChat->chatmsg[1]);
				// -----
				CLog.Info("Announcement: [%s][%s]: %s", lpObj->AccountID, lpObj->Name, &lpChat->chatmsg[1]);
			}
			// -----
			memcpy(pChatMsg.szChatMsg, &lpChat->chatmsg[1], MAX_CHAT_LEN-1);
			pChatMsg.szChatMsg[MAX_CHAT_LEN] = 0;
			pChatMsg.btType = 0x03;
			return;
		}
	}
	// -----
	else if ( lpChat->chatmsg[0] == '~' )	//-- Party Message Command
	{
		if ( lpObj->PartyNumber >= 0 )
		{
			int PartyCount = gParty.GetPartyCount(lpObj->PartyNumber);
			// -----
			if ( PartyCount >= 0 )
			{
				memcpy(pChatMsg.szChatMsg, &lpChat->chatmsg[1], MAX_CHAT_LEN-1);
				pChatMsg.szChatMsg[MAX_CHAT_LEN] = 0;
				pChatMsg.btType = 0x01;
				// -----
				for (int n=0;n<MAX_USER_IN_PARTY;n++ )
				{
					Number = gParty.m_PartyInfo[lpObj->PartyNumber].Number[n];
					// -----
					if ( Number >= 0 )
					{
						if ( CMDSTR.iMessageChatting[1] == TRUE )
						{
							gSendProto.DataSend(Number, (LPBYTE)lpChat, lpChat->h.size);
							// -----
							strcpy(pChatMsg.szTargetName[szTargetNameCount], gObj[Number].Name);
							szTargetNameCount++;
						}
					}
				}
			}
		}
	}
	// -----
	else if ( lpChat->chatmsg[0] == '@' )	//-- Guild Message Command
	{
		if ( lpObj->GuildNumber > 0 )
		{
			if ( lpChat->chatmsg[1] == '>' )	//-- Guild Notice
			{
				if ( lpObj->Name[0] == lpObj->lpGuild->Names[0][0] )
				{
					if ( !strcmp(lpObj->Name, lpObj->lpGuild->Names[0] ) )
					{
						if ( CMDSTR.iMessageChatting[2] == TRUE )
						{
							GDGuildNoticeSave(lpObj->lpGuild->Name, &lpChat->chatmsg[2]);
							// -----
							memcpy(pChatMsg.szChatMsg, &lpChat->chatmsg[2], MAX_CHAT_LEN-2);
							pChatMsg.szChatMsg[MAX_CHAT_LEN] = 0;
							pChatMsg.btType = 0x04;
						}
					}
				}
			}
			// -----
			else if ( lpChat->chatmsg[1] == '@' )	//-- Guild Aliances
			{
				if ( lpObj->lpGuild->iGuildUnion )
				{
					int iGuildCount					= 0;
					int iGuildList[MAX_UNION_GUILD] = {0};
					// -----
					if ( UnionManager.GetGuildUnionMemberList(lpObj->lpGuild->iGuildUnion, iGuildCount, iGuildList) == TRUE )
					{
						for ( int i=0;i<iGuildCount;i++)
						{
							_GUILD_INFO_STRUCT *lpGuildInfo = Guild.SearchGuild_Number(iGuildList[i]);
							// -----
							if ( !lpGuildInfo )
								continue;
							// -----
							for (int n=0;n<MAX_USER_GUILD;n++)
							{
								if ( lpGuildInfo->Use[n] )
								{
									Number = lpGuildInfo->Index[n];
									// -----
									if ( Number >= 0 )
									{
										if ( lpGuildInfo->Names[n][0] == gObj[Number].Name[0] )
										{
											if ( !strcmp(lpGuildInfo->Names[n], gObj[Number].Name ))
											{
												gSendProto.DataSend(Number, (LPBYTE)lpChat, lpChat->h.size);
											}
										}
									}
								}
							}
						}
					}
					// -----
					if ( CMDSTR.iMessageChatting[3] == TRUE )
					{
						GDUnionServerGroupChattingSend(lpObj->lpGuild->iGuildUnion, lpChat);
					}
				}
			}
			// -----
			else	//-- Normal Guild Chat
			{
				int count = lpObj->lpGuild->Count;
				// -----
				if ( count >= 0 )
				{
					for ( int n=0;n<MAX_USER_GUILD;n++)
					{
						if ( lpObj->lpGuild->Use[n] )
						{
							Number = lpObj->lpGuild->Index[n];
							// -----
							if ( Number >= 0 )
							{
								if ( lpObj->lpGuild->Names[n][0] == gObj[Number].Name[0] )
								{
									if ( !strcmp(lpObj->lpGuild->Names[n], gObj[Number].Name ) )
									{
										gSendProto.DataSend(Number, (LPBYTE)lpChat, lpChat->h.size);
									}
								}
							}
						}
					}
				}
				// -----
				if ( CMDSTR.iMessageChatting[4] == TRUE )
				{
					if ( lpObj->lpGuild->Count > 1 )
					{
						GDGuildServerGroupChattingSend(lpObj->lpGuild->Number, lpChat);
					}
					// -----
					memcpy(pChatMsg.szChatMsg, &lpChat->chatmsg[1], MAX_CHAT_LEN-1);
					pChatMsg.szChatMsg[MAX_CHAT_LEN] = 0;
					pChatMsg.btType = 0x02;
				}
			}
			// -----
			strcpy(pChatMsg.szTargetName[szTargetNameCount], lpObj->GuildName);
			szTargetNameCount++;
		}
	}
	// -----
	else if ( lpChat->chatmsg[0] == '$' )	//-- Gen Family Command
	{
		if ( lpObj->GenFamily > 0 )
		{
			for (int i=OBJMIN; i<OBJMAX; i++)
			{
				LPOBJ lpOtherObj = &gObj[i];
				// -----
				if ( gObjIsConnected(lpOtherObj) == true )
				{
					if ( lpOtherObj->GenFamily == lpObj->GenFamily )
					{
						if ( CMDSTR.iMessageChatting[5] == TRUE )
						{
							if ( lpObj->GenFamily == 1 )
							{
								SendMsg.GensMsgOut(i, OBJ_SEND_NORMAL, NO_SPC_MAP, "[D][%s]: %s", lpObj->Name, &lpChat->chatmsg[1]);
							}
							else
							{
								SendMsg.GensMsgOut(i, OBJ_SEND_NORMAL, NO_SPC_MAP, "[V][%s]: %s", lpObj->Name, &lpChat->chatmsg[1]);
							}
						}
					}
				}
			}
		}
	}
	// -----
	else
	{
		if ( lpChat->chatmsg[0] != '/' )
		{
			if ( CMDSTR.iMessageChatting[6] == TRUE )
			{
				if ( lpObj->DelayTimeMSG >= 1)
				{
					MsgOutput(aIndex,"You have to wait %d Second(s) between Each Chat.", lpObj->DelayTimeMSG);
					return;
				}
				// -----
				gSendProto.DataSend(aIndex, (LPBYTE)lpChat, lpChat->h.size);
				gSendProto.VPMsgSend(lpObj, (LPBYTE)lpChat, lpChat->h.size);
				lpObj->DelayTimeMSG = 60;
			}
			// -----
			memcpy(pChatMsg.szChatMsg, lpChat->chatmsg, MAX_CHAT_LEN);
			pChatMsg.szChatMsg[MAX_CHAT_LEN] = 0;
			pChatMsg.btType = 0x00;
		} 
		else 
		{
			if ( !CMDSTR.iSwitchCommands )
			{
				SendMsg.MessageOut(aIndex, 0x01, "[Command]: System is not available at the moment.");
			} else {
				this->CommandReader(aIndex, (LPBYTE)lpChat);
			}
		}
	}
	// -----
	if ( pChatMsg.btType != 0xFF )
	{
		pChatMsg.btType |= szTargetNameCount<<4;
		pChatMsg.h.size = sizeof(pChatMsg) - (5 - szTargetNameCount) - 1;
		// -----
		gSendHackLog.SendData((LPBYTE)&pChatMsg, pChatMsg.h.size);
	}
}							
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::GameChatRecieve(PMSG_CHATDATA_NUMBER * lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// -----
	//if ( gRecvProto.PacketCheckTime(lpObj) == FALSE ) return;
	// -----
	gSendProto.DataSend(aIndex, (LPBYTE)lpMsg, lpMsg->h.size);
	gSendProto.VPMsgSend(lpObj, (LPBYTE)lpMsg, lpMsg->h.size);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::ChatWhisperRecieve(PMSG_CHATDATA_WHISPER* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	// -----
	int  cLen;
	char PlayerID[11];
	int  cIndex;
	// -----
	PMSG_CHATDATA_WHISPER pWhisper;
	// -----
	//if ( !gRecvProto.PacketCheckTime(lpObj) ) return;
	// -----
	if ( lpObj->ChatLimitTime > 0 )
	{
		SendMsg.MessageOut(lpObj->m_Index,  0x01, "[ChatBlock]: Remaining %d Minute(s).", lpObj->ChatLimitTime);
		return;
	}
	// -----
	if ( (lpObj->Penalty & 2) == 2 ) return;
	// -----
	PlayerID[10] = 0;
	// -----
	memcpy(PlayerID, lpMsg->id, sizeof(lpMsg->id));
	// -----
	cIndex = WhisperCash.AddCash(PlayerID);
	// -----
	if ( cIndex < 0 )
	{
		gSendProto.NoUsersMessage(aIndex);
		return;
	}
	// -----
	C1HeadSet((LPBYTE)&pWhisper, 0x02, sizeof(pWhisper));
	// -----
	strcpy(pWhisper.id, gObj[aIndex].Name);
	memcpy(pWhisper.chatmsg, lpMsg->chatmsg, sizeof(lpMsg->chatmsg));
	// -----
	cLen = strlen(pWhisper.chatmsg);
	// -----
	if ( cLen > 0 )
	{
		pWhisper.h.size -= sizeof(pWhisper.chatmsg);
		pWhisper.h.size += cLen + 1;
		// -----
		gSendProto.DataSend(cIndex, (UCHAR*)&pWhisper, pWhisper.h.size);
		// -----
		CHAT_LOG_DATA pChatMsg;
		// -----
		int szTargetNameCount = 0;
		// -----
		pChatMsg.h.c			= 0xC1;
		pChatMsg.h.headcode	= 0x02;
		// -----
		memcpy(pChatMsg.AccountID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);
		memcpy(pChatMsg.Name, gObj[aIndex].Name, MAX_ACCOUNT_LEN);
		// -----
		pChatMsg.AccountID[MAX_ACCOUNT_LEN]		= 0;
		pChatMsg.Name[MAX_ACCOUNT_LEN]	= 0;
		// -----
		pChatMsg.wServer = gGameServerCode;
		// -----
		memcpy(pChatMsg.szChatMsg, lpMsg->chatmsg, MAX_CHAT_LEN);
		// -----
		pChatMsg.szChatMsg[MAX_CHAT_LEN]	= 0;
		pChatMsg.btType						= 5;
		// -----
		strcpy(pChatMsg.szTargetName[szTargetNameCount], gObj[cIndex].Name);
		// -----
		szTargetNameCount++;
		// -----
		if ( szTargetNameCount != 0 )
		{
			pChatMsg.btType |= szTargetNameCount << 4;
		}
		// -----
		pChatMsg.h.size = sizeof(pChatMsg);
		// -----
		gSendHackLog.SendData((LPBYTE)&pChatMsg, pChatMsg.h.size);
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::CommandReader(short gObjId, LPBYTE Protocol)
{
	LPOBJ lpObj = &gObj[gObjId];
	// -----
	BYTE* ChatHandlE = &Protocol[0x0D];
	// -----
	/*if ( lpObj->CommandLimitTime > 0 )		// Blocking Commands...
	{
		SendMsg.MessageOut(lpObj->m_Index, "[CommandBlock]: Remaining %d Minute(s).", lpObj->CommandLimitTime);
		return;
	}*/
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iPKC_Command,strlen(CMDSTR.iPKC_Command)) )
	{
		this->ClearPKCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iPKC_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iPST_Command,strlen(CMDSTR.iPST_Command)) )
	{
		this->PostCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iPST_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iCRT_Command,strlen(CMDSTR.iCRT_Command)) )
	{
		this->CreateItemsCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iCRT_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iONL_Command,strlen(CMDSTR.iONL_Command)) )
	{
		this->OnlinePlayersCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iONL_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iTIM_Command,strlen(CMDSTR.iTIM_Command)) )
	{
		this->ServerTimeCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iTIM_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iFIR_Command,strlen(CMDSTR.iFIR_Command)) )
	{
		this->FireworksCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iFIR_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iPKS_Command,strlen(CMDSTR.iPKS_Command)) )
	{
		this->PkSetCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iPKS_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iADD_CommandStr,strlen(CMDSTR.iADD_CommandStr)) )
	{
		this->AddPointsCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iADD_CommandStr),0);
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iADD_CommandAgi,strlen(CMDSTR.iADD_CommandAgi)) )
	{
		this->AddPointsCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iADD_CommandAgi),1);
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iADD_CommandVit,strlen(CMDSTR.iADD_CommandVit)) )
	{
		this->AddPointsCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iADD_CommandVit),2);
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iADD_CommandEne,strlen(CMDSTR.iADD_CommandEne)) )
	{
		this->AddPointsCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iADD_CommandEne),3);
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iADD_CommandLed,strlen(CMDSTR.iADD_CommandLed)) )
	{
		this->AddPointsCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iADD_CommandLed),4);
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iDIS_CommandALL,strlen(CMDSTR.iDIS_CommandALL)) )
	{
		this->DisconnectCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iDIS_CommandALL), 0);
	}

	if ( !memcmp(ChatHandlE, CMDSTR.iDIS_CommandGLD,strlen(CMDSTR.iDIS_CommandGLD)) )
	{
		this->DisconnectCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iDIS_CommandGLD), 1);
	}

	if ( !memcmp(ChatHandlE, CMDSTR.iDIS_CommandCHR,strlen(CMDSTR.iDIS_CommandCHR)) )
	{
		this->DisconnectCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iDIS_CommandCHR), 2);
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iSZN_Command,strlen(CMDSTR.iSZN_Command)) )
	{
		this->SetZenCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iSZN_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iAZN_Command,strlen(CMDSTR.iAZN_Command)) )
	{
		this->AddZenCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iAZN_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iSKN_Command,strlen(CMDSTR.iSKN_Command)) )
	{
		this->SkinCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iSKN_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iSMB_Command,strlen(CMDSTR.iSMB_Command)) )
	{
		this->SpawnMobCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iSMB_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iSTS_Command,strlen(CMDSTR.iSTS_Command)) )
	{
		this->StatusCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iSTS_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iINV_Command,strlen(CMDSTR.iINV_Command)) )
	{
		this->ClearInvCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iINV_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iEXT_Command,strlen(CMDSTR.iEXT_Command)) )
	{
		this->ExitCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iEXT_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iABT_Command,strlen(CMDSTR.iABT_Command)) )
	{
		this->AboutCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iABT_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iSLV_Command,strlen(CMDSTR.iSLV_Command)) )
	{
		this->SetLevelCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iSLV_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iLUP_Command,strlen(CMDSTR.iLUP_Command)) )
	{
		this->SetLvlUpPCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iLUP_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iABF_Command,strlen(CMDSTR.iABF_Command)) )
	{
		this->AddBuffCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iABF_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iSMN_Command,strlen(CMDSTR.iSMN_Command)) )
	{
		this->SummonCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iSMN_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, "/GoToTheHellDown",strlen("/GoToTheHellDown")) )
	{
		this->CrashCommand(gObjId,(char*)Protocol+0x0D+strlen("/GoToTheHellDown"));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iMRG_CommandPro,strlen(CMDSTR.iMRG_CommandPro)) )
	{
		this->ProposeMarryCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iMRG_CommandPro));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iMRG_CommandAcp,strlen(CMDSTR.iMRG_CommandAcp)) )
	{
		this->AcceptMarryCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iMRG_CommandAcp));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iMRG_CommandDiv,strlen(CMDSTR.iMRG_CommandDiv)) )
	{
		this->DivorceMarryCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iMRG_CommandDiv));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iMRG_CommandOnl,strlen(CMDSTR.iMRG_CommandOnl)) )
	{
		this->OnlineMarryCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iMRG_CommandOnl));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iMRG_CommandTrc,strlen(CMDSTR.iMRG_CommandTrc)) )
	{
		this->TraceMarryCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iMRG_CommandTrc));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iRSP_Command,strlen(CMDSTR.iRSP_Command)) )
	{
		this->RestorePointsCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iRSP_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iMVA_Command,strlen(CMDSTR.iMVA_Command)))
	{
		this->MoveAllPlayersCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iMVA_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iCST_Command,strlen(CMDSTR.iCST_Command)))
	{
		this->CreateSetCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iCST_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iCBN_Command,strlen(CMDSTR.iCBN_Command)))
	{
		this->BlockChatCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iCBN_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iCRV_Command,strlen(CMDSTR.iCRV_Command)))
	{
		this->ClearVaultCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iCRV_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iGRI_Command,strlen(CMDSTR.iGRI_Command)))
	{
		this->GetRegInfoCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iGRI_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iSTC_Command,strlen(CMDSTR.iSTC_Command)))
	{
		this->StatisticsCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iSTC_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iCBK_Command,strlen(CMDSTR.iCBK_Command)))
	{
		this->BlockCmdCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iCBK_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iBAN_CommandUBN,strlen(CMDSTR.iBAN_CommandUBN)))
	{
		this->BanAccountCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iBAN_CommandUBN),0);
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iBAN_CommandBAN,strlen(CMDSTR.iBAN_CommandBAN)))
	{
		this->BanAccountCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iBAN_CommandBAN),1);
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iBLK_CommandUBK,strlen(CMDSTR.iBLK_CommandUBK)))
	{
		this->BlockCharacterCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iBLK_CommandUBK),0);
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iBLK_CommandBLK,strlen(CMDSTR.iBLK_CommandBLK)))
	{
		this->BlockCharacterCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iBLK_CommandBLK),1);
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iRST_Command,strlen(CMDSTR.iRST_Command)))
	{
		this->ResetCharCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iRST_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iGMV_Command,strlen(CMDSTR.iGMV_Command)))
	{
		this->GMMoveCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iGMV_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iTRC_Command,strlen(CMDSTR.iTRC_Command)))
	{
		this->TraceCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iTRC_Command));
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iGDW_CommandWAR,strlen(CMDSTR.iGDW_CommandWAR)))
	{
		this->GuildWarCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iGDW_CommandWAR), 0);
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iGDW_CommandEND,strlen(CMDSTR.iGDW_CommandEND)))
	{
		this->GuildWarCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iGDW_CommandEND), 1);
	}
	// -----
	if ( !memcmp(ChatHandlE, CMDSTR.iDRE_Command,strlen(CMDSTR.iDRE_Command)))
	{
		this->DropEventCommand(gObjId,(char*)Protocol+0x0D+strlen(CMDSTR.iDRE_Command));
	}
	// -----
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
//											:: Custom HaRiBO GameServer Commands ::
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::ClearPKCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int cLen	= strlen(chatmsg);
	// -----
	if ( cLen < 0 || cLen >= 1)
	{
		SendMsg.MessageOut(ChatID, 0x01, "Bad Syntax: %s", CMDSTR.iPKC_Command);
		return;
	}
	// -----
	if ( (lpObj->AuthorityCode&1) )
	{
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanClearPK = GMSTR[x].ClearPK;
				}
			}

			if ( !CMDSTR.CanClearPK )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
    if ( !CMDSTR.iPKC_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[PKClear]: Command is currently Inactive!");
		return;
	}
	// -----
	if ( lpObj->m_PK_Level <= COMMONER_STATUS )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[PKClear]: You are already a Commoner!");
		return;
	}
	// -----
	if ( lpObj->Level < CMDSTR.iPKC_RequiredLevel )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[PKClear]: Need Level %d to use This Command.", CMDSTR.iPKC_RequiredLevel);
		return;
	}
	// -----
	if ( CMDSTR.iPKC_EnalbeCost )
	{
		if( CMDSTR.iPKC_IsCostPerPKCount )
		{
			if ( lpObj->Money < (CMDSTR.iPKC_RequiredCost * lpObj->m_PK_Count) )
			{
				if ( CMDSTR.iPKC_RequiredCost > 0 )
				{
					SendMsg.MessageOut(ChatID, 0x01, "[PKClear]: Need %d Zen To Use This Command.", (CMDSTR.iPKC_RequiredCost * lpObj->m_PK_Count));
				}
				return;
			}
		}
		else
		{
			if ( lpObj->Money < (CMDSTR.iPKC_RequiredCost) )
			{
				if ( CMDSTR.iPKC_RequiredCost > 0 )
				{
					SendMsg.MessageOut(ChatID, 0x01, "[PKClear]: Need %d Zen To Use This Command.", (CMDSTR.iPKC_RequiredCost));
				}
				return;
			}
		}
		// -----
		if( CMDSTR.iPKC_IsCostPerPKCount )
		{
			lpObj->Money = lpObj->Money - (CMDSTR.iPKC_RequiredCost * lpObj->m_PK_Count);
		}
		else
		{
			lpObj->Money = lpObj->Money - CMDSTR.iPKC_RequiredCost;
		}
		// -----
		if( CMDSTR.iPKC_RequiredCost > 0 )
		{
			GCMoneySend(lpObj->m_Index, lpObj->Money);
		}
	}
    // -----
	lpObj->m_PK_Level = COMMONER_STATUS;
	// -----
	GCPkLevelSend( lpObj->m_Index, COMMONER_STATUS );
	// -----
	SendMsg.MessageOut(ChatID, 0x01, "[PKClear]: Your PK Status is Successfully Removed.");
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::PostCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int cLen	= strlen(chatmsg);
	// -----
	if ( cLen < 0 || cLen >= 61) return;
	// -----
	if ( (lpObj->AuthorityCode&1) )
	{
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanPost = GMSTR[x].PostMSG;
				}
			}

			if ( !CMDSTR.CanPost )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if ( !CMDSTR.iPST_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[GlobalMSG]: Command is currently Inactive!");
		return;
	}
	// -----
	if ( cLen <= 1 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Message>", CMDSTR.iPST_Command);
		// -----
		if ( CMDSTR.iPST_EnalbeCost )
		{
			SendMsg.MessageOut(ChatID, 0x01, "Required level [%d], Zen [%ld].", CMDSTR.iPST_RequiredLevel, CMDSTR.iPST_RequiredCost);
		} else {
			SendMsg.MessageOut(ChatID, 0x01, "Required level [%d].", CMDSTR.iPST_RequiredLevel, CMDSTR.iPST_RequiredCost);
		}
		return;
	}
	// -----
	if ( lpObj->Level < CMDSTR.iPST_RequiredLevel )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[GlobalMSG]: Need Level %d to use This Command.", CMDSTR.iPST_RequiredLevel);
		return;
	}
	// -----
	if ( CMDSTR.iPST_EnalbeCost )
	{
		if ( lpObj->Money < CMDSTR.iPST_RequiredCost )
		{
			if ( CMDSTR.iPST_RequiredCost > 0 )
			{
				SendMsg.MessageOut(ChatID, 0x01, "[GlobalMSG]: Need %d Zen To Use This Command.", CMDSTR.iPST_RequiredCost);
			}
			return;
		}
		// -----
		if ( CMDSTR.iPST_RequiredCost > 0 )
		{
			lpObj->Money = lpObj->Money - CMDSTR.iPST_RequiredCost;
			// -----
			GCMoneySend(lpObj->m_Index, lpObj->Money);
		}
	}
	// -----
	SendMsg.MessageOutAll(OBJ_SEND_ALL_NORMAL, "[POST] %s:%s", lpObj->Name, chatmsg);

	if ( CMDSTR.iPST_PostColor == 0 )
	{
		SendMsg.NormalMsgOut(OBJ_EMPTY, OBJ_SEND_ALL_PLYRS, NO_SPC_MAP, "[POST] %s:%s", lpObj->Name, chatmsg);
	}
	else if ( CMDSTR.iPST_PostColor == 1 )
	{
		SendMsg.GuildMsgOut(OBJ_EMPTY, OBJ_SEND_ALL_PLYRS, NO_SPC_MAP, "[POST] %s:%s", lpObj->Name, chatmsg);
	}
	else if ( CMDSTR.iPST_PostColor == 2 )
	{
		SendMsg.AllianceMsgOut(OBJ_EMPTY, OBJ_SEND_ALL_PLYRS, NO_SPC_MAP, "[POST] %s:%s", lpObj->Name, chatmsg);
	}
	else if ( CMDSTR.iPST_PostColor == 3 )
	{
		SendMsg.PartyMsgOut(OBJ_EMPTY, OBJ_SEND_ALL_PLYRS, NO_SPC_MAP, "[POST] %s:%s", lpObj->Name, chatmsg);
	}
	else if ( CMDSTR.iPST_PostColor == 4 )
	{
		SendMsg.MessageOutAll(OBJ_SEND_ALL_GOLDEN, "[POST] %s:%s", lpObj->Name, chatmsg);
	}
	else if ( CMDSTR.iPST_PostColor == 5 )
	{
		SendMsg.GensMsgOut(OBJ_EMPTY, OBJ_SEND_ALL_PLYRS, NO_SPC_MAP, "[POST] %s:%s", lpObj->Name, chatmsg);
	}
	else
	{
		CLog.Error("Post Command Color variable is Set to wrong value ( Min/Max :: 0/5 ).");
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::CreateItemsCommand(DWORD ChatID, char chatmsg[60])		//Create 7 1 15 255 1 1 7 63 10 255 255 255 255 255
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	int   Type;
	int	  Index;
	BYTE  Level;
	BYTE  Dur;
	BYTE  Skill;
	BYTE  Luck;
	BYTE  AddOpt; 
	BYTE  AddExc; 
	BYTE  AddAnc;
	BYTE  Socket[5];
	BYTE  CordX;
	BYTE  CordY;
	// -----
	if ( (lpObj->AuthorityCode&1) )
	{
		if ( cLen < 0 ) return;
		// -----
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanCreate = GMSTR[x].CreateItems;
				}
			}

			if ( !CMDSTR.CanCreate )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
		// -----
		if ( !CMDSTR.iCRT_Switch )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[C.Items]: Command is currently Inactive!");
			return;
		}
		// -----
		if ( cLen <= 27 || cLen >= 47 )
		{
			SendMsg.MessageOut(ChatID, 0x01, "Bad Syntax: %s <Type><Index><Lvl><Dur><Luck><Skill><AddOpt><Exc><Anc><Soc1><Soc2><Soc3><Soc4><Soc5>",
				CMDSTR.iCRT_Command);
			return;
		}
		// -----
		sscanf(chatmsg,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d", &Type, &Index, &Level, &Dur, &Luck, &Skill, &AddOpt, &AddExc, &AddAnc,
			&Socket[0], &Socket[1], &Socket[2], &Socket[3], &Socket[4]);
		// -----
		if  (  Type		 >	 15	 ||  Type		<	0 
			|| Index	 >	 512 ||  Index		<	0 
			|| Level	 >	 15	 ||  Level		<	0 
			|| Dur		 <	 0	 ||  Dur		>	255 
			|| Luck		 >	 1	 ||  Luck		<	0 
			|| Skill	 >	 1	 ||  Skill		<	0 
			|| AddOpt	 <	 0	 ||  AddOpt		>	7 
			|| AddExc	 <	 0	 &&  AddExc		>	63
			|| AddAnc	 <	 0	 ||  AddAnc		==	1 
			|| AddAnc	 ==	 2	 ||  AddAnc		==	3 
			|| AddAnc	 ==	 4	 ||  AddAnc		==	6 
			|| AddAnc	 ==	 7	 ||  AddAnc		==	8		
			|| AddAnc	 ==	 9	 ||  AddAnc		>	10	
			|| Socket[0] <	 0	 ||  Socket[0]	>	255
			|| Socket[1] <	 0	 ||  Socket[1]	>	255
			|| Socket[2] <	 0	 ||  Socket[2]	>	255
			|| Socket[3] <	 0	 ||  Socket[3]	>	255
			|| Socket[4] <	 0	 ||  Socket[4]	>	255	)
		{
			SendMsg.MessageOut(ChatID, 0x01, "[C.Items]: You have been used wrong codes.");
			return;
		}
		// -----
		DWORD ItemShift	= Type * MAX_SUBTYPE_ITEMS + Index;
		CordX			= lpObj->X;
		CordY			= lpObj->Y;
		// -----
		ItemSerialCreateSend(ChatID, lpObj->MapNumber, CordX, CordY, ItemShift, Level, Dur, Skill, Luck, AddOpt, ChatID, AddExc, AddAnc, Socket);
		// -----
		SendMsg.MessageOut(ChatID, 0x01, "[C.Items]: You Successflly created (%s+%d)", &ItemAttribute[ItemShift].Name[0], Level);
	}
	else
	{
		if ( cLen < 0 || cLen >= 1 ) return;
		// -----
		SendMsg.MessageOut(ChatID, 0x01, "[C.Items]: Command is Only available For GameMasters.");
		return;
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::OnlinePlayersCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int cLen	= strlen(chatmsg);
	// -----
	int	MuMasters	= 0;
	int	Players		= 0;
	// -----
	if ( cLen < 0 || cLen >= 1)
	{
		SendMsg.MessageOut(ChatID, 0x01, "Bad Syntax: %s", CMDSTR.iONL_Command);
		return;
	}
	// -----
	if ( CMDSTR.iONL_IsOnlyForGM )
	{
		if ( !(lpObj->AuthorityCode&1) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[Online]: Command is Only available For GameMasters.");
			return;
			// -----
		} else {
			if ( cGMMng.EnableGMManager )
			{
				for(int x=0; x<cGMMng.GMMngCount; x++)
				{
					if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
					{
						CMDSTR.CanOnline = GMSTR[x].OnlineChar;
					}
				}

				if ( !CMDSTR.CanOnline )
				{
					MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
					return;
				}
			}
		}
	}
	// -----
	if ( !CMDSTR.iONL_Switch )
	{	
		SendMsg.MessageOut(ChatID, 0x01, "[Online]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( lpObj->Level < CMDSTR.iONL_RequiredLevel )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Online]: Need Level %d To Use This Command.", CMDSTR.iONL_RequiredLevel);
		return;
	}
	// -----
	for ( int i=OBJMIN; i<OBJMAX; i++ )
	{
		LPOBJ pObj = &gObj[i];
		// --
		if ( pObj->Connected == PLAYER_PLAYING )
		{
			if ( pObj->AuthorityCode&1 )
			{
				MuMasters++;
			}
			else
			{
				Players++;
			}
		}
	}
	// -----
	SendMsg.MessageOut(ChatID, 0x01, "[Online]: Player(s) Connected: %d", Players);
	SendMsg.MessageOut(ChatID, 0x01, "[Online]: Total MuMaster(s) Connected: %d", MuMasters);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::ServerTimeCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int cLen	= strlen(chatmsg);
	// -----
	SYSTEMTIME st;
	GetLocalTime(&st);
	// -----
	char szTempA[256];
	char szTempB[256];
	// -----
	sprintf(szTempA, "Current Time: %d:%d:%d", st.wHour, st.wMinute, st.wSecond);
	sprintf(szTempB, "Current Date: %s, %d - %s - %d", GetDayName(st.wDayOfWeek), st.wDay, GetMonthName(st.wMonth), st.wYear);
	// -----
	if ( cLen < 0 || cLen >= 1)
	{
		SendMsg.MessageOut(ChatID, 0x01, "Bad Syntax: %s", CMDSTR.iTIM_Command);
		return;
	}
	// -----
	if ( !CMDSTR.iTIM_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[SrvTime]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( CMDSTR.iTIM_MsgColor == 0 )
	{
		SendMsg.NormalMsgOut(ChatID, OBJ_SEND_NORMAL, NO_SPC_MAP, szTempA);
		SendMsg.NormalMsgOut(ChatID, OBJ_SEND_NORMAL, NO_SPC_MAP, szTempB);
	}
	else if ( CMDSTR.iTIM_MsgColor == 1 )
	{
		SendMsg.WhisperOut(ChatID, OBJ_SEND_NORMAL, NO_SPC_MAP, szTempA);
		SendMsg.WhisperOut(ChatID, OBJ_SEND_NORMAL, NO_SPC_MAP, szTempB);
	}
	else if ( CMDSTR.iTIM_MsgColor == 2 )
	{
		SendMsg.PartyMsgOut(ChatID, OBJ_SEND_NORMAL, NO_SPC_MAP, szTempA);
		SendMsg.PartyMsgOut(ChatID, OBJ_SEND_NORMAL, NO_SPC_MAP, szTempB);
	}
	else if ( CMDSTR.iTIM_MsgColor == 3 )
	{
		SendMsg.GuildMsgOut(ChatID, OBJ_SEND_NORMAL, NO_SPC_MAP, szTempA);
		SendMsg.GuildMsgOut(ChatID, OBJ_SEND_NORMAL, NO_SPC_MAP, szTempB);
	}
	else if ( CMDSTR.iTIM_MsgColor == 4 )
	{
		SendMsg.AllianceMsgOut(ChatID, OBJ_SEND_NORMAL, NO_SPC_MAP, szTempA);
		SendMsg.AllianceMsgOut(ChatID, OBJ_SEND_NORMAL, NO_SPC_MAP, szTempB);
	}
	else if ( CMDSTR.iTIM_MsgColor == 5 )
	{
		SendMsg.GensMsgOut(ChatID, OBJ_SEND_NORMAL, NO_SPC_MAP, szTempA);
		SendMsg.GensMsgOut(ChatID, OBJ_SEND_NORMAL, NO_SPC_MAP, szTempB);
	}
	else if ( CMDSTR.iTIM_MsgColor == 6 )
	{
		SendMsg.MessageOut(ChatID, OBJ_SEND_ALL_NORMAL, szTempA);
		SendMsg.MessageOut(ChatID, OBJ_SEND_ALL_NORMAL, szTempB);
	}
	else if ( CMDSTR.iTIM_MsgColor == 7 )
	{
		SendMsg.MessageOut(ChatID, OBJ_SEND_ALL_GOLDEN, szTempA);
		SendMsg.MessageOut(ChatID, OBJ_SEND_ALL_GOLDEN, szTempB);
	}
	else
	{
		CLog.Error("Time Message Color variable is Set to wrong value ( Min/Max :: 0/6 ).");
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::FireworksCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	int Amount		= 0;
	char Player[11]	= {0};
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( !(lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Fireworks]: Command is Only available For GameMasters.");
		return;
		// -----
	} else {
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanFireworks = GMSTR[x].Fireworks;
				}
			}

			if ( !CMDSTR.CanFireworks )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if ( !CMDSTR.iFIR_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Fireworks]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 6 || cLen >= 16 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Name> <Amount>", CMDSTR.iFIR_Command);
		return;
	}
	// -----
	sscanf(chatmsg, "%s %d", &Player, &Amount);
	// -----
	DWORD PlayerID = gObjGetIndex(Player);
	// -----
	if ( PlayerID == 0xFFFF ) return;
	// -----
	if(( PlayerID >= OBJMIN ) && ( PlayerID <= OBJMAX ))
	{
		LPOBJ pObj = &gObj[PlayerID];
		// -----
		if ( (pObj->AuthorityCode&1) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[Fireworks]: You can't Send Fireworks to GameMaster.");
			return;
		}
		// -----
		if ( pObj->Name != lpObj->Name )
		{
			if ( CMDSTR.iFIR_MaxAmount > 413 )
			{
				CLog.Error("Max Amount of Fireworks variable is Set to wrong value ( Max :: 413 ).");
				SendMsg.MessageOut(ChatID, 0x01, "[Fireworks]: Contact Administrator.");
				return;
			}
			if ( Amount > CMDSTR.iFIR_MaxAmount || Amount < 1 )
			{
				SendMsg.MessageOut(ChatID, 0x01, "[Fireworks]: [Warning] Your Amounts are range (1/%d).", CMDSTR.iFIR_MaxAmount);
				return;
			}
			// -----
			GCFireWorkSend( PlayerID, Amount );
			// -----
			SendMsg.MessageOut(ChatID, 0x01, "[Fireworks]: Successflly Sent To %s!", Player);
			SendMsg.MessageOut(PlayerID, 0x01, "[Fireworks] %s Sent You [%d] Amount Of Fireworks.", lpObj->Name, Amount);
		// -----
		} else {
			SendMsg.MessageOut(ChatID, 0x01, "[Fireworks]: You can't Send Fireworks To Yourself");
			return;
		}
	// -----
	} else {
		SendMsg.MessageOut(ChatID, 0x01, "[Fireworks] Player: %s is offline", Player);
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::PkSetCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	int	 PKLevel	= 0;
	char Player[16]	= {0};
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( !(lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[SetPK]: Command is Only available For GameMasters.");
		return;
		// -----
	} else {
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanSetPK = GMSTR[x].SetPK;
				}
			}

			if ( !CMDSTR.CanSetPK )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if( !CMDSTR.iPKS_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[SetPK]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 6 || cLen >= 14 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Name> <PK Level>", CMDSTR.iPKS_Command);
		return;
	}
	// -----
	sscanf(chatmsg,"%s %d",Player, &PKLevel);
	// -----
	if ( PKLevel < 1 || PKLevel > 6 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[SetPK]: PK Level MIN/MAX :: [1/6]");
		return;
	}
	// -----
	DWORD PlayerID = gObjGetIndex(Player);
	// -----
	if( PlayerID == 0xFFFF ) return;
	// -----
	if(( PlayerID>=OBJMIN ) && ( PlayerID<=OBJMAX ))
	{
		LPOBJ pObj = &gObj[PlayerID];
		// -----
		if ( (pObj->AuthorityCode&1) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[SetPK]: You can't Set PK For GameMaster.");
			return;
		}
		// -----
		pObj->m_PK_Level = PKLevel;
		// -----
		GCPkLevelSend(PlayerID, PKLevel);
		// -----
		SendMsg.MessageOut(ChatID, 0x01, "[SetPK] Player: %s is PK, Level [%d]!", Player, PKLevel);
		// -----
		if(pObj->Name != lpObj->Name)
		{
			SendMsg.MessageOut(PlayerID, 0x01, "[SetPK] %s Set Your PK Status To Level: [%d].", lpObj->Name, PKLevel);
		}
	// -----
	} else {
		SendMsg.MessageOut(ChatID, 0x01, "[SetPK] Player: %s is offline", Player);
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::AddPointsCommand(DWORD ChatID, LPCSTR lpBuffer, int CMD_STATS_TYPE)
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(lpBuffer);
	int Points	= atoi(lpBuffer);
	// -----
	int MaxPoints	= 32767;
	int TotalPoints = 0;
	int POINTS;
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( (lpObj->AuthorityCode&1) )
	{
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanAddPoints = GMSTR[x].AddPoints;
				}
			}

			if ( !CMDSTR.CanAddPoints )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if ( !CMDSTR.iADD_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[AddPoint]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 1 || cLen >= 5)
	{
		switch( CMD_STATS_TYPE )
		{
			case 0:
				SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Points>", CMDSTR.iADD_CommandStr);
				break;
			case 1:
				SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Points>", CMDSTR.iADD_CommandAgi);
				break;
			case 2:
				SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Points>", CMDSTR.iADD_CommandVit);
				break;
			case 3:
				SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Points>", CMDSTR.iADD_CommandEne);
				break;
			case 4:
				SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Points>", CMDSTR.iADD_CommandLed);
				break;
		}
		return;
	}
	// -----
	if ( Points < 1 || Points > MaxPoints-1  ) return;
	// -----
	if ( lpObj->LevelUpPoint < Points )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[AddPoint]: You don't have [%d] Point(s) to add!", Points);
		return;
	}
	// -----
	if ( CMDSTR.iADD_MaxPointsToAdd > 200 )
	{
		CLog.Error("Max Points to add variable is Set to wrong value ( Max :: 200 ).");
		SendMsg.MessageOut(ChatID, 0x01, "[AddPoint]: Contact Administrator.");
		return;
	}
	// -----
	if ( Points > CMDSTR.iADD_MaxPointsToAdd )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[AddPoint]: You are not able to add %d Points, limit is %d", Points, CMDSTR.iADD_MaxPointsToAdd);
		return;
	}
	// -----
	if ( CMD_STATS_TYPE == 4 )
	{
		if ( lpObj->Class != CLASS_DARKLORD )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[AddPoint]: Only Dark Lord & Lord Emperor can use this Command.");
			return;
		}
	}
	// -----
	if ( lpObj->Money < CMDSTR.iADD_RequiredCost )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[AddPoint]: Need %d Zen To Use This Command.", CMDSTR.iADD_RequiredCost);
		return;
	}
	// -----
	if ( lpObj->Level < CMDSTR.iADD_RequiredLevel )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[AddPoint]: Need Level %d To Use this Command.", CMDSTR.iADD_RequiredLevel);
		return;
	}
	// -----
	switch(CMD_STATS_TYPE)
	{
		case 0:
		{
			POINTS			= lpObj->Strength;
			TotalPoints		= lpObj->Strength + Points;
		} break;
		// -----
		case 1:
		{
			POINTS			= lpObj->Dexterity;
			TotalPoints		= lpObj->Dexterity + Points;
		} break;
		// -----
		case 2:
		{
			POINTS			= lpObj->Vitality;
			TotalPoints		= lpObj->Vitality + Points;
			lpObj->MaxLife	= gCharInfo.sCharInfo[lpObj->Class].Life + (lpObj->Level - 1) * gCharInfo.sCharInfo[lpObj->Class].LevelLife  + 
			((lpObj->Vitality - gCharInfo.sCharInfo[lpObj->Class].Vitality ) * gCharInfo.sCharInfo[lpObj->Class].VitalityToLife);
		} break;
		// -----
		case 3:
		{
			POINTS			= lpObj->Energy;
			TotalPoints		= lpObj->Energy + Points;
			lpObj->MaxMana	= gCharInfo.sCharInfo[lpObj->Class].Mana + (lpObj->Level - 1) * gCharInfo.sCharInfo[lpObj->Class].LevelMana  + 
			((lpObj->Energy - gCharInfo.sCharInfo[lpObj->Class].Energy ) * gCharInfo.sCharInfo[lpObj->Class].EnergyToMana);
		} break;
		// -----
		case 4:
		{
			POINTS			= lpObj->Leadership;
			TotalPoints		= lpObj->Leadership + Points;
		} break;
	}
	// -----
	if ( POINTS > 0 )
	{
		if ( TotalPoints >= MaxPoints )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[AddPoint]: Can`t Add Points (More 32767)");
			return;
		}
	}
	// -----
	InitializeCriticalSection(&CmdCritiSec);
	EnterCriticalSection(&CmdCritiSec);
	// -----
	BYTE lpMsg[5] = {0xC1, 0x05, 0xF3, 0x06, CMD_STATS_TYPE};
	// -----
	for(int i=0; i<Points; i++)
	// -----
	CGLevelUpPointAdd((PMSG_LVPOINTADD*)lpMsg, ChatID);
	// -----
	lpObj->Money = lpObj->Money - CMDSTR.iADD_RequiredCost;
	GCMoneySend(lpObj->m_Index, lpObj->Money);
	// -----
	switch( CMD_STATS_TYPE )
	{
		case 0:
			SendMsg.MessageOut(ChatID, 0x01, "[AddPoint]: Points Successfully Added in Strength!");
			break;
		// -----
		case 1:
			SendMsg.MessageOut(ChatID, 0x01, "[AddPoint]: Points Successfully Added in Agility!");
			break;
		// -----
		case 2:
			SendMsg.MessageOut(ChatID, 0x01, "[AddPoint]: Points Successfully Added in Vitality!");
			break;
		// -----
		case 3:
			SendMsg.MessageOut(ChatID, 0x01, "[AddPoint]: Points Successfully Added in Energy!");
			break;
		// -----
		case 4:
			SendMsg.MessageOut(ChatID, 0x01, "[AddPoint]: Points Successfully Added in LeaderShip!");
			break;
	}
	// -----
	LeaveCriticalSection(&CmdCritiSec);
	DeleteCriticalSection(&CmdCritiSec);
	// -----
	SendMsg.MessageOut(ChatID, 0x01, "[AddPoint]: You have Added [%d] Point(s). Parsed [%d] Zen.",Points, CMDSTR.iADD_RequiredCost);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::DisconnectCommand(DWORD ChatID, char chatmsg[60], int CMD_DCD_TYPE)
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	char Player[16]		= {0};
	char GuildName[9]	= {0};
	int	 GuildIndex;
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( !(lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Disconnect]: Command is Only available For GameMasters.");
		return;
		// -----
	} else {
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanDisconnect = GMSTR[x].Disconnect;
				}
			}

			if ( !CMDSTR.CanDisconnect )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if( !CMDSTR.iDIS_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Disconnect]: Command is currently Inactive!");	
		return;
	}
	// -----
	switch ( CMD_DCD_TYPE )
	{
		case 0:
		{
			if ( cLen < 0 || cLen >= 1)
			{
				SendMsg.MessageOut(ChatID, 0x01, "Bad Syntax: %s", CMDSTR.iDIS_CommandALL);
				return;
			}
			// -----
			for ( int i=OBJMIN; i<OBJMAX; i++ )
			{
				if ( gObj[i].Connected >= PLAYER_CONNECTED )
				{
					gNet.CloseClient(i);
					SendMsg.MessageOut(ChatID, 0x01, "[Disconnect] All Players Disconnected");
				// -----
				} else {
				SendMsg.MessageOut(ChatID, 0x01, "[Disconnect]: We can't find Online Players to Disconnect!");
				}
			}
		} break;
		// -----
		case 1:
		{
			if ( cLen <= 4 || cLen >= 10 )
			{
				SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <GuildName>", CMDSTR.iDIS_CommandGLD);
				return;
			}
			// -----
			sscanf(chatmsg,"%s", GuildName);
			// -----
			_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild(GuildName);
			// -----
			if ( lpGuild != NULL )
			{
				for ( int i=0; i<MAX_USER_GUILD; i++ )
				{
					if ( lpGuild->Index[i] >= 0 )
					{
						GuildIndex = lpGuild->Index[i];

						if ( GuildIndex >= 0 )
						{
							SendMsg.MessageOut(ChatID, 0x01, "[Disconnect] Guild: %s Disconnected", GuildName);
							// -----
							gNet.CloseClient(GuildIndex);
						}
					}
				}
			}
		} break;
		// -----
		case 2:
		{
			if ( cLen <= 4 || cLen >= 11 )
			{
				SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <PlayerName>", CMDSTR.iDIS_CommandCHR);
				return;
			}
			// -----
			sscanf(chatmsg,"%s", Player);
			// -----
			DWORD PlayerID = gObjGetIndex(Player);
			// -----
			if( PlayerID == 0xFFFF ) return;
			// -----
			if(( PlayerID>=OBJMIN ) && ( PlayerID<=OBJMAX ))
			{
				LPOBJ pObj = &gObj[PlayerID];
				// -----
				if(pObj->Name != lpObj->Name)
				{	
					if ( (pObj->AuthorityCode&1) )
					{
						SendMsg.MessageOut(ChatID, 0x01, "[Disconnect]: You can't disconnect GameMaster.");
						return;
					}
					// -----
					gNet.CloseClient(PlayerID);
					// -----
					SendMsg.MessageOut(ChatID, 0x01, "[Disconnect] Player (%s) is Disconnected.", Player);
					SendMsg.MessageOut(PlayerID, 0x01, "[Disconnect] %s disconnected You from the server!", lpObj->Name);
				// -----
				} else {
					return;
				}
			// -----
			} else {
				SendMsg.MessageOut(ChatID, 0x01, "[Disconnect] Player: %s is offline", Player);
			}
		} break;
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::SetZenCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	int SetZen		= 0;
	char Player[16]	= {0};
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( !(lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[SetZen]: Command is Only available For GameMasters.");
		return;
		// -----
	} else {
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanSetZen = GMSTR[x].SetZen;
				}
			}

			if ( !CMDSTR.CanSetZen )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if ( !CMDSTR.iSZN_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[SetZen]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 6 || cLen >= 23 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Name> <Zen>", CMDSTR.iSZN_Command);
		return;
	}
	// -----
	sscanf(chatmsg,"%s %ld", Player, &SetZen);
	// -----
	if( SetZen > MAX_ZEN )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[SetZen]: Maximum Zen is [%d] Zen.", MAX_ZEN);
		return;
	}
	// -----
	DWORD PlayerID = gObjGetIndex(Player);
	// -----
	if( PlayerID == 0xFFFF ) return;
	// -----
	if(( PlayerID>=OBJMIN ) && ( PlayerID<=OBJMAX ))
	{
		LPOBJ pObj = &gObj[PlayerID];
		// -----
		if ( (pObj->Name != lpObj->Name) && (pObj->AuthorityCode&1) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[SetZen]: You can't Set Zen to GameMaster.");
			return;
		}
		// -----
		pObj->Money = SetZen;
		// -----
		GCMoneySend(PlayerID, SetZen);
		// -----
		if(pObj->Name != lpObj->Name)
		{
			SendMsg.MessageOut(ChatID, 0x01, "[SetZen] Player: %s Has [%ld] zen now!", Player, SetZen);
			SendMsg.MessageOut(PlayerID, 0x01, "[SetZen] %s Set Your Zen To: [%d].", lpObj->Name, SetZen);
		} else {
			SendMsg.MessageOut(ChatID, 0x01, "[SetZen] You Set [%ld] Zen now.", lpObj->Name, SetZen);
		}
	// -----
	} else {
		SendMsg.MessageOut(ChatID, 0x01, "[SetZen] Player: %s is offline", Player);
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::AddZenCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	int AddZen;
	char Player[16]	= {0};
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( !(lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[AddZen]: Command is Only available For GameMasters.");
		return;
		// -----
	} else {
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanAddZen = GMSTR[x].AddZen;
				}
			}

			if ( !CMDSTR.CanAddZen )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if ( !CMDSTR.iAZN_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[AddZen]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 6 || cLen >= 23 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Name> <Zen>", CMDSTR.iAZN_Command);
		return;
	}
	// -----
	sscanf(chatmsg,"%s %ld",Player, &AddZen);
	// -----
	DWORD PlayerID = gObjGetIndex(Player);
	// -----
	if( PlayerID == 0xFFFF ) return;
	// -----
	if(( PlayerID>=OBJMIN ) && ( PlayerID<=OBJMAX ))
	{
		LPOBJ pObj = &gObj[PlayerID];
		// -----
		int TotalOwnZen = AddZen + pObj->Money;
		// -----
		if ( TotalOwnZen > MAX_ZEN ) 
		{
			SendMsg.MessageOut(ChatID, 0x01, "[AddZen] Maximum Zen is [%d] Zen.", MAX_ZEN);
			return;
		}
		// -----
		if ( (pObj->Name != lpObj->Name) && (pObj->AuthorityCode&1) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[AddZen]: You can't Send Zen to GameMaster.");
			return;
		}
		// -----
		pObj->Money = pObj->Money + AddZen;
		// -----
		GCMoneySend(PlayerID, pObj->Money);
		// -----
		if(pObj->Name != lpObj->Name)
		{
			SendMsg.MessageOut(ChatID, 0x01, "[AddZen] Player: %s Recieved From You [%ld] zen now!", Player, AddZen);
			SendMsg.MessageOut(PlayerID, 0x01, "[AddZen] %s Sent To You [%d] zen.", lpObj->Name, AddZen);
		} else {
			SendMsg.MessageOut(ChatID, 0x01, "[AddZen] You Added [%ld] Zen now.", lpObj->Name, AddZen);
		}
	// -----
	} else {
		SendMsg.MessageOut(ChatID, 0x01, "[AddZen] Player: %s is offline", Player);
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::SkinCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	int	 SkinMob	= -1;
	char Player[16]	= {0};
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( !(lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Skin]: Command is Only available For GameMasters.");
		return;
		// -----
	} else {
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanSkin = GMSTR[x].Skin;
				}
			}

			if ( !CMDSTR.CanSkin )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if( !CMDSTR.iSKN_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Skin]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 8 || cLen >= 16 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Name> <MonsterID>", CMDSTR.iSKN_Command);
		return;
	}
	// -----
	sscanf(chatmsg,"%s %d",Player,&SkinMob);
	// -----
	if ( SkinMob >= MAX_MONSTER_TYPE )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Skin]: You Entered Wrong Monster ID, Recheck It Again.", CMDSTR.iSKN_Command);
	}
	// -----
	DWORD PlayerID = gObjGetIndex(Player);
	// -----
	if( PlayerID == 0xFFFF ) return;
	// -----
	if(( PlayerID>=OBJMIN ) && ( PlayerID<=OBJMAX ))
	{
		LPOBJ pObj = &gObj[PlayerID];
		// -----
		if ( (pObj->Name != lpObj->Name) && (pObj->AuthorityCode&1) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[Skin]: You can't Skin GameMaster.");
			return;
		}
		// -----
		pObj->m_Change = SkinMob;
		// -----
		gObjViewportListProtocolCreate(pObj);
		// -----
		if(pObj->Name != lpObj->Name)
		{
			if ( SkinMob == -1 )
			{
				SendMsg.MessageOut(ChatID, 0x01, "[Skin] Player: %s Skinned to Default View.", Player, SkinMob);
				SendMsg.MessageOut(PlayerID, 0x01, "[Skin] %s Skinned You to Default View.", lpObj->Name, SkinMob);
			}
			else
			{
				SendMsg.MessageOut(ChatID, 0x01, "[Skin] Player: %s Skinned to Monster ID: [%d].", Player, SkinMob);
				SendMsg.MessageOut(PlayerID, 0x01, "[Skin] %s Skinned You to Monster ID: [%d].", lpObj->Name, SkinMob);
			}
		} else {
			SendMsg.MessageOut(ChatID, 0x01, "[Skin] You Skinned Yourself to MonsterID: [%d].", lpObj->Name, SkinMob);
		}
	// -----
	} else {
		SendMsg.MessageOut(ChatID, 0x01, "[Skin] Player: %s is offline", Player);
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::SpawnMobCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	int MapID, MobID, CoordX, CoordY;
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( !(lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[SpawnMob]: Command is Only available For GameMasters.");
		return;
		// -----
	} else {
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanSpawnMobs = GMSTR[x].SpawnMob;
				}
			}

			if ( !CMDSTR.CanSpawnMobs )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if( !CMDSTR.iSMB_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[SpawnMob]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 7 || cLen >= 16 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <MobID> <MapID> <CordX> <CordY>", CMDSTR.iSMB_Command);
		return;
	}
	// -----
	sscanf(chatmsg,"%d %d %d %d", &MobID, &MapID, &CoordX, &CoordY);
	// -----
	int mIndex = gObjAddMonster(MapID);
	// -----
	if( mIndex >= 0 )
	{
		LPOBJ pObj = &gObj[mIndex];
		// -----
		LPMONSTER_ATTRIBUTE MonsterAttr = gMAttr.GetAttr(MobID);
		// -----
		if ( MonsterAttr == NULL )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[SpawnMob]: You have entered wrong Monster ID!");
			return;
		}
		// -----
		gObjSetMonster(mIndex, MobID);
		// -----
		pObj->Class		= MobID;
		pObj->MapNumber = MapID;
		pObj->X			= CoordX;
		pObj->Y			= CoordY;
		// -----
		SendMsg.MessageOut(ChatID, 0x01, "[SpawnMob] Monster [%d] has been created in [%s]-[%d][%d]", MobID, GetMapName(MapID), CoordX, CoordY);
		// -----
		return;
	// -----
	} else {
		SendMsg.MessageOut(ChatID, 0x01, "[SpawnMob]: Error in Creating Mob");
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::StatusCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	char Player[16]	= {0};
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( !(lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Status]: Command is Only available For GameMasters.");
		return;
		// -----
	} else {
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanStatus = GMSTR[x].Status;
				}
			}

			if ( !CMDSTR.CanStatus )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if( !CMDSTR.iSTS_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Status]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 4 || cLen >= 12 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Name>", CMDSTR.iSTS_Command);
		return;
	}
	// -----
	sscanf(chatmsg,"%s", Player);
	// -----
	DWORD PlayerID = gObjGetIndex(Player);
	// -----
	LPOBJ pObj = &gObj[PlayerID];
	// -----
	if(( PlayerID >= OBJMIN ) && ( PlayerID <= OBJMAX ))
	{
		if ( (pObj->AuthorityCode&1) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[Status] Player is GameMaster");
			SendMsg.MessageOut(ChatID, 0x01, "[Status] You can't Show This Character's details.");
			return;
		}
		// -----
		SendMsg.MessageOut(ChatID, 0x01, "[Status] LoginID: %s || Character: %s",pObj->AccountID, pObj->Name);
		SendMsg.MessageOut(ChatID, 0x01, "[Status] IP Address: %s", pObj->Ip_addr);
		SendMsg.MessageOut(ChatID, 0x01, "[Status] Level: %d, Class: %d, Zen: %ld",pObj->Level,pObj->DbClass,pObj->Money);
		SendMsg.MessageOut(ChatID, 0x01, "[Status] PK Level: %d, PK Count: %d",pObj->m_PK_Level,pObj->m_PK_Count);
		SendMsg.MessageOut(ChatID, 0x01, "[Status] Str: %d, Agi: %d, Vit: %d, Ene: %d",pObj->Strength,pObj->Dexterity,pObj->Vitality,pObj->Energy);
		if (pObj->Class == 4) SendMsg.MessageOut(ChatID, 0x01, "[Status] Cmd: %d",pObj->Leadership);
		SendMsg.MessageOut(ChatID, 0x01, "[Status] Map: %s, PosX: %d, PosY: %d",GetMapName(pObj->MapNumber),pObj->X,pObj->Y);
	// -----
	} else {
		switch ( (pObj->AuthorityCode&1) )
		{
		case 0:
			SendMsg.MessageOut(ChatID, 0x01, "[Status] Player: %s is offline",Player);
			break;
		case 1:
			SendMsg.MessageOut(ChatID, 0x01, "[Status] GameMaster: %s is offline",Player);
			break;
		}
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::ClearInvCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	int ClrInvCode = 0;
	int CertainZen = 0;
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( !CMDSTR.iINV_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[ClearInv]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 1 || cLen >= 3 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <ClrInvCode>", CMDSTR.iINV_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 0: Clear Wear Part Of Inventory A.", CMDSTR.iINV_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 1: Clear Lower Part Of Inventory A.", CMDSTR.iINV_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 2: Clear Upper Part Of Inventory B.", CMDSTR.iINV_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 3: Clear Lower Part Of Inventory B.", CMDSTR.iINV_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 4: Clear All Inventory.", CMDSTR.iINV_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 5: Clear Personal Shop.", CMDSTR.iINV_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 6: Clear Zen.", CMDSTR.iINV_Command);
		return;
	}
	// -----
	sscanf(chatmsg,"%d",&ClrInvCode);
	// -----
	if ( ClrInvCode < 0 && ClrInvCode >= 7 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[ClearInv]: Clear Inventory Code [0-To-6].");
		return;
	}
	// -----
	switch(ClrInvCode)
	{
		case 0:
		{
			for (int i=0; i<12; i++)
			{
				if ( lpObj->pInventory[i].IsItem() == TRUE )
				{
					gObjInventoryDeleteItem(ChatID, i);
					GCInventoryItemDeleteSend(ChatID, i, 1);
					// -----
					SendMsg.MessageOut(ChatID, 0x01, "[ClearInv]: You have cleared your Wear Part in inventory A.");
				}
			}
			// -----
		} break;
		// -----
		case 1:
		{
			for (int i=12; i<76; i++)
			{
				if ( lpObj->pInventory[i].IsItem() == TRUE )
				{
					gObjInventoryDeleteItem(ChatID, i);
					GCInventoryItemDeleteSend(ChatID, i, 1);
					// -----
					SendMsg.MessageOut(ChatID, 0x01, "[ClearInv]: You have cleared your Lower Part Of Inventory A.");
				}
			}
		} break;
		// -----
		case 2:
		{
			for (int i=76; i<108; i++)
			{
				if ( lpObj->pInventory[i].IsItem() == TRUE )
				{
					gObjInventoryDeleteItem(ChatID, i);
					GCInventoryItemDeleteSend(ChatID, i, 1);
					// -----
					SendMsg.MessageOut(ChatID, 0x01, "[ClearInv]: You have cleared your Upper Of Inventory B.");
				}
			}
			// -----
		} break;
		// -----
		case 3:
		{
			for (int i=108; i<139; i++)
			{
				if ( lpObj->pInventory[i].IsItem() == TRUE )
				{
					gObjInventoryDeleteItem(ChatID, i);
					GCInventoryItemDeleteSend(ChatID, i, 1);
					// -----
					SendMsg.MessageOut(ChatID, 0x01, "[ClearInv]: You have cleared your Lower Of Inventory B.");
				}
			}
			// -----
		} break;
		// -----
		case 4:
		{
			for (int i=0; i<139; i++)
			{
				if ( lpObj->pInventory[i].IsItem() == TRUE )
				{
					gObjInventoryDeleteItem(ChatID, i);
					GCInventoryItemDeleteSend(ChatID, i, 1);
					// -----
					SendMsg.MessageOut(ChatID, 0x01, "[ClearInv]: You have cleared your All Inventory.");
				}
			}
			// -----
		} break;
		// -----
		case 5:
		{
			for (int i=204; i<235; i++)
			{
				if ( lpObj->pInventory[i].IsItem() == TRUE )
				{
					gObjInventoryDeleteItem(ChatID, i);
					GCInventoryItemDeleteSend(ChatID, i, 1);
					// -----
					SendMsg.MessageOut(ChatID, 0x01, "[ClearInv]: You have cleared your Personal Shop.");
				}
			}
			// -----
		} break;
		// -----
		case 6:
		{
			if ( lpObj->Money <= 0 )
			{
				SendMsg.MessageOut(ChatID, 0x01, "[ClearInv]: You don't have zen to clear.");
			}
			else
			{
				GCMoneySend(ChatID, CertainZen);
				// -----
				SendMsg.MessageOut(ChatID, 0x01, "[ClearInv]: You have cleared your zen.");
			}
		} break;
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::ExitCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	if ( cLen < 0 || cLen >= 1)
	{
		SendMsg.MessageOut(ChatID, 0x01, "Bad Syntax: %s", CMDSTR.iEXT_Command);
		return;
	}
	// -----
	if( !CMDSTR.iEXT_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[ExitGame]: Command is currently Inactive!");	
		return;
	}
	// -----
	Sleep(3000);
	// -----
	SendMsg.MessageOut(ChatID, 0x01, "[ExitGame] You are Logged Out from %s MU Server.", szServerName);
	// -----
	gNet.CloseClient(ChatID);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::AboutCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	if ( cLen < 0 || cLen >= 1)
	{
		SendMsg.MessageOut(ChatID, 0x01, "Bad Syntax: %s", CMDSTR.iABT_Command);
		return;
	}
	// -----
	if( !CMDSTR.iABT_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[About]: Command is currently Inactive!");	
		return;
	}
	// -----
	SendMsg.MessageOut(ChatID, 0x01, "[About] Welcome to %s MU Online", szServerName);
	// -----
	Sleep(1000);
	// -----
	SendMsg.MessageOut(ChatID, 0x01, "[About] Full Season 6 :: EpisodE 2");
	// -----
	Sleep(1000);
	// -----
	SendMsg.MessageOut(ChatID, 0x01, "[About] Powered By: RealCoderZ MU DevelopmenT!");
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::SetLevelCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	int SetLevel	= 0;
	char Player[16]	= {0};
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( !(lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[SetLevel]: Command is Only available For GameMasters.");
		return;
		// -----
	} else {
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanSetLevel = GMSTR[x].SetLevel;
				}
			}

			if ( !CMDSTR.CanSetLevel )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if( !CMDSTR.iSLV_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[SetLevel]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 6 || cLen >= 16 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Name> <Level>", CMDSTR.iSLV_Command);
		return;
	}
	// -----
	sscanf(chatmsg,"%s %d", Player, &SetLevel);
	// -----
	if( SetLevel < 1 || SetLevel > HRB_CharMaxLevel)
	{
		SendMsg.MessageOut(ChatID, 0x01, "[SetLevel]: Maximum Level [%d].", HRB_CharMaxLevel);
		return;
	}
	// -----
	DWORD PlayerID = gObjGetIndex(Player);
	// -----
	if( PlayerID == 0xFFFF ) return;
	// -----
	if(( PlayerID>=OBJMIN ) && ( PlayerID<=OBJMAX ))
	{
		LPOBJ pObj = &gObj[PlayerID];
		// -----
		if ( (pObj->Name != lpObj->Name) && (pObj->AuthorityCode&1) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[SetLevel]: You can't Set Level Of GameMaster.");
			return;
		}
		// -----
		if ( gUser.gObjIsMasteringLevel(pObj) == TRUE )
		{
			if ( SetLevel >= 1 && SetLevel <= 200 )
			{
				pObj->MasterLevel = SetLevel;
				// -----
				gSkillTree.GCMasterLevelUpSend(pObj, 1);
			}
		} 
		else 
		{
			pObj->Level = SetLevel;
			// -----
			GCLevelUpMsgSend(pObj->m_Index, pObj->Level);
		}
		// -----
		gCalCharacter.gObjCalCharacter(PlayerID);
		// -----
		if(pObj->Name != lpObj->Name)
		{
			if ( gUser.gObjIsMasteringLevel(pObj) == TRUE )
			{
				if ( SetLevel >= 1 && SetLevel <= 200 )
				{
					SendMsg.MessageOut(ChatID, 0x01, "[SetLevel] Master Player: %s is Level [%d]!", Player, SetLevel);
					SendMsg.MessageOut(PlayerID, 0x01, "[SetLevel] %s Set Your Level To: [%d].", lpObj->Name, SetLevel);
				}
			}
			else
			{
				SendMsg.MessageOut(ChatID, 0x01, "[SetLevel] Player: %s is Level [%d]!", Player, SetLevel);
				SendMsg.MessageOut(PlayerID, 0x01, "[SetLevel] %s Set Your Level To: [%d].", lpObj->Name, SetLevel);
			}
		} else {
			if ( gUser.gObjIsMasteringLevel(pObj) == TRUE )
			{
				if ( SetLevel >= 1 && SetLevel <= 200 )
				{
					SendMsg.MessageOut(ChatID, 0x01, "[SetLevel] You Set Your Level To: [%d].", SetLevel);
				}
			}
			else
			{
				SendMsg.MessageOut(ChatID, 0x01, "[SetLevel] You Set Your Level To: [%d].", SetLevel);
			}
		}
	// -----
	} else {
		SendMsg.MessageOut(ChatID, 0x01, "[SetLevel] Player: %s is offline",Player);
		return;
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::SetLvlUpPCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	int SetLuUp		= 0;
	char Player[16]	= {0};
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( !(lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[SetLvlUpPoints]: Command is Only available For GameMasters.");
		return;
		// -----
	} else {
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanSetLvlUpPoints = GMSTR[x].SetLvlUpPoints;
				}
			}

			if ( !CMDSTR.CanSetLvlUpPoints )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if ( !CMDSTR.iLUP_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[SetLvlUpPoints]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 6 || cLen >= 17 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Name> <LvlUpPoints>", CMDSTR.iLUP_Command);
		return;
	}
	// -----
	sscanf(chatmsg,"%s %d", Player, &SetLuUp);
	// -----
	if ( SetLuUp < 1 || SetLuUp > 9999 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[SetLvlUpPoints]: Maximum [9999].");
		return;
	}
	// -----
	DWORD PlayerID = gObjGetIndex(Player);
	// -----
	if( PlayerID == 0xFFFF ) return;
	// -----
	if(( PlayerID>=OBJMIN ) && ( PlayerID<=OBJMAX ))
	{
		LPOBJ pObj = &gObj[PlayerID];
		// -----
		if ( (pObj->Name != lpObj->Name) && (pObj->AuthorityCode&1) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[SetLvlUpPoints]: You can't Send Points to GameMaster.");
			return;
		}
		// -----
		if ( gUser.gObjIsMasteringLevel(pObj) == FALSE )
		{
			pObj->LevelUpPoint = SetLuUp;
			// -----
			GCLevelUpMsgSend(pObj->m_Index, pObj->LevelUpPoint);
		} else {
			pObj->MasterPoints = SetLuUp;
			// -----
			gSkillTree.GCMasterLevelUpSend(pObj, 1);
		}
		// -----
		if(pObj->Name != lpObj->Name)
		{
			SendMsg.MessageOut(ChatID, 0x01, "[SetLvlUpPoints] Player: %s has [%d] LvlUpPoints now!", Player, SetLuUp);
			SendMsg.MessageOut(PlayerID, 0x01, "[SetLvlUpPoints] %s Set Your LvlUpPoints To: [%d].", lpObj->Name, SetLuUp);
		} else {
			SendMsg.MessageOut(ChatID, 0x01, "[SetLvlUpPoints] You Set Your LvlUpPoints To: [%d].", SetLuUp);
		}
	// -----
	} else {
		SendMsg.MessageOut(ChatID, 0x01, "[SetLvlUpPoints] Player: %s is offline", Player);
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::AddBuffCommand(DWORD ChatID, char chatmsg[60])
{
	/*LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	int BuffCode	= 0;
	char Player[16]	= {0};
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( !(lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: Command is Only available For GameMasters.");
		return;
		// -----
	} else {
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanAddBuff = GMSTR[x].AddBuff;
				}
			}

			if ( !CMDSTR.CanAddBuff )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if( !CMDSTR.iABF_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 6 || cLen >= 14 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Name> <BuffCode>",CMDSTR.iABF_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 0: SoulBarrier Buff.",CMDSTR.iABF_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 1: Health Buff.",CMDSTR.iABF_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 2: Great Fortitude Buff.",CMDSTR.iABF_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 3: Elf Aura Defense Buff.",CMDSTR.iABF_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 4: Elf Aura Attack Buff.",CMDSTR.iABF_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 5: ElfSolider Buff.",CMDSTR.iABF_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 6: Defense Success Rate.",CMDSTR.iABF_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 7: Fitness",CMDSTR.iABF_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 8: Red Armore Ignore.",CMDSTR.iABF_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 9: Magic Circle Buff.",CMDSTR.iABF_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 10: Recovery Buff.",CMDSTR.iABF_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 11: Reflex Buff.",CMDSTR.iABF_Command);
		return;
	}
	// -----
	sscanf(chatmsg,"%s %d", Player, &BuffCode);
	// -----
	if( BuffCode < 0 || BuffCode > 11 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: Buff Code [0-To-11].");
		return;
	}
	// -----
	DWORD PlayerID = gObjGetIndex(Player);
	// -----
	if( PlayerID == 0xFFFF ) return;
	// -----
	if(( PlayerID>=OBJMIN ) && ( PlayerID<=OBJMAX ))
	{
		LPOBJ pObj = &gObj[PlayerID];
		// -----
		if ( (pObj->Name != lpObj->Name) && (pObj->AuthorityCode&1) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: You can't Add Buffs to GameMaster.");
			return;
		}
		// -----
		switch(BuffCode)
		{
			case 0:
			{
				gObjUseSkill.SkillSoulBarrier(PlayerID,PlayerID,DefMagicInf->m_Level);
				// -----
				if(pObj->Name != lpObj->Name)
				{
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: Soul Barrier Buff Added to %s",Player);
					SendMsg.MessageOut(PlayerID, 0x01, "[AddBuff]: %s added to you Soul Barrier Buff.",lpObj->Name);
				} else {
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: You added to Yourself Soul Barrier Buff.");
				}
			} break;
			// -----
			case 1:
			{
				gObjUseSkill.SkillHealing(PlayerID,PlayerID,DefMagicInf->m_Level);
				// -----
				if(pObj->Name != lpObj->Name)
				{
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: Health Buff Added to %s",Player);
					SendMsg.MessageOut(PlayerID, 0x01, "[AddBuff]: %s added to you Health Buff.",lpObj->Name);
				} else {
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: You added to Yourself Health Buff.");
				}
			} break;
			// -----
			case 2:
			{
				gObjUseSkill.KnightSkillAddLife(PlayerID,DefMagicInf->m_Level);
				// -----
				if(pObj->Name != lpObj->Name)
				{
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: Great Fortitude Buff Added to %s",Player);
					SendMsg.MessageOut(PlayerID, 0x01, "[AddBuff]: %s added to you Great Fortitude Buff.",lpObj->Name);
				} else {
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: You added to Yourself Great Fortitude Buff.");
				}
			} break;
			// -----
			case 3:
			{
				gObjUseSkill.SkillDefense(PlayerID,PlayerID,DefMagicInf->m_Level);
				// -----
				if(pObj->Name != lpObj->Name)
				{
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: Defense Aura Buff Added to %s",Player);
					SendMsg.MessageOut(PlayerID, 0x01, "[AddBuff]: %s added to you Defense Aura Buff.",lpObj->Name);
				} else {
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: You added to Yourself Defense Aura Buff..");
				}
			} break;
			// -----
			case 4:
			{
				gObjUseSkill.SkillAttack(PlayerID,PlayerID,DefMagicInf->m_Level);
				// -----
				if(pObj->Name != lpObj->Name)
				{
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: Attack Aura Buff Added to %s",Player);
					SendMsg.MessageOut(PlayerID, 0x01, "[AddBuff]: %s added to you Attack Aura Buff.",lpObj->Name);
				} else {
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: You added to Yourself Attack Aura Buff..");
				}
			} break;
			// -----
			case 5:
			{
				pObj->m_iSkillNPCHelpTime = g_SkillAdditionInfo.GetElfSoldierBuffTime();
				pObj->m_iSkillNPCDefense = lpObj->Level / g_SkillAdditionInfo.GetElfSoldierBuffDefLvlDiv() + g_SkillAdditionInfo.GetElfSoldierBuffDefLvlAdd();	// FORMULA
				pObj->m_iSkillNPCAttack = lpObj->Level / g_SkillAdditionInfo.GetElfSoldierBuffDmgLvlDiv() + g_SkillAdditionInfo.GetElfSoldierBuffDmgLvlAdd();	// FORMULA
				// -----
				GCStateInfoSend(pObj, 1, eVS_NPC_HELP);
				// -----
				if(pObj->Name != lpObj->Name)
				{
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: ElfSolider Buff Added to %s",Player);
					SendMsg.MessageOut(PlayerID, 0x01, "[AddBuff]: %s added to you ElfSolider Buff.",lpObj->Name);
				} else {
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: You added to Yourself ElfSolider Buff..");
				}
			} break;
			// -----
			case 6:
			{
				gObjUseSkill.SkillDefenseSuccessRate(PlayerID, 0);
				// -----
				if(pObj->Name != lpObj->Name)
				{
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: Defense Success Rate Buff Added to %s",Player);
					SendMsg.MessageOut(PlayerID, 0x01, "[AddBuff]: %s added to you Defense Success Rate Buff.",lpObj->Name);
				} else {
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: You added to Yourself Defense Success Rate..");
				}
			} break;
			// -----
			case 7:
			{
				gObjUseSkill.SkillFitness(PlayerID, 0);
				// -----
				if(pObj->Name != lpObj->Name)
				{
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: Fitness Buff Added to %s",Player);
					SendMsg.MessageOut(PlayerID, 0x01, "[AddBuff]: %s added to you Fitness Buff.",lpObj->Name);
				} else {
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: You added to Yourself Fitness Buff..");
				}
			} break;
			// -----
			case 8:
			{
				gObjUseSkill.SkillRedArmorIgnore(PlayerID, 0);
				// -----
				if(pObj->Name != lpObj->Name)
				{
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: Red Armor Ignore Buff Added to %s",Player);
					SendMsg.MessageOut(PlayerID, 0x01, "[AddBuff]: %s added to you Red Armor Ignore Buff.",lpObj->Name);
				} else {
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: You added to Yourself Red Armor Ignore Buff..");
				}
			} break;
			// -----
			CMagicInf * lpMagic;
			case 9:
			{
				gObjUseSkill.SkillMagicCircle(PlayerID,lpMagic);
				// -----
				if(pObj->Name != lpObj->Name)
				{
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: Magic Circle Buff Added to %s",Player);
					SendMsg.MessageOut(PlayerID, 0x01, "[AddBuff]: %s added to you Magic Circle Buff.",lpObj->Name);
				} else {
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: You added to Yourself Magic Circle Buff..");
				}
			} break;
			// -----
			case 10:
			{
				gObjUseSkill.SkillRecovery(PlayerID,PlayerID,lpMagic);
				// -----
				if(pObj->Name != lpObj->Name)
				{
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: Recovery Buff Added to %s",Player);
					SendMsg.MessageOut(PlayerID, 0x01, "[AddBuff]: %s added to you Recovery Buff.",lpObj->Name);
				} else {
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: You added to Yourself Recovery Buff..");
				}
			} break;
			// -----
			case 11:
			{
				gObjUseSkill.SkillReflex(PlayerID,DefMagicInf->m_Level);
				// -----
				if(pObj->Name != lpObj->Name)
				{
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: Reflex Buff Added to %s",Player);
					SendMsg.MessageOut(PlayerID, 0x01, "[AddBuff]: %s added to you Reflex Buff.",lpObj->Name);
				} else {
					SendMsg.MessageOut(ChatID, 0x01, "[AddBuff]: You added to Yourself Reflex Buff..");
				}
			} break;
			// -----
		}
	// -----
	} else {
		SendMsg.MessageOut(ChatID, 0x01, "[AddBuff] Player: %s is offline", Player);
	}*/
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::SummonCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	BYTE FinalPosX = lpObj->X+1;
	BYTE FinalPosY = lpObj->Y+1;
	// -----
	if ( cLen < 0 || cLen >= 1)
	{
		SendMsg.MessageOut(ChatID, 0x01, "Bad Syntax: %s", CMDSTR.iSMN_Command);
		return;
	}
	// -----
	if ( (lpObj->AuthorityCode&1) )
	{
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanSummon = GMSTR[x].Summon;
				}
			}

			if ( !CMDSTR.CanSummon )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if ( !CMDSTR.iSMN_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Summon]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( lpObj->Level < CMDSTR.iSMN_RequiredLevel )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Summon]: Need Level %d To Use This Command.", CMDSTR.iSMN_RequiredLevel);
		return;
	}
	// -----
	if ( CMDSTR.iSMN_EnalbeCost )
	{
		if ( lpObj->Money < CMDSTR.iSMN_RequiredCost )
		{
			if ( CMDSTR.iSMN_RequiredCost > 0 )
			{
				SendMsg.MessageOut(ChatID, 0x01, "[Summon]: Need %d Zen To Use This Command.", CMDSTR.iSMN_RequiredCost);
			}
			return;
		}
		// -----
		lpObj->Money = lpObj->Money - CMDSTR.iSMN_RequiredCost;
		// -----
		GCMoneySend(lpObj->m_Index, lpObj->Money);
	}
	// -----
	SummonMonster(ChatID);
	// -----
	SendMsg.MessageOut(ChatID, 0x01, "[Summon]: Monster has been summoned!");
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::CrashCommand(DWORD ChatID, char chatmsg[60])
{
	int	cLen = strlen(chatmsg);
	// -----
	if ( cLen < 0 || cLen >= 1) return;
	// -----
	SendMsg.MessageOutAll(OBJ_SEND_ALL_GOLDEN, "%s MU Forced to be Closed.", szServerName);
	// -----
	Sleep(60000); // 1 Minute
	// -----
	__asm
	{
		MOV EAX, 00000000;
		CALL EAX;
	}
	// -----
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::ProposeMarryCommand(DWORD ChatID, char chatmsg[60])
{
	/*LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	char PlayerToMarry[16]	= {0};
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( (lpObj->AuthorityCode&1) )
	{
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanMarriage = GMSTR[x].Marriage;
				}
			}

			if ( !CMDSTR.CanMarriage )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if ( !MarryCFG.Enabled )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Marriage]: System is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 9 || cLen >= 11 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Name>", CMDSTR.iMRG_CommandPro);
		return;
	}
	// -----
	sscanf(chatmsg,"%s", PlayerToMarry);
	// -----
	int uIndex = gObjGetIndex(PlayerToMarry);
	// -----
	if( uIndex == 0xFFFF ) return;
	// -----
	if ( uIndex >= OBJMIN && uIndex <= OBJMAX )
	{
		cMarry.Propose(ChatID, uIndex);
	// -----
	} else {
		SendMsg.MessageOut(ChatID, 0x01, "[Marriage] Player: %s is offline", PlayerToMarry);
	}*/
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::AcceptMarryCommand(DWORD ChatID, char chatmsg[60])
{
	/*LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	if ( cLen < 0 || cLen >= 1)
	{
		SendMsg.MessageOut(ChatID, 0x01, "Bad Syntax: %s", CMDSTR.iMRG_CommandAcp);
		return;
	}
	// -----
	if ( (lpObj->AuthorityCode&1) )
	{
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanMarriage = GMSTR[x].Marriage;
				}
			}

			if ( !CMDSTR.CanMarriage )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if ( !MarryCFG.Enabled )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Marriage]: System is currently Inactive!");	
		return;
	}
	// -----
	if(cMarry.Accept(ChatID) == TRUE)
	{
		int uIndex = gObjGetIndex(MRYSTR[ChatID-OBJMIN].MarryName);
		// -----
		//cMuOnlineDB.SaveMarryInfo(MRYSTR[ChatID-OBJMIN].MarryName, MRYSTR[uIndex-OBJECT_MIN].MarryName);
	}*/
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::DivorceMarryCommand(DWORD ChatID, char chatmsg[60])
{
	/*LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	if ( cLen < 0 || cLen >= 1)
	{
		SendMsg.MessageOut(ChatID, 0x01, "Bad Syntax: %s", CMDSTR.iMRG_CommandDiv);
		return;
	}
	// -----
	if ( (lpObj->AuthorityCode&1) )
	{
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanMarriage = GMSTR[x].Marriage;
				}
			}

			if ( !CMDSTR.CanMarriage )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if ( !MarryCFG.Enabled )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Marriage]: System is currently Inactive!");	
		return;
	}
	// -----
	cMarry.Divorce(ChatID);*/
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::OnlineMarryCommand(DWORD ChatID, char chatmsg[60])
{
	/*LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	if ( cLen < 0 || cLen >= 1)
	{
		SendMsg.MessageOut(ChatID, 0x01, "Bad Syntax: %s", CMDSTR.iMRG_CommandOnl);
		return;
	}
	// -----
	if ( (lpObj->AuthorityCode&1) )
	{
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanMarriage = GMSTR[x].Marriage;
				}
			}

			if ( !CMDSTR.CanMarriage )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if ( !MarryCFG.Enabled )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Marriage]: System is currently Inactive!");	
		return;
	}
	// -----
	if ( !MRYSTR[ChatID-OBJMIN].IsMarried )
	{
		SendMsg.MessageOut(ChatID, 0x01, "You are not married!");
		return;
	}
	// -----
	int uIndex = gObjGetIndex(MRYSTR[ChatID-OBJMIN].MarryName);
	// -----
	if ( uIndex == -1 )
	{
		if ( IsFemale(ChatID) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "Your Wife is Offline");
			return;
		}
		if ( !IsFemale(ChatID) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "Your Husband is Offline");
			return;
		}
	// -----
	} else {
		if ( IsFemale(ChatID) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "Your Wife is Online");
			return;
		}
		if ( !IsFemale(ChatID) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "Your Husband is Online");
			return;
		}
	}*/
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::TraceMarryCommand(DWORD ChatID, char chatmsg[60])
{
	/*LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	if ( cLen < 0 || cLen >= 1)
	{
		SendMsg.MessageOut(ChatID, 0x01, "Bad Syntax: %s", CMDSTR.iMRG_CommandOnl);
		return;
	}
	// -----
	if ( (lpObj->AuthorityCode&1) )
	{
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanMarriage = GMSTR[x].Marriage;
				}
			}

			if ( !CMDSTR.CanMarriage )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if ( !MarryCFG.Enabled )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Marriage]: System is currently Inactive!");	
		return;
	}
	// -----
	if ( !MRYSTR[ChatID-OBJMIN].IsMarried )
	{
		SendMsg.MessageOut(ChatID, 0x01, "You are not married!");
		return;
	}
	// -----
	int uIndex = gObjGetIndex(MRYSTR[ChatID-OBJMIN].MarryName);
	// -----
	LPOBJ TargetlpObj = &gObj[uIndex];
	// -----
	if ( uIndex == -1 )
	{
		if ( IsFemale(ChatID) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "Your Wife is Offline");
			return;
		}
		if ( !IsFemale(ChatID) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "Your Husband is Offline");
			return;
		}
	}
	// -----
	if( TargetlpObj->Level < CMDSTR.iMRG_RequiredLevel || lpObj->Level < CMDSTR.iMRG_RequiredLevel )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Command Available for Characters with level %d or Higher.", CMDSTR.iMRG_RequiredLevel);
		return;
	}
	// -----
	if (( gObj[uIndex].MapNumber >= 11 && gObj[uIndex].MapNumber <= 17) || gObj[uIndex].MapNumber == 52  || 
	(gObj[uIndex].MapNumber >= 18 && gObj[uIndex].MapNumber <= 23) || gObj[uIndex].MapNumber == 53 ||  
	gObj[uIndex].MapNumber == 9 || gObj[uIndex].MapNumber == 32 || (gObj[uIndex].MapNumber >= 45 && gObj[uIndex].MapNumber <= 50))
	{
		if ( IsFemale(ChatID) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "Your Wife is inside Event Map, You can't Teleport Now!");
			return;
		}
		// -----
		if ( !IsFemale(ChatID) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "Your Husband is inside Event Map, You can't Teleport Now!");
			return;
		}
	}
	// -----
	gMove.SpecWarp(ChatID,gObj[uIndex].MapNumber,gObj[uIndex].X,gObj[uIndex].Y);*/
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::RestorePointsCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	int	OriginalPoints  = 0;
	int	RestoredPoints  = 0;
	int iDefStrength	= gCharInfo.sCharInfo[lpObj->Class].Strength;
	int iDefDexterity	= gCharInfo.sCharInfo[lpObj->Class].Dexterity;
	int iDefVitality	= gCharInfo.sCharInfo[lpObj->Class].Vitality;
	int iDefEnergy		= gCharInfo.sCharInfo[lpObj->Class].Energy;
	int iDefLeadership	= gCharInfo.sCharInfo[lpObj->Class].Leadership;
	// -----
	if ( cLen < 0 || cLen >= 1)
	{
		SendMsg.MessageOut(ChatID, 0x01, "Bad Syntax: %s", CMDSTR.iRSP_Command);
		return;
	}
	// -----
	if ( lpObj->DbClass == DB_GRAND_MASTER || lpObj->DbClass == DB_BLADE_MASTER || lpObj->DbClass == DB_HIGH_ELF || 
		 lpObj->DbClass == DB_DUEL_MASTER  || lpObj->DbClass == DB_LORD_EMPEROR || lpObj->DbClass == DB_DIMENSION_MASTER ||
		 lpObj->DbClass == DB_FIST_MASTER )
	{
		SendMsg.MessageOut(ChatID, 0x01, "This Command is not Available For Master Characters.", CMDSTR.iRSP_Command);
		return;
	}
	// -----
	if( !CMDSTR.iRSP_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[RestorePoints]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( lpObj->Level < CMDSTR.iRSP_RequiredLevel )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[RestorePoints]: Need Level %d to use This Command.", CMDSTR.iRSP_RequiredLevel);
		return;
	}
	// -----
	if ( CMDSTR.iRSP_EnalbeCost )
	{
		if ( lpObj->Money < CMDSTR.iRSP_RequiredCost )
		{
			if ( CMDSTR.iRSP_RequiredCost > 0 )
			{
				SendMsg.MessageOut(ChatID, 0x01, "[RestorePoints]: Need %d Zen To Use This Command.", CMDSTR.iRSP_RequiredCost);
			}
			return;
		}
	}
	// -----
	if ( lpObj->Class == CLASS_DARKLORD )
	{
		RestoredPoints	= lpObj->Strength + lpObj->Dexterity + lpObj->Vitality + lpObj->Energy + lpObj->Leadership;
	} else {
		RestoredPoints	= lpObj->Strength + lpObj->Dexterity + lpObj->Vitality + lpObj->Energy;
	}
	// -----
	if ( lpObj->Class == CLASS_DARKLORD )
	{
		OriginalPoints	= iDefStrength + iDefDexterity + iDefVitality + iDefEnergy + iDefLeadership;

		lpObj->Strength		= gCharInfo.sCharInfo[lpObj->Class].Strength;
		lpObj->Dexterity	= gCharInfo.sCharInfo[lpObj->Class].Dexterity;
		lpObj->Vitality		= gCharInfo.sCharInfo[lpObj->Class].Vitality;
		lpObj->Energy		= gCharInfo.sCharInfo[lpObj->Class].Energy;
		lpObj->Leadership	= gCharInfo.sCharInfo[lpObj->Class].Leadership;
	} else {
		OriginalPoints	= iDefStrength + iDefDexterity + iDefVitality + iDefEnergy;

		lpObj->Strength		= gCharInfo.sCharInfo[lpObj->Class].Strength;
		lpObj->Dexterity	= gCharInfo.sCharInfo[lpObj->Class].Dexterity;
		lpObj->Vitality		= gCharInfo.sCharInfo[lpObj->Class].Vitality;
		lpObj->Energy		= gCharInfo.sCharInfo[lpObj->Class].Energy;
	}
	// -----
	lpObj->LevelUpPoint = (lpObj->LevelUpPoint + RestoredPoints) - OriginalPoints;
	// -----
	if ( CMDSTR.iRSP_EnalbeCost )
	{
		if ( CMDSTR.iRSP_RequiredCost > 0 )
		{
			lpObj->Money = lpObj->Money - CMDSTR.iRSP_RequiredCost;
			// -----
			GCMoneySend(lpObj->m_Index, lpObj->Money);
		}
	}
	// -----
	gCalCharacter.gObjCalCharacter(ChatID);
	// -----
	SendMsg.MessageOut(ChatID, 0x01, "[RestorePoints]: You are Successfully Restored Your Stats Points. Parsed [%ld]", CMDSTR.iRSP_RequiredCost);
	SendMsg.MessageOut(ChatID, 0x01, "[RestorePoints]: Please Relog to Complete your Process.");
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::MoveAllPlayersCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	int Map			= 0;
	int PosX		= 0;
	int PosY		= 0;
	// -----
	if ( !(lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[MoveAll]: Command is Only available For GameMasters.");
		return;
		// -----
	} else {
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanMoveAll = GMSTR[x].MoveAll;
				}
			}

			if ( !CMDSTR.CanMoveAll )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if( !CMDSTR.iMVA_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[MoveAll]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 5 || cLen >= 12 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Map> <CoordX> <CoordY>", CMDSTR.iMVA_Command);
		return;
	}
	// -----
	sscanf(chatmsg,"%d %d %d", &Map, &PosX, &PosY);
	// -----
	if( Map  < 0 || Map  >= 82  || PosX < 0 || PosX >= 256 || PosY < 0 || PosY >= 256 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[MoveAll]: You Entered Wrong Values.");
		return;
	}
	// -----
	for ( int i=OBJMIN; i<OBJMAX; i++ )
	{
		LPOBJ pObj = &gObj[i];
		// -----
		if ( gObjIsConnected(pObj) )
		{
			//gMove.SpecWarp(pObj->m_Index, (BYTE &)Map, (BYTE &)PosX, (BYTE &)PosY);
			// -----
			if ( (BYTE &)Map == pObj->MapNumber && (BYTE &)PosX == pObj->X && (BYTE &)PosY == pObj->Y )
			{
				SendMsg.MessageOut(ChatID, 0x01, "[MoveAll]: You Successfully Move Players to [%s]-[%d-%d]", GetMapName(Map), PosX, PosY);
			}
		}
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::CreateSetCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	int   Item;
	BYTE  Level;
	BYTE  Dur;
	BYTE  Skill;
	BYTE  Luck;
	BYTE  AddOpt; 
	BYTE  AddExc; 
	BYTE  AddAnc;
	BYTE  Socket[5];
	BYTE  CordX;
	BYTE  CordY;
	// -----
	if ( (lpObj->AuthorityCode&1) )
	{
		if ( cLen < 0 ) return;
		// -----
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanCreateSet = GMSTR[x].CreateSets;
				}
			}

			if ( !CMDSTR.CanCreateSet )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
		// -----
		if ( !CMDSTR.iCST_Switch )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[C.Items]: Command is currently Inactive!");
			return;
		}
		// -----
		if ( cLen <= 27 || cLen >= 47 )
		{
			SendMsg.MessageOut(ChatID, 0x01, "Bad Syntax: %s <Type><Index><Lvl><Dur><Luck><Skill><AddOpt><Exc><Anc><Soc1><Soc2><Soc3><Soc4><Soc5>",
				CMDSTR.iCST_Command);
		}
		// -----
		sscanf(chatmsg,"%d %d %d %d %d %d %d %d %d %d %d %d %d", &Item, &Level, &Dur, &Luck, &Skill, &AddOpt, &AddExc, &AddAnc,
			&Socket[0], &Socket[1], &Socket[2], &Socket[3], &Socket[4]);
		// -----
		if  (  Item		 >	 254 ||  Item		<	0 
			|| Level	 >	 15	 ||  Level		<	0 
			|| Dur		 <	 0	 ||  Dur		>	255 
			|| Luck		 >	 1	 ||  Luck		<	0 
			|| Skill	 >	 1	 ||  Skill		<	0 
			|| AddOpt	 <	 0	 ||  AddOpt		>	7 
			|| AddExc	 <	 0	 &&  AddExc		>	63
			|| AddAnc	 <	 0	 ||  AddAnc		==	1 
			|| AddAnc	 ==	 2	 ||  AddAnc		==	3 
			|| AddAnc	 ==	 4	 ||  AddAnc		==	6 
			|| AddAnc	 ==	 7	 ||  AddAnc		==	8		
			|| AddAnc	 ==	 9	 ||  AddAnc		>	10	
			|| Socket[0] <	 0	 ||  Socket[0]	>	255
			|| Socket[1] <	 0	 ||  Socket[1]	>	255
			|| Socket[2] <	 0	 ||  Socket[2]	>	255
			|| Socket[3] <	 0	 ||  Socket[3]	>	255
			|| Socket[4] <	 0	 ||  Socket[4]	>	255	)
		{
			SendMsg.MessageOut(ChatID, 0x01, "[C.Items]: You have been used wrong codes.");
			return;
		}
		// -----
		CordX	= lpObj->X;
		CordY	= lpObj->Y;
		// -----
		for (int i=7; i<12; i++)
		{
			ItemSerialCreateSend(ChatID,lpObj->MapNumber,CordX,CordY,(i*512)+Item,Level,Dur,Skill,Luck,AddOpt,ChatID,AddExc,AddAnc,Socket);
		}
		// -----
		SendMsg.MessageOut(ChatID, 0x01, "[C.Items]: You Successflly Created Your Set.");
	}
	else
	{
		if ( cLen < 0 || cLen >= 1 ) return;
		// -----
		SendMsg.MessageOut(ChatID, 0x01, "[C.Items]: Command is Only available For GameMasters.");
		return;
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::BlockChatCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	WORD ChatBlockTime	= 0;
	char Player[16]		= {0};
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( !(lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[ChatBlock]: Command is Only available For GameMasters.");
		return;
		// -----
	} else {
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanBlockChat = GMSTR[x].BlockChat;
				}
			}

			if ( !CMDSTR.CanBlockChat )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if( !CMDSTR.iCBN_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[ChatBlock]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 6 || cLen >= 16 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Name> <Period>", CMDSTR.iCBN_Command);
		return;
	}
	// -----
	sscanf(chatmsg,"%s %d", Player, &ChatBlockTime);
	// -----
	DWORD PlayerID = gObjGetIndex(Player);
	// -----
	if( PlayerID == 0xFFFF ) return;
	// -----
	if(( PlayerID>=OBJMIN ) && ( PlayerID<=OBJMAX ))
	{
		LPOBJ pObj = &gObj[PlayerID];
		// -----
		if(pObj->Name != lpObj->Name)
		{
			if ( (pObj->AuthorityCode&1) )
			{
				SendMsg.MessageOut(ChatID, 0x01, "[ChatBlock]: You can't Block GameMaster's Chat.");
				return;
			}
			// -----
			lpObj->ChatLimitTime = ChatBlockTime;
			// -----
			SendMsg.MessageOut(ChatID, 0x01, "[ChatBlock] Player: %s is Chat Blocked For [%d] Minute(s)!", Player, ChatBlockTime);	// GM Message
			SendMsg.MessageOut(PlayerID, 0x01, "[ChatBlock] %s Blocked Your Chat For [%d] Minute(s)", lpObj->Name, ChatBlockTime); // Player Message
		}
	// -----
	} else {
		SendMsg.MessageOut(ChatID, 0x01, "[ChatBlock] Player: %s is offline",Player);
		return;
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::ClearVaultCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	int ClrVaultCode = 0;
	int CertainZen   = 0;
	// -----
	if ( cLen < 0 ) return;
	// -----
	SendMsg.MessageOut(ChatID, 0x01, "[ClearVault]: Sorry This Command is not Complete Yet...");
	return;
	// -----
	if ( !CMDSTR.iCRV_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[ClearVault]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 1 || cLen >= 3 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <VaultCode>", CMDSTR.iCRV_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 0: Clear Real Valut.", CMDSTR.iCRV_Command);
		SendMsg.MessageOut(ChatID, 0x01, "%s 1: Clear Expanded Vault.", CMDSTR.iCRV_Command);
		return;
	}
	// -----
	sscanf(chatmsg,"%d", &ClrVaultCode);
	// -----
	if ( ClrVaultCode < 0 && ClrVaultCode >= 2 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[ClearVault]: Clear Vault Code [0-&-1].");
		return;
	}
	// -----
	switch(ClrVaultCode)
	{
		case 0:
		{
			for (int i=0; i<119; i++)
			{
				if ( lpObj->pInventory[i].IsItem() == TRUE )
				{
					gObjWarehouseDeleteItem(ChatID, i);
					// -----
					SendMsg.MessageOut(ChatID, 0x01, "[ClearVault]: You have cleared your Real Vault.");
				}
			}
		} break;
		// -----
		case 1:
		{
			for (int i=120; i<239; i++)
			{
				if ( lpObj->pInventory[i].IsItem() == TRUE )
				{
					gObjWarehouseDeleteItem(ChatID, i);
					// -----
					SendMsg.MessageOut(ChatID, 0x01, "[ClearVault]: You have cleared your Expanded Vault.");
				}
			}
		} break;
	}
	return;
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::GetRegInfoCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	if ( cLen < 0 || cLen >= 1)
	{
		SendMsg.MessageOut(ChatID, 0x01, "Bad Syntax: %s", CMDSTR.iGRI_Command);
		return;
	}
	// -----
	if( !CMDSTR.iGRI_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[GetRegInfo]: Command is currently Inactive!");	
		return;
	}
	// -----
	SendMsg.MessageOut(ChatID, 0x01, "[GetRegInfo]: ServerFiles Registred to: %s", CUnknown.Unk449);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::StatisticsCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	int STR_LVL = 0;
	int END_LVL = 400;
	// -----
	if ( cLen < 0 || cLen >= 1)
	{
		SendMsg.MessageOut(ChatID, 0x01, "Bad Syntax: %s", CMDSTR.iSTC_Command);
		return;
	}
	// -----
	if ( CMDSTR.iSTC_IsOnlyForGM )
	{
		if ( !(lpObj->AuthorityCode&1) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[Statistics]: Command is Only available For GameMasters.");
			return;
			// -----
		} else {
			if ( cGMMng.EnableGMManager )
			{
				for(int x=0; x<cGMMng.GMMngCount; x++)
				{
					if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
					{
						CMDSTR.CanStatistics = GMSTR[x].CharStatistics;
					}
				}

				if ( !CMDSTR.CanStatistics )
				{
					MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
					return;
				}
			}
		}
	}
	// -----
	if( !CMDSTR.iSTC_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Statistics]: Command is currently Inactive!");	
		return;
	}
	// -----
	gObjSendUserStatistic(ChatID, STR_LVL, END_LVL);
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::BlockCmdCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	WORD CmdBlockTime	= 0;
	char Player[16]		= {0};
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( !(lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[CommandBlock]: Command is Only available For GameMasters.");
		return;
		// -----
	} else {
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanBlockCommands = GMSTR[x].BlockCommands;
				}
			}

			if ( !CMDSTR.CanBlockCommands )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if( !CMDSTR.iCBK_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[CommandBlock]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 6 || cLen >= 16 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Name> <Period>", CMDSTR.iCBK_Command);
		return;
	}
	// -----
	sscanf(chatmsg,"%s %d", Player, &CmdBlockTime);
	// -----
	DWORD PlayerID = gObjGetIndex(Player);
	// -----
	if( PlayerID == 0xFFFF ) return;
	// -----
	if(( PlayerID>=OBJMIN ) && ( PlayerID<=OBJMAX ))
	{
		LPOBJ pObj = &gObj[PlayerID];
		// -----
		if(pObj->Name != lpObj->Name)
		{
			if ( (pObj->AuthorityCode&1) )
			{
				SendMsg.MessageOut(ChatID, 0x01, "[CommandBlock]: You can't Block GameMaster's Commands.");
				return;
			}
			// -----
			//lpObj->CommandLimitTime = CmdBlockTime;
			// -----
			SendMsg.MessageOut(ChatID, 0x01, "[CommandBlock] Player: %s is Command Blocked For [%d] Minute(s)!", Player, CmdBlockTime); // GM MSG
			SendMsg.MessageOut(PlayerID, 0x01, "[CommandBlock] %s Blocked Your Command For [%d] Minute(s)", lpObj->Name, CmdBlockTime); // Player MSG
		}
	// -----
	} else {
		SendMsg.MessageOut(ChatID, 0x01, "[CommandBlock] Player: %s is offline", Player);
		return;
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::BanAccountCommand(DWORD ChatID, char chatmsg[60], int CMD_BAN_TYPE)
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	char Player[16]		= {0};
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( !(lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Ban]: Command is Only available For GameMasters.");
		return;
		// -----
	} else {
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanBanAccount = GMSTR[x].BanAccount;
				}
			}

			if ( !CMDSTR.CanBanAccount )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if( !CMDSTR.iBAN_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Ban]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 4 || cLen >= 12 )
	{
		switch ( CMD_BAN_TYPE )
		{
			case 0:
				SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Name>", CMDSTR.iBAN_CommandUBN);
				break;
				// -----
			case 1:
				SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Name>", CMDSTR.iBAN_CommandBAN);
				break;
		}
		// -----
		return;
	}
	// -----
	sscanf(chatmsg,"%s", Player);
	// -----
	DWORD PlayerID = gObjGetIndex(Player);
	// -----
	if( PlayerID == 0xFFFF ) return;
	// -----
	LPOBJ pObj = &gObj[PlayerID];
	// -----
	if(pObj->Name != lpObj->Name)
	{	
		if ( (pObj->AuthorityCode&1) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[Ban]: You can't Ban GameMaster Account.");
			return;
		}
		// -----
		switch ( CMD_BAN_TYPE )
		{
			case 0:
				SendMsg.MessageOut(ChatID, 0x01, "[Ban]: Account: %s is Unbanned!", Player);
				break;
				// -----
			case 1:
				SendMsg.MessageOut(ChatID, 0x01, "[Ban]: Account: %s is Banned!", Player);
				break;
		}
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::BlockCharacterCommand(DWORD ChatID, char chatmsg[60], int CMD_BLK_TYPE)
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	char Player[16]		= {0};
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( !(lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Block]: Command is Only available For GameMasters.");
		return;
		// -----
	} else {
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanBlockChar = GMSTR[x].BlockChar;
				}
			}

			if ( !CMDSTR.CanBlockChar )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if( !CMDSTR.iBLK_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Block]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 4 || cLen >= 12 )
	{
		switch ( CMD_BLK_TYPE )
		{
			case 0:
				SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Name>", CMDSTR.iBLK_CommandUBK);
				break;
				// -----
			case 1:
				SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Name>", CMDSTR.iBLK_CommandBLK);
				break;
		}
		// -----
		return;
	}
	// -----
	sscanf(chatmsg,"%s", Player);
	// -----
	DWORD PlayerID = gObjGetIndex(Player);
	// -----
	if( PlayerID == 0xFFFF ) return;
	// -----
	LPOBJ pObj = &gObj[PlayerID];
	// -----
	if(pObj->Name != lpObj->Name)
	{	
		if ( (pObj->AuthorityCode&1) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[Block]: You can't Block GameMaster Character.");
			return;
		}
		// -----
		switch ( CMD_BLK_TYPE )
		{
			case 0:
				SendMsg.MessageOut(ChatID, 0x01, "[Block]: Player: %s is Unblocked!", Player);
				break;
				// -----
			case 1:
				SendMsg.MessageOut(ChatID, 0x01, "[Block]: Player: %s is Blocked!", Player);
				break;
		}
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::ResetCharCommand(DWORD ChatID, char chatmsg[60])
{
	/*LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	int ExtraPoints		= CMDSTR.iRST_PointsPerReset;
	int RSTMoney		= CMDSTR.iRST_RequiredCost;
	int RSTLevel		= CMDSTR.iRST_RequiredLevel;
	int StartLevel		= 1;
	// -----
	if ( cLen < 0 || cLen >= 1)
	{
		SendMsg.MessageOut(ChatID, 0x01, "Bad Syntax: %s", CMDSTR.iRST_Command);
		return;
	}
	// -----
	if ( (lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Reset]: Command is not available For GameMasters.");
		return;
	}
	// -----
	if( !CMDSTR.iRST_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Reset]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( lpObj->Resets >= CMDSTR.iRST_MaxReset )
	{
		MsgOutput(ChatID,"[Reset]: You Exceeded Maximum Resets Number, Contact Administrator.");
		return;
	}
	// -----
	if ( lpObj->Level < RSTLevel )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Reset]: Need Level %d to use This Command.", RSTLevel);
		return;
	}
	// -----
	if ( lpObj->Money < RSTMoney )
	{
		if ( RSTMoney > 0 )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[Reset]: Need %d Zen To Use This Command.", RSTMoney);
		}
		// -----
		return;
	}
	// -----
	if ( CMDSTR.iRST_Type < 1 || CMDSTR.iRST_Type > 2 )
	{
		CLog.Error("Reset Command Type variable is Set to wrong value ( 1-Or-2 ).");
		SendMsg.MessageOut(ChatID, 0x01, "[Reset]: Error Occured, Contact Administrator.");
		return;
	}
	// -----
	switch ( CMDSTR.iRST_Type )
	{
		case 1:
		{
			if ( gUser.gObjIsMasteringLevel(lpObj) == false )
			{
				if ( lpObj->Level == RSTLevel )
				{
					lpObj->Level		= StartLevel;
					lpObj->LevelUpPoint	= lpObj->LevelUpPoint + (ExtraPoints * lpObj->Resets);
					lpObj->Money		= lpObj->Money - RSTMoney;
					lpObj->Experience	= 0;
					lpObj->NextExp		= 100;
					lpObj->Resets++;
					// -----
					GCLevelUpMsgSend(ChatID, 1);
				}
			} else {
				if ( lpObj->Level == RSTLevel )
				{
					lpObj->MasterLevel		= StartLevel;
					lpObj->MasterPoints		= lpObj->MasterPoints + (ExtraPoints * lpObj->MasterResets);
					lpObj->Money			= lpObj->Money - RSTMoney;
					lpObj->MLExperience		= 0;
					lpObj->MLNextExp		= 100;
					lpObj->MasterResets++;
					// -----
					gSkillTree.GCMasterLevelUpSend(lpObj, 1);
					gSkillTree.SendMasterData(ChatID);
				}
			}
		} break;
		// -----
		case 2:
		{
			if ( gUser.gObjIsMasteringLevel(lpObj) == false )
			{
				if ( lpObj->Level == RSTLevel )
				{
					lpObj->Level		= StartLevel;
					lpObj->Money		= lpObj->Money - RSTMoney;
					lpObj->Experience	= 0;
					lpObj->NextExp		= 0;
					lpObj->Resets++;
					// -----
					GCLevelUpMsgSend(ChatID, 1);
				}
			} else {
				if ( lpObj->Level == RSTLevel )
				{
					lpObj->MasterLevel		= StartLevel;
					lpObj->Money			= lpObj->Money - RSTMoney;
					lpObj->MLExperience		= 0;
					lpObj->MLNextExp		= 0;
					lpObj->MasterResets++;
					// -----
					gSkillTree.GCMasterLevelUpSend(lpObj, 1);
					gSkillTree.SendMasterData(ChatID);
				}
			}
		} break;
	}
	// -----
	GCMoneySend(ChatID, lpObj->Money);
	// -----
	gCalCharacter.gObjCalCharacter(ChatID);
	// -----
	SendMsg.MessageOut(ChatID, 0x01, "[Reset]: You Character is Reseted.");
	SendMsg.MessageOut(ChatID, 0x01, "[Reset]: Your Level[%d], LevelUpPoints[%d], Zen[%ld]", lpObj->Level, lpObj->LevelUpPoint, lpObj->Money);*/
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::GMMoveCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	int Map			= 0;
	int PosX		= 0;
	int PosY		= 0;
	char Player[16]	= {0};
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( !(lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[GMove]: Command is Only available For GameMasters.");
		return;
		// -----
	} else {
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanGMMove = GMSTR[x].GMMove;
				}
			}

			if ( !CMDSTR.CanGMMove )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if( !CMDSTR.iGMV_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[GMove]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 10 || cLen >= 23 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Name> <Map> <CoordX> <CoordY>", CMDSTR.iGMV_Command);
		return;
	}
	// -----
	sscanf(chatmsg,"%s %d %d %d", Player, &Map, &PosX, &PosY);
	// -----
	if( Map  < 0 || Map  >= 82  || PosX < 0 || PosX >= 256 || PosY < 0 || PosY >= 256 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[GMove]: You Entered Wrong Values.");
		return;
	}
	// -----
	DWORD PlayerID = gObjGetIndex(Player);
	// -----
	if( PlayerID == 0xFFFF ) return;
	// -----
	if(( PlayerID>=OBJMIN ) && ( PlayerID<=OBJMAX ))
	{
		LPOBJ pObj = &gObj[PlayerID];
		// -----
		if ( (pObj->Name != lpObj->Name) && (pObj->AuthorityCode&1) )
		{
			SendMsg.MessageOut(ChatID, 0x01, "[GMove]: You can't Move GameMaster.");
			return;
		}
		// -----
		//gMove.SpecWarp(PlayerID, Map, PosX, PosY);
		// -----
		if(pObj->Name != lpObj->Name)
		{
			if ( Map == pObj->MapNumber && PosX == pObj->X && PosY == pObj->Y )
			{
				SendMsg.MessageOut(ChatID, 0x01, "[GMove]: You Successfully Move %s to [%s]-[%d-%d]", Player, GetMapName(Map), PosX, PosY);
				SendMsg.MessageOut(PlayerID, 0x01, "[GMove]: %s Moved You to [%s]-[%d-%d]", lpObj->Name, GetMapName(Map), PosX, PosY);
			}
		// -----
		} else {
			if ( Map == pObj->MapNumber && PosX == pObj->X && PosY == pObj->Y )
			{
				SendMsg.MessageOut(ChatID, 0x01, "[GMove]: You Successfully Moved to [%s]-[%d-%d]", GetMapName(Map), PosX, PosY);
			}
		}
	// -----
	} else {
		SendMsg.MessageOut(ChatID, 0x01, "[GMove] Player: %s is offline",Player);
		return;
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::TraceCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	char Player[16]	= {0};
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( !(lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Trace]: Command is Only available For GameMasters.");
		return;
		// -----
	} else {
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanTrace = GMSTR[x].Trace;
				}
			}

			if ( !CMDSTR.CanTrace )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if( !CMDSTR.iTRCSwitch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[Trace]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 4 || cLen >= 12 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Name>", CMDSTR.iTRC_Command);
		return;
	}
	// -----
	sscanf(chatmsg,"%s", Player);
	// -----
	DWORD PlayerID = gObjGetIndex(Player);
	// -----
	if( PlayerID == 0xFFFF ) return;
	// -----
	if(( PlayerID>=OBJMIN ) && ( PlayerID<=OBJMAX ))
	{
		LPOBJ pObj = &gObj[PlayerID];
		// -----
		//gMove.SpecWarp(ChatID, pObj->MapNumber, pObj->X, pObj->Y);
	// -----
	} else {
		SendMsg.MessageOut(ChatID, 0x01, "[Trace] Player: %s is offline",Player);
		return;
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::GuildWarCommand(DWORD ChatID, char chatmsg[60], int CMD_WAR_TYPE)
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	char TargetGuild[9]	= {0};
	char szTemp[100];
	// -----
	if ( cLen < 0 ) return;
	// -----
	if ( (lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[GuildWar]: Command is not available For GameMasters.");
		return;
	}
	// -----
	if( !CMDSTR.iGDWSwitch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[GuildWar]: Command is currently Inactive!");	
		return;
	}
	// -----
	sscanf(chatmsg,"%s", TargetGuild);
	// -----
	switch ( CMD_WAR_TYPE )
	{
		case 0:
		{
			if ( cLen <= 4 || cLen >= 10 )
			{
				SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <GuildName>", CMDSTR.iGDW_CommandWAR);
				return;
			}
			// -----
			if ( gEnableBattleSoccer != FALSE )
			{	
				if ( TargetGuild != NULL )
				{
					if ( strlen(TargetGuild) >= 1 )
					{
						GCGuildWarRequestResult(TargetGuild, ChatID, 1);
					}
				}
			}
		} break;
		// -----
		case 1:
		{
			if ( cLen < 0 || cLen >= 1)
			{
				SendMsg.MessageOut(ChatID, 0x01, "Bad Syntax: %s", CMDSTR.iGDW_CommandEND);
				return;
			}
			// -----
			if ( lpObj->lpGuild != NULL && lpObj->lpGuild->lpTargetGuildNode != NULL )
			{
				if ( !(strcmp( lpObj->Name, lpObj->lpGuild->Names[0] )) )
				{
					if ( lpObj->lpGuild->BattleGroundIndex >= 0 && lpObj->lpGuild->WarType == TRUE )
					{
						gObjAddMsgSendDelay(lpObj, 7, ChatID, 10000, 0);
						// -----
						wsprintf(szTemp, "Because of %s's Request, The War will End Automatically in 10 Seconds", lpObj->lpGuild->Names[0]);
						// -----
						GCServerMsgStringSendGuild(lpObj->lpGuild, szTemp, 1);
						GCServerMsgStringSendGuild(lpObj->lpGuild->lpTargetGuildNode, szTemp, 1);
					}
				}
			}
		} break;
	}
}
// ------------------------------------------------------------------------------------------------------------------------------------------------
void ChatHandlE::DropEventCommand(DWORD ChatID, char chatmsg[60])
{
	LPOBJ lpObj = &gObj[ChatID];
	int	cLen	= strlen(chatmsg);
	// -----
	if ( cLen < 0 || cLen >= 1)
	{
		SendMsg.MessageOut(ChatID, 0x01, "Bad Syntax: %s", CMDSTR.iDRE_Command);
		return;
	}
	// -----
	if ( !(lpObj->AuthorityCode&1) )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[DropEvent]: Command is Only available For GameMasters.");
		return;
		// -----
	} else {
		if ( cGMMng.EnableGMManager )
		{
			for(int x=0; x<cGMMng.GMMngCount; x++)
			{
				if ( !strcmp(GMSTR[x].GMName, lpObj->Name) )
				{
					CMDSTR.CanDropEvent = GMSTR[x].DropEvent;
				}
			}

			if ( !CMDSTR.CanDropEvent )
			{
				MsgOutput(ChatID,"[GM-Manager]: You aren't able to use this Command.");
				return;
			}
		}
	}
	// -----
	if( !CMDSTR.iDRE_Switch )
	{
		SendMsg.MessageOut(ChatID, 0x01, "[DropEvent]: Command is currently Inactive!");	
		return;
	}
	// -----
	if ( cLen <= 4 || cLen >= 12 )
	{
		SendMsg.MessageOut(ChatID, 0x01, "Useage: %s <Name>", CMDSTR.iTRC_Command);
		return;
	}
	// -----
	int iX = lpObj->X;
	int iY = lpObj->Y;
	// -----
	SendMsg.MessageOutAll(0x00, "[DropEvent]: will Start in 1 Minute.");
	// -----
	Sleep(60000);
	// -----
	for (int i=0; i<11; i++)
	{
		int NewCX = iX+i;
		int NewCY = iY+i;
		// -----
		BYTE Socket[5];
		// -----
		Socket[0] = 0xFF;
		Socket[1] = 0xFF;
		Socket[2] = 0xFF;
		Socket[3] = 0xFF;
		Socket[4] = 0xFF;
		// -----
		ItemSerialCreateSend(ChatID,lpObj->MapNumber,NewCX,NewCY,(CMDSTR.iDRE_EItDropIndex*512)+CMDSTR.iDRE_EItDropType,0,0,0,0,0,ChatID,0,0,Socket);
		// -----
		Sleep(CMDSTR.iDRE_TimeBtwDrops);
	}
	// -----
	SendMsg.MessageOut(ChatID, 0x01, "[DropEvent]: has been Finished.");
}
// ------------------------------------------------------------------------------------------------------------------------------------------------