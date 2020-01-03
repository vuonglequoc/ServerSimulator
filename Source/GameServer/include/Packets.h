#include "StdAfx.h"
#include "User.h"


struct SDHP_CHECKMAIN_INC
{
	PBMSG_HEAD hEaD;
	char ID[10];
	char Password[10];
	short Number;
	char IpAddress[17];
};

struct SDHP_DBCHARINFOREQUEST
{
	PBMSG_HEAD h;	// C1:06
	char AccountID[11];	// 3
	char Name[11];	// E
	short Number;	// 1A

};
struct SDHP_CREATECHAR
{
	PBMSG_HEAD hEaD;
	int UserNumber;
	int DBNumber;
	short Number;
	char AccountID[10];
	char PlayerName[10];
	BYTE ClassSkin;
};

struct SDHP_CHARDELETE
{
	PBMSG_HEAD	h;				// C1:05
	short		Number;			// 4
	char		AccountID[10];	// 6
	char		Name[10];		// 10
	BYTE		Guild;			// [0:NoGuild] [1:Master] [2:Member] 1A
	char		GuildName[8];	// 1B
};

struct PMSG_LVPOINTADDRESULT
{
	PBMSG_HEAD hEaD;
	BYTE SubCode;
	BYTE ResultType;
	WORD MaxLifeAndMana;
	WORD wMaxShield;
	WORD MaxBP;
};

struct PMSG_CHARCHECK
{
	PBMSG_HEAD2 h;
	char szName[MAX_ACCOUNT_LEN];
};

struct PMSG_CHARCHECK_RESULT
{
	PBMSG_HEAD2 h;
	char szName[MAX_ACCOUNT_LEN];
	BYTE bIsFromOtherAccount;
};

struct PMSG_SERVERINFO
{
	PBMSG_HEAD h;	// C1:01
	short ServerCode;	// 4
	BYTE Percent;	// 6
	short UserCount;	// 8
	short AccountCount;	// A
	short PCbangCount;	// C
	short MaxUserCount;	// E
};

bool PacketCheckTime(LPOBJ lpObj);
bool DataEncryptCheck(int aIndex, BYTE protoNum, BOOL Encrypt);
void ToHex(unsigned char * out, unsigned char * data, int len);