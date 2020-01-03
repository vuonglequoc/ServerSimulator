//----------------------------------------------------------------------------------
// # Project:	Zodea eMU GameServer
// # Company:	RealCoderZ Software Incorpration. © 2012 http://www.realcoderz.com
// # Author:	WolF & M.E.S & Kristians
// # Hint:		This Script For Move System Class.
//----------------------------------------------------------------------------------
#ifndef MOVE_SYSTEM_H
#define MOVE_SYSTEM_H

#include "StdAfx.H"
#include "prodef.H"
#include "User.h"


struct PMSG_RECV_POSISTION_SET
{
	PBMSG_HEAD	h;
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		X;
	BYTE		Y;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_TARGET_TELEPORT
{
	PBMSG_HEAD	h;
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		MapX;
	BYTE		MapY;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_MOVE
{
	PBMSG_HEAD	h;
	BYTE		X;
	BYTE		Y;
	BYTE		Path[8];
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_REQ_MAPSVRMOVE	// GS<->JS	0x7A
{
	PBMSG_HEAD	h;
	int			iIndex;
	char		szAccountID[11];
	char		szCharName[11];
	WORD		wCurMapSvrCode;
	WORD		wDstMapSvrCode;
	WORD		wMapNumber;
	BYTE		btX;
	BYTE		btY;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_POSISTION_SET
{
	PBMSG_HEAD	h;
	BYTE		X;
	BYTE		Y;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_REQ_MOVE_OTHERSERVER
{
	PBMSG_HEAD	h;
	char		LastJoominNumber[10];
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct SDHP_CHARACTER_TRANSFER
{
	PBMSG_HEAD	h;
	char		Account[10];
	short		Number;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_TELEPORT
{
	PBMSG_HEAD	h;
	BYTE		Dir;
	WORD		MoveNumber;
	BYTE		MapX;
	BYTE		MapY;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_TELEPORT_RESULT
{
	PBMSG_HEAD	h;
	BYTE		GateH;
	BYTE		GateL;
	BYTE		MoveNumber;
	BYTE		MapNumber;
	BYTE		MapX;
	BYTE		MapY;
	BYTE		Dir;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_ACTIONRESULT
{
	PBMSG_HEAD	h;
	BYTE		NumberH;
	BYTE		NumberL;
	BYTE		Dir;
	BYTE		ActionNumber;
	BYTE		TargetNumberH;
	BYTE		TargetNumberL;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct SDHP_CHARACTER_TRANSFER_RESULT
{
	PBMSG_HEAD	h;
	char		Account[10];
	short		Number;
	BYTE		Result;

};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
struct MOVESTRUCT
{
	short	aIndex;
	short	MinLevel;
	short	MaxLevel;
	short	TargetGate;
	int		AllowPK;
	int		Money;
	int		IsBattleZone;
	int		MoveForChar;
	char	MapName[40];
	BYTE	FinalX;
	BYTE	FinalY;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
class CMoveSystem
{
public:
	CMoveSystem();
	virtual ~CMoveSystem();
	// -----
	void	InitMove();
	void	LoadMoveFile(LPSTR filename);
	void	SetInfo(int iIndex,char *iMapName,int iMoney,int iMinLevel,int iMaxLevel,int iAllowPK,int iMoveForChar,int iIsBattleZone,int iGate);
	void	CheckRequirements(int aIndex, int MapNumber, BYTE X, BYTE Y);
	void	RandWarp(int aIndex, int MapNumber);
	void	SpecWarp(int aIndex, int MapNumber, BYTE X, BYTE Y);
	// -----
	MOVESTRUCT MOVSTR[MOVMAX];
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
extern CMoveSystem gMove;
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void PMoveProc(PMSG_MOVE* lpMove, int aIndex);
bool gObjCheckXYMapTile(LPOBJ lpObj, int iDbgName);
void gObjSetPosition(int aIndex, int x, int y);
void gObjTeleport(int aIndex, BYTE map, BYTE x, BYTE y);
void RecvPositionSetProc(PMSG_POSISTION_SET* lpMove, int aIndex);
void GJReqMapSvrMove(int iIndex, WORD wDesMapSvrCode, WORD wMapNumber, BYTE btX, BYTE btY);
void GCTeleportSend(LPOBJ lpObj, BYTE MoveNumber, BYTE MapNumber, BYTE MapX, BYTE MapY, BYTE Dir);
void CGTeleportRecv(PMSG_TELEPORT* lpMsg, int aIndex);
void CGTargetTeleportRecv(PMSG_TARGET_TELEPORT* lpMsg, int aIndex);
void CGReqMoveOtherServer(PMSG_REQ_MOVE_OTHERSERVER* lpMsg, int aIndex);
void DGMoveOtherServer( SDHP_CHARACTER_TRANSFER_RESULT* lpMsg);
void MoveMonsterProc();
int GetMapMoveLevel(LPOBJ lpObj, int mapnumber, int max_over);
// -------------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* MOVE_SYSTEM_H */
// -------------------------------------------------------------------------------------------------------------------------------------------------------