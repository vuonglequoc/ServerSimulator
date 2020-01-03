//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: IllusionTemple Event									//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#pragma once

#include <list>
#include "User.h"

using namespace std;


enum IT_PROTOCOL 
{
	IT_PROTOCOL_REGISTER	= 0x00,
	IT_PROTOCOL_USESKILL	= 0x02,
	IT_PROTOCOL_ADDREWARD	= 0x05,
};

#pragma pack(1)

struct PMSG_IT_REGISTER
{
	PBMSG_HEAD2 h;
	BYTE TargetTemple;
	BYTE TicketPosition;
};

struct PMSG_ILLUSION_USESKILL
{
	PBMSG_HEAD2	h;
	WORD m_SkillIndex;
	WORD m_TargetIndex;
	BYTE m_Unknow;
};

#pragma pack()

typedef struct
{
	int m_MinLevel;
	int m_MaxLevel;
} ILLUSION_ENTER_RULE, *LPILLUSION_ENTER_RULE;

typedef struct
{
	int m_iStartHour;
	int m_iStartMinute;
} ILLUSION_START_TIME, *LPILLUTION_START_TIME;

typedef struct
{
	int		m_Index;
	int		m_BonusCount;
	int		m_TeamIndex;
	int		m_TempleIndex;
} ILLUSION_OBJ, *LPILLUSION_OBJ;

typedef list<ILLUSION_OBJ>::iterator	ILLUSION_OBJIT;

typedef struct 
{
	int					m_Score;
	list<ILLUSION_OBJ>	m_Members;
	int					m_BallOwner;
} ILLUSION_TEAM, *LPILLUSION_TEAM;

#define ILLUSION_PLAY_TIME			15
#define MIN_ILLUSION_TEAM_MEMBERS	1
#define MAX_ILLUSION_TEAM_MEMBERS	5
#define MAX_ILLUSION_TEAMS			2

#define IT_MASK_RED		405
#define IT_MASK_BLUE	404

enum IT_KILLPOINT_OPERATION
{
	IT_KILLPOINT_OPERATION_ADD = 0x37C,
	IT_KILLPOINT_OPERATION_SUBTRACT = 0x37F
};

enum IT_TEAMS
{
	IT_TEAM_RED,
	IT_TEAM_BLUE
};

enum IT_STATE
{
	IT_STATE_NONE,
	IT_STATE_CLOSED,
	IT_STATE_OPEN,
	IT_STATE_PREPARE,
	IT_STATE_BATTLE,
	IT_STATE_END
};

enum IT_STATE_MSG
{
	IT_STATE_MSG_CLOSING = 10,
	IT_STATE_MSG_READY = 20
};

enum IT_STATUE
{
	IT_STATUE_UP,
	IT_STATUE_DOWN
};

#define MAX_IT_STATUES	2

static const struct IT_STATUE_POSITION
{
	int PositionX;
	int PositionY;
	int Direction;
} s_itStatuePosition[MAX_IT_STATUES] = 
{
	{ 134, 121, 2 },
	{ 207,  47, 2 }
};

typedef struct 
{
	int				m_TempleIndex;
	int				m_TempleState;
	int				m_RemainTime;
	int				m_TickCount;
	int				m_StatisticsTickCount;
	int				m_ScoreFactorA;
	int				m_ScoreFactorB;
	ILLUSION_TEAM	m_Teams[MAX_ILLUSION_TEAMS];
	int				m_SendNoticeCount;
	int				m_iUserCount;
	list<int>		m_SpawnedMonsters;
	int				m_Statues[MAX_IT_STATUES];
	int				m_BallOwner;
} ILLUSION_TEMPLE_DATA, *LPILLUSION_TEMPLE_DATA;

#define MAX_ILLUSION_TEMPLES		6
#define MIN_ILLUSION_PLAYERCOUNT	2

#define IT_OPEN_TIME_MIN			1
#define IT_PREPARE_TIME				1

typedef struct
{
	int					m_ItemType;
	int					m_ItemIndex;
	int					m_ItemLevel;
} ILLUSION_REWARD_INFO, *LPILLUSION_REWARD_INFO;

#define IT_FLOOR_RANGE(index)	(((index) < 0) ? FALSE : ((index) > (MAX_ILLUSION_TEMPLES-1)) ? FALSE : TRUE )
#define IT_FLOOR_RANGEX(index)	(((index) < 1) ? FALSE : ((index) > (MAX_ILLUSION_TEMPLES)) ? FALSE : TRUE )

#if(!GS_CASTLE)
class CIllusionTemple
{
private:
	bool						m_IllusionLoaded;
	bool						m_IllusionActive;
	bool						m_IllusionInit;
	bool						m_IllusionOpen;
	int							m_IllusionStartTimer;
	list<ILLUSION_START_TIME>	m_IllusionOpenTime;
	ILLUSION_ENTER_RULE			m_IllusionRules[MAX_ILLUSION_TEMPLES];
	ILLUSION_REWARD_INFO		m_IllusionRewards[MAX_ILLUSION_TEMPLES];
	CRITICAL_SECTION			m_Critical;
	DWORD						m_dwUserCheckTick;

public:
	ILLUSION_TEMPLE_DATA		m_IllusionTemple[MAX_ILLUSION_TEMPLES];

private:
	int							m_iMonsterKillPoint;
	int							m_iPlayerKillPoint;
	int							m_iBallKillPoint;

public:
	CIllusionTemple(void)
	{
		InitializeCriticalSection(&this->m_Critical);
		this->m_IllusionLoaded = false;
		this->m_IllusionActive = false;
		this->m_IllusionInit = false;
		this->m_IllusionOpen = false;
		this->m_dwUserCheckTick = GetTickCount();
		for(int i  = 0; i < MAX_ILLUSION_TEMPLES; i++)
		{
			this->m_IllusionTemple[i].m_TempleIndex = i;
		}
	}

	~CIllusionTemple(void)
	{
		DeleteCriticalSection(&this->m_Critical);
	}
	
	void Load(const char* szFile);

	void Run();
	int GetObjTempleIndex(OBJECTSTRUCT* lpObj);
	void SendAllUserAnyMsg(LPBYTE lpMsg, int iSize);
	int IsObjectInEvent(int iUserIndex);
	int IsObjectInEvent(OBJECTSTRUCT* lpObj);
	void ProcessDieUser(OBJECTSTRUCT* lpObj);
	
	void UseSkillProc(OBJECTSTRUCT* lpObj, PMSG_ILLUSION_USESKILL* pMsg);

	void ProcessUserKiller(OBJECTSTRUCT* lpObj, OBJECTSTRUCT* lpTarget);
	void ProcessMonsterKiller(OBJECTSTRUCT* lpObj, OBJECTSTRUCT* lpTarget);

	int GetUserCount(int iTempleIndex);
	void UpdateTempleStatus(int iTempleIndex);

	void SetTempleMonsters(int iTempleIndex);
	void UnSetTempleMonsters(int iTempleIndex);

	/*
	protocol
	*/
	void ProtocolCore(unsigned char* lpBuffer, int iUserIndex);

	/*
	sync issues
	*/
	void Synchronize();
	void SynchronizeTemple(int iTempleIndex);
	bool UpdateTemple(int iTempleIndex);
	void UserStateCheck(int iTempleIndex);
	void SetTempleState(int iTempleIndex, int iTempleState);
	void SetState(int iTempleState);
	void ManualStateSwitch(bool bOpen)
	{
		if(bOpen)
		{
			this->SetState(IT_STATE_OPEN);
		}
		else
		{
			this->SetState(IT_STATE_CLOSED);
		}
	}

	/*
	event states proc
	*/
	void DoProc_Closed(int iTempleIndex);
	void DoProc_Open(int iTempleIndex);
	void DoProc_Prepare(int iTempleIndex);
	void DoProc_Battle(int iTempleIndex);
	void DoProc_End(int iTempleIndex);

	bool SetProc_Closed(int iTempleIndex);
	bool SetProc_Open(int iTempleIndex);
	bool SetProc_Prepare(int iTempleIndex);
	bool SetProc_Battle(int iTempleIndex);
	bool SetProc_End(int iTempleIndex);

	/*
	npcs
	*/
	bool NpcTalk(OBJECTSTRUCT* lpNpc, OBJECTSTRUCT* lpObj);
	void NpcRegister(OBJECTSTRUCT* lpObj, PMSG_IT_REGISTER* pMsg);

	/*
	helpers
	*/
	void SendMsgWaitRoom(int iTempleIndex, int iMessageType, char* Message, ...);
	void SendMsgTemple(int iTempleIndex, int iMessageType, char* Message, ...);
	void SendMsgTempleTeam(int TempleIndex, int TeamIndex, int iMessageType, char* Message, ...);
	void SendDataWaitRoom(int iTempleIndex, BYTE* szBuffer, int iSize);
	void SendDataTemple(int iTempleIndex, BYTE* szBuffer, int iSize);
	void SendDataTempleTeam(int iTempleIndex, int iTeamIndex, BYTE* szBuffer, int iSize);

};
extern CIllusionTemple	g_IllusionTemple;

#endif