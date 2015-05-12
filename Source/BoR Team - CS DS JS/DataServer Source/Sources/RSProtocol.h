//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef RS_PRTOCOOL
#define RS_PROTOCOL
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "TRSManager.h"
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct PMSG_ANS_EVENTUSERSCORE
{
  PBMSG_HEAD		h;
  char				AccountID[10];	// 3
  char				GameID[10];		// d
  int				ServerCode;		// 18
  int				Score;			// 1c
  int				Class;			// 20
  int				SquareNum;		// 24
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct PMSG_REQ_BLOODCASTLE_ENTERCOUNT
{
  PBMSG_HEAD		h;
  char				AccountID[10];		// 3
  char				GameID[10];			// d
  int				ServerCode;			// 18
  int				iObjIndex;			// 1c
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct PMSG_ANS_BLOODCASTLESCORE_5TH	
{
  PBMSG_HEAD		h;
  char				AccountID[10];		// 3
  char				GameID[10];			// d
  int				ServerCode;			// 18
  int				Score;				// 1c
  int				Class;				// 20
  int				BridgeNum;			// 24
  int				iLeftTime;			// 28
  int				iAlivePartyCount;	// 2c
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

struct PMSG_ANS_BLOODCASTLE_ENTERCOUNT 
{
  PBMSG_HEAD		h;
  char				AccountID[10];
  char				GameID[10];
  int				ServerCode;
  int				iObjIndex;
  int				iLeftCount;
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void InitRankingDB();
void RSProtocolCore(TRSManager * thisManager, int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize);
void DevilSqureScore(TRSManager * thisManager, PMSG_ANS_EVENTUSERSCORE * pMsg);
void EGReqBloodCastleEnterCount(TRSManager * thisManager, int aIndex,PMSG_REQ_BLOODCASTLE_ENTERCOUNT * lpMsg);
void RankUpdate(TRSManager * thisManager);
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* RS_PRTOCOOL */
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------