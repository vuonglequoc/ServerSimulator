/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Project:		Zodea MU GameServer - Season 6
 * Developer:	RealCoderZ MU Development © 2011
 * Hint:		Main Configs For Zodea GameServer
 * Coded By:	WolF & mesosa
 */
#include "stdafx.h"
#include "Configs.H"
#include "Structures.H"
#include "GameMain.H"


MainConfig MCfg;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
MainConfig :: MainConfig()
{
	return;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
MainConfig :: ~MainConfig()
{
	return;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void MainConfig :: Initialize()
{
	ReadConnectInfo	( RFile.szRFileName[0] );
	//ReadCommandInfo ( RFile.szRFileName[0] );
	// -----
	RFile.GetString((LPSTR)IPAdress[0],  IP_ADR_HDR,	"JoinServerIP",		RFile.szRFileName[0]);
	RFile.GetString((LPSTR)IPAdress[1],  IP_ADR_HDR,	"DataServer1IP",	RFile.szRFileName[0]);
	RFile.GetString((LPSTR)IPAdress[2],  IP_ADR_HDR,	"DataServer2IP",	RFile.szRFileName[0]);
	RFile.GetString((LPSTR)IPAdress[3],  IP_ADR_HDR,	"ExDBIP",			RFile.szRFileName[0]);
	// -----
	Port[0]		= RFile.GetInt(PORT_HDR,	"GameServerPort",		RFile.szRFileName[0]);
	Port[1]		= RFile.GetInt(PORT_HDR,	"JoinServerPort",		RFile.szRFileName[0]);
	Port[2]		= RFile.GetInt(PORT_HDR,	"DataServer1Port",		RFile.szRFileName[0]);
	Port[3]		= RFile.GetInt(PORT_HDR,	"DataServer2Port",		RFile.szRFileName[0]);
	Port[4]		= RFile.GetInt(PORT_HDR,	"ExDBPort",				RFile.szRFileName[0]);
	Port[5]		= RFile.GetInt(PORT_HDR,	"RankingServerPort",	RFile.szRFileName[0]);
	Port[6]		= RFile.GetInt(PORT_HDR,	"EventChipServerPort",	RFile.szRFileName[0]);
	// -----
	gLanguage	= RFile.GetInt(GS_INFO_HDR, "Protocol",			RFile.szRFileName[0]);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void MainConfig :: ReadConnectInfo( LPSTR FileName )
{
	MSTR.NumOfLoginAttemps = RFile.GetInt("Common",		"NumOfLoginAttemps",	FileName);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void MainConfig	:: ReadCommandInfo( LPSTR FileName )
{
	CMDSTR.iSwitchCommands			= GetPrivateProfileInt("ChatCommand","SwitchCommands",1,FileName);
	// -----
	CMDSTR.iMessageChatting[0]		= GetPrivateProfileInt("ChatCommand","IsGlobalNoticeChat",1,FileName);
	CMDSTR.iMessageChatting[1]		= GetPrivateProfileInt("ChatCommand","IsPartyChat",1,FileName);
	CMDSTR.iMessageChatting[2]		= GetPrivateProfileInt("ChatCommand","IsGuildGMNoticeChat",1,FileName);
	CMDSTR.iMessageChatting[3]		= GetPrivateProfileInt("ChatCommand","IsGuildAllianceChat",1,FileName);
	CMDSTR.iMessageChatting[4]		= GetPrivateProfileInt("ChatCommand","IsGuildChat",1,FileName);
	CMDSTR.iMessageChatting[5]		= GetPrivateProfileInt("ChatCommand","IsGenChat",1,FileName);
	CMDSTR.iMessageChatting[6]		= GetPrivateProfileInt("ChatCommand","IsNormalChat",1,FileName);
	// -----
	CMDSTR.iPKC_Switch				= GetPrivateProfileInt("ClearPK","SwitchCommand",1,FileName);
	CMDSTR.iPKC_RequiredLevel		= GetPrivateProfileInt("ClearPK","NeedLevel",100,FileName);
	CMDSTR.iPKC_EnalbeCost			= GetPrivateProfileInt("ClearPK","EnableCost",1,FileName);
	CMDSTR.iPKC_IsCostPerPKCount	= GetPrivateProfileInt("ClearPK","EnableCostPerPKCount",1,FileName);
	CMDSTR.iPKC_RequiredCost		= GetPrivateProfileInt("ClearPK","NeedZen",1000,FileName);
	GetPrivateProfileStringA			("ClearPK","Command","/ClearPK",CMDSTR.iPKC_Command,50,FileName);
	// -----
	CMDSTR.iPST_Switch				= GetPrivateProfileInt("GlobalMSG","SwitchCommand",1,FileName);
	CMDSTR.iPST_RequiredLevel		= GetPrivateProfileInt("GlobalMSG","NeedLevel",100,FileName);
	CMDSTR.iPST_EnalbeCost			= GetPrivateProfileInt("GlobalMSG","EnableCost",1,FileName);
	CMDSTR.iPST_RequiredCost		= GetPrivateProfileInt("GlobalMSG","NeedZen",1000,FileName);
	CMDSTR.iPST_PostColor			= GetPrivateProfileInt("GlobalMSG","PostColor",4,FileName);
	GetPrivateProfileStringA			("GlobalMSG","Command","/Post",CMDSTR.iPST_Command,50,FileName);
	// -----
	CMDSTR.iCRT_Switch				= GetPrivateProfileInt("CreateItems","SwitchCommand",1,FileName);
	GetPrivateProfileStringA			("CreateItems","Command","/Create",CMDSTR.iCRT_Command,50,FileName);
	// -----
	CMDSTR.iONL_Switch				= GetPrivateProfileInt("Online","SwitchCommand",1,FileName);
	CMDSTR.iONL_RequiredLevel		= GetPrivateProfileInt("Online","NeedLevel",10,FileName);
	CMDSTR.iONL_IsOnlyForGM			= GetPrivateProfileInt("Online","IsOnlyForGM",1,FileName);
	GetPrivateProfileStringA			("Online","Command","/Online",CMDSTR.iONL_Command,50,FileName);
	// -----
	CMDSTR.iTIM_Switch				= GetPrivateProfileInt("CurrentTime","SwitchCommand",1,FileName);
	CMDSTR.iTIM_MsgColor			= GetPrivateProfileInt("CurrentTime","MessageColor",5,FileName);
	GetPrivateProfileStringA			("CurrentTime","Command","/Time",CMDSTR.iTIM_Command,50,FileName);
	// -----
	CMDSTR.iFIR_Switch				= GetPrivateProfileInt("FireWorks","SwitchCommand",1,FileName);
	CMDSTR.iFIR_MaxAmount			= GetPrivateProfileInt("FireWorks","MaxAmountToSend",413,FileName);
	GetPrivateProfileStringA			("FireWorks","Command","/Fireworks",CMDSTR.iFIR_Command,50,FileName);
	// -----
	CMDSTR.iPKS_Switch				= GetPrivateProfileInt("SetPK","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("SetPK","Command","/SetPK",CMDSTR.iPKS_Command,50,FileName);
	// -----
	CMDSTR.iADD_Switch				= GetPrivateProfileInt("AddPoints","SwitchCommand",1,FileName);
	CMDSTR.iADD_RequiredLevel		= GetPrivateProfileInt("AddPoints","NeedLevel",10,FileName);
	CMDSTR.iADD_RequiredCost		= GetPrivateProfileInt("AddPoints","NeedZen",1000,FileName);
	CMDSTR.iADD_MaxPointsToAdd		= GetPrivateProfileInt("AddPoints","MaxPointToAdd",200,FileName);
	GetPrivateProfileStringA		("AddPoints","StrengthCommand","/AddStr",CMDSTR.iADD_CommandStr,50,FileName);
	GetPrivateProfileStringA		("AddPoints","AgilityCommand","/AddAgi",CMDSTR.iADD_CommandAgi,50,FileName);
	GetPrivateProfileStringA		("AddPoints","VitalityCommand","/AddVit",CMDSTR.iADD_CommandVit,50,FileName);
	GetPrivateProfileStringA		("AddPoints","EnergyCommand","/AddEne",CMDSTR.iADD_CommandEne,50,FileName);
	GetPrivateProfileStringA		("AddPoints","LeaderShipCommand","/AddLed",CMDSTR.iADD_CommandLed,50,FileName);
	// -----
	CMDSTR.iDIS_Switch				= GetPrivateProfileInt("Disconnect","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("Disconnect","DCAllCommand","/DisconnectAll",CMDSTR.iDIS_CommandALL,50,FileName);
	GetPrivateProfileStringA		("Disconnect","DCGldCommand","/DisconnectGuild",CMDSTR.iDIS_CommandGLD,50,FileName);
	GetPrivateProfileStringA		("Disconnect","DCChrCommand","/DisconnectChar",CMDSTR.iDIS_CommandCHR,50,FileName);
	// -----
	CMDSTR.iSZN_Switch				= GetPrivateProfileInt("SetZen","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("SetZen","Command","/SetZen",CMDSTR.iSZN_Command,50,FileName);
	// -----
	CMDSTR.iAZN_Switch				= GetPrivateProfileInt("AddZen","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("AddZen","Command","/AddZen",CMDSTR.iAZN_Command,50,FileName);
	// -----
	CMDSTR.iSKN_Switch				= GetPrivateProfileInt("Skin","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("Skin","Command","/Skin",CMDSTR.iSKN_Command,50,FileName);
	// -----
	CMDSTR.iSMB_Switch				= GetPrivateProfileInt("SpawnMob","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("SpawnMob","Command","/SpawnMob",CMDSTR.iSMB_Command,50,FileName);
	// -----
	CMDSTR.iSTS_Switch				= GetPrivateProfileInt("Status","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("Status","Command","/Status",CMDSTR.iSTS_Command,50,FileName);
	// -----
	CMDSTR.iINV_Switch				= GetPrivateProfileInt("ClearInv","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("ClearInv","Command","/ClearInv",CMDSTR.iINV_Command,50,FileName);
	// -----
	CMDSTR.iEXT_Switch				= GetPrivateProfileInt("ExitGame","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("ExitGame","Command","/Exit",CMDSTR.iEXT_Command,50,FileName);
	// -----
	CMDSTR.iABT_Switch				= GetPrivateProfileInt("AboutServer","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("AboutServer","Command","/About",CMDSTR.iABT_Command,50,FileName);
	// -----
	CMDSTR.iSLV_Switch				= GetPrivateProfileInt("SetLevel","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("SetLevel","Command","/SetLevel",CMDSTR.iSLV_Command,50,FileName);
	// -----
	CMDSTR.iLUP_Switch				= GetPrivateProfileInt("SetLvlUpPoints","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("SetLvlUpPoints","Command","/SetLvlUpPoints",CMDSTR.iLUP_Command,50,FileName);
	// -----
	CMDSTR.iABF_Switch				= GetPrivateProfileInt("AddBuff","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("AddBuff","Command","/AddBuff",CMDSTR.iABF_Command,50,FileName);
	// -----
	CMDSTR.iSMN_Switch				= GetPrivateProfileInt("Summon","SwitchCommand",1,FileName);
	CMDSTR.iSMN_RequiredLevel		= GetPrivateProfileInt("Summon","NeedLevel",100,FileName);
	CMDSTR.iSMN_EnalbeCost			= GetPrivateProfileInt("Summon","EnableCost",1,FileName);
	CMDSTR.iSMN_RequiredCost		= GetPrivateProfileInt("Summon","NeedZen",1000,FileName);
	CMDSTR.iSMN_DWMonster			= GetPrivateProfileInt("Summon","SummonDWMonster",100,FileName);
	CMDSTR.iSMN_SMMonster			= GetPrivateProfileInt("Summon","SummonSMMonster",100,FileName);
	CMDSTR.iSMN_GMMonster			= GetPrivateProfileInt("Summon","SummonGMMonster",100,FileName);
	CMDSTR.iSMN_DKMonster			= GetPrivateProfileInt("Summon","SummonDKMonster",100,FileName);
	CMDSTR.iSMN_BKMonster			= GetPrivateProfileInt("Summon","SummonBKMonster",100,FileName);
	CMDSTR.iSMN_BMMonster			= GetPrivateProfileInt("Summon","SummonBMMonster",100,FileName);
	CMDSTR.iSMN_FEMonster			= GetPrivateProfileInt("Summon","SummonFEMonster",100,FileName);
	CMDSTR.iSMN_MEMonster			= GetPrivateProfileInt("Summon","SummonMEMonster",100,FileName);
	CMDSTR.iSMN_HEMonster			= GetPrivateProfileInt("Summon","SummonHEMonster",100,FileName);
	CMDSTR.iSMN_MGMonster			= GetPrivateProfileInt("Summon","SummonMGMonster",100,FileName);
	CMDSTR.iSMN_DMMonster			= GetPrivateProfileInt("Summon","SummonDMMonster",100,FileName);
	CMDSTR.iSMN_DLMonster			= GetPrivateProfileInt("Summon","SummonDLMonster",100,FileName);
	CMDSTR.iSMN_LEMonster			= GetPrivateProfileInt("Summon","SummonLEMonster",100,FileName);
	CMDSTR.iSMN_SUMonster			= GetPrivateProfileInt("Summon","SummonSUMonster",100,FileName);
	CMDSTR.iSMN_BSMonster			= GetPrivateProfileInt("Summon","SummonBSMonster",100,FileName);
	CMDSTR.iSMN_DSMonster			= GetPrivateProfileInt("Summon","SummonDSMonster",100,FileName);
	CMDSTR.iSMN_RFMonster			= GetPrivateProfileInt("Summon","SummonRFMonster",100,FileName);
	CMDSTR.iSMN_FMMonster			= GetPrivateProfileInt("Summon","SummonFMMonster",100,FileName);
	GetPrivateProfileStringA		("Summon","Command","/Summon",CMDSTR.iSMN_Command,50,FileName);
	// -----
	CMDSTR.iMRG_RequiredLevel		= GetPrivateProfileInt("Marriage","NeedLevel",100,FileName);
	GetPrivateProfileStringA		("Marriage","CommandPro","/MPropse",CMDSTR.iMRG_CommandPro,50,FileName);
	GetPrivateProfileStringA		("Marriage","CommandAcp","/MAccept",CMDSTR.iMRG_CommandAcp,50,FileName);
	GetPrivateProfileStringA		("Marriage","CommandDiv","/MDivorce",CMDSTR.iMRG_CommandDiv,50,FileName);
	GetPrivateProfileStringA		("Marriage","CommandOnl","/MarryOnline",CMDSTR.iMRG_CommandOnl,50,FileName);
	GetPrivateProfileStringA		("Marriage","CommandTrc","/MarryTrace",CMDSTR.iMRG_CommandTrc,50,FileName);
	// -----
	CMDSTR.iRSP_Switch				= GetPrivateProfileInt("RestorePoints","SwitchCommand",1,FileName);
	CMDSTR.iRSP_RequiredLevel		= GetPrivateProfileInt("RestorePoints","NeedLevel",100,FileName);
	CMDSTR.iRSP_EnalbeCost			= GetPrivateProfileInt("RestorePoints","EnableCost",1,FileName);
	CMDSTR.iRSP_RequiredCost		= GetPrivateProfileInt("RestorePoints","NeedZen",1000,FileName);
	GetPrivateProfileStringA		("RestorePoints","Command","/RestorePoints",CMDSTR.iRSP_Command,50,FileName);
	// -----
	CMDSTR.iMVA_Switch				= GetPrivateProfileInt("MovesAll","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("MovesAll","Command","/WarpAll",CMDSTR.iMVA_Command,50,FileName);
	// -----
	CMDSTR.iCST_Switch				= GetPrivateProfileInt("CreateSet","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("CreateSet","Command","/CreateSet",CMDSTR.iCST_Command,50,FileName);
	// -----
	CMDSTR.iCBN_Switch				= GetPrivateProfileInt("ChatBlock","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("ChatBlock","Command","/ChatBlock",CMDSTR.iCBN_Command,50,FileName);
	// -----
	CMDSTR.iCRV_Switch				= GetPrivateProfileInt("ClrVault","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("ClrVault","Command","/ClrVault",CMDSTR.iCRV_Command,50,FileName);
	// -----
	CMDSTR.iGRI_Switch				= GetPrivateProfileInt("GetRegInfo","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("GetRegInfo","Command","/GetRegInfo",CMDSTR.iGRI_Command,50,FileName);
	// -----
	CMDSTR.iSTC_Switch				= GetPrivateProfileInt("Statistics","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("Statistics","Command","/Statistics",CMDSTR.iSTC_Command,50,FileName);
	// -----
	CMDSTR.iCBK_Switch				= GetPrivateProfileInt("CommandBlock","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("CommandBlock","Command","/CommandBlock",CMDSTR.iCBK_Command,50,FileName);
	// -----
	CMDSTR.iBAN_Switch				= GetPrivateProfileInt("BanAccount","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("BanAccount","UnbanCommand","/Unban",CMDSTR.iBAN_CommandUBN,50,FileName);
	GetPrivateProfileStringA		("BanAccount","BanCommand","/Ban",CMDSTR.iBAN_CommandBAN,50,FileName);
	// -----
	CMDSTR.iBLK_Switch				= GetPrivateProfileInt("BlockChar","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("BlockChar","UnblockCommand","/Unblock",CMDSTR.iBLK_CommandUBK,50,FileName);
	GetPrivateProfileStringA		("BlockChar","BlockCommand","/Block",CMDSTR.iBLK_CommandBLK,50,FileName);
	// -----
	CMDSTR.iRST_Switch				= GetPrivateProfileInt("Reset","SwitchCommand",1,FileName);
	CMDSTR.iRST_RequiredLevel		= GetPrivateProfileInt("Reset","NeedLevel",400,FileName);
	CMDSTR.iRST_RequiredCost		= GetPrivateProfileInt("Reset","NeedZen",1000000,FileName);
	CMDSTR.iRST_Type				= GetPrivateProfileInt("Reset","Type",1,FileName);
	CMDSTR.iRST_PointsPerReset		= GetPrivateProfileInt("Reset","PointsPerReset",500,FileName);
	CMDSTR.iRST_MaxReset			= GetPrivateProfileInt("Reset","MaxResets",999,FileName);
	GetPrivateProfileStringA		("Reset","Command","/Reset",CMDSTR.iRST_Command,50,FileName);
	// -----
	CMDSTR.iGMV_Switch				= GetPrivateProfileInt("GMove","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("GMove","Command","/GMove",CMDSTR.iGMV_Command,50,FileName);
	// -----
	CMDSTR.iTRCSwitch				= GetPrivateProfileInt("Trace","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("Trace","Command","/Trace",CMDSTR.iTRC_Command,50,FileName);
	// -----
	CMDSTR.iGDWSwitch				= GetPrivateProfileInt("GuildWar","SwitchCommand",1,FileName);
	GetPrivateProfileStringA		("GuildWar","WarCommand","/RequestWar",CMDSTR.iGDW_CommandWAR,50,FileName);
	GetPrivateProfileStringA		("GuildWar","EndCommand","/EndWar",CMDSTR.iGDW_CommandEND,50,FileName);
	// -----
	CMDSTR.iDRE_Switch				= GetPrivateProfileInt("DropEvent","SwitchCommand",1,FileName);
	CMDSTR.iDRE_EItDropIndex		= GetPrivateProfileInt("DropEvent","ItemDropIndex",1,FileName);
	CMDSTR.iDRE_EItDropType			= GetPrivateProfileInt("DropEvent","ItemDropType",1,FileName);
	CMDSTR.iDRE_TimeBtwDrops		= GetPrivateProfileInt("DropEvent","TimeBetweenDrops",1,FileName) * 1000;
	GetPrivateProfileStringA		("DropEvent","Command","/DropEvent",CMDSTR.iDRE_Command,50,FileName);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------