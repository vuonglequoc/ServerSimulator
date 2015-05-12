/*
	//-----------------------------------------------------------------------------------------------------------
#ifndef GDS_PRTOCOOL
#define GDS_PROTOCOL
//-----------------------------------------------------------------------------------------------------------

#include "..\..\Protocol\DataProtocol.h"	// ??
#include "TExManager.h"
//-----------------------------------------------------------------------------------------------------------

#if !defined(AFX_EDSPROTOCOL_H__675D1E8D_B9AC_45E3_8077_17EC82A9A5A5__INCLUDED_)
#define AFX_EDSPROTOCOL_H__675D1E8D_B9AC_45E3_8077_17EC82A9A5A5__INCLUDED_
//-----------------------------------------------------------------------------------------------------------

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//-----------------------------------------------------------------------------------------------------------

#define MAX_MAP_GROUPS 20		// mapserver
#define MAX_MAP_SUBGROUPS 20	// mapserver
// ----
#define MAX_MEMBER_GUILD	80
#define MAX_LOGIN_USER	(45000)
#define MAX_LOGIN_SERVER (MAX_MAP_GROUPS*MAX_MAP_SUBGROUPS+1)
//-----------------------------------------------------------------------------------------------------------

struct SDHP_USERCLOSE
{
	PBMSG_HEAD		h;
	char			CharName[10];	//	3
	char			GuildName[8];	//	D
	unsigned char	Type;			//	15
};
//-----------------------------------------------------------------------------------------------------------

struct SDHP_GUILDCREATE
{
	PBMSG_HEAD	h;
	char		GuildName[9];	// 3
	char		Master[11];		// C
	BYTE		Mark[32];		// 17
	BYTE		NumberH;		// 37
	BYTE		NumberL;		// 38
	BYTE		btGuildType;	// 39
};
//-----------------------------------------------------------------------------------------------------------

struct SDHP_GUILDCREATE_RESULT
{
	PBMSG_HEAD		h;
	unsigned char	Result;			//	3
	unsigned char	Flag;			//	4
	unsigned long	GuildNumber;	//	8
	unsigned char	NumberH;		//	C
	unsigned char	NumberL;		//	D
	char			Master[11];		//	E
	char			GuildName[9];	//	19
	unsigned char	Mark[32];		//	22
	unsigned char	btGuildType;	//	42
};
//-----------------------------------------------------------------------------------------------------------

struct SDHP_GUILDDESTROY
{
	PBMSG_HEAD	h;
	BYTE		NumberH;		// 3
	BYTE		NumberL;		// 4
	char		GuildName[8];	// 5
	char		Master[10];		// D
};
//-----------------------------------------------------------------------------------------------------------

struct SDHP_GUILDDESTROY_RESULT
{
	PBMSG_HEAD		h;
	unsigned char	Result;			//	3
	unsigned char	Flag;			//	4
	unsigned char	NumberH;		//	5
	unsigned char	NumberL;		//	6
	char			GuildName[9];	//	7
	char			Master[11];		//	10
};
//-----------------------------------------------------------------------------------------------------------

struct SDHP_GUILDMEMBERADD
{
	PBMSG_HEAD	h;
	char		GuildName[9];	// 3
	char		MemberID[11];	// C
	BYTE		NumberH;		// 17
	BYTE		NumberL;		// 18
};
//-----------------------------------------------------------------------------------------------------------

struct SDHP_GUILDMEMBERADD_RESULT
{
	PBMSG_HEAD		h;
	unsigned char	Result;			//	3
	unsigned char	Flag;			//	4
	unsigned char	NumberH;		//	5
	unsigned char	NumberL;		//	6
	char			GuildName[9];	//	7
	char			MemberID[11];	//	10
	short			pServer;		//	1C
};
//-----------------------------------------------------------------------------------------------------------

struct SDHP_GUILDMEMBERDEL
{
	PBMSG_HEAD	h;				// C1:33
	BYTE		NumberH;		// 3
	BYTE		NumberL;		// 4
	char		GuildName[8];	// 5
	char		MemberID[10];	// D
};
//-----------------------------------------------------------------------------------------------------------

struct SDHP_GUILDMEMBERDEL_RESULT
{
	PBMSG_HEAD		h;
	unsigned char	Result;			//	3
	unsigned char	Flag;			//	4
	unsigned char	NumberH;		//	5
	unsigned char	NumberL;		//	6
	char			GuildName[9];	//	7
	char			MemberID[11];	//	10
};
//-----------------------------------------------------------------------------------------------------------

struct SDHP_GUILDUPDATE
{
	PBMSG_HEAD		h;				//	
	char			GuildName[9];	//	3
	char			Master[11];		//	C
	unsigned char	Mark[32];		//	17
	int				Score;			//	28
	unsigned char	Count;			//	3C
};
//-----------------------------------------------------------------------------------------------------------

struct SDHP_GUILDMEMBER_INFO_REQUEST
{
	PBMSG_HEAD	h;				// C1:35
	BYTE		NumberH;		// 3
	BYTE		NumberL;		// 4
	char		MemberID[10];	// 5
};
//-----------------------------------------------------------------------------------------------------------

struct SDHP_GUILDMEMBER_INFO
{
	PBMSG_HEAD		h;				//	
	char			GuildName[9];	//	3
	char			MemberID[11];	//	C
	unsigned char	btGuildStatus;	//	17
	unsigned char	btGuildType;	//	18
	short			pServer;		//	1A
};
//-----------------------------------------------------------------------------------------------------------

struct SDHP_GUILDALL_COUNT
{
	PWMSG_HEAD		h;
	int				Number;					// 4
	char			GuildName[9];			// 8
	char			Master[11];				// 11
	unsigned char	Mark[32];				// 1C
	int				score;					// 3C
	BYTE			btGuildType;			// 40
	int				iGuildUnion;			// 44
	int				iGuildRival;			// 48
	char			szGuildRivalName[9];	// 4C
	unsigned char	Count;					// 55
};
//-----------------------------------------------------------------------------------------------------------

struct SDHP_GUILDALL
{
	char	MemberID[11];	// 0
	BYTE	btGuildStatus;	// B
	short	pServer;		// C
};
//-----------------------------------------------------------------------------------------------------------

struct SDHP_GUILDSCOREUPDATE
{
	PBMSG_HEAD	h;				//	
	char		GuildName[9];	//	3
	int			Score;			//	C
};
//-----------------------------------------------------------------------------------------------------------

struct SDHP_GUILDNOTICE
{
	PBMSG_HEAD	h;					//	
	char		GuildName[9];		//	3
	char		szGuildNotice[60];	//	C
};
//-----------------------------------------------------------------------------------------------------------

struct SDHP_GUILDCREATED
{
	PBMSG_HEAD		h;				//	
	unsigned short	Number;			//	4
	char			GuildName[9];	//	6
	char			Master[11];		//	F
	unsigned char	Mark[32];		//	1A
	int				score;			//	3C
};
//-----------------------------------------------------------------------------------------------------------

struct SDHP_GUILDLISTSTATE
{
	PBMSG_HEAD		h;		//	
	unsigned char	State;	//	3
	int				Count;	//	4
};
//-----------------------------------------------------------------------------------------------------------

struct EXSDHP_GUILD_ASSIGN_STATUS_REQ
{
	PBMSG_HEAD	h;
	WORD		wUserIndex;			// 4
	BYTE		btType;				// 6
	BYTE		btGuildStatus;		// 7
	char		szGuildName[9];		// 8
	char		szTargetName[11];	// 11
};
//-----------------------------------------------------------------------------------------------------------

struct EXSDHP_GUILD_ASSIGN_TYPE_REQ
{
	PBMSG_HEAD	h;
	WORD		wUserIndex;		// 4
	BYTE		btGuildType;	// 6;
	char		szGuildName[9];	// 7
};
//-----------------------------------------------------------------------------------------------------------

struct EXSDHP_GUILD_ASSIGN_TYPE_RESULT
{
	PBMSG_HEAD		h;				//	
	unsigned char	btFlag;			//	3
	unsigned short	wUserIndex;		//	4
	unsigned char	btGuildType;	//	6
	unsigned char	btResult;		//	7
	char			szGuildName[9];	//	8
};
//-----------------------------------------------------------------------------------------------------------

struct EXSDHP_GUILD_ASSIGN_STATUS_RESULT
{
	PBMSG_HEAD		h;					//	
	unsigned char	btFlag;				//	3
	unsigned short	wUserIndex;			//	4
	unsigned char	btType;				//	6
	unsigned char	btResult;			//	7
	unsigned char	btGuildStatus;		//	8
	char			szGuildName[9];		//	9
	char			szTargetName[11];	//	12
};
//-----------------------------------------------------------------------------------------------------------

struct EXSDHP_SERVERGROUP_GUILD_CHATTING_RECV
{
	PBMSG_HEAD	h;						//	
	int			iGuildNum;				//	4
	char		szCharacterName[10];	//	8
	char		szChattingMsg[60];		//	12
};
//-----------------------------------------------------------------------------------------------------------

struct EXSDHP_SERVERGROUP_UNION_CHATTING_RECV
{
	PBMSG_HEAD	h;						//	
	int			iUnionNum;				//	4
	char		szCharacterName[10];	//	8
	char		szChattingMsg[60];		//	12
};
//-----------------------------------------------------------------------------------------------------------

struct EXSDHP_RELATIONSHIP_JOIN_RESULT
{
	PBMSG_HEAD		h;						//	
	unsigned char	btFlag;					//	3
	unsigned short	wRequestUserIndex;		//	4
	unsigned short	wTargetUserIndex;		//	6
	unsigned char	btResult;				//	8
	unsigned char	btRelationShipType;		//	9
	int				iRequestGuildNum;		//	C
	int				iTargetGuildNum;		//	10
	char			szRequestGuildName[9];	//	14
	char			szTargetGuildName[9];	//	1D
};
//-----------------------------------------------------------------------------------------------------------

struct EXSDHP_RELATIONSHIP_BREAKOFF_RESULT
{
	PBMSG_HEAD		h;					//	
	unsigned char	btFlag;				//	3
	unsigned short	wRequestUserIndex;	//	4
	unsigned short	wTargetUserIndex;	//	6
	unsigned char	btResult;			//	8
	unsigned char	btRelationShipType;	//	9
	int				iRequestGuildNum;	//	C
	int				iTargetGuildNum;	//	10
};
//-----------------------------------------------------------------------------------------------------------

struct EXSDHP_NOTIFICATION_RELATIONSHIP
{
	PWMSG_HEAD		h;					//	
	unsigned char	btFlag;				//	4
	unsigned char	btUpdateFlag;		//	5
	unsigned char	btGuildListCount;	//	6
	int				iGuildList[100];	//	8
};
//-----------------------------------------------------------------------------------------------------------

struct EXSDHP_UNION_RELATIONSHIP_LIST
{
	PWMSG_HEAD		h;							//	
	unsigned char	btFlag;						//	4
	unsigned char	btRelationShipType;			//	5
	unsigned char	btRelationShipMemberCount;	//	6
	char			szUnionMasterGuildName[9];	//	7
	int				iUnionMasterGuildNumber;	//	10
	int				iRelationShipMember[100];	//	14
};
//-----------------------------------------------------------------------------------------------------------

struct EXSDHP_KICKOUT_UNIONMEMBER_RESULT
{
	PBMSG_HEAD2		h;							//	
	unsigned char	btFlag;						//	4
	unsigned short	wRequestUserIndex;			//	5
	unsigned char	btRelationShipType;			//	8
	unsigned char	btResult;					//	9
	char			szUnionMasterGuildName[9];	//	A
	char			szUnionMemberGuildName[9];	//	13
};
//-----------------------------------------------------------------------------------------------------------

struct FHP_WAITFRIENDLIST_COUNT
{
	PBMSG_HEAD	h;				//	
	short		Number;			//	4
	char		Name[10];		//	6
	char		FriendName[10];	//	10
};
//-----------------------------------------------------------------------------------------------------------

struct PMSG_FRIEND_ADD_REQ
{
	PBMSG_HEAD	h;
	char		Name[10];	// 3
};
//-----------------------------------------------------------------------------------------------------------

struct PMSG_FRIEND_STATE_C
{
	PBMSG_HEAD		h;			//	
	unsigned char	ChatVeto;	//	3
};
//-----------------------------------------------------------------------------------------------------------

struct FHP_FRIEND_STATE
{
	PBMSG_HEAD		h;				//	
	short			Number;			//	4
	char			Name[10];		//	6
	char			FriendName[10];	//	10
	unsigned char	State;			//	1A
};
//-----------------------------------------------------------------------------------------------------------

struct FHP_FRIEND_ADD_RESULT
{
	PBMSG_HEAD		h;
	short			Number;			// 4
	unsigned char	Result;			// 6
	char			Name[10];		// 7
	char			FriendName[10];	// 11
	unsigned char	Server;			// 1B
};
//-----------------------------------------------------------------------------------------------------------

struct PMSG_FRIEND_ADD_SIN_RESULT
{
	PBMSG_HEAD			h;		//	
	unsigned char	Result;		//	3
	char			Name[10];	//	4
};
//-----------------------------------------------------------------------------------------------------------

struct FHP_WAITFRIEND_ADD_RESULT
{
	PBMSG_HEAD		h;				//	
	short			Number;			//	4
	unsigned char	Result;			//	6
	char			Name[10];		//	7
	char			FriendName[10];	//	11
	unsigned char	pServer;		//	1B
};
//-----------------------------------------------------------------------------------------------------------

struct PMSG_FRIEND_DEL_REQ
{
	PBMSG_HEAD	h;			//	
	char		Name[10];	//	3
};
//-----------------------------------------------------------------------------------------------------------

struct FHP_FRIEND_DEL_RESULT
{
	PBMSG_HEAD		h;				//	
	short			Number;			//	4
	unsigned char	Result;			//	6
	char			Name[10];		//	7
	char			FriendName[10];	//	11
};
//-----------------------------------------------------------------------------------------------------------

struct PMSG_FRIEND_ROOMCREATE_REQ
{
	PBMSG_HEAD	h;			//	
	char		Name[10];	//	3
};
//-----------------------------------------------------------------------------------------------------------

struct FHP_FRIEND_CHATROOM_CREATE_RESULT
{
	PBMSG_HEAD		h;				//	
	unsigned char	Result;			//	3
	short			Number;			//	4
	char			Name[10];		//	6
	char			FriendName[10];	//	10
	char			ServerIp[15];	//	1A
	unsigned short	RoomNumber;		//	2A
	unsigned long	Ticket;			//	2C
	unsigned char	Type;			//	30
};
//-----------------------------------------------------------------------------------------------------------

struct PMSG_FRIEND_MEMO
{
	PWMSG_HEAD		h;				//	
	unsigned long	WindowGuid;		//	4
	char			Name[10];		//	8
	char			Subject[32];	//	12
	unsigned char	Dir;			//	32
	unsigned char	Action;			//	33
	short			MemoSize;		//	34
	char			Memo[1000];		//	36
};
//-----------------------------------------------------------------------------------------------------------

struct PMSG_JG_MEMO_SEND
{
	PWMSG_HEAD	h;
	char		Name[10];		// 4
	char		TargetName[10];	// E
	char		Subject[32];	// 18
	short		MemoSize;		// 38
	char		Memo[1000];		// 3A
};
//-----------------------------------------------------------------------------------------------------------

struct FHP_FRIEND_MEMO_SEND_RESULT
{
	PBMSG_HEAD		h;			//	
	short			Number;		//	4
	char			Name[10];	//	6
	unsigned char	Result;		//	10
	unsigned long	WindowGuid;	//	14
};
//-----------------------------------------------------------------------------------------------------------

struct FHP_FRIEND_MEMO_LIST
{
	PWMSG_HEAD		h;
	unsigned short	Number;			//	4
	unsigned short	MemoIndex;		//	6
	char			SendName[10];	//	8
	char			RecvName[10];	//	12
	char			Date[30];		//	1C
	char			Subject[32];	//	3A
	unsigned char	read;			//	5A
};
//-----------------------------------------------------------------------------------------------------------

struct PMSG_FRIEND_READ_MEMO_REQ
{
	PBMSG_HEAD		h;
	unsigned short	MemoIndex;	//	4
};
//-----------------------------------------------------------------------------------------------------------

struct FHP_FRIEND_MEMO_RECV
{
	PWMSG_HEAD		h;
	short			Number;		//	4
	char			Name[10];	//	6
	unsigned short	MemoIndex;	//	10
	short			MemoSize;	//	12
	unsigned char	Photo[18];	//	14
	unsigned char	Dir;		//	26
	unsigned char	Action;		//	27
	char			Memo[1000];	//	28
};
//-----------------------------------------------------------------------------------------------------------

struct PMSG_FRIEND_MEMO_DEL_REQ
{
	PBMSG_HEAD		h;
	unsigned short	MemoIndex;	//	4
};
//-----------------------------------------------------------------------------------------------------------

struct FHP_FRIEND_MEMO_DEL_RESULT
{
	PBMSG_HEAD		h;	
	unsigned char	Result;		//	3
	unsigned short	MemoIndex;	//	4
	short			Number;		//	6
	char			Name[10];	//	8
};
//-----------------------------------------------------------------------------------------------------------

struct PMSG_ROOM_INVITATION
{
	PBMSG_HEAD		h;
	char			Name[10];	//	3
	unsigned short	RoomNumber;	//	E
	unsigned long	WindowGuid;	//	10
};
//-----------------------------------------------------------------------------------------------------------

struct FHP_FRIEND_INVITATION_RET
{
	PBMSG_HEAD		h;
	unsigned char	Result;		// 3
	short			Number;		// 4
	char			Name[10];	// 6
	DWORD			WindowGuid;	// 10
};
//-----------------------------------------------------------------------------------------------------------

struct tagGUILD_INFO_STRUCT
{
	tagGUILD_INFO_STRUCT()
	{
		m_iNumber		= -1;
		m_btGuildType	= -1;
		m_iRivalGuild	= 0;
		m_iUnionGuild	= 0;
		m_Score			= 0;
		// ----
		memset(m_szGuildName, 0, sizeof(m_szGuildName));
		memset(m_szGuildMaster, 0, sizeof(m_szGuildMaster));
		memset(m_szGuildMember, 0, sizeof(m_szGuildMember));
		memset(m_btGuildStatus, 0, sizeof(m_btGuildStatus));
		memset(m_btConnected, 0, sizeof(m_btConnected));
		memset(m_Mark, 0, sizeof(m_Mark));
	}

	int		m_iNumber;
	char	m_szGuildName	[8];
	char	m_szGuildMaster	[10];
	BYTE	m_btGuildType;
	char	m_szGuildMember	[MAX_MEMBER_GUILD][10];
	BYTE	m_btGuildStatus	[MAX_MEMBER_GUILD];
	BYTE	m_btConnected	[MAX_MEMBER_GUILD];
	int		m_iRivalGuild;
	int		m_iUnionGuild;
	BYTE	m_Mark			[32];
	int		m_Score;
};
//-----------------------------------------------------------------------------------------------------------

struct strCmp
{
	bool operator()(const std::string s1, const std::string s2) const 
	{
		return strcmp(s1.data(), s2.data()) < 0;
	}
};
//-----------------------------------------------------------------------------------------------------------

#pragma pack()
//-----------------------------------------------------------------------------------------------------------

struct tagUNION_DATA
{
	std::vector<int>	m_vecUnionMember;
	std::vector<int>	m_vecRivalMember;
};
//-----------------------------------------------------------------------------------------------------------

BOOL												g_bCanCreateDeleteGuild = true;	// need config in file
std::map<std::string, tagGUILD_INFO_STRUCT, strCmp>	g_mapGuildManager;
std::map<int, tagUNION_DATA>						g_mapUnionManager;
//-----------------------------------------------------------------------------------------------------------

void	ExDataServerLogin(TExManager * thisManager, int aIndex, SDHP_SERVERINFO * lpMsg);
void	ExProtocolCore(TExManager * thisManager, int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize);
void	GDCharCloseRecv(TExManager * thisManager, int aIndex, SDHP_USERCLOSE * aRecv);
void	GDGuildCreateSend(TExManager * thisManager, int aIndex, SDHP_GUILDCREATE * aRecv);
void	GDGuildDestroyRecv(TExManager * thisManager, int aIndex, SDHP_GUILDDESTROY * aRecv);
void	GDGuildMemberAdd(TExManager * thisManager, int aIndex, SDHP_GUILDMEMBERADD * aRecv);
void	GDGuildMemberDel(TExManager * thisManager, int aIndex, SDHP_GUILDMEMBERDEL * aRecv);
void	DGGuildMemberInfoRequest(TExManager * thisManager, int aIndex, SDHP_GUILDMEMBER_INFO_REQUEST * aRecv);
void	DGGuildScoreUpdate(TExManager * thisManager, int aIndex, SDHP_GUILDSCOREUPDATE * aRecv);
void	GDGuildNoticeSave(TExManager * thisManager, int aIndex, SDHP_GUILDNOTICE * aRecv);
void	GDGuildReqAssignStatus(TExManager * thisManager, int aIndex, EXSDHP_GUILD_ASSIGN_STATUS_REQ * aRecv);
void	GDGuildReqAssignType(TExManager * thisManager, int aIndex, EXSDHP_GUILD_ASSIGN_TYPE_REQ * aRecv);
//-----------------------------------------------------------------------------------------------------------

/*int		ServerFind(WORD ServerCode)
{
	int iReturn		= -1;
	// ---
	for(int i = 0; i < MAX_LOGIN_SERVER; i++)
	{
		if(g_MuLoginServerData[i].m_wServerCode == ServerCode)
		{
			iReturn = i;
		}
	}
	// ----
	return iReturn;
}*//*
//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------

#endif // !defined(AFX_EDSPROTOCOL_H__675D1E8D_B9AC_45E3_8077_17EC82A9A5A5__INCLUDED_)
//-----------------------------------------------------------------------------------------------------------
#endif /* GDS_PRTOCOOL */
//-----------------------------------------------------------------------------------------------------------*/