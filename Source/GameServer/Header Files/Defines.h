/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Project:		Zodea MU GameServer - Season 6
 * Developer:	RealCoderZ MU Development © 2011
 * Hint:		Definitions For Zodea GameServer
 * Coded By:	WolF & mesosa
 */
#ifndef DEFINES_HEADER
#define DEFINES_HEADER

/* * * * * * * * * * * * * * * * * * *
 *	Integers Declarations
 */
#define MAX_STRING_LEN			255
#define Unk238					6
#define MAX_SENDBUFF_SIZE		7024
#define MAX_IO_BUFFER_SIZE		6000
#define MAX_IO_THREAD_HANDLES	16
#define OP_SEND 				0
#define OP_RECV 				1
#define	DWAPI					static DWORD WINAPI
// ---------------------------------------------------------------------------------------------------------------------------------------------------
/* * * * * * * * * * * * * * * * * * *
 *	Strings Declarations
 */
#define SRV_NAME			"MuBK"
#define SOFT_VER			"1.01.01"
//#define GS_CODER			"WolF & M.E.S"
#define GS_CODER			"VuongLQ"
#define VA_START			_crt_va_start
#define VA_END				_crt_va_end
#define BeginThread					_beginthread
#define EndThread					_endthread

// Headers
#define GS_INFO_HDR			"GameServerInfo"
#define COMMON_HDR			"Common"
#define IP_ADR_HDR			"IP-Address"
#define PORT_HDR			"Ports"
#define SKILL_INFO_HDR		"SkillInfo"
#define SECURITY_HDR		"Security"
#define MOSS_HDR			"MossMerchant"
#define RAKLION_HDR			"Raklion"
#define CHAOS_HDR			"ChaosCastle"
#define ITEM380_HDR			"Item380System"
#define HARMONY_HDR			"JewelOfHarmony"
#define CHERRY_HDR			"CherryBlossom"
#define PKSYS_HDR			"PKSystem"
#define MARRY_HDR			"MarrySystem"
#define DOUBLEGR_HDR		"DoubleGoer"
#define IMPERIAL_HDR		"ImperialFort"
#define DEVILSQ_HDR			"DevilSquare"
#define FENRIR_HDR			"Fenrir"
#define DAKRSPIRIT_HDR		"DarkSpirit"
#define DARKHORSE_HDR		"DarkHorse"
#define BLUEEVENT_HDR		"BlueEvent"
#define LVUP_HDR			"LevelUpPoints"

// Paths
#define COMMONSERVER_PATH	"..\\Data\\CommonServer.cfg"
#define BLOODCASTLE_PATH	"..\\Data\\Events\\BloodCastle\\BloodCastle.ini"
#define BLODDCASTLE_REWARD	"..\\Data\\Events\\BloodCastle\\BloodCastleReward.txt"
#define DEVILSQUARE_PATH	"..\\Data\\Events\\DevilSquare\\DevilSquare.ini"
#define CHAOSCASTLE_PATH	"..\\Data\\Events\\ChaosCastle\\ChaosCastle.ini"
#define CHAOSCASTLE_TIME	"..\\Data\\Events\\ChaosCastle\\ChaosCastleTime.txt"
#define COMMON_FILE_PATH	"..\\Data\\Common.ini"
#define COMMAND_FILE_PATH	"..\\Data\\Commands.ini"
#define ITEMS_FILE_PATH		"..\\Data\\Items.ini"
#define GENS_FILE_PATH		"..\\Data\\GenSystem.ini"
#define EVENTBAG_FILE_PATH	"..\\Data\\EventItemBags.ini"
#define LIC_FILE_PATH		"..\\Data\\License.lic"
#define IMPERIAL_FILE_PATH	"..\\Data\\Events\\ImperialFort\\ImperialFort.ini"
#define IMPERIAL_REW_FILE	"..\\Data\\Events\\ImperialFort\\ImperialFortRewards.txt"
#define DOUBLEGR_FILE_PATH	"..\\Data\\Events\\DoubleGoer\\DoubleGoer.ini"
#define DOUBLEGR_DROP_FILE	"..\\Data\\Events\\DoubleGoer\\DoubleGoerReward.txt"
#define DOUBLEGR_CFG_FILE	"..\\Data\\Events\\DoubleGoer\\MonsterConfig.txt"
#define RAKLION_FILE_PATH	"..\\Data\\Events\\Raklion\\Raklion.ini"
#define RAKLIONDR_FILE_PATH	"..\\Data\\Events\\Raklion\\SelupanDrop.txt"
#define SUMMER_FILE_PATH	"..\\Data\\Events\\Summer.ini"
#define HAPPYPOUC_FILE_PATH	"..\\Data\\Events\\HappyPouch.ini"
#define BLUEEVENTFILE_PATH	"..\\Data\\Events\\BlueEvent.ini"
#define SKYEVENT_FILE_PATH	"..\\Data\\Events\\SkyEvent.ini"
#define EVENT_MANAGEMENT	"..\\Data\\Events\\EventManagement.dat"
#define RINGAT_FILE_PATH	"..\\Data\\Events\\WhiteWizard.ini"
#define GOLDEN_FILE_PATH	"..\\Data\\Events\\GoldenInvasion.ini"
#define UNDERWD_FILE_PATH	"..\\Data\\Events\\UnderWorld.ini"
#define REDDRAGON_FILE_PATH	"..\\Data\\Events\\RedDragon.ini"
#define SANTA_FILE_PATH		"..\\Data\\Events\\SantaEvent.ini"
#define CHERRY_FILE_PATH	"..\\Data\\Events\\CherryBlossom\\CherryBlossom.ini"
#define CHERRY_MIX_PATH		"..\\Data\\Events\\CherryBlossom\\CherryBlossomMix.txt"
#define KANTURU_FILE_PATH	"..\\Data\\Events\\Kanturu\\Kanturu.ini"
#define MOSS_FILE_PATH		"..\\Data\\Events\\MossMerchant\\MossMerchant.ini"
#define MOSSSELL_FILE_PATH	"..\\Data\\Events\\MossMerchant\\MossMerchantItem.txt"
#define PKSYSTEM_FILE_PATH	"..\\Data\\PKSystem.ini"
#define NEWS_FILE_PATH		"..\\Data\\News.ini"
#define FIREWALL_FILE_PATH	"..\\Data\\IPBlock.txt"
#define ITEM380_FILE_PATH	"..\\Data\\Item380System.txt"
#define HARMONY_FILE_PATH	"..\\Data\\JewelOfHarmony.txt"
#define MARRY_FILE_PATH		"..\\Data\\MarrySystem.ini"
#define DEVIL_FILE_PATH		"..\\Data\\Events\\DevilSquare.ini"
#define PETS_FILE_PATH		"..\\Data\\Pets.ini"
#define ITEMDROP_FILE_PATH	"..\\Data\\ItemDropManager.txt"
#define GA_AWARD_FILE_PATH	"..\\Data\\Events\\GoldenArcher.txt"
#define BUFF_EFFECT_FILE	"..\\Data\\BuffEffect.txt"


#define LODWORD(l)           ((DWORD)(((DWORDLONG)(l)) & 0xffffffff))
#define HIDWORD(l)           ((DWORD)((((DWORDLONG)(l)) >> 32) & 0xffffffff))

#define MAX_PACKET_LEN				0xFF
#define COMMONER_STATUS				3
#define APP_MAIN			int APIENTRY
#define APP_ARGS			HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd
#define LOAD_APP_WIN		hInstance, IDS_GAMESERVER, gMainApp.szWindowClass, MAX_STRING_LEN
#define APP_ARGS_PUSH		hInstance,nShowCmd
#define APP_LPRESULTCB		LRESULT CALLBACK
#define WNDPROC_ARGS		HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam
#define APP_INTPTRCB		INT_PTR CALLBACK
#define ABOUT_ARGS			HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam
#define SOFT_WORK_THREAD			void MainTask(LPVOID Param) { gMainApp.Worker(); }




//////////////////////////
#define MAX_CLASS_TYPE			7
#define MAX_PLAYER_EQUIPMENT	12
#define EQUIPMENT_WEAPON_RIGHT	0
#define EQUIPMENT_WEAPON_LEFT	1
#define MOVMAX					255
#define MOVMIN					1


#define REQ_LEVEL_MOVE_RANGE(Level) ( ((Level) < MOVSTR[MapNumber].MinLevel )?false:( (Level) > MOVSTR[MapNumber].MaxLevel )?false:true )

#endif