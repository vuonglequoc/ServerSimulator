//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Game Main Initialization
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------------------
#ifndef GAMEMAIN_H
#define GAMEMAIN_H
// -----------------------------------------------------------------------------------------------------------------------------------------

#include "wsJoinServerCli.h"
#include "wsGameServer.h"
#include "MapClass.h"
#include "NSerialCheck.h"
#include "../Test/CharacterInfo.h"
#include "CLoginCount.h"
#include "WzUdp.h"
#include "WhisperCash.h"
#include "DbSave.h"
#include "SimpleModulus.h"
#include "MonsterAttr.h"
#include "MonsterSetBase.h"
#include "ItemBagEx.h"
#include "Randomizer.h"


struct PMSG_FRIEND_STATE
{
	PBMSG_HEAD h;
	char Name[10];
	BYTE State;
};
// -----------------------------------------------------------------------------------------------------------------------------------------
enum MU_EVENT_TYPE
{
	MU_EVENT_ALL							= 0x00,
	MU_EVENT_DEVILSQUARE					= 0x01,
	MU_EVENT_BLOODCASTLE					= 0x02,
	MU_EVENT_ATTACKEVENTTROOP				= 0x03,
	MU_EVENT_GOLDENTROOP					= 0x04,
	MU_EVENT_WHITEMAGETROOP					= 0x05,
	MU_EVENT_LOVEPANGPANG					= 0x06,
	MU_EVENT_FIRECRACKER					= 0x07,
	MU_EVENT_MEDALION						= 0x08,
	MU_EVENT_XMASSTAR						= 0x09,
	MU_EVENT_HEARTOFLOVE					= 0x0A,
	MU_EVENT_SAY_HAPPYNEWYEAR				= 0x0B,
	MU_EVENT_SAY_MERRYXMAS					= 0x0C,
	MU_EVENT_CHAOSCASTLE					= 0x0D,
	MU_EVENT_CHRISTMAS_RIBBONBOX			= 0x0E,
	MU_EVENT_VALENTINESDAY_CHOCOLATEBOX		= 0x0F,
	MU_EVENT_WHITEDAY_CANDYBOX				= 0x10,
	MU_EVENT_GMBOX							= 0x11,
	MU_EVENT_SANTAINVASION					= 0x12,
};
// -----------------------------------------------------------------------------------------------------------------------------------------
enum MU_ETC_TYPE
{
	MU_ETC_ALL 								= 0x00,
	MU_ETC_CREATECHARACTER					= 0x01,
	MU_ETC_GUILD							= 0x02,
	MU_ETC_TRADE							= 0x03,
	MU_ETC_USECHAOSBOX						= 0x04,
	MU_ETC_PERSONALSHOP						= 0x05,
	MU_ETC_PKITEMDROP						= 0x06,
	MU_ETC_ITEMDROPRATE						= 0x07,
	MU_ETC_SPEEDHACK						= 0x08,
	MU_ETC_GAMEGUARD						= 0x09,
};
// -----------------------------------------------------------------------------------------------------------------------------------------
#if(GS_CASTLE)
extern BOOL				gIsDropSetItemInCastleHuntZone;
extern int				gSetItemInCastleHuntZoneDropRate;
extern int				gSetItemInCastleHuntZoneDropLevel;
extern BOOL				g_bDoCastleDeepEvent;
extern int				g_iCastleItemMixLimit;
extern CItemBagEx*		CastleItemMixItemBag;
extern CItemBagEx*		CastleHuntZoneBossItemBag;
#endif
// -----------------------------------------------------------------------------------------------------------------------------------------
/*
IPAdress[0][256]	=	JoinServer
IPAdress[1][256]	=	DataServer01
IPAdress[2][256]	=	DataServer02
IPAdress[3][256]	=	ExDB
*/
extern char				IPAdress[4][256];
// -----------------------------------------------------------------------------------------------------------------------------------------
/*
Connected[0]	=	JoinServer
Connected[1]	=	DataServer
Connected[2]	=	DevilSquare Event
Connected[3]	=	Is DevilSquare Event
Connected[4]	=	Event ChipServer
Connected[5]	=	Is Event ChipServer
*/
extern BYTE				Connected[6];
// -----------------------------------------------------------------------------------------------------------------------------------------
/*
Port[0]			=	GameServer;
Port[1]			=	JoinServer;
Port[2]			=	DataServer01;
Port[3]			=	DataServer02;
Port[4]			=	ExDb;
Port[5]			=	Ranking;
Port[6]			=	EventChip;
*/
extern int				Port[7];
// -----------------------------------------------------------------------------------------------------------------------------------------
extern unsigned char	bCanConnectMember;
extern int				gSpeedHackPenalty;
extern int				gMonsterHp;
extern unsigned char	gLanguage;
extern unsigned char	gChaosEvent;
extern char				gChaosEventServerIp[20];
extern char				gDevilSquareEventServerIp[20];
extern char				gHackLogServerIp[20];
extern char				gEventChipServerIp[20];
extern unsigned char	gWriteSkillLog;
extern unsigned char	bIsIgnorePacketSpeedHackDetect;
extern unsigned char	gIsKickDetecHackCountLimit;
extern unsigned char	gDoPShopOpen;
extern unsigned char	gWriteChatLog;
extern unsigned char	gDisconnectHackUser;
extern unsigned char	g_iBlockCastleSiegeMapEnter;
extern unsigned char	GSInfoSendFlag;
extern int				gFSGate;
// -----------------------------------------------------------------------------------------------------------------------------------------
extern CwsGameServer	wsGServer;			 //-- GameServer
extern wsJoinServerCli	wsJServerCli;		 //-- Join Server
extern wsJoinServerCli	wsDataCli;			 //-- DataServer
extern wsJoinServerCli	wsExDbCli;			 //-- Extra DataBase Server
extern wsJoinServerCli	wsRServerCli;		 //-- Ranking Server
extern wsJoinServerCli	wsEvenChipServerCli; //-- Event Server Chip
// -----------------------------------------------------------------------------------------------------------------------------------------
extern MapClass			MapC[MAX_NUMBER_MAP];
extern CMonsterAttr		gMAttr;
extern CMonsterSetBase	gMSetBase;
extern CWhisperCash		WhisperCash;
extern Randomizer		g_Rand;
#if(GS_CASTLE)
//extern CCastleDeepEvent g_CastleDeepEvent;
#endif
// -----------------------------------------------------------------------------------------------------------------------------------------
extern WzUdp			gUdpSocCE;
extern WzUdp			gUdpSocCER;
extern WzUdp			gUdpSocCRank;
extern WzUdp			gUdpSocCRankR;
// -----------------------------------------------------------------------------------------------------------------------------------------
extern CGuildClass		Guild;
extern NSerialCheck		gNSerialCheck[OBJMAX];
extern CLoginCount		gLCount[3];
// -----------------------------------------------------------------------------------------------------------------------------------------
extern char				szGameServerExeVersion[8];
extern char				szGameServerExeSerial[24];
extern char				szServerName[50];
// -----------------------------------------------------------------------------------------------------------------------------------------
extern unsigned char	g_iUseCharacterAutoRecuperationSystem;
extern short			g_iCharacterRecuperationMaxLevel;
extern unsigned char	g_iServerGroupGuildChatting;
extern unsigned char	g_iServerGroupUnionChatting;
extern int				g_iCrywolfApplyMvpBenefit;
extern int				g_iCrywolfApplyMvpPenalty;
extern int				g_iSkillDistanceCheck;
extern int				g_iSkillDistanceCheckTemp;
extern int				g_iSkillDistanceKick;
extern int				g_iSkillDistanceKickCount;
extern int				g_iSkillDiatanceKickCheckTime;
extern DWORD			dwgCheckSum[MAX_CHECKSUM_KEY];
extern char				ConnectServerIP[20];
extern int				ConnectServerPort;
extern short			gGameServerCode;
extern BOOL				g_bCastleGuildDestoyLimit;
extern int				gItemNumberCount;
extern int				gYear;
// -----------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------------------
extern int				giKundunRefillHPSec;
extern int				giKundunRefillHP;
extern int				giKundunRefillHPTime;
extern int				giKundunHPLogSaveTime;
extern int				g_ShieldSystemOn;
extern int				g_iDamageDevideToSDRate;
extern int				g_iDamageDevideToHPRate;
extern float			g_fSuccessAttackRateOption;
extern int				g_iSDChargingOption;
extern int				g_iConstNumberOfShieldPoint;
extern int				g_ShieldAutoRefillOn;
extern int				g_ShieldAutoRefillOnSafeZone;
extern BOOL				g_bShieldComboMissOptionOn;
extern int				g_iShieldPotionLv1MixSuccessRate;
extern int				g_iShieldPotionLv1MixMoney;
extern int				g_iShieldPotionLv2MixSuccessRate;
extern int				g_iShieldPotionLv2MixMoney;
extern int				g_iShieldPotionLv3MixSuccessRate;
extern int				g_iShieldPotionLv3MixMoney;
extern int				g_iShieldGageConstA;
extern int				g_iShieldGageConstB;
extern char				gMapName[MAX_NUMBER_MAP][255];
// -----------------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------
// Load Item Bags
//--------------------------------------------
extern CItemBagEx *		LuckyBoxItemBag;
extern CItemBagEx *		StarOfXMasItemBag;
extern CItemBagEx *		FireCrackerItemBag;
extern CItemBagEx *		HeartOfLoveItemBag;
extern CItemBagEx *		SilverMedalItemBag;
extern CItemBagEx *		GoldMedalItemBag;
extern CItemBagEx *		HeavenBoxItemBag;
extern CItemBagEx *		KudnunBox01ItemBag;
extern CItemBagEx *		KudnunBox02ItemBag;
extern CItemBagEx *		KudnunBox03ItemBag;
extern CItemBagEx *		KudnunBox04ItemBag;
extern CItemBagEx *		KudnunBox05ItemBag;
extern CItemBagEx *		DarkLordHeartItemBag;
extern CItemBagEx *		WizRing40ItemBag;
extern CItemBagEx *		WizRing80ItemBag;
extern CItemBagEx *		SealedGoldenItemBag;
extern CItemBagEx *		SealedSilverItemBag;
extern CItemBagEx *		ShiningJwlryItemBag;
extern CItemBagEx *		ElegantJwlryItemBag;
extern CItemBagEx *		SteelJewelryItemBag;
extern CItemBagEx *		OldJewelryItemBag;
extern CItemBagEx *		RedRibbonBoxItemBag;
extern CItemBagEx *		GreenRibbonBoxItemBag;
extern CItemBagEx *		BlueRibbonBoxItemBag;
extern CItemBagEx *		PinkChocolateBoxItemBag;
extern CItemBagEx *		RedChocolateBoxItemBag;
extern CItemBagEx *		BlueChocolateBoxItemBag;
extern CItemBagEx *		LightPurpleCandyBoxItemBag;
extern CItemBagEx *		VermilionCandyBoxItemBag;
extern CItemBagEx *		DeepBlueCandyBoxItemBag;
extern CItemBagEx *		HalloweenDayBoxItemBag;
extern CItemBagEx *		CherryBlossomBoxItemBag;
extern CItemBagEx *		GreenBoxItemBag;
extern CItemBagEx *		RedBoxItemBag;
extern CItemBagEx *		PurpleBoxItemBag;
extern CItemBagEx *		GreenChaosBoxItemBag;
extern CItemBagEx *		RedChaosBoxItemBag;
extern CItemBagEx *		PurpleChaosBoxItemBag;
extern CItemBagEx *		GameMasterBoxItemBag;
extern CItemBagEx *		BlueHappyPouchBag;
extern CItemBagEx *		RedHappyPouchBag;
// -----------------------------------------------------------------------------------------------------------------------------------------
extern char				szGameServerVersion[12];
extern char				szClientVersion[8];
extern unsigned char	bCanTrade;
extern unsigned char	bCanChaosBox;
extern unsigned char	bCanWarehouseLock;
extern unsigned char	gEnableBattleSoccer;
extern unsigned char	gCreateCharacter;
extern unsigned char	gGuildCreate;
extern unsigned char	gGuildDestroy;
extern BOOL				gEnableCheckPenetrationSkill;
extern BOOL				gItemSerialCheck;
extern bool				Unknown592;
extern int				MapMinUserLevel[MAX_NUMBER_MAP];
// -----------------------------------------------------------------------------------------------------------------------------------------
extern short			gLootingTime;
extern short			gItemDropPer;
// -----------------------------------------------------------------------------------------------------------------------------------------
extern int				gCharacterDeleteMinLevel;
extern int				gGuildCreateLevel;
extern int				gAddExperience;
extern int				gQuestNPCTeleportTime;
extern int				gZenDurationTime;
extern int				gMonsterHPAdjust;
// -----------------------------------------------------------------------------------------------------------------------------------------
extern DWORD			gLevelExperience[401];
extern DWORD			gAttackSpeedTimeLimit;
extern DWORD			gHackCheckCount;
extern float			gDecTimePerAttackSpeed;
extern int				gMinimumAttackSpeedTime;
extern int				gDetectedHackKickCount;
// -----------------------------------------------------------------------------------------------------------------------------------------
extern std::vector<WzUdp*>m_vecUDPSockets;
extern std::vector<CItemBag*>m_vecItemBags;
extern std::vector<CItemBagEx*>m_vecItemBagsEx;
// -----------------------------------------------------------------------------------------------------------------------------------------
extern char				ConnectGameNotice[50];
extern unsigned short	UDPPort;
extern int				HRB_EnableCheckSum;
extern int				HRB_LifeSucessRate;
extern int				HRB_MaxAdditionalOption;
extern int				HRB_FixPotions;
extern int				HRB_DeleteCharNoPass;
extern int				HRB_DeleteGuildNoPass;
extern int				HRB_LeaveGuildNoPass;
extern int				HRB_GMCanUseMWarp;
extern int				HRB_CheckMoveLevelMsg;
extern int				HRB_CheckMoveMoneyMsg;
extern int				HRB_CharMaxLevel;
extern int				HRB_SoulSuccessRate;
// -----------------------------------------------------------------------------------------------------------------------------------------
enum eUDPType
{
	eUDPConnectServer,
};
// -----------------------------------------------------------------------------------------------------------------------------------------
void	CreateForm();
void	gSetDate();
void	GameMainInit(HWND hWnd);
void	GameMainFree();
bool	GMJoinServerConnect(char* ConnectServer, DWORD wMsg);
bool	GMRankingServerConnect(char* RankingServer, DWORD wMsg);
bool	GMEventChipServerConnect(char* ServerIP, DWORD wMsg);
bool	GMDataServerConnect(char* ConnectServer, WPARAM wMsg);
bool	ExDataServerConnect(char* ConnectServer, DWORD wMsg);
bool	GameMainServerCreate(DWORD sMsg, DWORD cMsg);
int		GMServerMsgProc( WPARAM wParam, LPARAM lParam);
void	GMClientMsgProc( WPARAM wParam, LPARAM lParam);
void	GMJoinClientMsgProc(WPARAM wParam, LPARAM lParam);
void	GMRankingClientMsgProc(WPARAM wParam, LPARAM lParam);
void	GMEventChipClientMsgProc(WPARAM wParam, LPARAM lParam);
void	ExDataClientMsgProc(WPARAM wParam, LPARAM lParam);
void	GMDataClientMsgProc(WPARAM wParam, LPARAM lParam);
void	ReadServerInfo();
void	ReadCommonServerInfo();
void	GameServerInfoSendStop();
void	GameServerInfoSendStart();
void	GameServerInfoSend();
void	CreateEventItemBags();
void	ReadEventInfo(MU_EVENT_TYPE eEventType);
void	ReadGameEtcInfo(MU_ETC_TYPE eGameEtcType);
// -----------------------------------------------------------------------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------------------------------------------------------------------