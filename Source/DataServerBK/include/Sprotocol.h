// Sprotocol.h: interface for the Sprotocol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPROTOCOL_H__5309DA38_D6AD_420B_896B_CB391FCD3624__INCLUDED_)
#define AFX_SPROTOCOL_H__5309DA38_D6AD_420B_896B_CB391FCD3624__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MAX_LOGIN_USER	(45000)
#define MAX_LOGIN_SERVER (MAX_MAP_GROUPS*MAX_MAP_SUBGROUPS+1)






struct SDHP_IDPASS
{
	PBMSG_HEAD h;	// C1:01	[11::LOGIN-TEST]
	char Id[10];	// 3
	char Pass[10];	// D
	short Number;	// 18
	char IpAddress[17];	// 1A
};


// EventControl
//-----------------------------
#define DRAGON_EVENT		0
#define XMAS_EVENT			1
#define FIRECRACKER_EVENT	2
#define HEARTOFLOVE_EVENT	3
#define MEDAL_EVENT			4

// OptionControl
//-----------------------------
#define OPTION_RELOAD		0
#define MONSTER_RELOAD		1
#define SHOP_RELOAD			2
#define ALLOPTION_RELOAD	3
#define EVENTINFO_RELOAD	4
#define ETCINFO_RELOAD		5
#define OPTION_CONTROL		6








/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet User Close by Id
 *	Direction : GameServer -> JoinServer
 *  Code : 0xC1
 *	HeadCode : 0x05
 */
struct SDHP_USERCLOSE_ID
{
	PBMSG_HEAD h;
	char szId[10];
	char szName[10];
	unsigned short Level;
	unsigned char DbClass;

};




/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Id Pass Result
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x01
 */
struct SDHP_IDPASSRESULT
{
	PBMSG_HEAD h;
	unsigned char result;	// 3
	short Number;	// 4
	char Id[10];	//	6
	int UserNumber;
	int DBNumber;
	char JoominNumber[13];
};




/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Join Fail
 *	Direction : GameServer -> JoinServer
 *  Code : 0xC1
 *	HeadCode : 0x02
 */
struct SDHP_JOINFAIL
{
	PBMSG_HEAD h;
	short Number;
	char Id[11];
	int UserNumber;
	int DBNumber;

};




/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Join Fail
 *	Direction : GameServer -> JoinServer
 *  Code : 0xC1
 *	HeadCode : 0x04
 */
struct SDHP_COMMAND_BLOCK
{
	PBMSG_HEAD h;
	short Number;
	char Id[11];
	int UserNumber;
	int DBNumber;
	unsigned char BlockCode;
};



struct SDHP_BILLSEARCH
{
	PBMSG_HEAD h;	// C1:06
	char Id[10];	// 3
	short Number;	// E
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Bill Search Result
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x06
 */
struct SDHP_BILLSEARCH_RESULT
{
	PBMSG_HEAD h;
	char Id[10];
	short Number;
	unsigned char cCertifyType;
	unsigned char PayCode;
	char EndsDays[12];
	int EndTime;
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Bill Kill User
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x07, 0x09
 */
struct SDHP_BILLKILLUSER
{
	struct PBMSG_HEAD h;
	char Id[10];
	short Number;
};







/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Other Join Message
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x08
 */
struct SDHP_OTHERJOINMSG
{
	struct PBMSG_HEAD h;
	char AccountID[10];
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Other Join Message
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x20
 */
struct SDHP_EXITMSG
{
	PBMSG_HEAD h;
	unsigned char ExitCode[3];
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Other Join Message
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x30
 */
struct SDHP_EVENTSTART
{
	PBMSG_HEAD h;
	unsigned char Event;
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Other Join Message
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x40
 */
struct SDHP_EVENTCONTROL
{
	PBMSG_HEAD h;
	unsigned char Event;
	unsigned char Stat;
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Other Join Message
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x41
 */
struct SDHP_OPTIONCONTROL
{
	PBMSG_HEAD h;
	unsigned char Option;
};





/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Join Fail
 *	Direction : GameServer -> JoinServer
 *  Code : 0xC1
 *	HeadCode : 0x30
 */
struct SDHP_LOVEHEARTEVENT
{
	PBMSG_HEAD h;
	char Account[10];
	char Name[10];
};






/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Other Join Message
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x61
 */
struct SDHP_LOVEHEARTEVENT_RESULT
{
	PBMSG_HEAD h;
	unsigned char Result;
	char Name[10];
	int Number;
};






/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Join Fail
 *	Direction : GameServer -> JoinServer
 *  Code : 0xC1
 *	HeadCode : 0x31
 */
struct SDHP_LOVEHEARTCREATE
{
	PBMSG_HEAD h;
	unsigned char x;
	unsigned char y;
	unsigned char MapNumber;
};




/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Other Join Message
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x62
 */
struct SDHP_NOTICE
{
	PBMSG_HEAD h;
	char Notice[61];
};




/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Other Join Message
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x63
 */
struct SDHP_USER_NOTICE
{
	PBMSG_HEAD h;
	char szId[10];
	char Notice[61];
};




/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Join Fail
 *	Direction : GameServer -> JoinServer
 *  Code : 0xC1
 *	HeadCode : 0x7A
 */
struct PMSG_REQ_MAPSVRMOVE
{
	PBMSG_HEAD h;
	int iIndex;
	char szAccountID[11];
	char szCharName[11];
	unsigned short wCurMapSvrCode;
	unsigned short wDstMapSvrCode;
	unsigned short wMapNumber;
	unsigned char btX;
	unsigned char btY;
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Other Join Message
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x7A
 */
struct PMSG_ANS_MAPSVRMOVE
{
	PBMSG_HEAD h;
	int iIndex;
	char szAccountID[11];
	char szCharName[11];
	unsigned short wCurMapSvrCode;
	unsigned short wDstMapSvrCode;
	unsigned short wMapNumber;
	unsigned char btX;
	unsigned char btY;
	int iResult;
	int iJoinAuthCode1;
	int iJoinAuthCode2;
	int iJoinAuthCode3;
	int iJoinAuthCode4;
};






/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Map Server Auth Answer
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x7A ?????????????????????????????? #error
 */
struct PMSG_ANS_MAPSVRAUTH
{
	PBMSG_HEAD h;
	int iIndex;
	char szAccountID[11];
	char szCharName[11];
	WORD wPrevMapSvrCode;
	WORD wMapNumber;
	BYTE btX;
	BYTE btY;
	int iResult;
	int iUserNumber;
	int iDBNumber;
	char cJoominNumber[13];
	unsigned char btBlockCode;
};




/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Join Fail
 *	Direction : GameServer -> JoinServer
 *  Code : 0xC1
 *	HeadCode : 0x7B 
 */
struct PMSG_REQ_MAPSVRAUTH
{
	PBMSG_HEAD h;
	int iIndex;
	char szAccountID[11];
	char szCharName[11];
	unsigned short wDstMapSvrCode;
	int iJoinAuthCode1;
	int iJoinAuthCode2;
	int iJoinAuthCode3;
	int iJoinAuthCode4;
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Notify Max User
 *	Direction : GameServer -> JoinServer
 *  Code : 0xC1
 *	HeadCode : 0x7C
 */
struct PMSG_NOTIFY_MAXUSER
{
	PBMSG_HEAD h;
	int iSvrCode;	// 4
	int iMaxUserCount;	// 8
	int iCurUserCount;	//	C
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Join Fail
 *	Direction : GameServer -> 
 *  Code : 0xC1
 *	HeadCode : 0x60
 */
/*struct PMSG_JG_MEMO_SEND
{
	PWMSG_HEAD h;
	char Name[10];
	char TargetName[10];
	char Subject[32];
	short MemoSize;
	char Memo[1000];
};*/





#pragma pack(1)

struct tagJOIN_SERVER_USER_DATA
{
	char m_AccoundID[10];
	WORD m_ServerCode;
	bool m_bRequestMapSvrMove;
	int iUserNumber;
};


struct tagJOIN_SERVER_SERVER_DATA
{
	char m_szServerName[50];
	WORD m_wServerCode;
	WORD m_wPort;
	WORD m_wCurrentUser;
	WORD m_wMaxUser;
};



struct tagUSER_DATA 
{
	PMSG_ANS_MAPSVRMOVE pMapServerMoveData;
	int iUserNumber;
};


extern tagJOIN_SERVER_USER_DATA g_MuLoginUserData[MAX_LOGIN_USER];
extern tagJOIN_SERVER_SERVER_DATA g_MuLoginServerData[MAX_LOGIN_SERVER];
extern int g_MuLoginUserDataCount;
extern int g_MuLoginServerDataCount;
extern int g_iUserNumber;

#pragma pack()


void InitMeMuOnlineDB();
void JoinServerLogin(int aIndex, SDHP_SERVERINFO * lpMsg);
void SProtocolCore(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize);
void JGPAccountRequest(int aIndex, SDHP_IDPASS * aRecv);
void GJPAccountFail(int aIndex, SDHP_JOINFAIL * aRecv);
void GJPAccountBlock(int aIndex, SDHP_COMMAND_BLOCK * aRecv);
void GJPUserClose(int aIndex, SDHP_USERCLOSE_ID * aRecv );
void JGOtherJoin(int aIndex, LPTSTR szAccountID);
void GCJoinBillCheckSend(int aIndex, SDHP_BILLSEARCH * aRecv);
void LoveHeartEventRecv(int aIndex, SDHP_LOVEHEARTEVENT * aRecv);
void LoveHeartCreateSend(int aIndex, SDHP_LOVEHEARTCREATE * aRecv);
void GJReqMapSvrMove(int aIndex, PMSG_REQ_MAPSVRMOVE * aRecv);
void GJReqMapSvrAuth(int aIndex, PMSG_REQ_MAPSVRAUTH * aRecv);
void GJNotifyMaxUserCount(int aIndex, PMSG_NOTIFY_MAXUSER * aRecv);

#endif // !defined(AFX_SPROTOCOL_H__5309DA38_D6AD_420B_896B_CB391FCD3624__INCLUDED_)
