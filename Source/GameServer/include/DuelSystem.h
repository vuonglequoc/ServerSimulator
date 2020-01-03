//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Duel System
// # Developed By:	M.E.S & WolF
//---------------------------------------------------------------------------

#ifndef DUEL_SYSTEM_H
#define DUEL_SYSTEM_H

#include "User.h"

#define MAX_DUEL_ROOMS			4
#define MAX_DUEL_LEARNERS		10
#define DUEL_TIME				30 // 15 minutos

struct PMSG_DUEL_QUESTION_START // 0x01
{
	PBMSG_HEAD2 h;
	unsigned char NumberH;
	unsigned char NumberL;
	unsigned char szName[10];
};

struct PMSG_DUEL_SPEC_REQUEST
{
	PBMSG_HEAD2 h;
	unsigned char Room;
	unsigned char Unk;
};

struct PMSG_DUEL_LIFEBAR_INIT // 0x0D
{
	PBMSG_HEAD2 h;
	unsigned char Unknow00;
};

struct PMSG_DUEL_LIFEBAR_NAME // 0x07 00 
{
	PBMSG_HEAD2 h;
	WORD Type; // 4
	unsigned char szName1[10]; // 5
	unsigned char szName2[10]; // f
	unsigned char btObjId1H;
	unsigned char btObjId1L;
	unsigned char btObjId2H;
	unsigned char btObjId2L;
};

struct PMSG_DUEL_SPEC_ADD_DEL
{
	PBMSG_HEAD2 h;
	unsigned char szName[10];
};

struct PMSG_DUEL_SPEC_ADD // 0x08
{
	PBMSG_HEAD2 h;
	unsigned char szName[10];
};

struct PMSG_DUEL_SPEC_LIST // 0x0B
{
	PBMSG_HEAD2 h;
	unsigned char btCount;
	unsigned char szName[MAX_DUEL_LEARNERS][10];
};

struct PMSG_DUEL_START // 0x02
{
	PBMSG_HEAD2 h;	// C1:AA
	unsigned char bDuelStart;	// 3
	unsigned char NumberH;	// 4
	unsigned char NumberL;	// 5
	char szName[10];	// 6
};

struct PMSG_DUEL_SCORE // 0x04
{
	PBMSG_HEAD2 h;	
	unsigned char NumberH1;	
	unsigned char NumberL1;	
	unsigned char NumberH2;	
	unsigned char NumberL2;	
	unsigned char btDuelScore1;	
	unsigned char btDuelScore2;	
};

struct PMSG_DUEL_ROOMSTATUS // Complemento de 0x06
{
	unsigned char szName1[10];
	unsigned char szName2[10];
	unsigned char btDuelRunning;
	unsigned char btDuelOpen;
};

struct PMSG_DUEL_STATUS // 0x06
{
	PBMSG_HEAD2 h;
	PMSG_DUEL_ROOMSTATUS pRoomStatus[MAX_DUEL_ROOMS];
};

struct PMSG_DUEL_LIFEBAR_REFILL // 0x05
{
	PBMSG_HEAD2 h;
	unsigned char btObjId01H;
	unsigned char btObjId01L;
	unsigned char btObjId02H;
	unsigned char btObjId02L;
	unsigned char btLife01;
	unsigned char btLife02;
	unsigned char btShield01;
	unsigned char btShield02;
};

struct DUEL_ROOM
{
	bool	bFree;
	bool	bWaiting;

	LPOBJ	lpObj01;
	BYTE	btPoints01;

	LPOBJ	lpObj02;
	BYTE	btPoints02;

	LPOBJ	lpLearners[MAX_DUEL_LEARNERS];

	DWORD	dwTickCount;
	DWORD	dwStartTime;

	bool	bHasWinner;

};

struct DUEL_RESPAWN
{
	unsigned char LowX;
	unsigned char LowY;
	unsigned char HighX;
	unsigned char HighY;
};

struct DUEL_GATES
{
	int UserGate01;
	int UserGate02;
	int LearnerGate;
};

class CDuelSystem
{
public:
	CDuelSystem(void);
	~CDuelSystem(void);
	// ----
	DUEL_ROOM		m_Rooms[MAX_DUEL_ROOMS];
	DUEL_GATES		m_Gates[MAX_DUEL_ROOMS];
	DUEL_RESPAWN	m_Respawn[MAX_DUEL_ROOMS];
	// ----
	void Init();
	void Run();
	// ---- Protocol Manager
	void DuelProtocolManager(unsigned char * aRecv, int aIndex);
	void DuelRequest(unsigned char * aRecv, int aIndex);
	void DuelRequestAnswer(unsigned char * aRecv, int aIndex);
	void DuelRequestSpectator(unsigned char * aRecv, int aIndex);
	void DuelExitSpectator(int Room, int aIndex);
	void SendDuelEnd(LPOBJ lpObj);
	// ----
	void DuelCheckEnd(LPOBJ lpObj, LPOBJ lpTargetObj);
	void MoveToDuelMap(LPOBJ lpObj, LPOBJ lpTargetObj);
	void MoveDeadUser(LPOBJ lpObj);
	void SendDuelStatus(LPOBJ lpObj);
	void UpdateDuelScore(int iRoom);
	void SendLifebarStatus(int iRoom);
	void SendLifebarStatus(LPOBJ lpObj, int iRoom);
	void SendLifebarInit(LPOBJ lpObj, int iRoom);
	void SendSpectatorList(int iRoom);
	void SendSpectatorList(LPOBJ lpObj, int iRoom);
	void SendSpectatorAdd(int iSpecIndex, int iRoom);
	void SendSpectatorRemove(int iSpecIndex, int iRoom);
	int GetSpectatorCount(int Room);
	int GetFreeRoom();
	int GetUserDuelRoom(LPOBJ lpObj);
	bool CanDuel(LPOBJ lpObj, LPOBJ lpTargetObj);
	bool IsOnDuel(LPOBJ lpObj);
	bool AreOnDuel(LPOBJ lpObj, LPOBJ lpTargetObj);
	void UserDuelReset(LPOBJ lpObj);
	void RoomReset(int Room, bool dontmove, bool dontsendend);
	void DuelReset(LPOBJ lpObj);
	// ----
	bool PlayersLevelCheck(LPOBJ lpObj, LPOBJ lpTargetObj);
	bool PlayersMoneyCheck(LPOBJ lpObj, LPOBJ lpTargetObj);
	// ----
private:
	// ---- Duel Configs
	bool DuelEnabled;
	short DuelMinLevel;
	short DuelMaxLevel;
	bool NeedMoneyToDuel;
	int MoneyToDuel;
	// ----
	DWORD m_UpdateTickCount;
	DWORD m_UpdateLifebarTime;
};

extern CDuelSystem gDuel;

#endif