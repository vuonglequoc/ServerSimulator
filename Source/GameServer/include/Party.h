//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Party Class
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef __PARTY_H__
#define __PARTY_H__
// ------------------------------------------------------------------------------------------------------------------------------------------------------

#include "StdAfx.H"
#include "prodef.H"
#include "User.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------
#define PartyModule					"Party"
#define MAX_USER_IN_PARTY			5
#define MAX_PARTY_DISTANCE_EFFECT	10
// ------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_PARTYREQUEST
{
	PBMSG_HEAD h;
	BYTE NumberH;
	BYTE NumberL;
};
// ------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_PARTYREQUESTRESULT
{
	PBMSG_HEAD h;
	BYTE Result;
	BYTE NumberH;
	BYTE NumberL;
};
// ------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_PARTYDELUSER
{
	PBMSG_HEAD h;
	BYTE Number;
};
// ------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_PARTYREQUESTSEND
{
	PBMSG_HEAD h;
	BYTE NumberH;
	BYTE NumberL;
};
// ------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_PARTYLIST
{
	char szId[10];
	BYTE Number;
	BYTE MapNumber;
	BYTE X;
	BYTE Y;
	int Life;
	int MaxLife;
};
// ------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_PARTYLISTCOUNT
{
	PBMSG_HEAD h;
	BYTE Result;
	BYTE Count;
};
// ------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_PARTYDELUSERSEND
{
	PBMSG_HEAD h;
};
// ------------------------------------------------------------------------------------------------------------------------------------------------------
struct PARTY_STRUCT
{
	int Count;
	int Number[MAX_USER_IN_PARTY];
	int DbNumber[MAX_USER_IN_PARTY];
	short m_MaxLevel;
	short m_MinLevel;
};
// ------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_DEFAULT_COUNT
{
	PBMSG_HEAD h;
	BYTE Count;
};
// ------------------------------------------------------------------------------------------------------------------------------------------------------
struct PMSG_PARTYLIFEALL
{
	BYTE Number;
};
// ------------------------------------------------------------------------------------------------------------------------------------------------------
class PartyClass
{
public:
	PartyClass();
	virtual ~PartyClass();
	// -----
	bool	IsParty(int Number);
	int		GetCount(int Number);
	bool	GetIndexUser(int Number, int aIndex, int& UserNum,  int& DBNum);
	int		GetIndex(int Number, int UserNum, int DBNum);
	int		Create(int usernumber, int dbnumber, int level);
	bool	Destroy(int Number);
	int		AddMember(int Number, int UserNum, int DBNum, int Level);
	int		DelMember(int Number, int UserNum, int DBNum);
	void	DelMember(int Number, int aIndex);
	bool	SetLevel(int Number, int Level);
	bool	GetLevel(int Number, int& MaxLevel, int& MinLevel);
	int		GetPartyCount(int Number);
	void	Paint(int Number);
	void	PartyMemberLifeSend(int Number);
	
	
	
	
	
	
	
	
	// -----
public:
	int m_PartyCount;
	PARTY_STRUCT m_PartyInfo[OBJMAX];
	char m_szTempPaint[256];
};
// ------------------------------------------------------------------------------------------------------------------------------------------------------
extern PartyClass gParty;
// ------------------------------------------------------------------------------------------------------------------------------------------------------

void CGPartyRequestRecv(PMSG_PARTYREQUEST* lpMsg, int aIndex);
void CGPartyRequestResultRecv(PMSG_PARTYREQUESTRESULT* lpMsg, int aIndex);
void CGPartyList(int aIndex);
void CGPartyListAll(int pnumber);
void CGPartyDelUser(PMSG_PARTYDELUSER* lpMsg, int aIndex);
void GCPartyDelUserSend(int aIndex);

// ------------------------------------------------------------------------------------------------------------------------------------------------------
#endif
// ------------------------------------------------------------------------------------------------------------------------------------------------------