#include "../Header Files/stdafx.h"
#include "../Header Files/GameMain.H"
#include "../Header Files/Functions.h"
#include "../Header Files/Packets.h"
#include "../Header Files/AcceptIp.H"
#include "../Header Files/Main.H"
#include "../Header Files/LogProc.H"
#include "../Header Files/wsJoinServerCli.H"
#include "../Header Files/DSProtocol.H"
#include "../Header Files/CLoginCount.H"
#include "../Header Files/MapClass.H"
#include "../Test/CharacterInfo.h"
#include "../Header Files/prodef.h"
#include "../Header Files/wsGameServer.H"
#include "../Header Files/GoldenEvent.H"
#include "../Header Files/WhisperCash.H"
#include "../Header Files/JSProtocol.H"
#include "../Header Files/ChaosCastle.H"
#include "../Header Files/DevilSquare.H"
#include "../Header Files/Gate.H"
#include "../Header Files/user.H"
#include "../Header Files/Event.H"
#include "../Header Files/PacketCheckSum.H"
#include "../Header Files/SendHackLog.H"
#include "../Header Files/SimpleModulus.H"
#include "../Header Files/MapServerManager.H"
#include "../Header Files/QuestInfo.H"
#include "../Header Files/SkillHitBox.H"
#include "../Header Files/ObjUseSkill.H"
#include "../Header Files/EventManagement.H"
#include "../Header Files/SetItemOption.h"
#include "../Header Files/EDSProtocol.H"
#include "../Header Files/MonsterItemMng.H"
#include "../Header Files/ConMember.H"
#include "../Header Files/BattleSoccerManager.H"
#include "../Header Files/GuildClass.H"
#include "../Header Files/MoveSystem.H"
#include "../Header Files/ChaosBox.H"
#include "../Header Files/BloodCastle.H"
#include "../Header Files/Shop.H"
#include "../Header Files/NpcTalk.H"
#include "../Header Files/ItemAddOption.H"
#include "../Header Files/CastleSiege.H"
#include "../Header Files/Crywolf.H"
#include "../Header Files/CrywolfSync.H"
#include "../Header Files/Kanturu.H"
#include "../Header Files/KanturuMonsterMng.H"
#include "../Header Files/SkillAdditionInfo.H"
#include "../Header Files/TMonsterSkillElement.H"
#include "../Header Files/TMonsterSkillUnit.H"
#include "../Header Files/TMonsterSkillManager.H"
#include "../Header Files/TMonsterAIElement.H"
#include "../Header Files/TMonsterAIAutomata.H"
#include "../Header Files/TMonsterAIUnit.H"
#include "../Header Files/TMonsterAIRule.H"
#include "../Header Files/TMonsterAIGroup.H"
#include "../Header Files/CashShop.H"
#include "../Header Files/CashItemPeriodSystem.H"
#include "../Header Files/CashLotterySystem.H"
#include "../Header Files/CastleDeepEvent.H"
#include "../Header Files/ItemDropManager.H"
#include "../Header Files/Defines.H"
#include "../Header Files/ReadScript.H"
#include "../Header Files/IllusionTemple.H"
#include "../Header Files/Randomizer.H"
#include "../Header Files/Defines.H"
#include "../Header Files/GenSystem.H"
#include "../Header Files/RingAttackEvent.H"
#include "../Header Files/SantaEvent.H"
#include "../Header Files/CWhatsUpDummyServer.H"
#include "../Header Files/UnderWorld.h"
#include "../Header Files/RedDragon.h"
#include "../Header Files/News.h"
#include "../Header Files/Firewall.h"
#include "../Header Files/Raklion.h"
#include "../Header Files/MarrySystem.h"
#include "../Header Files/Pets.h"
#include "../Header Files/SpecialItems.h"
#include "../Header Files/ItemDropManager.h"
#include "../Header Files/EventManagement.h"
#include "../Header Files/BlueEvent.h"
#include "../Header Files/InitConfigs.h"
#include "../Header Files/GoldenArcher.h"
#include "../Header Files/LuckyCoin.h"
#include "../Header Files/PKSystem.h"
#include "../Header Files/SQuest.h"
#include "../Header Files/SendProto.H"
#include "../Test/Scripting.h"
#include "../Test/CharacterInfo.h"
#include "../Header Files/Structures.H"


#if(GS_CASTLE)
BOOL			gIsDropSetItemInCastleHuntZone;
int				gSetItemInCastleHuntZoneDropRate;
int				gSetItemInCastleHuntZoneDropLevel;
BOOL			g_bDoCastleDeepEvent;
int				g_iCastleItemMixLimit = 1;
CItemBagEx*		CastleItemMixItemBag;
CItemBagEx*		CastleHuntZoneBossItemBag;
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------
/*
IPAdress[0]	::	JoinServer
IPAdress[1]	::	DataServer01
IPAdress[2]	::	DataServer02
IPAdress[3]	::	ExDB
*/
char			IPAdress[4][256];
// ------------------------------------------------------------------------------------------------------------------------------------------
/*
Connected[0] ::	JoinServer
Connected[1] ::	DataServer
Connected[2] ::	DevilSquare Event
Connected[3] ::	Is DevilSquare Event
Connected[4] ::	Event ChipServer
Connected[5] ::	Is Event ChipServer
*/
BYTE			Connected[6];
// ------------------------------------------------------------------------------------------------------------------------------------------
/*
Port[0]		 ::	GameServer
Port[1]		 ::	JoinServer
Port[2]		 ::	DataServer01
Port[3]		 ::	DataServer02
Port[4]		 ::	ExDb
Port[5]		 ::	Ranking
Port[6]		 ::	EventChip
*/
int				Port[7];
// ------------------------------------------------------------------------------------------------------------------------------------------
unsigned char	bCanConnectMember;
int				gSpeedHackPenalty;
int				gMonsterHp;
unsigned char	gLanguage;
unsigned char	gChaosEvent;
char			gChaosEventServerIp[20];
char			gDevilSquareEventServerIp[20];
char			gHackLogServerIp[20];
char			gEventChipServerIp[20];
unsigned char	gWriteSkillLog;
unsigned char	bIsIgnorePacketSpeedHackDetect;
unsigned char	gIsKickDetecHackCountLimit;
unsigned char	gDoPShopOpen;
unsigned char	gWriteChatLog;
unsigned char	gDisconnectHackUser;
unsigned char	g_iBlockCastleSiegeMapEnter;
unsigned char	GSInfoSendFlag;
int				gFSGate;
// -----------------------------------------------------------------------------------------------------------------------------------------
CwsGameServer	wsGServer;			 //-- GameServer
wsJoinServerCli wsJServerCli;		 //-- Join Server
wsJoinServerCli wsDataCli;			 //-- DataServer
wsJoinServerCli wsExDbCli;			 //-- Extra DataBase Server
wsJoinServerCli wsRServerCli;		 //-- Ranking Server
wsJoinServerCli wsEvenChipServerCli; //-- Event Server Chip
// -----------------------------------------------------------------------------------------------------------------------------------------
MapClass		MapC[MAX_NUMBER_MAP];
CMonsterAttr	gMAttr;
CMonsterSetBase gMSetBase;
CWhisperCash	WhisperCash;
Randomizer		g_Rand;
// ------------------------------------------------------------------------------------------------------------------------------------------
WzUdp			gUdpSocCE;
WzUdp			gUdpSocCER;
WzUdp			gUdpSocCRank;
WzUdp			gUdpSocCRankR;
// ------------------------------------------------------------------------------------------------------------------------------------------
CGuildClass		Guild;
NSerialCheck	gNSerialCheck[OBJMAX];
CLoginCount		gLCount[3];
// ------------------------------------------------------------------------------------------------------------------------------------------
char			szGameServerExeVersion[8];
char			szGameServerExeSerial[24];
char			szServerName[50];
// ------------------------------------------------------------------------------------------------------------------------------------------
unsigned char	g_iUseCharacterAutoRecuperationSystem;
short			g_iCharacterRecuperationMaxLevel;
unsigned char	g_iServerGroupGuildChatting;
unsigned char	g_iServerGroupUnionChatting;
int				g_iCrywolfApplyMvpBenefit;
int				g_iCrywolfApplyMvpPenalty;
int				g_iSkillDistanceCheck;
int				g_iSkillDistanceCheckTemp;
int				g_iSkillDistanceKick;
int				g_iSkillDistanceKickCount;
int				g_iSkillDiatanceKickCheckTime;
DWORD			dwgCheckSum[MAX_CHECKSUM_KEY];
char			ConnectServerIP[20];
int				ConnectServerPort;
short			gGameServerCode;
BOOL			g_bCastleGuildDestoyLimit;
int				gItemNumberCount;
int				gYear;
// ------------------------------------------------------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------------------------------------------------------
int				giKundunRefillHPSec;
int				giKundunRefillHP;
int				giKundunRefillHPTime;
int				giKundunHPLogSaveTime;
int				g_ShieldSystemOn;
int				g_iDamageDevideToSDRate;
int				g_iDamageDevideToHPRate;
float			g_fSuccessAttackRateOption;
int				g_iSDChargingOption;
int				g_iConstNumberOfShieldPoint;
int				g_ShieldAutoRefillOn;
int				g_ShieldAutoRefillOnSafeZone;
BOOL			g_bShieldComboMissOptionOn;
int				g_iShieldPotionLv1MixSuccessRate;
int				g_iShieldPotionLv1MixMoney;
int				g_iShieldPotionLv2MixSuccessRate;
int				g_iShieldPotionLv2MixMoney;
int				g_iShieldPotionLv3MixSuccessRate;
int				g_iShieldPotionLv3MixMoney;
int				g_iShieldGageConstA;
int				g_iShieldGageConstB;
char			gMapName[MAX_NUMBER_MAP][255];
// ------------------------------------------------------------------------------------------------------------------------------------------
// Load Item Bags
//--------------------------------------------
CItemBagEx *	LuckyBoxItemBag;
CItemBagEx *	StarOfXMasItemBag;
CItemBagEx *	FireCrackerItemBag;
CItemBagEx *	HeartOfLoveItemBag;
CItemBagEx *	SilverMedalItemBag;
CItemBagEx *	GoldMedalItemBag;
CItemBagEx *	HeavenBoxItemBag;
CItemBagEx *	KudnunBox01ItemBag;
CItemBagEx *	KudnunBox02ItemBag;
CItemBagEx *	KudnunBox03ItemBag;
CItemBagEx *	KudnunBox04ItemBag;
CItemBagEx *	KudnunBox05ItemBag;
CItemBagEx *	DarkLordHeartItemBag;
CItemBagEx *	WizRing40ItemBag;
CItemBagEx *	WizRing80ItemBag;
CItemBagEx *	SealedGoldenItemBag;
CItemBagEx *	SealedSilverItemBag;
CItemBagEx *	ShiningJwlryItemBag;
CItemBagEx *	ElegantJwlryItemBag;
CItemBagEx *	SteelJewelryItemBag;
CItemBagEx *	OldJewelryItemBag;
CItemBagEx *	RedRibbonBoxItemBag;
CItemBagEx *	GreenRibbonBoxItemBag;
CItemBagEx *	BlueRibbonBoxItemBag;
CItemBagEx *	PinkChocolateBoxItemBag;
CItemBagEx *	RedChocolateBoxItemBag;
CItemBagEx *	BlueChocolateBoxItemBag;
CItemBagEx *	LightPurpleCandyBoxItemBag;
CItemBagEx *	VermilionCandyBoxItemBag;
CItemBagEx *	DeepBlueCandyBoxItemBag;
CItemBagEx *	HalloweenDayBoxItemBag;
CItemBagEx *	CherryBlossomBoxItemBag;
CItemBagEx *	GreenBoxItemBag;
CItemBagEx *	RedBoxItemBag;
CItemBagEx *	PurpleBoxItemBag;
CItemBagEx *	GreenChaosBoxItemBag;
CItemBagEx *	RedChaosBoxItemBag;
CItemBagEx *	PurpleChaosBoxItemBag;
CItemBagEx *	GameMasterBoxItemBag;
CItemBagEx *	BlueHappyPouchBag;
CItemBagEx *	RedHappyPouchBag;
// -----------------------------------------------------------------------------------------------------------------------------------------
char			szGameServerVersion[12]			= SOFT_VER;
char			szClientVersion[8]				= "000000";
unsigned char	bCanTrade;
unsigned char	bCanChaosBox;
unsigned char	bCanWarehouseLock;
unsigned char	gEnableBattleSoccer;
unsigned char	gCreateCharacter;
unsigned char	gGuildCreate;
unsigned char	gGuildDestroy;
BOOL			gEnableCheckPenetrationSkill	= TRUE;
BOOL			gItemSerialCheck				= FALSE;
bool			Unknown592						= true;
int				MapMinUserLevel[MAX_NUMBER_MAP] = { 0, 20, 15, 10, 80, 0, 0, 60, 130, 0, 160, 10, 36, 80, 130, 170, 210, 310, 15, 30, 100,
													160, 220, 280, 15, 50, 120, 180, 240, 300, 10, 10, 0};
// -----------------------------------------------------------------------------------------------------------------------------------------
short			gLootingTime					= 3;
short			gItemDropPer					= 10;
// -----------------------------------------------------------------------------------------------------------------------------------------
int				gCharacterDeleteMinLevel		= 40;
int				gGuildCreateLevel				= 100;
int				gAddExperience;
int				gQuestNPCTeleportTime			= 60;
int				gZenDurationTime				= 30;
int				gMonsterHPAdjust				= 100;
// -----------------------------------------------------------------------------------------------------------------------------------------
DWORD			gLevelExperience[401];
DWORD			gAttackSpeedTimeLimit			= 130;
DWORD			gHackCheckCount					= 5;
float			gDecTimePerAttackSpeed			= 5.33f;
int				gMinimumAttackSpeedTime			= 8;
int				gDetectedHackKickCount			= 10;
// -----------------------------------------------------------------------------------------------------------------------------------------
std::vector<WzUdp*>m_vecUDPSockets;
std::vector<CItemBag*>m_vecItemBags;
std::vector<CItemBagEx*>m_vecItemBagsEx;
// ------------------------------------------------------------------------------------------------------------------------------------------
char			ConnectGameNotice[50];
unsigned short	UDPPort;
int				HRB_EnableCheckSum;
int				HRB_LifeSucessRate;
int				HRB_MaxAdditionalOption;
int				HRB_FixPotions;
int				HRB_DeleteCharNoPass;
int				HRB_DeleteGuildNoPass;
int				HRB_LeaveGuildNoPass;
int				HRB_GMCanUseMWarp;
int				HRB_CheckMoveLevelMsg;
int				HRB_CheckMoveMoneyMsg;
int				HRB_CharMaxLevel;
int				HRB_SoulSuccessRate;
// -----------------------------------------------------------------------------------------------------------------------------------------
void gSetDate()
{
	tm *	today;
	time_t	ltime;
	// -----
	time(&ltime);
	today			= localtime(&ltime);
	// -----
	today->tm_year	= today->tm_year + 1900;
	gYear			= today->tm_year;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void GameMainInit(HWND hWnd)
{
	int		n;
	int		LevelOver_N;
	// -----
	while (m_vecUDPSockets.size() > 0)
	{
		WzUdp* pUdp = m_vecUDPSockets[m_vecUDPSockets.size() -1];
		m_vecUDPSockets.pop_back();

		delete pUdp;
		pUdp = 0;
	}
	// -----
	srand(time(NULL));
	// -----
	ReadServerInfo();
	// -----
	CLog.Initialize(hWnd);
	// -----
	gSetDate();
	// -----
	WzUdp* pUDPSocket = new WzUdp();
	m_vecUDPSockets.push_back(pUDPSocket);
	// -----
	ReadCommonServerInfo();
	// -----
	#if (SCRIPT_ENABLED == 1)
		ExecCommonFile("GameServer.RCS");
	#endif
	// -----
	g_MapServerManager.LoadData( RFile.szRFileName[16] );
	// -----

	// -----
	if(m_vecUDPSockets[eUDPConnectServer]->CreateSocket() == 0 )
	{
		MessageBoxA(hWnd, "UDP Socket Create Error", "Terminate HRB GameServer", MB_OK);
		return;
	}
	// -----
	if( gUdpSocCER.CreateSocket() == 0 )
	{
		MessageBoxA(hWnd, "UDP Socket Create Error", "Terminate HRB GameServer", MB_OK);
		return;
	}
	// -----
	gUdpSocCER.RecvSet( UDPPort );
	gUdpSocCER.Run();
	// -----
	if ( gUdpSocCE.CreateSocket() == 0)
	{
		MessageBoxA(hWnd, "UDP Socket Create Error", "Terminate HRB GameServer", MB_OK);
		return;
	}
	// -----
	gUdpSocCER.SetProtocolCore(ChaosEventProtocolCore);
	gUdpSocCE.SetProtocolCore(ChaosEventProtocolCore); 
	// -----
	gObjInit();
	// -----
	InitBattleSoccer();
	// -----
	gMAttr.LoadAttr( RFile.szRFileName[17] );
	// -----
	gMSetBase.LoadSetBase( RFile.szRFileName[18] );
	// -----
	g_MonsterItemMng.Init();
	// -----
	gLevelExperience[0]	= 0;
	LevelOver_N			= 1;
	// -----
	for ( n=1;n<HRB_CharMaxLevel+1;n++)
	{
		gLevelExperience[n] = (((n+9)*n)*n)*10;

		if ( n > 255 )
		{
			gLevelExperience[n] += ((((LevelOver_N+9)*LevelOver_N)*LevelOver_N)*1000);
			LevelOver_N++;
		}
	}
	// -----
	ReadMapAttrName();
	// -----
	#if (GS_CASTLE)
		g_Crywolf.LoadCrywolfMapAttr("..\\Data\\Terrains\\Terrain35_PEACE.att", 0);
		g_Crywolf.LoadCrywolfMapAttr("..\\Data\\Terrains\\Terrain35_OCCUPIED.att", 1);
		g_Crywolf.LoadCrywolfMapAttr("..\\Data\\Terrains\\Terrain35_WAR.att", 2);
	#else
		g_Kanturu.LoadKanturuMapAttr("..\\Data\\Terrains\\Terrain40_CLOSE.att", 0);
		g_Kanturu.LoadKanturuMapAttr("..\\Data\\Terrains\\Terrain40_OPEN.att", 1);
	#endif
	// -----
	ShopDataLoad();
	wsGServer.CreateSocket(hWnd);
	wsJServerCli.CreateSocket(hWnd);
	wsDataCli.CreateSocket(hWnd);
	// -----
	wsRServerCli.CreateSocket(hWnd);
	wsEvenChipServerCli.CreateSocket(hWnd);
	GameServerInfoSend();
	AddAllMonsters();
	// -----
	acceptIP.Load( RFile.szRFileName[19] );
	ConMember.Load( RFile.szRFileName[15] );
	GCTeleportSend(gObj, 1, 1, 1, 2, 3);
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void GameMainFree()
{
	gDbSave.End();
	// -----
	GDGameServerInfoSave();
	// -----
	gObjEnd();
	// -----
	ClearBattleSoccer();
}
// -----------------------------------------------------------------------------------------------------------------------------------------
WzUdp* GetUDPSocket(eUDPType _eUDPType)
{
	return (m_vecUDPSockets[_eUDPType]);
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool GMJoinServerConnect(LPSTR ConnectServer, DWORD wMsg)
{
	wsJServerCli.SetProtocolCore(JSProtocolCore);
	// -----
	int bResult = wsJServerCli.Connect(ConnectServer, Port[1], wMsg);
	// -----
	if (!bResult) return FALSE;
	// -----
	JSProto.JServerLoginInfo();
	// -----
	CLog.Connect("LoginServer IP-Address: (%s)", ConnectServer);
	// -----
	return TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool GMRankingServerConnect(char* RankingServer, DWORD wMsg)
{
	wsRServerCli.SetProtocolCore(DevilSquareProtocolCore);
	// -----
	int bResult = wsRServerCli.Connect(RankingServer, Port[5], wMsg);
	// -----
	if (!bResult) return FALSE;
	// -----
	CLog.Connect("RankingServer IP-Address: (%s)", RankingServer);
	// -----
	return TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool GMEventChipServerConnect(char* ServerIP, DWORD wMsg)
{
	wsEvenChipServerCli.SetProtocolCore(EventChipEventProtocolCore);
	// -----
	int bResult = wsEvenChipServerCli.Connect(ServerIP, Port[6], wMsg);
	// -----
	if (!bResult) return FALSE;
	// -----
	CLog.Connect("EventChipServer IP-Address: (%s)", ServerIP);
	// -----
	return TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool GMDataServerConnect(char* ConnectServer, WPARAM wMsg)
{
	wsDataCli.SetProtocolCore(DataServerProtocolCore);
	// -----
	int bResult = wsDataCli.Connect(ConnectServer, Port[2], wMsg);
	// -----
	if (!bResult)
	{
		CLog.Error("DataServer Connecting Failed IP-Address: (%s) Port: (%d)", ConnectServer, Port[2]);
		return FALSE;
	}
	// -----
	DataServerLogin(0);
	// -----
	CLog.Connect("DataServer IP-Address: (%s) Port: (%d)", ConnectServer, Port[2]);
	// -----
	return TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool ExDataServerConnect(char* ConnectServer, DWORD wMsg)
{
	wsExDbCli.SetProtocolCore(ExDataServerProtocolCore);
	// -----
	int bResult = wsExDbCli.Connect(ConnectServer, Port[4], wMsg);
	// -----
	if (!bResult)
	{
		CLog.Error("ExDB Connect Failed IP-Address: (%s) Port: (%d)", ConnectServer, Port[4]);
		return FALSE;
	}
	// -----
	ExDataServerLogin();
	// -----
	CLog.Connect("ExDB Connected Successfully.");
	// -----
	return TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool GameMainServerCreate(DWORD sMsg, DWORD cMsg)
{
	gDbSave.Begin();
	// -----
	CLog.Connect("GameServer Create Port: (%d)", Port[0]);
	// -----
	return TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
int GMServerMsgProc( WPARAM wParam, LPARAM lParam)
{
	int bResult = lParam & 0xFFFF & 0xFFFF;
	// -----
	return bResult;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void GMClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	int wp = wParam;
	int lp = lParam;
	// -----
	switch ( lParam & 0xFFFF & 0xFFFF)
	{
		SOCKET socket;
		// -----
		case 1:
			wsGServer.DataRecv(wParam);
			break;
		// -----
		case 2:
			wsGServer.FDWRITE_MsgDataSend(wParam);
			break;
		// -----
		case 32:
			socket = gObjGetSocket(wParam);
			break;
		// -----
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void GMJoinClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			wsJServerCli.DataRecv();
			break;
		// -----
		case 2:
			wsJServerCli.FDWRITE_MsgDataSend();
			break;
		// -----
		case 32:
			CLog.Disconnect("GameServer Closed :: Authentication Server is Closed.");
			SendMessage(gMainApp.m_hWnd, WM_CLOSE, 0, 0);
		// -----
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void GMRankingClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			wsRServerCli.DataRecv();
			break;
		// -----
		case 2:
			wsRServerCli.FDWRITE_MsgDataSend();
			break;
		// -----
		case 32:
			Connected[3] = FALSE;
		// -----
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void GMEventChipClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			wsEvenChipServerCli.DataRecv();
			break;
		// -----
		case 2:
			wsEvenChipServerCli.FDWRITE_MsgDataSend();
			break;
		// -----
		case 32:
			Connected[5] = FALSE;
		// -----
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void ExDataClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			wsExDbCli.DataRecv();
			break;
		// -----
		case 2:
			wsExDbCli.FDWRITE_MsgDataSend();
			break;
		// -----
		case 32:
			CLog.Disconnect("GamerServer Closed :: DataServer is Closed.");
			// -----
			wsExDbCli.Close();
			// -----
			for ( int i=0; i<OBJMAX; i++)
			{
				if ( gObjIsConnectedGP(i) == TRUE )
				{
					PMSG_FRIEND_STATE pMsg;
					
					pMsg.h.set((LPBYTE)&pMsg, 0xC4 , sizeof(pMsg) );
					pMsg.State = -4;

					gSendProto.DataSend(i, (unsigned char*)&pMsg, sizeof(pMsg) );

					gObj[i].m_FriendServerOnline = FRIEND_SERVER_STATE_OFFLINE;
				}
			}
			// -----
			CLog.Disconnect("Error() : Friend Server Down (State Send OK)");
			break;
		// -----
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void GMDataClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			wsDataCli.DataRecv();
			break;
		// -----
		case 2:
			wsDataCli.FDWRITE_MsgDataSend();
			break;
		// -----
		case 32:
			CLog.Disconnect("GamerServer Closed :: DataServer is Closed.");
		// -----
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void ReadServerInfo()
{
	gGameServerCode			= GetPrivateProfileInt(GS_INFO_HDR,"ServerCode",0, RFile.szRFileName[0] );
	bCanConnectMember		= GetPrivateProfileInt(GS_INFO_HDR,"ConnectMemberLoad",0, RFile.szRFileName[0] );
	// -----------------------------------------------------------------------------------------------------------------------
	GetPrivateProfileString	( GS_INFO_HDR, "ServerName",		"", szServerName,			50, RFile.szRFileName[0] );
	GetPrivateProfileString	( GS_INFO_HDR, "ClientExeVersion",	"", szGameServerExeVersion,	8,	RFile.szRFileName[0] );
	GetPrivateProfileString	( GS_INFO_HDR, "ClientExeSerial",	"", szGameServerExeSerial,	20,	RFile.szRFileName[0] );
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void ReadCommonServerInfo()
{
	char	szTemp[256];
	char *	cvstr;
	// -----
	ReadServerInfo();
	// -----
	if(g_SimpleModulusCS.LoadDecryptionKey( RFile.szRFileName[20] ) == FALSE)
	{
		CLog.MsgBox("Dec1.dat file not found");
	}
	if(g_SimpleModulusSC.LoadEncryptionKey( RFile.szRFileName[21] ) == FALSE)
	{
		CLog.MsgBox("Enc2.dat file not found");
	}
	// -----
	memcpy(dwgCheckSum, RFile.szRFileName[1], sizeof(RFile.szRFileName[1]));
	// -----
	lMsg.LoadWTF( RFile.szRFileName[2] );
	// -----
	OpenItemScript( RFile.szRFileName[3] );
	// -----
	MagicDamageC.LogSkillList( RFile.szRFileName[4] );
	// -----
	gMove.LoadMoveFile( RFile.szRFileName[5] );
	// -----
	GateC.LoadGate( RFile.szRFileName[6] );
	// -----
	g_QuestInfo.LoadQuestInfo( RFile.szRFileName[7] );
	// -----
	gSetItemOption.LoadOptionInfo( RFile.szRFileName[8] );
	// -----
	gSetItemOption.LoadTypeInfo( RFile.szRFileName[9] );
	// -----
	SkillSpearHitBox.Load( RFile.szRFileName[10] );
	// -----
	SkillElectricSparkHitBox.Load( RFile.szRFileName[11] );
	// -----
	g_kJewelOfHarmonySystem.LoadScript( RFile.szRFileName[12] );
	// -----
	g_kJewelOfHarmonySystem.LoadScriptOfSmelt( RFile.szRFileName[13] );
	// -----
	g_Item380System.Load380ItemOptionInfo( RFile.szRFileName[14] );
	// -----
	g_ItemAddOption.Load( RFile.szRFileName[14] );
	// -----
	ConMember.Load( RFile.szRFileName[15] );
	// -----
	cvstr = strtok(szGameServerExeVersion, ".");
	szClientVersion[0] = cvstr[0];
	cvstr = strtok(NULL, ".");
	szClientVersion[1] = cvstr[0];
	szClientVersion[2] = cvstr[1];
	cvstr = strtok(NULL, ".");
	szClientVersion[3] = cvstr[0];
	szClientVersion[4] = cvstr[1];
	// -----
	TMonsterSkillElement::LoadData( RFile.szRFileName[22] );
	TMonsterSkillUnit::LoadData( RFile.szRFileName[23] );
	TMonsterSkillManager::LoadData( RFile.szRFileName[24] );
	TMonsterAIElement::LoadData( RFile.szRFileName[25] );
	TMonsterAIAutomata::LoadData( RFile.szRFileName[26] );
	TMonsterAIUnit::LoadData( RFile.szRFileName[27] );
	TMonsterAIRule::LoadData( RFile.szRFileName[28] );
	TMonsterAIGroup::LoadData( RFile.szRFileName[29] );
	// -----
#if(GS_CASTLE)
	g_Crywolf.LoadData( RFile.szRFileName[30] );
#endif
	// -----
	g_SkillAdditionInfo.Load( RFile.szRFileName[31] );
	// -----
#if(GS_CASTLE==0)
	g_Kanturu.LoadData( RFile.szRFileName[32] );
	g_KanturuMonsterMng.LoadData( RFile.szRFileName[33] );
#endif
	// -----
	gCreateCharacter = GetPrivateProfileInt(GS_INFO_HDR,"CreateCharacter",0,RFile.szRFileName[0]);
	// -----
	gCharacterDeleteMinLevel = GetPrivateProfileInt(GS_INFO_HDR,"CharacterDeleteMinLevel",0,RFile.szRFileName[0]);
	// -----
#if (!GS_CASTLE)
	int iMaxUser = GetPrivateProfileInt(GS_INFO_HDR,"NumberOfMaxUser",5, RFile.szRFileName[0]);
#else
	int iMaxUser = GetPrivateProfileInt(GS_INFO_HDR,"NumberOfCastleSiegeMaxUser",1500, RFile.szRFileName[0]);
#endif

	if(iMaxUser >= 0 && iMaxUser <= OBJMAXUSER)
	{
		gServerMaxUser = iMaxUser;
	}
	else if ( iMaxUser < 0 )
	{
		iMaxUser = 0;
	}
	else if ( iMaxUser > OBJMAXUSER )
	{
		iMaxUser = 1000;
	}

	// Guild Settingss
	g_iServerGroupGuildChatting = GetPrivateProfileInt(GS_INFO_HDR,"ServerGroupGuildChatting",0, RFile.szRFileName[0]);
	g_iServerGroupUnionChatting = GetPrivateProfileInt(GS_INFO_HDR,"ServerGroupUnionChatting",0, RFile.szRFileName[0]);
	gGuildCreate = GetPrivateProfileInt(GS_INFO_HDR,"GuildCreate",0,RFile.szRFileName[0]);
	gGuildDestroy = GetPrivateProfileInt(GS_INFO_HDR,"GuildDestroy",0,RFile.szRFileName[0]);
	gGuildCreateLevel = GetPrivateProfileInt(GS_INFO_HDR,"GuildCreateLevel",0,RFile.szRFileName[0]);
	g_bCastleGuildDestoyLimit = GetPrivateProfileInt(GS_INFO_HDR, "CastleOwnerGuildDestroyLimit", 0, RFile.szRFileName[0]);

	gItemSerialCheck = GetPrivateProfileInt(GS_INFO_HDR, "ItemSerialCheck", 0, RFile.szRFileName[0]);
	gAddExperience = GetPrivateProfileInt(GS_INFO_HDR,"AddExperience",0,RFile.szRFileName[0]);
	bCanTrade = GetPrivateProfileInt(GS_INFO_HDR,"Trade",0,RFile.szRFileName[0]);
	bCanChaosBox = GetPrivateProfileInt(GS_INFO_HDR,"ChaosBox",0, RFile.szRFileName[0]);
	gChaosEvent = GetPrivateProfileInt(GS_INFO_HDR,"ChaosEvent",0, RFile.szRFileName[0]);

	GetPrivateProfileString(GS_INFO_HDR, "ChaosEventServer", "210.181.89.241", gChaosEventServerIp, 20, RFile.szRFileName[0]);

	gMonsterHp = GetPrivateProfileInt(GS_INFO_HDR,"MonsterHp",0, RFile.szRFileName[0]);

	gLootingTime = GetPrivateProfileInt(GS_INFO_HDR,"LootingTime",0, RFile.szRFileName[0]);

	gItemDropPer = GetPrivateProfileInt(GS_INFO_HDR,"ItemDropPer",0, RFile.szRFileName[0]);

	gZenDurationTime = GetPrivateProfileInt(GS_INFO_HDR,"ZenDurationTime",0, RFile.szRFileName[0]);

	// Antihack Settings
	gAttackSpeedTimeLimit = GetPrivateProfileInt(GS_INFO_HDR,"AttackSpeedTimeLimit",800, RFile.szRFileName[0]);
	bIsIgnorePacketSpeedHackDetect = GetPrivateProfileInt(GS_INFO_HDR,"IsIgnorePacketHackDetect",0, RFile.szRFileName[0]);
	gHackCheckCount = GetPrivateProfileInt(GS_INFO_HDR,"HackCheckCount",5, RFile.szRFileName[0]);
	gMinimumAttackSpeedTime = GetPrivateProfileInt(GS_INFO_HDR,"MinimumAttackSpeedTime",200, RFile.szRFileName[0]);
	gDetectedHackKickCount = GetPrivateProfileInt(GS_INFO_HDR,"DetectedHackKickCount",10, RFile.szRFileName[0]);
	gIsKickDetecHackCountLimit = GetPrivateProfileInt(GS_INFO_HDR,"IsKickDetecHackCountLimit",0, RFile.szRFileName[0]);

	// HAck Log Server
	gWriteChatLog = GetPrivateProfileInt(GS_INFO_HDR,"WriteChatLog",0, RFile.szRFileName[0]);

	// Hack Settings
	gDisconnectHackUser = GetPrivateProfileInt(GS_INFO_HDR,"DisconnectHackUser",0, RFile.szRFileName[0]);
	GetPrivateProfileString(GS_INFO_HDR, "DecTimePerAttackSpeed", "5.33", szTemp, 5, RFile.szRFileName[0]);
	gDecTimePerAttackSpeed = (float)atof(szTemp);
	gSpeedHackPenalty = GetPrivateProfileInt(GS_INFO_HDR,"SpeedHackPenalty",0, RFile.szRFileName[0]);

	// Skill Log
	gWriteSkillLog = GetPrivateProfileInt(GS_INFO_HDR,"WriteSkillLog",0, RFile.szRFileName[0]);

	// Marlon Teleport
	gQuestNPCTeleportTime = GetPrivateProfileInt(GS_INFO_HDR,"QuestNPCTeleportTime",0, RFile.szRFileName[0]);

	// KUNDUN Settings
	giKundunRefillHPSec = GetPrivateProfileInt(GS_INFO_HDR,"KundunRefillHPSec",500, COMMONSERVER_PATH);
	giKundunRefillHP = GetPrivateProfileInt(GS_INFO_HDR,"KundunRefillHP",10000, COMMONSERVER_PATH);
	giKundunRefillHPTime = GetPrivateProfileInt(GS_INFO_HDR,"KundunRefillHPTime",600, COMMONSERVER_PATH);
	giKundunHPLogSaveTime = GetPrivateProfileInt(GS_INFO_HDR,"KundunHPLogSaveTime",120, COMMONSERVER_PATH);

	// Crywolf Benefit
	g_iCrywolfApplyMvpBenefit = GetPrivateProfileInt(GS_INFO_HDR,"CrywolfApplyMvpBenefit",0, COMMONSERVER_PATH);

		int iCrywolfPlusChaosRateBenefit = GetPrivateProfileInt(GS_INFO_HDR,"CrywolfPlusChaosRateBenefit",0, COMMONSERVER_PATH);
		int iCrywolfMonHPRateBenefit = GetPrivateProfileInt(GS_INFO_HDR,"CrywolfMonHPRateBenefit", 100, COMMONSERVER_PATH);
		int iCrywolfKundunHPRefillState = GetPrivateProfileInt(GS_INFO_HDR,"CrywolfKundunHPRefillBenefit", 1, COMMONSERVER_PATH);

		g_CrywolfSync.SetKundunHPRefillState(iCrywolfKundunHPRefillState);
		g_CrywolfSync.SetPlusChaosRate(iCrywolfPlusChaosRateBenefit);
		g_CrywolfSync.SetMonHPBenefitRate(iCrywolfMonHPRateBenefit);

	// Crywolf Penalty
	g_iCrywolfApplyMvpPenalty = GetPrivateProfileInt(GS_INFO_HDR,"CrywolfApplyMvpPenalty",0, COMMONSERVER_PATH);

		int iCrywolfGemDropPenaltyRate = GetPrivateProfileInt(GS_INFO_HDR,"CrwyolfGemDropPenaltyRate",100, COMMONSERVER_PATH);
		int iCrywolfGettingExpPenaltyRate = GetPrivateProfileInt(GS_INFO_HDR,"CrwyolfGettingExpPenaltyRate",100, COMMONSERVER_PATH);

		g_CrywolfSync.SetGemDropPenaltiyRate(iCrywolfGemDropPenaltyRate);
		g_CrywolfSync.SetGettingExpPenaltyRate(iCrywolfGettingExpPenaltyRate);

		// Map Enter Settings
		g_iBlockCastleSiegeMapEnter = GetPrivateProfileInt(GS_INFO_HDR,"BlockCastleSiegeMapEnter",0, COMMONSERVER_PATH);

	
	// Servers
	GetPrivateProfileString("ConnectServerInfo", "IPAdress", "", ConnectServerIP, 20, RFile.szRFileName[0]);
	GetPrivateProfileString("ConnectServerInfo", "GSPORT", "", szTemp, 10, RFile.szRFileName[0]);
	ConnectServerPort = atoi(szTemp);

	m_vecUDPSockets[eUDPConnectServer]->SendSet(ConnectServerIP, ConnectServerPort);
	gUdpSocCE.SendSet(gChaosEventServerIp, 60005);

	Connected[2] = GetPrivateProfileInt(GS_INFO_HDR,"DevilSquareEventConnect", 1, COMMONSERVER_PATH);
	GetPrivateProfileString(GS_INFO_HDR, "DevilSquareEventServer", "127.0.0.1", gDevilSquareEventServerIp, 20, COMMONSERVER_PATH);
	Connected[4] = GetPrivateProfileInt(GS_INFO_HDR,"EventChipServerConnect", 0, COMMONSERVER_PATH);
	GetPrivateProfileString(GS_INFO_HDR, "EventChipServerIp", "127.0.0.1", gEventChipServerIp, 20, COMMONSERVER_PATH);

	// Battle Soccer
	gEnableBattleSoccer = GetPrivateProfileInt(GS_INFO_HDR,"EnableBattleSoccer", 0, RFile.szRFileName[0]);

	// Monster HP Adjust
	gMonsterHPAdjust = GetPrivateProfileInt(GS_INFO_HDR,"MonsterHPAdjust", 100, RFile.szRFileName[0]);

	// Hack Log SERVER
	GetPrivateProfileString(GS_INFO_HDR, "HackLogServer", "127.0.0.1", gHackLogServerIp, 20, COMMONSERVER_PATH);
	gSendHackLog.SendSet(gHackLogServerIp, 60005);

	// Penetration Skill
	gEnableCheckPenetrationSkill = GetPrivateProfileInt(GS_INFO_HDR,"EnableCheckPenetrationSkill", 1, COMMONSERVER_PATH);

	// Shield System
	g_ShieldSystemOn = GetPrivateProfileInt(GS_INFO_HDR,"ShieldSystemOn", 0, COMMONSERVER_PATH);

		g_iDamageDevideToSDRate = GetPrivateProfileInt(GS_INFO_HDR,"DamageDevideToSD", 90, COMMONSERVER_PATH);
		g_iDamageDevideToHPRate = GetPrivateProfileInt(GS_INFO_HDR,"DamageDevideToHP", 10, COMMONSERVER_PATH);
		g_fSuccessAttackRateOption = (double)(GetPrivateProfileInt(GS_INFO_HDR,"SuccessAttackRateOption", 20000, COMMONSERVER_PATH) / 10000 );
		g_iSDChargingOption = GetPrivateProfileInt(GS_INFO_HDR,"SDChargingOption", 1, COMMONSERVER_PATH);
		g_iConstNumberOfShieldPoint = GetPrivateProfileInt(GS_INFO_HDR,"ConstNumberOfShieldPoint", 20, COMMONSERVER_PATH);
		g_ShieldAutoRefillOn = GetPrivateProfileInt(GS_INFO_HDR,"ShieldAutoRefillOn", 0, COMMONSERVER_PATH);
		g_ShieldAutoRefillOnSafeZone = GetPrivateProfileInt(GS_INFO_HDR,"ShieldAutoRefilOnSafeZone", 1, COMMONSERVER_PATH);

	// Shile System Combo
	g_bShieldComboMissOptionOn = GetPrivateProfileInt(GS_INFO_HDR,"ShieldComboMissOptionOn", 0, COMMONSERVER_PATH);

	// Shield Potions
	g_iShieldPotionLv1MixSuccessRate = GetPrivateProfileInt(GS_INFO_HDR,"ShieldPotionLv1MixSuccessRate", 50, COMMONSERVER_PATH);
	g_iShieldPotionLv1MixMoney = GetPrivateProfileInt(GS_INFO_HDR,"ShieldPotionLv1MixMoney", 100000, COMMONSERVER_PATH);

	g_iShieldPotionLv2MixSuccessRate = GetPrivateProfileInt(GS_INFO_HDR,"ShieldPotionLv2MixSuccessRate", 30, COMMONSERVER_PATH);
	g_iShieldPotionLv2MixMoney = GetPrivateProfileInt(GS_INFO_HDR,"ShieldPotionLv2MixMoney", 500000, COMMONSERVER_PATH);

	g_iShieldPotionLv3MixSuccessRate = GetPrivateProfileInt(GS_INFO_HDR,"ShieldPotionLv3MixSuccessRate", 30, COMMONSERVER_PATH);
	g_iShieldPotionLv3MixMoney = GetPrivateProfileInt(GS_INFO_HDR,"ShieldPotionLv3MixMoney", 1000000, COMMONSERVER_PATH);
	
	// Shield GAGE
	g_iShieldGageConstA = GetPrivateProfileInt(GS_INFO_HDR,"ShieldGageConstA", 12, COMMONSERVER_PATH);
	g_iShieldGageConstB = GetPrivateProfileInt(GS_INFO_HDR,"ShieldGageConstB", 30, COMMONSERVER_PATH);

	// Item Bag Load
	CreateEventItemBags();

	gPacketCheckSum.Init();

	gDoPShopOpen = GetPrivateProfileInt(GS_INFO_HDR,"PersonalShopOpen", 0, RFile.szRFileName[0]);

	ReadEventInfo(MU_EVENT_ALL );

	// AutoRecuperation Level
	g_iUseCharacterAutoRecuperationSystem = GetPrivateProfileInt(GS_INFO_HDR,"UseCharacterAutoRecuperationSystem", 0, RFile.szRFileName[0]);
	g_iCharacterRecuperationMaxLevel = GetPrivateProfileInt(GS_INFO_HDR,"CharacterRecuperationMaxLevel", 0, RFile.szRFileName[0]);

	// Skill Check Settings
	g_iSkillDistanceCheck = GetPrivateProfileInt(GS_INFO_HDR,"SkillDistanceCheck", 0, COMMONSERVER_PATH);
	g_iSkillDistanceCheckTemp = GetPrivateProfileInt(GS_INFO_HDR,"SkillDistanceCheckTemp", 2, COMMONSERVER_PATH);
	g_iSkillDistanceKick = GetPrivateProfileInt(GS_INFO_HDR,"SkillDistanceKick", 0, COMMONSERVER_PATH);
	g_iSkillDistanceKickCount = GetPrivateProfileInt(GS_INFO_HDR,"SkillDistanceKickCount", 5, COMMONSERVER_PATH);
	g_iSkillDiatanceKickCheckTime = GetPrivateProfileInt(GS_INFO_HDR,"SkillDistanceKickCheckTime", 10, COMMONSERVER_PATH);

#if( SERVER_COUNTRY == PROTO_JAPAN )
	g_CashShop.CashShopOptioNReload();
	g_CashItemPeriodSystem.Initialize();
	g_CashLotterySystem.Load("..\\Data\\ChaosCardProbability.txt"));
#endif

	// ****************************************************
	//  WolF [Mon 13-12-2010] New Custom Configurations
	// ****************************************************
	GetPrivateProfileString		(GS_INFO_HDR, "ConnectNotice","", ConnectGameNotice, 50, RFile.szRFileName[0]);
	UDPPort						= GetPrivateProfileInt(GS_INFO_HDR, "UDPPort", 60006, RFile.szRFileName[0]);

	HRB_EnableCheckSum			= GetPrivateProfileInt(COMMON_HDR, "UseCheckSum", 0, COMMON_FILE_PATH);
	HRB_LifeSucessRate			= GetPrivateProfileInt(COMMON_HDR, "LifeSuccessRate", 0, COMMON_FILE_PATH);
	HRB_MaxAdditionalOption		= GetPrivateProfileInt(COMMON_HDR, "MaxAddOption", 0, COMMON_FILE_PATH);
	HRB_FixPotions				= GetPrivateProfileInt(COMMON_HDR, "PotionSupport255", 0, COMMON_FILE_PATH);
	HRB_DeleteCharNoPass		= GetPrivateProfileInt(COMMON_HDR, "DeleteCharNoPass", 0, COMMON_FILE_PATH);
	HRB_DeleteGuildNoPass		= GetPrivateProfileInt(COMMON_HDR, "DeleteGuildNoPass", 0, COMMON_FILE_PATH);
	HRB_LeaveGuildNoPass		= GetPrivateProfileInt(COMMON_HDR, "LeaveGuildNoPass", 0, COMMON_FILE_PATH);
	HRB_GMCanUseMWarp			= GetPrivateProfileInt(COMMON_HDR, "GMCanUseMWarp", 0, COMMON_FILE_PATH);
	HRB_CheckMoveLevelMsg		= GetPrivateProfileInt(COMMON_HDR, "CheckMoveLevelMsg", 0, COMMON_FILE_PATH);
	HRB_CheckMoveMoneyMsg		= GetPrivateProfileInt(COMMON_HDR, "CheckMoveMoneyMsg", 0, COMMON_FILE_PATH);
	HRB_CharMaxLevel			= GetPrivateProfileInt(COMMON_HDR, "MaxLevel", 0, COMMON_FILE_PATH);
	HRB_SoulSuccessRate			= GetPrivateProfileInt(COMMON_HDR, "SoulSuccessRate", 0, COMMON_FILE_PATH);
	
	gGAEvent.ReqItem			= GetPrivateProfileInt(COMMON_HDR,"RequiredRenas",5,COMMON_FILE_PATH);


	//////////////////////////////////////////////////////// NEW ZODEA CONFIG 2012 ///////////////////////////////////////////////////////////////////////
	MSTR.NormalPoint[0]			= GetPrivateProfileInt(LVUP_HDR,"DarkWizard",5,RFile.szRFileName[63]);
	MSTR.NormalPoint[1]			= GetPrivateProfileInt(LVUP_HDR,"DarkKnight",5,RFile.szRFileName[63]);
	MSTR.NormalPoint[2]			= GetPrivateProfileInt(LVUP_HDR,"FairyElf",5,RFile.szRFileName[63]);
	MSTR.NormalPoint[3]			= GetPrivateProfileInt(LVUP_HDR,"MagicGladiator",7,RFile.szRFileName[63]);
	MSTR.NormalPoint[4]			= GetPrivateProfileInt(LVUP_HDR,"DarkLord",7,RFile.szRFileName[63]);
	MSTR.NormalPoint[5]			= GetPrivateProfileInt(LVUP_HDR,"Summoner",5,RFile.szRFileName[63]);
	MSTR.NormalPoint[6]			= GetPrivateProfileInt(LVUP_HDR,"RageFighter",7,RFile.szRFileName[63]);
	MSTR.SecondQuestPointPlus[0] = GetPrivateProfileInt(LVUP_HDR,"PlusPointSM",1,RFile.szRFileName[63]);
	MSTR.SecondQuestPointPlus[1] = GetPrivateProfileInt(LVUP_HDR,"PlusPointBK",1,RFile.szRFileName[63]);
	MSTR.SecondQuestPointPlus[2] = GetPrivateProfileInt(LVUP_HDR,"PlusPointME",1,RFile.szRFileName[63]);
	MSTR.SecondQuestPointPlus[3] = GetPrivateProfileInt(LVUP_HDR,"PlusPointBS",1,RFile.szRFileName[63]);
	MSTR.MasterPointsPlus[0]		= GetPrivateProfileInt(LVUP_HDR,"PlusMPointGM",1,RFile.szRFileName[63]);
	MSTR.MasterPointsPlus[1]		= GetPrivateProfileInt(LVUP_HDR,"PlusMPointBM",1,RFile.szRFileName[63]);
	MSTR.MasterPointsPlus[2]		= GetPrivateProfileInt(LVUP_HDR,"PlusMPointHE",1,RFile.szRFileName[63]);
	MSTR.MasterPointsPlus[3]		= GetPrivateProfileInt(LVUP_HDR,"PlusMPointDM",1,RFile.szRFileName[63]);
	MSTR.MasterPointsPlus[4]		= GetPrivateProfileInt(LVUP_HDR,"PlusMPointLE",1,RFile.szRFileName[63]);
	MSTR.MasterPointsPlus[5]		= GetPrivateProfileInt(LVUP_HDR,"PlusMPointDM",1,RFile.szRFileName[63]);
	MSTR.MasterPointsPlus[6]		= GetPrivateProfileInt(LVUP_HDR,"PlusMPointFM",1,RFile.szRFileName[63]);


	// Socket System
	gItemSocket.Load("..\\Data\\SocketItemSystem.txt");

	// MapServer Info File
	g_MapServerManager.LoadData(RFile.szRFileName[16]);

	// Load Characters Class Define Settings
	gCharInfo.Initialize();

	// Load News System
	NewsC.Init();

	// -----
	cGMMng.Init();			//-- GM Manager
	
	// Load Firewall System
	//FirewallC.Init();

#if (GC_CASTLE==0)
	// Read Raklion Drop
	gRaklion.ReadFile();

	// Devil Square
	g_DevilSquare.Init();

	// Golden Archer Init
	gGAEvent.Init(GA_AWARD_FILE_PATH);

	// Lucky Coin
	gLuckyCoin.Init();

	// Blue Event
	gBlueEvent.Init();

	// Event Management
	gEventManager.Init();

#endif

	// Load Marry System
	gMarry.Init();
	
	// Load Pets Info
	gPets.Init();

	// Special Items
	gSpecialItem.Init();
	
	// Item Drop Manager
	gItemDropManager.Init();

	// Gen System Init
	Gens.Init();

	// Init Configs
	gInitConfigs.Init();

	// PK System Init
	gPKSystem.Init();

	// Season 5 Quest
	QuestNew.Init();
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void GameServerInfoSendStop()
{
	GSInfoSendFlag = 0;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void GameServerInfoSendStart()
{
	GSInfoSendFlag = 1;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void GameServerInfoSend()
{
	PMSG_SERVERINFO pMsg;
	// -----
	if ( gDisconnect == TRUE )	return;
	// -----
	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x01;
	pMsg.h.size = sizeof( pMsg );
	// -----
	if ( gObjTotalUser == 0 )
	{
		pMsg.Percent = 0;
	}
	else
	{
		pMsg.Percent = (gObjTotalUser * 100)/gServerMaxUser;
	}
	// -----
	pMsg.ServerCode = gGameServerCode;
	pMsg.UserCount = gObjTotalUser;
	pMsg.AccountCount = gLCount[0].Get();
	pMsg.PCbangCount = gLCount[1].Get();
	pMsg.MaxUserCount = gServerMaxUser;
	// -----
	WzUdp* pUdp = GetUDPSocket(eUDPConnectServer);
	pUdp->SendData((LPBYTE)&pMsg, pMsg.h.size);
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void CreateEventItemBags()
{
	// ---------------------------------------------------------------------------------------
	// [1] EventItemBag :: Box Of Luck :: ItemType:14, Index:11, Level:0
	// ---------------------------------------------------------------------------------------
	if( LuckyBoxItemBag != FALSE ) { delete LuckyBoxItemBag; }

	LuckyBoxItemBag = new CItemBagEx;

	if ( LuckyBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	LuckyBoxItemBag->Init("BoxOfLuck.txt");

	// ---------------------------------------------------------------------------------------
	// [2] EventItemBagEx :: Star Of Scared Birh :: ItemType:14, Index:11, Level:1
	// ---------------------------------------------------------------------------------------
	if( StarOfXMasItemBag != FALSE ) { delete StarOfXMasItemBag; }

	StarOfXMasItemBag = new CItemBagEx;

	if ( StarOfXMasItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	StarOfXMasItemBag->Init("StarOfSacredBirth.txt");

	// ---------------------------------------------------------------------------------------
	// [3] EventItemBagEx :: Fire Cracker :: ItemType:14, Index:11, Level:2
	// ---------------------------------------------------------------------------------------
	if( FireCrackerItemBag != FALSE ) { delete FireCrackerItemBag; }

	FireCrackerItemBag = new CItemBagEx;

	if ( FireCrackerItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	FireCrackerItemBag->Init("FireCracker.txt");

	// ---------------------------------------------------------------------------------------
	// [4] EventItemBagEx :: Heart Of Love :: ItemType:14, Index:11, Level:3
	// ---------------------------------------------------------------------------------------
	if( HeartOfLoveItemBag != FALSE ) { delete HeartOfLoveItemBag; }

	HeartOfLoveItemBag = new CItemBagEx;

	if ( HeartOfLoveItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	HeartOfLoveItemBag->Init("HeartOfLove.txt");

	// ---------------------------------------------------------------------------------------
	// [5] EventItemBagEx :: Silver Medal :: ItemType:14, Index:11, Level:5
	// ---------------------------------------------------------------------------------------
	if( SilverMedalItemBag != FALSE ) { delete SilverMedalItemBag; }

	SilverMedalItemBag = new CItemBagEx;

	if ( SilverMedalItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	SilverMedalItemBag->Init("SilverMedal.txt");

	// ---------------------------------------------------------------------------------------
	// [6] EventItemBagEx :: Gold Medal :: ItemType:14, Index:11, Level:6
	// ---------------------------------------------------------------------------------------
	if( GoldMedalItemBag != FALSE ) { delete GoldMedalItemBag; }

	GoldMedalItemBag = new CItemBagEx;

	if ( GoldMedalItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	GoldMedalItemBag->Init("GoldMedal.txt");

	// ---------------------------------------------------------------------------------------
	// [7] EventItemBag :: Box Of Heaven :: ItemType:14, Index:11, Level:7
	// ---------------------------------------------------------------------------------------
	if( HeavenBoxItemBag != FALSE ) { delete HeavenBoxItemBag; }

	HeavenBoxItemBag = new CItemBagEx;

	if ( HeavenBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	HeavenBoxItemBag->Init("BoxOfHeaven.txt");

	// ---------------------------------------------------------------------------------------
	// [8] EventItemBag :: Box Of Kundun+1 :: ItemType:14, Index:11, Level:8
	// ---------------------------------------------------------------------------------------
	if( KudnunBox01ItemBag != FALSE ) { delete KudnunBox01ItemBag; }

	KudnunBox01ItemBag = new CItemBagEx;

	if ( KudnunBox01ItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	KudnunBox01ItemBag->InitKundun("BoxOfKundum1.txt");

	// ---------------------------------------------------------------------------------------
	// [9] EventItemBag :: Box Of Kundun+2 :: ItemType:14, Index:11, Level:9
	// ---------------------------------------------------------------------------------------
	if( KudnunBox02ItemBag != FALSE ) { delete KudnunBox02ItemBag; }

	KudnunBox02ItemBag = new CItemBagEx;

	if ( KudnunBox02ItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	KudnunBox02ItemBag->InitKundun("BoxOfKundum2.txt");

	// ---------------------------------------------------------------------------------------
	// [10] EventItemBag :: Box Of Kundun+3 :: ItemType:14, Index:11, Level:10
	// ---------------------------------------------------------------------------------------
	if( KudnunBox03ItemBag != FALSE ) { delete KudnunBox03ItemBag; }

	KudnunBox03ItemBag = new CItemBagEx;

	if ( KudnunBox03ItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	KudnunBox03ItemBag->InitKundun("BoxOfKundum3.txt");

	// ---------------------------------------------------------------------------------------
	// [11] EventItemBag :: Box Of Kundun+4 :: ItemType:14, Index:11, Level:11
	// ---------------------------------------------------------------------------------------
	if( KudnunBox04ItemBag != FALSE ) { delete KudnunBox04ItemBag; }

	KudnunBox04ItemBag = new CItemBagEx;

	if ( KudnunBox04ItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	KudnunBox04ItemBag->InitKundun("BoxOfKundum4.txt");

	// ---------------------------------------------------------------------------------------
	// [12] EventItemBag :: Box Of Kundun+5 :: ItemType:14, Index:11, Level:12
	// ---------------------------------------------------------------------------------------
	if( KudnunBox05ItemBag != FALSE ) { delete KudnunBox05ItemBag; }

	KudnunBox05ItemBag = new CItemBagEx;

	if ( KudnunBox05ItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	KudnunBox05ItemBag->InitKundun("BoxOfKundum5.txt");

	// ---------------------------------------------------------------------------------------
	// [13] EventItemBag :: Heart Of DarkLord :: ItemType:14, Index:11, Level:13
	// ---------------------------------------------------------------------------------------
	if( DarkLordHeartItemBag != FALSE ) { delete DarkLordHeartItemBag; }

	DarkLordHeartItemBag = new CItemBagEx;

	if ( DarkLordHeartItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	DarkLordHeartItemBag->Init("HeartOfDarkLord.txt");

	// ---------------------------------------------------------------------------------------
	// [14] EventItemBag :: Wizard Ring 40 :: ItemType:13, Index:20, Level:1
	// ---------------------------------------------------------------------------------------
	if( WizRing40ItemBag != FALSE ) { delete WizRing40ItemBag; }

	WizRing40ItemBag = new CItemBagEx;

	if ( WizRing40ItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	WizRing40ItemBag->Init("WarriorRing40.txt");

	// ---------------------------------------------------------------------------------------
	// [15] EventItemBag :: Wizard Ring 80 :: ItemType:13, Index:20, Level:2
	// ---------------------------------------------------------------------------------------
	if( WizRing80ItemBag != FALSE ) { delete WizRing80ItemBag; }

	WizRing80ItemBag = new CItemBagEx;

	if ( WizRing80ItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	WizRing80ItemBag->Init("WarriorRing80.txt");

	// ---------------------------------------------------------------------------------------
	// [16] EventItemBag :: Sealed Golden Box :: ItemType:14, Index:123
	// ---------------------------------------------------------------------------------------
	if( SealedGoldenItemBag != FALSE ) { delete SealedGoldenItemBag; }

	SealedGoldenItemBag = new CItemBagEx;

	if ( SealedGoldenItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	SealedGoldenItemBag->Init("GoldenBox.txt");

	// ---------------------------------------------------------------------------------------
	// [17] EventItemBag :: Sealed Silver Box :: ItemType:14, Index:124
	// ---------------------------------------------------------------------------------------
	if( SealedSilverItemBag != FALSE ) { delete SealedSilverItemBag; }

	SealedSilverItemBag = new CItemBagEx;

	if ( SealedSilverItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	SealedSilverItemBag->Init("SilverBox.txt");

	// ---------------------------------------------------------------------------------------
	// [18] EventItemBag :: Shining Jewelry Case :: ItemType:14, Index:141
	// ---------------------------------------------------------------------------------------
	if( ShiningJwlryItemBag != FALSE ) { delete ShiningJwlryItemBag; }

	ShiningJwlryItemBag = new CItemBagEx;

	if ( ShiningJwlryItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	ShiningJwlryItemBag->Init("ShiningJewerlyCase.txt");

	// ---------------------------------------------------------------------------------------
	// [19] EventItemBag :: Elegant Jewelry Case :: ItemType:14, Index:142
	// ---------------------------------------------------------------------------------------
	if( ElegantJwlryItemBag != FALSE ) { delete ElegantJwlryItemBag; }

	ElegantJwlryItemBag = new CItemBagEx;

	if ( ElegantJwlryItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	ElegantJwlryItemBag->Init("ElegantJewerlyCase.txt");

	// ---------------------------------------------------------------------------------------
	// [20] EventItemBag :: Steel Jewelry Case :: ItemType:14, Index:143
	// ---------------------------------------------------------------------------------------
	if( SteelJewelryItemBag != FALSE ) { delete SteelJewelryItemBag; }

	SteelJewelryItemBag = new CItemBagEx;

	if ( SteelJewelryItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	SteelJewelryItemBag->Init("SteelJewerlyCase.txt");

	// ---------------------------------------------------------------------------------------
	// [21] EventItemBag :: Old Jewelry Case :: ItemType:14, Index:144
	// ---------------------------------------------------------------------------------------
	if( OldJewelryItemBag != FALSE ) { delete OldJewelryItemBag; }

	OldJewelryItemBag = new CItemBagEx;

	if ( OldJewelryItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	OldJewelryItemBag->Init("OldJewerlyCase.txt");

	// ---------------------------------------------------------------------------------------
	// [22] EventItemBag :: Red Ribbon Box :: ItemType:12, Index:32
	// ---------------------------------------------------------------------------------------
	if( RedRibbonBoxItemBag != FALSE ) { delete RedRibbonBoxItemBag; }

	RedRibbonBoxItemBag = new CItemBagEx;

	if ( RedRibbonBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	RedRibbonBoxItemBag->Init("RedRibbonBox.txt");

	// ---------------------------------------------------------------------------------------
	// [23] EventItemBag :: Green Ribbon Box :: ItemType:12, Index:33
	// ---------------------------------------------------------------------------------------
	if( GreenRibbonBoxItemBag != FALSE ) { delete GreenRibbonBoxItemBag; }

	GreenRibbonBoxItemBag = new CItemBagEx;

	if ( GreenRibbonBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	GreenRibbonBoxItemBag->Init("GreenRibbonBox.txt");

	// ---------------------------------------------------------------------------------------
	// [24] EventItemBag :: Blue Ribbon Box :: ItemType:12, Index:34
	// ---------------------------------------------------------------------------------------
	if( BlueRibbonBoxItemBag != FALSE ) { delete BlueRibbonBoxItemBag; }

	BlueRibbonBoxItemBag = new CItemBagEx;

	if ( BlueRibbonBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	BlueRibbonBoxItemBag->Init("BlueRibbonBox.txt");

	// ---------------------------------------------------------------------------------------
	// [25] EventItemBag :: Pink Chocolate Box :: ItemType:14, Index:32, Level:0
	// ---------------------------------------------------------------------------------------
	if( PinkChocolateBoxItemBag != FALSE ) { delete PinkChocolateBoxItemBag; }

	PinkChocolateBoxItemBag = new CItemBagEx;

	if ( PinkChocolateBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	PinkChocolateBoxItemBag->Init("PinkChocolateBox.txt");

	// ---------------------------------------------------------------------------------------
	// [26] EventItemBag :: Red Chocolate Box :: ItemType:14, Index:33, Level:0
	// ---------------------------------------------------------------------------------------
	if( RedChocolateBoxItemBag != FALSE ) { delete RedChocolateBoxItemBag; }

	RedChocolateBoxItemBag = new CItemBagEx;

	if ( RedChocolateBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	RedChocolateBoxItemBag->Init("RedChocolateBox.txt");

	// ---------------------------------------------------------------------------------------
	// [27] EventItemBag :: Blue Chocolate Box :: ItemType:14, Index:34, Level:0
	// ---------------------------------------------------------------------------------------
	if( BlueChocolateBoxItemBag != FALSE ) { delete BlueChocolateBoxItemBag; }

	BlueChocolateBoxItemBag = new CItemBagEx;

	if ( BlueChocolateBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	BlueChocolateBoxItemBag->Init("BlueChocolateBox.txt");

	// ---------------------------------------------------------------------------------------
	// [28] EventItemBag :: Light Purple Candy Box :: ItemType:14, Index:32, Level:1
	// ---------------------------------------------------------------------------------------
	if( LightPurpleCandyBoxItemBag != FALSE ) { delete LightPurpleCandyBoxItemBag; }

	LightPurpleCandyBoxItemBag = new CItemBagEx;

	if ( LightPurpleCandyBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	LightPurpleCandyBoxItemBag->Init("LightPurpleCandyBox.txt");

	// ---------------------------------------------------------------------------------------
	// [29] EventItemBag :: Vermilion Candy Box :: ItemType:14, Index:33, Level:1
	// ---------------------------------------------------------------------------------------
	if( VermilionCandyBoxItemBag != FALSE ) { delete VermilionCandyBoxItemBag; }

	VermilionCandyBoxItemBag = new CItemBagEx;

	if ( VermilionCandyBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	VermilionCandyBoxItemBag->Init("VermilionCandyBox.txt");

	// ---------------------------------------------------------------------------------------
	// [30] EventItemBag :: Deep Blue Candy Box :: ItemType:14, Index:34, Level:1
	// ---------------------------------------------------------------------------------------
	if( DeepBlueCandyBoxItemBag != FALSE ) { delete DeepBlueCandyBoxItemBag; }

	DeepBlueCandyBoxItemBag = new CItemBagEx;

	if ( DeepBlueCandyBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	DeepBlueCandyBoxItemBag->Init("DeepBlueCandyBox.txt");

	// ---------------------------------------------------------------------------------------
	// [31] EventItemBag :: Hallowin Day Box :: ItemType:14, Index:45
	// ---------------------------------------------------------------------------------------
	if( HalloweenDayBoxItemBag != FALSE ) { delete HalloweenDayBoxItemBag; }

	HalloweenDayBoxItemBag = new CItemBagEx;

	if ( HalloweenDayBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	HalloweenDayBoxItemBag->Init("HalloweenBox.txt");

	// ---------------------------------------------------------------------------------------
	// [32] EventItemBag :: Cherry Blossom Box :: ItemType:14, Index:84
	// ---------------------------------------------------------------------------------------
	if( CherryBlossomBoxItemBag != FALSE ) { delete CherryBlossomBoxItemBag; }

	CherryBlossomBoxItemBag = new CItemBagEx;

	if ( CherryBlossomBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	CherryBlossomBoxItemBag->Init("CherryBlossomBox.txt");

	// ---------------------------------------------------------------------------------------
	// [33] EventItemBag :: Green Box :: ItemType:14, Index:157
	// ---------------------------------------------------------------------------------------
	if( GreenBoxItemBag != FALSE ) { delete GreenBoxItemBag; }

	GreenBoxItemBag = new CItemBagEx;

	if ( GreenBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	GreenBoxItemBag->Init("GreenBox.txt");

	// ---------------------------------------------------------------------------------------
	// [34] EventItemBag :: Red Box :: ItemType:14, Index:158
	// ---------------------------------------------------------------------------------------
	if( RedBoxItemBag != FALSE ) { delete RedBoxItemBag; }

	RedBoxItemBag = new CItemBagEx;

	if ( RedBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	RedBoxItemBag->Init("RedBox.txt");

	// ---------------------------------------------------------------------------------------
	// [35] EventItemBag :: Purple Box :: ItemType:14, Index:159
	// ---------------------------------------------------------------------------------------
	if( PurpleBoxItemBag != FALSE ) { delete PurpleBoxItemBag; }

	PurpleBoxItemBag = new CItemBagEx;

	if ( PurpleBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	PurpleBoxItemBag->Init("PurpleBox.txt");

	// ---------------------------------------------------------------------------------------
	// [36] EventItemBag :: Green Chaos Box :: ItemType:14, Index:55
	// ---------------------------------------------------------------------------------------
	if( GreenChaosBoxItemBag != FALSE ) { delete GreenChaosBoxItemBag; }

	GreenChaosBoxItemBag = new CItemBagEx;

	if ( GreenChaosBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	GreenChaosBoxItemBag->Init("GreenChaosBox.txt");

	// ---------------------------------------------------------------------------------------
	// [37] EventItemBag :: Red Chaos Box :: ItemType:14, Index:56
	// ---------------------------------------------------------------------------------------
	if( RedChaosBoxItemBag != FALSE ) { delete RedChaosBoxItemBag; }

	RedChaosBoxItemBag = new CItemBagEx;

	if ( RedChaosBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	RedChaosBoxItemBag->Init("RedChaosBox.txt");

	// ---------------------------------------------------------------------------------------
	// [38] EventItemBag :: Purple Chaos Box :: ItemType:14, Index:57
	// ---------------------------------------------------------------------------------------
	if( PurpleChaosBoxItemBag != FALSE ) { delete PurpleChaosBoxItemBag; }

	PurpleChaosBoxItemBag = new CItemBagEx;

	if ( PurpleChaosBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	PurpleChaosBoxItemBag->Init("PurpleChaosBox.txt");

	// ---------------------------------------------------------------------------------------
	// [39] EventItemBag :: GameMaster Box :: ItemType:14, Index:52
	// ---------------------------------------------------------------------------------------
	if( GameMasterBoxItemBag != FALSE ) { delete GameMasterBoxItemBag; }

	GameMasterBoxItemBag = new CItemBagEx;

	if ( GameMasterBoxItemBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	GameMasterBoxItemBag->Init("GameMasterBox.txt");

	// ---------------------------------------------------------------------------------------
	// [40] EventItemBag :: Blue Happy Pouch Box :: ItemType:14, Index:11, Level:14
	// ---------------------------------------------------------------------------------------
	if( BlueHappyPouchBag != FALSE ) { delete BlueHappyPouchBag; }

	BlueHappyPouchBag = new CItemBagEx;

	if ( BlueHappyPouchBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	BlueHappyPouchBag->Init("BlueHappyPouchBag.txt");

	// ---------------------------------------------------------------------------------------
	// [41] EventItemBag :: Blue Happy Pouch Box :: ItemType:14, Index:11, Level:15
	// ---------------------------------------------------------------------------------------
	if( RedHappyPouchBag != FALSE ) { delete RedHappyPouchBag; }

	RedHappyPouchBag = new CItemBagEx;

	if ( RedHappyPouchBag == NULL )
	{
		CLog.MsgBox("CItemBag :: Not Allocated in Memory");
		return;
	}

	RedHappyPouchBag->Init("RedHappyPouchBag.txt");
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void ReadEventInfo(MU_EVENT_TYPE eEventType)
{
	switch(eEventType)
	{
		case 0: //All Events
			
#if (GS_CASTLE)
			gIsDropSetItemInCastleHuntZone = GetPrivateProfileInt(GS_INFO_HDR,"IsDropSetItemInCastleHuntZone", 0, COMMONSERVER_PATH);
			gSetItemInCastleHuntZoneDropRate = GetPrivateProfileInt(GS_INFO_HDR,"SetItemInCastleHuntZoneDropRate", 1, COMMONSERVER_PATH);
			gSetItemInCastleHuntZoneDropLevel = GetPrivateProfileInt(GS_INFO_HDR,"SetItemInCastleHuntZoneDropLevel", 75, COMMONSERVER_PATH);
			GetPrivateProfileString(GS_INFO_HDR, "CastleDeepEvent", "0", szTemp, 5, COMMONSERVER_PATH);
			g_bDoCastleDeepEvent = atoi(szTemp);
			g_CastleDeepEvent.Load("..\\Data\\CastleDeepEvent.dat");
			g_CastleDeepEvent.EnableEvent(g_bDoCastleDeepEvent);
#endif

						
#if(!GS_CASTLE)
			g_DevilSquare.Load("..\\Data\\Events\\DevilSquare.dat");
			g_IllusionTemple.Load("..\\Data\\IllusionTemple.dat");
#endif
			break;

		case 1: //Devil Square
			g_DevilSquare.Load("..\\Data\\Events\\DevilSquare.dat");
			break;

		case 2: //Blood Castle
			break;

		case 3: //Attack Event
			gUnderworld.Init();
			break;

		case 4: //Golden Event
			gGoldenEvent.Init();
			break;

		case 5: //Ring Event
			gRingAttackEvent.Init();
			break;

		case 6: // Red Dragon
			gRedDragon.Init();
			break;

		case 7: //Fire Cracker
			// ----
			break;

		case 8: //Medal Event
			// ----
			break;

		case 9: //XMax Event
			// ----
			break;

		case 10: //Heart Of Love Event
			// ----
			break;

		case 11: //Happy New Year Event
			// ----
			break;

		case 12: //Merry  XMax Event NPC talk
			// ----
			break;

		case 13: //Chaos Castle
			// ----
			break;

		case 14:	// Ribbon Box
			// ----
			break;

		case 15:	// Chocolate Box
			// ----
			break;

		case 16:	// Candy Box
			// ----
			break;

		case 18:	//Santa Event
			// ----
			break;
	}
}
// -----------------------------------------------------------------------------------------------------------------------------------------
void ReadGameEtcInfo(MU_ETC_TYPE eGameEtcType)
{
	switch ( eGameEtcType )
	{
		case 0:
			gGuildCreate = GetPrivateProfileInt(GS_INFO_HDR,"GuildCreate",0,RFile.szRFileName[0]);
			gGuildDestroy = GetPrivateProfileInt(GS_INFO_HDR,"GuildDestroy",0,RFile.szRFileName[0]);
			gGuildCreateLevel = GetPrivateProfileInt(GS_INFO_HDR,"GuildCreateLevel",0,RFile.szRFileName[0]);
			g_bCastleGuildDestoyLimit = GetPrivateProfileInt(GS_INFO_HDR, "CastleOwnerGuildDestroyLimit", 1, RFile.szRFileName[0]);

			gItemSerialCheck = GetPrivateProfileInt(GS_INFO_HDR,"ItemSerialCheck",0,RFile.szRFileName[0]);
			gAddExperience = GetPrivateProfileInt(GS_INFO_HDR,"AddExperience",0,RFile.szRFileName[0]);
			bCanTrade = GetPrivateProfileInt(GS_INFO_HDR,"Trade",0,RFile.szRFileName[0]);
			bCanChaosBox = GetPrivateProfileInt(GS_INFO_HDR,"ChaosBox",0, RFile.szRFileName[0]);
			gChaosEvent = GetPrivateProfileInt(GS_INFO_HDR,"ChaosEvent",0, RFile.szRFileName[0]);

			gCreateCharacter = GetPrivateProfileInt(COMMON_HDR,"CreateCharacter",0,RFile.szRFileName[0]);

			gDoPShopOpen = GetPrivateProfileInt(COMMON_HDR, "PersonalShopOpen", 0, RFile.szRFileName[0]);

			gItemDropPer = GetPrivateProfileInt(COMMON_HDR, "ItemDropPer", 0, RFile.szRFileName[0]);

			gAttackSpeedTimeLimit = GetPrivateProfileInt(GS_INFO_HDR, "AttackSpeedTimeLimit", 800, RFile.szRFileName[0]);
			bIsIgnorePacketSpeedHackDetect = GetPrivateProfileInt(GS_INFO_HDR, "IsIgnorePacketHackDetect", 0, RFile.szRFileName[0]);
			gHackCheckCount = GetPrivateProfileInt(GS_INFO_HDR, "HackCheckCount", 5, RFile.szRFileName[0]);
			gMinimumAttackSpeedTime = GetPrivateProfileInt(GS_INFO_HDR, "MinimumAttackSpeedTime", 200 , RFile.szRFileName[0]);
			gDetectedHackKickCount = GetPrivateProfileInt(GS_INFO_HDR, "DetectedHackKickCount", 10, RFile.szRFileName[0]);
			gIsKickDetecHackCountLimit = GetPrivateProfileInt(GS_INFO_HDR, "IsKickDetecHackCountLimit", 0, RFile.szRFileName[0]);
			break;

		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
	}
}