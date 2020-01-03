// RSprotocol.h: interface for the RSprotocol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSPROTOCOL_H__23D179D6_DE22_40BA_B396_4AB2135D59D7__INCLUDED_)
#define AFX_RSPROTOCOL_H__23D179D6_DE22_40BA_B396_4AB2135D59D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



void RSProtocolCore(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize);
void InitRankingDB();

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu RankingServer DevilSquare Score
 *	Direction : GameServer -> RankingServer
 *  Code : 0xC1
 *	HeadCode : 0x01
 */

struct PMSG_ANS_EVENTUSERSCORE {
  struct PBMSG_HEAD h;
  char AccountID[10];	// 3
  char GameID[10];	// d
  int ServerCode;	// 18
  int Score;	// 1c
  int Class;	// 20
  int SquareNum; // 24
};
// <size 0x28>

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu RankingServer Get BC Enter Count
 *	Direction : GameServer -> RankingServer
 *  Code : 0xC1
 *	HeadCode : 0x0B
 */

struct PMSG_REQ_BLOODCASTLE_ENTERCOUNT {
  struct PBMSG_HEAD h;
  char AccountID[10];	// 3
  char GameID[10];	// d
  int ServerCode;	// 18
  int iObjIndex;	// 1c
};
// <size 0x20>

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu RankingServer BloodCastle_5TH Score
 *	Direction : GameServer -> RankingServer
 *  Code : 0xC1
 *	HeadCode : 0x0D
 */

struct PMSG_ANS_BLOODCASTLESCORE_5TH {
  struct PBMSG_HEAD h;
  char AccountID[10];	// 3
  char GameID[10];	// d
  int ServerCode;	// 18
  int Score;	// 1c
  int Class;	// 20
  int BridgeNum;	// 24
  int iLeftTime;	// 28
  int iAlivePartyCount;	// 2c
};
// <size 0x30>

void DevilSqureScore(PMSG_ANS_EVENTUSERSCORE * pMsg);
void EGReqBloodCastleEnterCount(int aIndex,PMSG_REQ_BLOODCASTLE_ENTERCOUNT * lpMsg);
void BloodCastleScore_5TH(PMSG_ANS_BLOODCASTLESCORE_5TH * lpMsg);
//void BloodCastleAccPoint(PMSG_ANS_BCACCPOINT * pMsg);
//void RankUpdate();

struct PMSG_ANS_BLOODCASTLE_ENTERCOUNT {
  struct PBMSG_HEAD h;
  char AccountID[10];
  char GameID[10];
  int ServerCode;
  int iObjIndex;
  int iLeftCount;
};
// <size 0x24>

#endif // !defined(AFX_RSPROTOCOL_H__23D179D6_DE22_40BA_B396_4AB2135D59D7__INCLUDED_)
