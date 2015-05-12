//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Chat & Commands Class
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------
#ifndef CHAT_HANDLER_H
#define CHAT_HANDLER_H

#include "StdAfx.H"
#include "prodef.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *	Commands List ~	Variety Of HaRiBO GameServer Commands ~ 33 
 * 
 * Clear PK				~ Done ... Works 100%						~ Normal Players
 * Global MSG			~ Done ... Works 100%						~ Normal Players
 * Create Items			~ Done ... Works 100%						~ Game Masters
 * Online				~ Done ... Works 100%						~ Game Masters
 * Server Time			~ Done ... Works 100%						~ Normal Players
 * Fireworks			~ Done ... Works 100%						~ Game Masters
 * PK Set				~ Done ... Works 100%						~ Game Masters
 * Add Points			~ Done ... Works 100%						~ Normal Players
 * Disconnect			~ Done ... Works 100%						~ Game Masters
 * Set Zen				~ Done ... Works 100%						~ Game Masters
 * Add Zen				~ Done ... Works 100%						~ Game Masters
 * Skin					~ Done ... Works 100%						~ Game Masters
 * Spwan Mob			~ Done ... Works 100%						~ Game Masters
 * Status				~ Done ... Works 100%						~ Game Masters
 * Clear Inv			~ Done ... Works 100%						~ Normal Players
 * Exit Game			~ Done ... Works 100%						~ Normal Players
 * About Server			~ Done ... Works 100%						~ Normal Players
 * Set Level			~ Done ... Works 100%						~ Game Masters
 * Set Lvl UP Points	~ Done ... Works 100%						~ Game Masters
 * Add Buff				~ Done ... Works 100%						~ Game Masters
 * Summon Monster		~ Done ... Works 100%						~ Normal Players
 * Marriage Commands	~ Done ... Works 70%	*HINT* QUERIES		~ Normal Players
 * Restore Points		~ Done ... Works 100%						~ Normal Players
 * Move All Players		~ Done ... Works 100%						~ Game Masters
 * Create Sets			~ Done ... Works 100%						~ Game Masters
 * Block Chat			~ Done ... Works 100%						~ Game Masters
 * Clear Vault			~ Done ... Works 100%						~ Normal Players
 * Get Register Info	~ Done ... Works 100%						~ Normal Players
 * Char Statistics		~ Done ... Works 100%						~ Normal Players
 * Block Commands		~ Done ... Works 100%						~ Game Masters
 * Ban Account			~ Done ... Works 70%	*HINT* QUERIES		~ Game Masters
 * Block Character		~ Done ... Works 70%	*HINT* QUERIES		~ Game Masters
 * Reset Character		~ Done ... Works 70%	*HINT* QUERIES		~ Normal Players
 * GM Move				~ Done ... Works 100%
 * Trace				~ Done ... Works 100%
 * Guild War			~ Done ... Works 100%
 * Drop Event			~ Done ... Works 100%						~ Game Masters
 */ 


struct PMSG_CHATDATA
{
	PBMSG_HEAD	h;
	char		chatid[10];
	char		chatmsg[60];
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct CHAT_LOG_DATA
{
	PBMSG_HEAD	h;
	WORD		wServer;
	char		AccountID[11];
	char		Name[11];
	BYTE		btType;
	char		szChatMsg[61];
	char		szTargetName[5][11];
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_CHATDATA_NUMBER
{
	PBMSG_HEAD	h;
	UCHAR		NumberH;
	UCHAR		NumberL;
	char		chatmsg[60];
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_CHATDATA_WHISPER
{
	PBMSG_HEAD	h;
	char		id[10];
	char		chatmsg[60];
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct COMMANDSTRUCT
{
	int		iMessageChatting[7];
	int		iSwitchCommands;
	// -----
	BYTE	iPKC_Switch;
	int		iPKC_RequiredLevel;
	int		iPKC_EnalbeCost;
	int		iPKC_IsCostPerPKCount;
	int		iPKC_RequiredCost;
	char	iPKC_Command[50];
	int		CanClearPK;
	// -----
	BYTE	iPST_Switch;
	int		iPST_RequiredLevel;
	int		iPST_EnalbeCost;
	int		iPST_RequiredCost;
	int		iPST_PostColor;
	char	iPST_Command[50];
	int		CanPost;
	// -----
	BYTE	iCRT_Switch;
	char	iCRT_Command[50];
	int		CanCreate;
	// -----
	BYTE	iONL_Switch;
	int		iONL_RequiredLevel;
	int		iONL_IsOnlyForGM;
	char	iONL_Command[50];
	int		CanOnline;
	// -----
	BYTE	iTIM_Switch;
	int		iTIM_MsgColor;
	char	iTIM_Command[50];
	// -----
	BYTE	iFIR_Switch;
	int		iFIR_MaxAmount;
	char	iFIR_Command[50];
	int		CanFireworks;
	// -----
	BYTE	iPKS_Switch;
	char	iPKS_Command[50];
	int		CanSetPK;
	// -----
	BYTE	iADD_Switch;
	int		iADD_RequiredLevel;
	int		iADD_RequiredCost;
	int		iADD_MaxPointsToAdd;
	char	iADD_CommandStr[50];
	char	iADD_CommandAgi[50];
	char	iADD_CommandVit[50];
	char	iADD_CommandEne[50];
	char	iADD_CommandLed[50];
	int		CanAddPoints;
	// -----
	BYTE	iDIS_Switch;
	char	iDIS_CommandALL[50];
	char	iDIS_CommandGLD[50];
	char	iDIS_CommandCHR[50];
	int		CanDisconnect;
	// -----
	BYTE	iSZN_Switch;
	char	iSZN_Command[50];
	int		CanSetZen;
	// -----
	BYTE	iAZN_Switch;
	char	iAZN_Command[50];
	int		CanAddZen;
	// -----
	BYTE	iSKN_Switch;
	char	iSKN_Command[50];
	int		CanSkin;
	// -----
	BYTE	iSMB_Switch;
	char	iSMB_Command[50];
	int		CanSpawnMobs;
	// -----
	BYTE	iSTS_Switch;
	char	iSTS_Command[50];
	int		CanStatus;
	// -----
	BYTE	iINV_Switch;
	char	iINV_Command[50];
	// -----
	BYTE	iEXT_Switch;
	char	iEXT_Command[50];
	// -----
	BYTE	iABT_Switch;
	char	iABT_Command[50];
	// -----
	BYTE	iSLV_Switch;
	char	iSLV_Command[50];
	int		CanSetLevel;
	// -----
	BYTE	iLUP_Switch;
	char	iLUP_Command[50];
	int		CanSetLvlUpPoints;
	// -----
	BYTE	iABF_Switch;
	char	iABF_Command[50];
	int		CanAddBuff;
	// -----
	BYTE	iSMN_Switch;
	int		iSMN_RequiredLevel;
	int		iSMN_EnalbeCost;
	int		iSMN_RequiredCost;
	char	iSMN_Command[50];
	int		iSMN_DWMonster;
	int		iSMN_SMMonster;
	int		iSMN_GMMonster;
	int		iSMN_DKMonster;
	int		iSMN_BKMonster;
	int		iSMN_BMMonster;
	int		iSMN_FEMonster;
	int		iSMN_MEMonster;
	int		iSMN_HEMonster;
	int		iSMN_MGMonster;
	int		iSMN_DMMonster;
	int		iSMN_DLMonster;
	int		iSMN_LEMonster;
	int		iSMN_SUMonster;
	int		iSMN_BSMonster;
	int		iSMN_DSMonster;
	int		iSMN_RFMonster;
	int		iSMN_FMMonster;
	int		CanSummon;
	// -----
	int		iMRG_RequiredLevel;
	char	iMRG_CommandPro[50];
	char	iMRG_CommandAcp[50];
	char	iMRG_CommandDiv[50];
	char	iMRG_CommandOnl[50];
	char	iMRG_CommandTrc[50];
	int		CanMarriage;
	// -----
	BYTE	iRSP_Switch;
	int		iRSP_RequiredLevel;
	int		iRSP_EnalbeCost;
	int		iRSP_RequiredCost;
	char	iRSP_Command[50];
	// -----
	BYTE	iMVA_Switch;
	char	iMVA_Command[50];
	int		CanMoveAll;
	// -----
	BYTE	iCST_Switch;
	char	iCST_Command[50];
	int		CanCreateSet;
	// -----
	BYTE	iCBN_Switch;
	char	iCBN_Command[50];
	int		CanBlockChat;
	// -----
	BYTE	iCRV_Switch;
	char	iCRV_Command[50];
	// -----
	BYTE	iGRI_Switch;
	char	iGRI_Command[50];
	// -----
	BYTE	iSTC_Switch;
	int		iSTC_IsOnlyForGM;
	char	iSTC_Command[50];
	int		CanStatistics;
	// -----
	BYTE	iCBK_Switch;
	char	iCBK_Command[50];
	int		CanBlockCommands;
	// -----
	BYTE	iBAN_Switch;
	char	iBAN_CommandUBN[50];
	char	iBAN_CommandBAN[50];
	int		CanBanAccount;
	// -----
	BYTE	iBLK_Switch;
	char	iBLK_CommandUBK[50];
	char	iBLK_CommandBLK[50];
	int		CanBlockChar;
	// -----
	BYTE	iRST_Switch;
	int		iRST_RequiredLevel;
	int		iRST_RequiredCost;
	int		iRST_Type;
	int		iRST_PointsPerReset;
	int		iRST_MaxReset;
	char	iRST_Command[50];
	// -----
	BYTE	iGMV_Switch;
	char	iGMV_Command[50];
	int		CanGMMove;
	// -----
	BYTE	iTRCSwitch;
	char	iTRC_Command[50];
	int		CanTrace;
	// -----
	BYTE	iGDWSwitch;
	char	iGDW_CommandWAR[50];
	char	iGDW_CommandEND[50];
	// -----
	BYTE	iDRE_Switch;
	int		iDRE_EItDropIndex;
	int		iDRE_EItDropType;
	int		iDRE_TimeBtwDrops;
	char	iDRE_Command[50];
	int		CanDropEvent;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
class ChatHandlE
{
public:
	void GameChatProcess(PMSG_CHATDATA* lpChat, short aIndex);
	void GameChatRecieve(PMSG_CHATDATA_NUMBER* lpMsg, int aIndex);
	void ChatWhisperRecieve(PMSG_CHATDATA_WHISPER* lpMsg, int aIndex);
	void CommandReader(short gObjId, LPBYTE Protocol);
	// -----
private:
	CRITICAL_SECTION	CmdCritiSec;
	// -----
private:
	void ClearPKCommand(DWORD ChatID, char ChatMSG[60]);
	void PostCommand(DWORD ChatID, char ChatMSG[60]);
	void CreateItemsCommand(DWORD ChatID, char ChatMSG[60]);
	void OnlinePlayersCommand(DWORD ChatID, char ChatMSG[60]);
	void ServerTimeCommand(DWORD ChatID, char ChatMSG[60]);
	void FireworksCommand(DWORD ChatID, char ChatMSG[60]);
	void PkSetCommand(DWORD ChatID, char ChatMSG[60]);
	void AddPointsCommand(DWORD ChatID, LPCSTR lpBuffer, int CMD_STATS_TYPE);
	void DisconnectCommand(DWORD ChatID, char ChatMSG[60], int CMD_DCD_TYPE);
	void SetZenCommand(DWORD ChatID, char ChatMSG[60]);
	void AddZenCommand(DWORD ChatID, char ChatMSG[60]);
	void SkinCommand(DWORD ChatID, char ChatMSG[60]);
	void SpawnMobCommand(DWORD ChatID, char ChatMSG[60]);
	void StatusCommand(DWORD ChatID, char ChatMSG[60]);
	void ClearInvCommand(DWORD ChatID, char ChatMSG[60]);
	void ExitCommand(DWORD ChatID, char ChatMSG[60]);
	void AboutCommand(DWORD ChatID, char ChatMSG[60]);
	void SetLevelCommand(DWORD ChatID, char ChatMSG[60]);
	void SetLvlUpPCommand(DWORD ChatID, char ChatMSG[60]);
	void AddBuffCommand(DWORD ChatID, char ChatMSG[60]);
	void SummonCommand(DWORD ChatID, char ChatMSG[60]);
	void CrashCommand(DWORD ChatID, char ChatMSG[60]);
	void ProposeMarryCommand(DWORD ChatID, char ChatMSG[60]);
	void AcceptMarryCommand(DWORD ChatID, char ChatMSG[60]);
	void DivorceMarryCommand(DWORD ChatID, char ChatMSG[60]);
	void OnlineMarryCommand(DWORD ChatID, char ChatMSG[60]);
	void TraceMarryCommand(DWORD ChatID, char ChatMSG[60]);
	void RestorePointsCommand(DWORD ChatID, char ChatMSG[60]);
	void MoveAllPlayersCommand(DWORD ChatID, char ChatMSG[60]);
	void CreateSetCommand(DWORD ChatID, char ChatMSG[60]);
	void BlockChatCommand(DWORD ChatID, char ChatMSG[60]);
	void ClearVaultCommand(DWORD ChatID, char ChatMSG[60]);
	void GetRegInfoCommand(DWORD ChatID, char ChatMSG[60]);
	void StatisticsCommand(DWORD ChatID, char ChatMSG[60]);
	void BlockCmdCommand(DWORD ChatID, char ChatMSG[60]);
	void BanAccountCommand(DWORD ChatID, char ChatMSG[60], int CMD_BAN_TYPE);
	void BlockCharacterCommand(DWORD ChatID, char ChatMSG[60], int CMD_BLK_TYPE);
	void ResetCharCommand(DWORD ChatID, char ChatMSG[60]);
	void GMMoveCommand(DWORD ChatID, char ChatMSG[60]);
	void TraceCommand(DWORD ChatID, char ChatMSG[60]);
	void GuildWarCommand(DWORD ChatID, char ChatMSG[60], int CMD_WAR_TYPE);
	void DropEventCommand(DWORD ChatID, char ChatMSG[60]);
};
extern ChatHandlE		TChat;
extern COMMANDSTRUCT	CMDSTR;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
#endif
// -------------------------------------------------------------------------------------------------------------------------------------------------------