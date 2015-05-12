//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Main Protocol Structures								//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "prodef.h"
#include "GuildClass.h"
#include "user.h"
#include "MapClass.h"


extern int iCount;

struct PMSG_KILLPLAYER_EXT
{
	PBMSG_HEAD h;
	unsigned char NumberH;
	unsigned char NumberL;
	unsigned short ExpH;
	unsigned short ExpL;
	unsigned char DamageH;
	unsigned char DamageL;
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Send Warehouse Info Packet
 *	Direction : GameServer -> Client
 *  Code      : 0xC3
 *	HeadCode  : 0x30
 */
struct PMSG_TALKRESULT
{
	PBMSG_HEAD h;
	unsigned char result;
	unsigned char level1;
	unsigned char level2;
	unsigned char level3;
	unsigned char level4;
	unsigned char level5;
	unsigned char level6;
	unsigned char level7;
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Result Packet
 *	Direction : GameServer -> Client
 *  Code     : 0xC1
 *	HeadCode : 0xF3
 *	SubCode  : 0x02
 */
struct PMSG_RESULT
{
	PBMSG_HEAD h;
	unsigned char subcode;
	unsigned char result;
};

struct PMSG_KILLCOUNT
{
	PBMSG_HEAD2 h;
	unsigned char btKillCount;
};

struct PMSG_ATTACKRESULT
{
	PBMSG_HEAD h;
	unsigned char NumberH;
	unsigned char NumberL;
	unsigned char DamageH;
	unsigned char DamageL;
	unsigned char DamageType;
	unsigned char btShieldDamageH;
	unsigned char btShieldDamageL;
};

struct PMSG_USEREQUIPMENTCHANGED
{
	PBMSG_HEAD h;
	unsigned char NumberH;
	unsigned char NumberL;
	unsigned char ItemInfo[MAX_ITEM_INFO];
};

struct PMSG_SERVERCMD
{
	PBMSG_HEAD h;
	unsigned char subcode;
	unsigned char CmdType;
	unsigned char X;
	unsigned char Y;
};

struct PMSG_ITEMTHROW_RESULT
{
	PBMSG_HEAD h;
	unsigned char Result;
	unsigned char Ipos;
};

struct PMSG_GUILDCREATED_RESULT
{
	PBMSG_HEAD h;
	unsigned char Result;
	unsigned char btGuildType;
};

struct PMSG_SIMPLE_GUILDVIEWPORT_COUNT
{
	PWMSG_HEAD h;
	unsigned char Count;
};

struct PMSG_SIMPLE_GUILDVIEWPORT
{
	int GuildNumber;
	unsigned char btGuildStatus;
	unsigned char btGuildType;
	unsigned char btGuildRelationShip;
	unsigned char NumberH;
	unsigned char NumberL;
};

struct PMSG_RECVMOVE
{
	PBMSG_HEAD h;
	unsigned char NumberH;
	unsigned char NumberL;
	unsigned char X;
	unsigned char Y;
	unsigned char Path;
};

struct PMSG_MAGICATTACK_RESULT
{
	PBMSG_HEAD h;
	unsigned char MagicNumberH;
	unsigned char MagicNumberL;
	unsigned char SourceNumberH;
	unsigned char SourceNumberL;
	unsigned char TargetNumberH;
	unsigned char TargetNumberL;
};

struct PMSG_BEATTACK_COUNT
{
	PBMSG_HEAD h;
	unsigned char X;
	unsigned char Y;
	unsigned char Serial;
	unsigned char Count;
	unsigned char MagicNumberH;
	unsigned char MagicNumberL;
};

struct PMSG_BEATTACK
{
	unsigned char MagicKey;
	unsigned char NumberH;
	unsigned char NumberL;
};

struct PMSG_SET_DEVILSQUARE
{
	PBMSG_HEAD h;
	unsigned char Type;
};

struct PMSG_GUILD_ASSIGN_STATUS_RESULT
{
	PBMSG_HEAD h;
	unsigned char btType;
	unsigned char btResult;
	char szTagetName[10];
};

struct PMSG_GUILD_ASSIGN_TYPE_RESULT
{
	PBMSG_HEAD h;
	unsigned char btGuildType;
	unsigned char btResult;
};

struct PMSG_CLIENTTIME
{
	PBMSG_HEAD h;
	unsigned long Time;
	unsigned short AttackSpeed;
	unsigned short MagicSpeed;
};

struct PMSG_CHECK_MAINEXE_RESULT
{
	PBMSG_HEAD h;
	unsigned long m_dwKey;
};

#pragma pack(1)

struct PMSG_CHECKMAIN_INC
{
	PBMSG_HEAD h;
	BYTE subcode;
	char Id[10];
	char Pass[10];
	WORD Unknow1;
	DWORD TickCount;
	BYTE CliVersion[5];
	BYTE CliSerial[16];
	DWORD Unknow2;
};

#pragma pack()

struct PMSG_CLIENTCLOSE
{
	PBMSG_HEAD h;
	BYTE SubCode;
	BYTE Flag;
};

struct PMSG_CLIENTMSG
{
	PBMSG_HEAD h;
	BYTE SubCode;
	BYTE Flag;
	BYTE subFlag;
};

struct PMSG_CHARCREATE
{
	PBMSG_HEAD h;
	BYTE subcode;
	char Name[10];
	BYTE ClassSkin;
};

struct PMSG_CHARDELETE
{
	PBMSG_HEAD h;
	BYTE subcode;
	char Name[10];
	char LastJoominNumber[10];
};

struct PMSG_CHARMAPJOIN
{
	PBMSG_HEAD h;
	BYTE subcode;
	char Name[10];
};

struct PMSG_LVPOINTADD
{
	PBMSG_HEAD h;
	BYTE subcode;
	BYTE Type;
};

struct PMSG_SKILLKEY
{
	PBMSG_HEAD h;
	BYTE subcode;
	BYTE SKillKey[10];
	BYTE GameOption;
	BYTE QkeyDefine;
	BYTE WkeyDefine;
	BYTE EkeyDefine;
	BYTE ChatWindow;
};

struct PMSG_ITEMGETREQUEST
{
	PBMSG_HEAD h;
	BYTE NumberH;
	BYTE NumberL;
};

struct PMSG_ITEMTHROW
{
	PBMSG_HEAD h;
	unsigned char px;
	unsigned char py;
	unsigned char Ipos;
};

struct PMSG_INVENTORYITEMMOVE
{
	PBMSG_HEAD h;
	unsigned char sFlag;
	unsigned char source_item_num;
	unsigned char sItemInfo[MAX_ITEM_INFO];
	unsigned char tFlag;
	unsigned char target_item_num;
};

struct PMSG_TALKREQUEST
{
	PBMSG_HEAD h;
	unsigned char NumberH;
	unsigned char NumberL;
};

struct PMSG_BUYREQUEST
{
	PBMSG_HEAD h;
	unsigned char Pos;
};

struct PMSG_SELLREQUEST
{
	PBMSG_HEAD h;
	unsigned char Pos;
};

struct PMSG_ITEMDURREPAIR
{
	PBMSG_HEAD h;
	BYTE Position;
	BYTE Requestpos;
};

struct PMSG_TRADE_REQUEST
{
	PBMSG_HEAD h;
	BYTE NumberH;
	BYTE NumberL;
};

struct PMSG_TRADE_RESPONSE
{
	PBMSG_HEAD h;
	BYTE Response;
	BYTE Id[10];
	WORD Level;
	int GuildNumber;
};

struct PMSG_TRADE_GOLD
{
	PBMSG_HEAD h;
	DWORD Money;
};

struct PMSG_TRADE_OKBUTTON
{
	PBMSG_HEAD h;
	BYTE Flag;
};

struct PMSG_GUILDJOINQ
{
	PBMSG_HEAD h;
	BYTE NumberH;
	BYTE NumberL;
};

struct PMSG_GUILDQRESULT
{
	PBMSG_HEAD h;
	BYTE Result;
	BYTE NumberH;
	BYTE NumberL;
};

struct PMSG_GUILDDELUSER
{
	PBMSG_HEAD h;
	char Name[10];
	char JoominNumber[10];
};

struct PMSG_GUILDMASTERANSWER
{
	PBMSG_HEAD h;
	BYTE Result;
};

struct PMSG_GUILDINFOSAVE
{
	PBMSG_HEAD h;
	BYTE btGuildType;
	char GuildName[8];
	BYTE Mark[32];
};

struct PMSG_GUILDWARSEND_RESULT
{
	PBMSG_HEAD h;
	BYTE Result;
};

struct PMSG_WAREHOUSEMONEYINOUT
{
	PBMSG_HEAD h;
	BYTE Type;
	int Money;
};

struct PMSG_WAREHOUSEPASSSEND
{
	PBMSG_HEAD h;
	BYTE Type;	// [0x00:Open Warehouse] [0x01:Set Pass] [0x02:Delete Pass] 3
	short Pass;
	char LastJoominNumber[10];
};

struct PMSG_CHAOSMIX
{
	PBMSG_HEAD h;
	BYTE Type;
	BYTE SeedType;
};

struct PMSG_ATTACK
{
	PBMSG_HEAD h;
	BYTE AttackAction;
	BYTE DirDis;
	BYTE NumberH;
	BYTE NumberL;
};

struct PMSG_NEWATTACK_S6
{
	PBMSG_HEAD h;
	unsigned char MagicNumberH;
	unsigned char MagicNumberL;
	unsigned char TargetH;
	unsigned char TargetL;
};


struct PMSG_ACTION
{
	PBMSG_HEAD h;
	BYTE Dir;
	BYTE ActionNumber;
	BYTE iTargetIndexH;
	BYTE iTargetIndexL;
};

struct PMSG_MAGICATTACK
{
	PBMSG_HEAD h;
	unsigned char MagicNumberH;
	unsigned char MagicNumberL;
	unsigned char Unk;
	unsigned char NumberH;
	unsigned char NumberL;
	unsigned char Dis;
};

struct PMSG_MAGICCANCEL 
{
	PBMSG_HEAD h;
	BYTE MagicNumber;
	BYTE NumberH;
	BYTE NumberL;
};

struct PMSG_DURATION_MAGIC_RECV
{
	PBMSG_HEAD h;
	BYTE X;
	BYTE Y;
	BYTE MagicNumberH;
	BYTE MagicNumberL;
	BYTE Dir;
	BYTE Dis;
	BYTE TargetPos;
	BYTE Unk;
	BYTE NumberH;
	BYTE NumberL;
	BYTE MagicKey;
};

struct PMSG_USEITEM
{
	PBMSG_HEAD h;
	BYTE inventoryPos;
	BYTE invenrotyTarget;
	BYTE btItemUseType;
};

struct PMSG_REQ_MOVEDEVILSQUARE
{
	PBMSG_HEAD h;
	BYTE SquareNumber;
	BYTE InvitationItemPos;
};

struct PMSG_REQ_EVENT_REMAINTIME
{
	PBMSG_HEAD h;
	BYTE hEventType;
	BYTE btItemLevel;
};

struct PMSG_PING_RESULT
{
	PBMSG_HEAD h;
};

struct PMSG_REGEVENTCHIP
{
	PBMSG_HEAD h;
	BYTE Type;
	BYTE ChipPos;
};

struct PMSG_GETMUTONUMBER
{
	PBMSG_HEAD h;
};

struct PMSG_EXCHANGE_EVENTCHIP
{
	PBMSG_HEAD h;
	BYTE btType;	// [0x00:Change Renas] [0x01:Change Stones]	3
};

struct PMSG_SETQUEST 
{
	PBMSG_HEAD h;
	BYTE QuestIndex;
	BYTE State;
	BYTE Unk;
};

struct PMSG_REQ_CL_ENTERCOUNT
{
	PBMSG_HEAD h;
	BYTE btEventType;
};

struct PMSG_REQ_2ANV_LOTTO_EVENT
{
	PBMSG_HEAD h;
	char SERIAL1[5];
	char SERIAL2[5];
	char SERIAL3[5];
};

struct PMSG_PACKETCHECKSUM
{
	PBMSG_HEAD h;
	BYTE funcindex;
	DWORD CheckSum;
};

struct PMSG_REQUEST_PET_ITEM_COMMAND
{
	PBMSG_HEAD h;
	BYTE PetType;
	BYTE Command;
	BYTE NumberH;
	BYTE NumberL;
};

struct PMSG_REQUEST_PET_ITEMINFO
{
	PBMSG_HEAD h;
	BYTE PetType;
	BYTE InvenType;
	BYTE nPos;
};

struct PMSG_REQ_GUILDVIEWPORT
{
	PBMSG_HEAD h;
	int GuildNumber;
};

struct PMSG_GUILD_ASSIGN_STATUS_REQ
{
	PBMSG_HEAD h;
	BYTE btType;
	BYTE btGuildStatus;	
	char szTagetName[10];
};

struct PMSG_GUILD_ASSIGN_TYPE_REQ
{
	PBMSG_HEAD h;
	BYTE btGuildType;
};

struct PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ
{
	PBMSG_HEAD h;
	BYTE btRelationShipType;
	BYTE btRequestType;
	BYTE btTargetUserIndexH;
	BYTE btTargetUserIndexL;
};

struct PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS
{
	PBMSG_HEAD h;
	BYTE btRelationShipType;
	BYTE btRequestType;
	BYTE btResult;
	BYTE btTargetUserIndexH;
	BYTE btTargetUserIndexL;
};

struct PMSG_UNIONLIST_REQ
{
  PBMSG_HEAD h;
};

struct PMSG_KICKOUT_UNIONMEMBER_REQ
{
	PBMSG_HEAD2 h;
	char szTargetGuildName[8];
};

struct PMSG_REQ_MAPSERVERAUTH
{
	PBMSG_HEAD2 h;
	char szAccountID[12];
	char szCharName[12];
	int iJoinAuthCode1;	
	int iJoinAuthCode2;
	int iJoinAuthCode3;
	int iJoinAuthCode4;
	int iTickCount;
	BYTE btCliVersion[5];
	BYTE btCliSerial[16];
};

struct PMSG_REQ_CASTLESIEGESTATE
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
};

struct PMSG_REQ_REGCASTLESIEGE
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
};

struct PMSG_REQ_GIVEUPCASTLESIEGE
{  
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ BYTE btGiveUp;
};

struct PMSG_REQ_GUILDREGINFO
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
};

struct CSP_ANS_GUILDREGINFO
{
	PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
	int iIndex;
	char szGuildName[8];
	int iRegMarkCount;
	unsigned char bIsGiveUp;
	BYTE btRegRank;
};

struct PMSG_REQ_REGGUILDMARK
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ BYTE btItemPos;
};

struct CSP_ANS_GUILDREGMARK
{
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x8|*/ char szGuildName[8];
  /*<thisrel this+0x18>*/ /*|0x4|*/ int iItemPos;
  /*<thisrel this+0x1c>*/ /*|0x4|*/ int iRegMarkCount;
};

struct PMSG_REQ_NPCBUY
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iNpcNumber;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iNpcIndex;
};



struct PMSG_REQ_NPCREPAIR
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iNpcNumber;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iNpcIndex;
};



struct PMSG_REQ_NPCUPGRADE
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iNpcNumber;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iNpcIndex;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iNpcUpType;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iNpcUpValue;
};

struct PMSG_REQ_TAXMONEYINFO
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
};

struct PMSG_REQ_TAXRATECHANGE
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ BYTE btTaxType;
  /*<thisrel this+0x5>*/ /*|0x1|*/ BYTE btTaxRate1;
  /*<thisrel this+0x6>*/ /*|0x1|*/ BYTE btTaxRate2;
  /*<thisrel this+0x7>*/ /*|0x1|*/ BYTE btTaxRate3;
  /*<thisrel this+0x8>*/ /*|0x1|*/ BYTE btTaxRate4;
};

struct PMSG_REQ_MONEYDRAWOUT
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ BYTE btMoney1;
  /*<thisrel this+0x5>*/ /*|0x1|*/ BYTE btMoney2;
  /*<thisrel this+0x6>*/ /*|0x1|*/ BYTE btMoney3;
  /*<thisrel this+0x7>*/ /*|0x1|*/ BYTE btMoney4;
};

struct PMSG_REQ_CSGATEOPERATE
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ BYTE btOperate;
  /*<thisrel this+0x5>*/ /*|0x1|*/ BYTE btIndex1;
  /*<thisrel this+0x6>*/ /*|0x1|*/ BYTE btIndex2;
};

struct PMSG_REQ_MINIMAPDATA
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
};

struct PMSG_REQ_STOPMINIMAPDATA
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h; 
};

struct PMSG_REQ_CSCOMMAND 
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ BYTE btTeam;
  /*<thisrel this+0x5>*/ /*|0x1|*/ BYTE btX;
  /*<thisrel this+0x6>*/ /*|0x1|*/ BYTE btY;
  /*<thisrel this+0x7>*/ /*|0x1|*/ BYTE btCommand;

};

struct PMSG_REQ_CSHUNTZONEENTER
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ BYTE btHuntZoneEnter;
};

struct PMSG_REQ_NPCDBLIST
{
  PBMSG_HEAD h;
  /*<thisrel this+0x3>*/ /*|0x1|*/ BYTE btMonsterCode;
};

struct PMSG_REQ_CSREGGUILDLIST
{
  PBMSG_HEAD h;
};

struct PMSG_REQ_CSATTKGUILDLIST
{
  PBMSG_HEAD h;
};

struct PMSG_REQ_USEWEAPON
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ BYTE btObjIndexH;
  /*<thisrel this+0x5>*/ /*|0x1|*/ BYTE btObjIndexL;
  /*<thisrel this+0x6>*/ /*|0x1|*/ BYTE btTargetIndex;
};

struct PMSG_REQ_WEAPON_DAMAGE_VALUE
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ BYTE btObjIndexH;
  /*<thisrel this+0x5>*/ /*|0x1|*/ BYTE btObjIndexL;
};

struct PMSG_REQ_GUILDMARK_OF_CASTLEOWNER
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
};

struct PMSG_REQ_JEWEL_MIX
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ BYTE btJewelType;
  /*<thisrel this+0x5>*/ /*|0x1|*/ BYTE btJewelMix;
};

struct PMSG_REQ_JEWEL_UNMIX
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ BYTE btJewelType;
  /*<thisrel this+0x5>*/ /*|0x1|*/ BYTE btJewelLevel;
  /*<thisrel this+0x6>*/ /*|0x1|*/ BYTE btJewelPos;
};

struct PMSG_REQ_CRYWOLF_INFO
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
};

struct PMSG_REQ_CRYWOLF_ALTAR_CONTRACT
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ BYTE btObjIndexH;
  /*<thisrel this+0x5>*/ /*|0x1|*/ BYTE btObjIndexL;
};

struct PMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
};

struct PMSG_REQ_KANTURU_STATE_INFO
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
};

struct PMSG_REQ_ENTER_KANTURU_BOSS_MAP
{
  /*<thisrel this+0x0>*/ /*|0x4|*/PBMSG_HEAD2 h;
};

struct CSP_ANS_CSINITDATA { // <size 0x40>
	PWMSG_HEAD h;	// 0
	int iResult;		// 4
	WORD wMapSvrNum;	// 8
	WORD wStartYear;	// a
	BYTE btStartMonth;	// c
	BYTE btStartDay;	// d
	WORD wEndYear; // e
	BYTE btEndMonth; // 10
	BYTE btEndDay;	// 11
	BYTE btIsSiegeGuildList;	// 12
	BYTE btIsSiegeEnded;	// 13
	BYTE btIsCastleOccupied;	// 14
	char szCastleOwnGuild[8]; // 15
	__int64 i64CastleMoney; // 20
	int iTaxRateChaos; // 28
	int iTaxRateStore; // 2c
	int iTaxHuntZone; // 30
	int iFirstCreate; // 34
	int iCount; // 38
};

struct CSP_CSINITDATA {// <size 0x1c>
	int iNpcNumber; // 0
	int iNpcIndex; // 4
	int iNpcDfLevel; // 8
	int iNpcRgLevel; // c
	int iNpcMaxHp; // 10
	int iNpcHp; // 14
	unsigned char btNpcX; // 18
	unsigned char btNpcY; // 19
	unsigned char btNpcDIR; // 1a
};

struct CSP_CALCREGGUILDLIST { // <size 0x18>
	char szGuildName[8]; // 0
	int iRegMarkCount; // 8
	int iGuildMemberCount; // c
	int iGuildMasterLevel; // 10
	int iSeqNum; // 14
};

struct CSP_CSGUILDUNIONINFO
{
	char szGuildName[8];
	int iCsGuildID;
};

struct CSP_CSLOADTOTALGUILDINFO
{
	char szGuildName[8];
	int iCsGuildID;
	int iGuildInvolved;
};

struct PMSG_CSATTKGUILDLIST
{
	unsigned char btCsJoinSide;
	unsigned char btGuildInvolved;
	char szGuildName[8];
};

struct CSP_ANS_NPCDATA
{
	PWMSG_HEAD h;
	int iResult;
	unsigned short wMapSvrNum;
	int iIndex;
	int iCount;
};

struct PMSG_ANS_NOTIFYCSSTART
{
	PBMSG_HEAD2 h;
	unsigned char btStartState;
};

struct PMSG_ANS_NOTIFYCSPROGRESS
{
	PBMSG_HEAD2 h;
	unsigned char btCastleSiegeState;
	unsigned char szGuildName[8];
};


struct PMSG_IT_MOVE_MAP
{
	PBMSG_HEAD2	h;
	BYTE btIllusionIndex;
	BYTE btIllusionTicketPos;

};

struct PMSG_NOTIFY_OBJECT_CREATION_STATE
{
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btObjIndexH;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btObjIndexL;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btCreationState;
};

struct CSP_ANS_ALLGUILDREGINFO
{
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iIndex;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iCount;
};

struct CSP_GUILDREGINFO
{
  /*<thisrel this+0x0>*/ /*|0x8|*/ char szGuildName[8];
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iRegMarkCount;
  /*<thisrel this+0xc>*/ /*|0x1|*/ unsigned char bIsGiveUp;
  /*<thisrel this+0xd>*/ /*|0x1|*/ unsigned char btRegRank;
};

struct PMSG_ANS_CSREGGUILDLIST
{
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iCount;
};

struct PMSG_CSREGGUILDLIST
{
  /*<thisrel this+0x0>*/ /*|0x8|*/ char szGuildName[8];
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char btRegMarks1;
  /*<thisrel this+0x9>*/ /*|0x1|*/ unsigned char btRegMarks2;
  /*<thisrel this+0xa>*/ /*|0x1|*/ unsigned char btRegMarks3;
  /*<thisrel this+0xb>*/ /*|0x1|*/ unsigned char btRegMarks4;
  /*<thisrel this+0xc>*/ /*|0x1|*/ unsigned char btIsGiveUp;
  /*<thisrel this+0xd>*/ /*|0x1|*/ unsigned char btSeqNum;
};

struct CSP_ANS_CALCREGGUILDLIST
{
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iCount;
};

struct CSP_ANS_CSGUILDUNIONINFO
{
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iCount;
};

struct PMSG_ANS_CSCOMMAND
{
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btTeam;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btX;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btY;
  /*<thisrel this+0x7>*/ /*|0x1|*/ unsigned char btCommand;
};

struct CSP_ANS_CSSAVETOTALGUILDINFO
{
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
};

struct CSP_ANS_CSLOADTOTALGUILDINFO
{
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iCount;
};

struct CSP_ANS_NPCUPDATEDATA
{
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iResult;
  /*<thisrel this+0x8>*/ /*|0x2|*/ unsigned short wMapSvrNum;
};


struct PMSG_ANS_NPCDBLIST
{
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iCount;
};

struct PMSG_ANS_SENDMINIMAPDATA
{
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iCount;
};

struct PMSG_ANS_SENDNPCMINIMAPDATA
{
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iCount;
};

struct PMSG_SENDNPCMINIMAPDATA
{
  /*<thisrel this+0x0>*/ /*|0x1|*/ unsigned char btNpcType;
  /*<thisrel this+0x1>*/ /*|0x1|*/ unsigned char btX;
  /*<thisrel this+0x2>*/ /*|0x1|*/ unsigned char btY;
};

struct PMSG_ANS_GUARD_IN_CASTLE_HUNTZONE
{
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btUsable;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iCurrentPrice;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iMaxPrice;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iUnitOfPrice;
};

struct PMSG_ANS_MOVE_TO_CASTLE_HUNTZONE
{
	PBMSG_HEAD2 h; // +0x0(0x4)
	BYTE btResult; // +0x4(0x1)
};

struct PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE
{
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iPrice;
};

struct PMSG_SHOPITEMCOUNT
{
	PWMSG_HEAD h;
	BYTE Type;
	BYTE count;
};

void ToHex(unsigned char * out, unsigned char * data, int len);
void ProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen, int aIndex, BOOL Encrypt, int serial);
void CGLiveClient(PMSG_CLIENTTIME* lpClientTime, short aIndex);
void CSCheckMainInclusions(PMSG_CHECKMAIN_INC* lpMsg, int aIndex);
void CGCloseGamePlay(PMSG_CLIENTCLOSE* lpMsg, int aIndex);
void CGHackCheckClient(PMSG_CLIENTMSG* lpMsg, int aIndex);
void CGCharacterCreate(PMSG_CHARCREATE* lpMsg, int aIndex);
void CGCharacterDelete(PMSG_CHARDELETE* lpMsg, int aIndex);
void CGCharacterJoinRequest(PMSG_CHARMAPJOIN* lpMsg, int aIndex);
void CGLevelUpPointAdd(PMSG_LVPOINTADD* lpMsg, int aIndex);
void DSCheckPJAccount(int aIndex);
void GCSkillKeyRecv(PMSG_SKILLKEY* lpMsg, int aIndex);
void CheckMainRecieve(PMSG_CHECK_MAINEXE_RESULT* lpMsg, int aIndex);

void NewSkillPacket(int aIndex, LPBYTE aRecv);

void GCCheckMainExeKeySend(int aIndex);
WORD EncryptCheckSumKey(WORD wSource);
void PEchoProc(unsigned char* aMsg, int aLen, short aIndex);
void GCResultSend(int aIndex, unsigned char headcode, unsigned char result);
void ChatSend(LPOBJ lpObj, char* szChat);
void vChatSend(LPOBJ lpObj, char* szChat, ...);
void ChatTargetSend(LPOBJ lpObj, char* szChat, int senduser);
void GCServerMsgSend(unsigned char msg, int aIndex);
void GCServerMsgStringSend(char* szMsg, int aIndex, BYTE type);
void GCServerMsgStringSendGuild(_GUILD_INFO_STRUCT* lpNode, char* szMsg, unsigned char type);
void GCEventStateSend(int aIndex, unsigned char state, unsigned char Event);
void GCMapEventStateSend(int map, unsigned char state, unsigned char Event);
void SCPJoinResultSend(int aIndex, unsigned char result);
void GCJoinResult(unsigned char result, int aIndex);
void GCJoinSocketResult(unsigned char result, SOCKET Socket);
void GCLevelUpMsgSend(int aIndex, int iSendEffect);

void GCInventoryItemOneSend(int aIndex, int pos);
void GCPkLevelSend(int aIndex, unsigned char pklevel);
void GCMagicListOneSend(int aIndex, char Pos, unsigned short type, unsigned char level, unsigned short skill, BYTE btListType);
void GCMagicListOneDelSend(int aIndex, char Pos, WORD type, BYTE level, WORD skill, BYTE btListType);
void GCMagicListMultiSend(LPOBJ lpObj, BYTE btListType);
void GCEquipmentSend(int aIndex);
void GCRecallMonLife(int aIndex, int maxlife, int life);
void GCTimeViewSend(int aIndex, int second);
void GCGoalSend(int aIndex, char* Name1, unsigned char score1, char* Name2, unsigned char score2);
void GCSkillKeySend(int aIndex, unsigned char* keybuffer, unsigned char GO, unsigned char Qk, unsigned char Wk, unsigned char Ek, /*<regrel ebp+0x20>*/ /*|0x1|*/ unsigned char ChatWnd);
void GCMoneySend(int aIndex, unsigned long money);
void GCItemInventoryPutSend(int aIndex, unsigned char result, unsigned char iteminfo1, unsigned char iteminfo2);
void CGItemGetRequest(PMSG_ITEMGETREQUEST* lpMsg, int aIndex);
BOOL CGItemDropRequest(PMSG_ITEMTHROW* lpMsg, int aIndex, int drop_type);
void GCItemMoveResultSend(int aIndex, unsigned char result, unsigned char pos, unsigned char* const ItemInfo);
void CGInventoryItemMove(PMSG_INVENTORYITEMMOVE* lpMsg, int aIndex);
void GCEquipmentChange(int aIndex, int pos);
void CGTalkRequestRecv(PMSG_TALKREQUEST* lpMsg, int aIndex);
void GCUserWarehouseSend(LPOBJ lpObj);
void CGBuyRequestRecv(PMSG_BUYREQUEST* lpMsg, int aIndex);
void CGSellRequestRecv(PMSG_SELLREQUEST* lpMsg, int aIndex);
int  GetNeedMoneyItemDurRepaire(class CItem* DurItem, int RequestPos);
void ItemDurRepaire(LPOBJ lpObj, class CItem* DurItem, int pos, int RequestPos);
void CGModifyRequestItem(PMSG_ITEMDURREPAIR* lpMsg, int aIndex);
void CGTradeRequestSend(PMSG_TRADE_REQUEST* lpMsg, int aIndex);
void CGTradeResponseRecv(PMSG_TRADE_RESPONSE* lpMsg, int aIndex);
void GCTradeResponseSend(BYTE response, int aIndex, LPSTR id, WORD level, int GuildNumber);
void GCTradeOtherDel(int aIndex, BYTE tradeindex);
void GCTradeOtherAdd(int aIndex, BYTE tradeindex, LPBYTE iteminfo);
void CGTradeMoneyRecv(PMSG_TRADE_GOLD* lpMsg, int aIndex);
void GCTradeMoneyOther(int aIndex, DWORD money);
void GCTradeOkButtonSend(int aIndex, unsigned char flag);
void CGTradeOkButtonRecv(PMSG_TRADE_OKBUTTON* lpMsg, int aIndex);
void CGTradeCancelButtonRecv(int aIndex);
void CGTradeResult(int aIndex, unsigned char result);
void CGGuildRequestRecv(PMSG_GUILDJOINQ* lpMsg, int aIndex);
void CGGuildRequestResultRecv(PMSG_GUILDQRESULT* lpMsg, int aIndex);
void CGGuildListAll(int pnumber);
void CGGuildDelUser(PMSG_GUILDDELUSER* lpMsg, int aIndex);
void GCGuildDelUserResult(int aIndex, unsigned char Result);
void GCGuildMasterQuestionSend(int aIndex);
void CGGuildMasterAnswerRecv(PMSG_GUILDMASTERANSWER* lpMsg, int aIndex);
void GCGuildMasterManagerRun(int aIndex);
void CGGuildMasterInfoSave(int aIndex,PMSG_GUILDINFOSAVE* lpMsg);
void CGGuildMasterCreateCancel(int aIndex);
void GCGuildViewportNowPaint(int aIndex, char* guildname, unsigned char* Mark, int isGuildMaster);
void GCGuildViewportDelNow(int aIndex, int isGuildMaster);
void GCManagerGuildWarEnd(char* GuildName);
void GCManagerGuildWarSet(char* GuildName1, char* GuildName2, int type);
void GCGuildWarRequestResult(char* GuildName, int aIndex, int type);
void GCGuildWarRequestSend(char* GuildName, int aIndex, int type);
void GCGuildWarRequestSendRecv(PMSG_GUILDWARSEND_RESULT* lpMsg, int aIndex);
void GCGuildWarDeclare(int aIndex, char* _guildname);
void GCGuildWarEnd(int aIndex, unsigned char result, char* _guildname);
void GCGuildWarScore(int aIndex);
void CGWarehouseMoneyInOut(int aIndex,PMSG_WAREHOUSEMONEYINOUT* lpMsg);
void GCWarehouseInventoryMoneySend(int aIndex, unsigned char result, int money, int wmoney);
void CGWarehouseUseEnd(int aIndex);
void GCWarehouseStateSend(int aIndex, unsigned char state);
void GCWarehouseRecivePassword(int aIndex,PMSG_WAREHOUSEPASSSEND* lpMsg);
void GCUserChaosBoxSend(LPOBJ lpObj, int iChaosBoxType);
void CGChaosBoxItemMixButtonClick(PMSG_CHAOSMIX* aRecv, int aIndex);
void CGChaosBoxUseEnd(int aIndex);
void CGAttack(PMSG_ATTACK* lpMsg, int aIndex);
void GCDamageSend(int aIndex, int damage, int iShieldDamage);
void GCDamageSend(int aIndex, int TargetIndex, int AttackDamage, int MSBFlag, int MSBDamage, int iShieldDamage);
void GCKillPlayerExpSend(int aIndex, int TargetIndex, int exp, int AttackDamage, int MSBFlag);
void GCDiePlayerSend(LPOBJ lpObj, int TargetIndex, WORD skill, int KillerIndex);
void GCActionSend(LPOBJ lpObj, BYTE ActionNumber, int aIndex, int aTargetIndex);
void CGActionRecv(PMSG_ACTION* lpMsg, int aIndex);
void CGMagicAttack(PMSG_MAGICATTACK* lpMsg, int aIndex);
void GCMagicAttackNumberSend(LPOBJ lpObj, WORD MagicNumber, int usernumber, unsigned char skillsuccess);
void GCMagicCancelSend(LPOBJ lpObj, WORD MagicNumber);
void GCMagicCancel(PMSG_MAGICCANCEL * lpMsg, int aIndex);
void GCUseEffectItem(LPOBJ lpObj, int iEffectUseOption, int iOptionType, int iEffectType, int iLeftTime);
void CGMagicCancel(PMSG_MAGICCANCEL* lpMsg, int aIndex);
void GCUseMonsterSkillSend(LPOBJ lpObj, LPOBJ lpTargetObj, int iSkillNumber);
void CGBeattackRecv(unsigned char* lpRecv, int aIndex, int magic_send);
void CGDurationMagicRecv(PMSG_DURATION_MAGIC_RECV* lpMsg, int aIndex);
void ObjectMapJoinPositionSend(short aIndex);
void CGUseItemRecv(PMSG_USEITEM* lpMsg, int aIndex);
void GCReFillSend(int aIndex, WORD Life, BYTE Ipos, unsigned char flag,  WORD wShield);
void GCManaSend(int aIndex, short Mana, BYTE Ipos, unsigned char flag,  WORD BP);
void GCInventoryItemDeleteSend(int aIndex, BYTE pos, unsigned char flag);
void GCItemUseSpecialTimeSend(int aIndex, unsigned char number, int time);
void GCItemDurSend(int aIndex, BYTE pos, unsigned char dur, unsigned char flag);
void GCItemDurSend2(int aIndex, BYTE pos, unsigned char dur, unsigned char flag);
void CGWeatherSend(int aIndex, BYTE weather);
void GCServerCmd(int aIndex, BYTE type, unsigned char Cmd1, unsigned char Cmd2);
void GCReqmoveDevilSquare(PMSG_REQ_MOVEDEVILSQUARE* lpMsg, int aIndex);
void GCReqEventRemainTime(PMSG_REQ_EVENT_REMAINTIME* lpMsg, int aIndex);
void AllSendMsg(unsigned char* Msg, int size);
void AllSendSameMapMsg(unsigned char* Msg, int size, unsigned char mapnumber);
void GCSendPing(int aIndex);
void GCPingSendRecv(PMSG_PING_RESULT* aRecv, int aIndex);
void GCRegEventChipRecv(PMSG_REGEVENTCHIP* lpMsg, int aIndex);
void GCGetMutoNumRecv(PMSG_GETMUTONUMBER* lpMsg, int aIndex);
void GCUseEndEventChipRescv(int aIndex);
void GCUseRenaChangeZenRecv(PMSG_EXCHANGE_EVENTCHIP* lpMsg, int aIndex);
void CGRequestQuestInfo(int aIndex);
void GCSendQuestInfo(int aIndex, int QuestIndex);
void CGSetQuestState(PMSG_SETQUEST* lpMsg, int aIndex);
void GCSendQuestPrize(int aIndex, unsigned char Type, unsigned char Count);
void CGCloseWindow(int aIndex);
void CGRequestEventEnterCount(PMSG_REQ_CL_ENTERCOUNT* lpMsg, int aIndex);
void CGRequestLottoRegister(PMSG_REQ_2ANV_LOTTO_EVENT* lpMsg, int aIndex);
void GCPacketCheckSumRecv(PMSG_PACKETCHECKSUM* aRecv, int aIndex);
void GCSendGetItemInfoForParty(int aIndex, class CMapItem* lpItem);
void GCSendEffectInfo(int aIndex, BYTE btType);
void CGRequestPetItemCommand(PMSG_REQUEST_PET_ITEM_COMMAND* lpMsg, int aIndex);
void CGRequestPetItemInfo(PMSG_REQUEST_PET_ITEMINFO* lpMsg, int aIndex);
void GCGuildViewportInfo(PMSG_REQ_GUILDVIEWPORT* aRecv, int aIndex);
void CGGuildAssignStatus(PMSG_GUILD_ASSIGN_STATUS_REQ* aRecv, int aIndex);
void CGGuildAssignType(PMSG_GUILD_ASSIGN_TYPE_REQ* aRecv, int aIndex);
void CGRelationShipReqJoinBreakOff(PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ* aRecv, int aIndex);
void CGRelationShipAnsJoinBreakOff(PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS* aRecv, int aIndex);
void CGUnionList(PMSG_UNIONLIST_REQ* aRecv, int aIndex);
void CGRelationShipReqKickOutUnionMember(PMSG_KICKOUT_UNIONMEMBER_REQ* aRecv, int aIndex);
void CGReqMapSvrAuth(PMSG_REQ_MAPSERVERAUTH* lpMsg, int iIndex);
void GCAnsMapSvrAuth(int iIndex, int iResult);
void CGReqCastleSiegeState(PMSG_REQ_CASTLESIEGESTATE* lpMsg, int iIndex);
void GCAnsCastleSiegeState(int iIndex, int iResult, char* lpszGuildName, char* lpszGuildMaster);
void CGReqRegCastleSiege(PMSG_REQ_REGCASTLESIEGE* lpMsg, int iIndex);
void GCAnsRegCastleSiege(int iIndex, int iResult, char* lpszGuildName);
void CGReqGiveUpCastleSiege(PMSG_REQ_GIVEUPCASTLESIEGE* lpMsg, int iIndex);
void GCAnsGiveUpCastleSiege(int iIndex, int iResult, int bGiveUp, int iMarkCount, char* lpszGuildName);
void CGReqGuildRegInfo(PMSG_REQ_GUILDREGINFO* lpMsg, int iIndex);
void GCAnsGuildRegInfo(int iIndex, int iResult,CSP_ANS_GUILDREGINFO* lpMsgResult);
void CGReqRegGuildMark(PMSG_REQ_REGGUILDMARK* lpMsg, int iIndex);
void GCAnsRegGuildMark(int iIndex, int iResult,CSP_ANS_GUILDREGMARK* lpMsgResult);
void CGReqNpcBuy(PMSG_REQ_NPCBUY* lpMsg, int iIndex);
void GCAnsNpcBuy(int iIndex, int iResult, int iNpcNumber, int iNpcIndex);
void CGReqNpcRepair(PMSG_REQ_NPCREPAIR* lpMsg, int iIndex);
void GCAnsNpcRepair(int iIndex, int iResult, int iNpcNumber, int iNpcIndex, int iNpcHP, int iNpcMaxHP);
void CGReqNpcUpgrade(PMSG_REQ_NPCUPGRADE* lpMsg, int iIndex);
void GCAnsNpcUpgrade(int iIndex, int iResult, int iNpcNumber, int iNpcIndex, int iNpcUpType, int iNpcUpValue);
void CGReqTaxMoneyInfo(PMSG_REQ_TAXMONEYINFO* lpMsg, int iIndex);
void GCAnsTaxMoneyInfo(int iIndex, int iResult, BYTE btTaxRateChaos, BYTE btTaxRateStore, __int64 i64Money);
void CGReqTaxRateChange(PMSG_REQ_TAXRATECHANGE* lpMsg, int iIndex);
void GCAnsTaxRateChange(int iIndex, int iResult, BYTE btTaxType, int iTaxRate);
void CGReqMoneyDrawOut(PMSG_REQ_MONEYDRAWOUT* lpMsg, int iIndex);
void GCAnsMoneyDrawOut(int iIndex, int iResult, __int64 i64Money);
void GCAnsCsGateState(int iIndex, int iResult, int iGateIndex);
void CGReqCsGateOperate(PMSG_REQ_CSGATEOPERATE* lpMsg, int iIndex);
void GCAnsCsGateOperate(int iIndex, int iResult, int iGateIndex, int iGateOperate);
void GCAnsCsGateCurState(int iIndex, int iGateIndex, int iGateOperate);
void GCAnsCsAccessSwitchState(int iIndex, int iSwitchIndex, int iSwitchUserIndex, BYTE btSwitchState);
void GCAnsCsAccessCrownState(int iIndex, BYTE btCrownState);
void GCAnsCsNotifyStart(int iIndex, BYTE btStartState);
void GCAnsCsNotifyProgress(int iIndex, BYTE btCastleSiegeState, char* lpszGuildName);
void GCAnsCsMapSvrTaxInfo(int iIndex, BYTE btTaxType, BYTE btTaxRate);
void CGReqCsMiniMapData(PMSG_REQ_MINIMAPDATA* lpMsg, int iIndex);
void GCAnsCsMiniMapData(int iIndex, BYTE btResult);
void CGReqStopCsMiniMapData(PMSG_REQ_STOPMINIMAPDATA* lpMsg, int iIndex);
void CGReqCsSendCommand(PMSG_REQ_CSCOMMAND* lpMsg, int iIndex);
void GCAnsCsSendCommand(int iCsJoinSide, BYTE btTeam, BYTE btX, BYTE btY, BYTE btCommand);
void GCAnsCsLeftTimeAlarm(BYTE btHour, BYTE btMinute);
void GCAnsSelfCsLeftTimeAlarm(int iIndex, BYTE btHour, BYTE btMinute);
void CGReqCsSetEnterHuntZone(PMSG_REQ_CSHUNTZONEENTER* lpMsg, int iIndex);
void GCAnsCsSetEnterHuntZone(int iIndex, BYTE btResult, BYTE btEnterHuntZone);
void CGReqNpcDbList(PMSG_REQ_NPCDBLIST* lpMsg, int iIndex);
void CGReqCsRegGuildList(PMSG_REQ_CSREGGUILDLIST* lpMsg, int iIndex);
void CGReqCsAttkGuildList(PMSG_REQ_CSATTKGUILDLIST* lpMsg, int iIndex);
void CGReqWeaponUse(PMSG_REQ_USEWEAPON* aRecv, int iIndex);
void CGReqWeaponDamageValue(PMSG_REQ_WEAPON_DAMAGE_VALUE* aRecv, int iIndex);
void CGReqGuildMarkOfCastleOwner(PMSG_REQ_GUILDMARK_OF_CASTLEOWNER* aRecv, int iIndex);
void CGReqJewelMix(PMSG_REQ_JEWEL_MIX* lpMsg, int iIndex);
void GCAnsJewelMix(int iIndex, int iResult);
void CGReqJewelUnMix(PMSG_REQ_JEWEL_UNMIX* lpMsg, int iIndex);
void GCAnsJewelUnMix(int iIndex, int iResult);
void CGReqCrywolfInfo(PMSG_REQ_CRYWOLF_INFO* lpMsg, int iIndex);
void GCAnsCrywolfInfo(int iIndex, BYTE btOccupationState, BYTE btCrywolfState);
void CGReqAlatrContract(PMSG_REQ_CRYWOLF_ALTAR_CONTRACT* lpMsg, int iIndex);
void CGReqPlusChaosRate(PMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE* lpMsg, int iIndex);
void CGReqKanturuStateInfo(PMSG_REQ_KANTURU_STATE_INFO* lpMsg, int iIndex);
void GCReqEnterKanturuBossMap(PMSG_REQ_ENTER_KANTURU_BOSS_MAP* lpMsg, int iIndex);
void GCStateInfoSend(OBJECTSTRUCT* lpObj, BYTE state, BYTE ViewSkillState, BOOL Send = TRUE, BOOL JustToViewport = FALSE);
void GCSendObjectCreationState(int iObjectIndex);
void CGReqCastleHuntZoneEntrance(PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE* aRecv, int iIndex);
void CGWereWolfQuarrelCheck(int aIndex);
void CGGateKeeperCheck(int aIndex);
void NewSeason6Attack(PMSG_NEWATTACK_S6 *lpMsg,int aIndex);

#endif