//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Gen System
// # Developed By:	M.E.S & WolF
//---------------------------------------------------------------------------

#ifndef GEN_SYSTEM
#define GEN_SYSTEM

#include "User.h"

struct PMSG_GEN_CHECK
{
	unsigned char c;
	unsigned char size;
	unsigned char headcode;
	unsigned char subcode;
	int Family;
	int Ranking;
	int Level;
	int Cont;
	int NextCont;
	/*unsigned char Family[4];
	unsigned char Ranking[4];
	unsigned char Level[4];
	unsigned char Cont[4];
	unsigned char NextCont[4];*/
};

struct PMSG_GENS_VIEWPORT
{
	unsigned char c;
	unsigned char sizeH;
	unsigned char sizeL;
	unsigned char headcode;
	unsigned char subcode;
	unsigned char Count;
	unsigned char Family;
	unsigned char IndexHi;
	unsigned char IndexLo;
	char null[5];
	unsigned char Level;
	char null1[6];
};

class GenSystem
{
public:
	GenSystem(void);
	~GenSystem(void);
	// ----
	void Init();
	void GenRanking();
	void GenSystemManager(LPOBJ lpObj, unsigned char * aRecv);
	void JoinGen(LPOBJ lpObj,int Type);
	void DeleteGen(LPOBJ lpObj);
	void RankingGen(LPOBJ lpObj,int Type);
	void GenCheck(LPOBJ lpObj);
	void SendLogo(LPOBJ lpObj,unsigned char * aSend);
	void GenLevelInc(LPOBJ lpObj);
	void GenLevelDec(LPOBJ lpObj);
	void GenAddAndDecContribution(LPOBJ lpObj,LPOBJ lpTargetObj);
	bool IsGenPVP(LPOBJ lpObj,LPOBJ lpTargetObj);
	bool IsGenZone(LPOBJ lpObj);
private:
	char Enabled;
	short MinLevel;
	int NeedContForLvlUp[15];
};

extern GenSystem Gens;

#endif