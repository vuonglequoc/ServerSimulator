//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU New Quest System Functions							//
// Coded & Modified By:	M.E.S & WolF									//
//**********************************************************************//

#ifndef _QUEST_S_
#define _QUEST_S_

struct PMSG_GIVE_UP_QUEST
{
	unsigned char c;
	unsigned char size;
	unsigned char headcode;
	unsigned char subcode;
	unsigned char subtype1;
	unsigned char subtype2;
	unsigned char type1;
	unsigned char type2;
};

struct QUEST_STRUCT
{
	// ---- Common
	unsigned char Type[200];
	unsigned char SubType[200];
	char Repeat[200];
	char GenType[200];
	char Number[200];
	short NPCNumber[200];
	char Register[200];
	char StartDayWeek[200];
	char EndDayWeek[200];
	unsigned char AgreeType[200];
	unsigned char AgreeSubType[200];
	unsigned char RefuseType[200];
	unsigned char RefuseSubType[200];
	short MinLevel[200];
	short MaxLevel[200];
	char SendInfo[200];
	char TotalObjCount[200];
	char TotalRewCount[200];
	char RequiredClass[7][200];
	// ---- Objetives
	char ObjetiveType[5][200];
	short ObjMobLvlSk[5][200];
	short ObjCount[5][200];
	int ObjItemType[5][200];
	int ObjItemIndex[5][200];
	int ObjItemLevel[5][200];
	unsigned char ObjItemDur[5][200];
	char ObjItemSkill[5][200];
	char ObjItemLuck[5][200];
	char ObjItemAddOpt[5][200];
	char ObjItemExe[5][200];
	char ObjItemAnc[5][200];
	unsigned char ObjItemSocket[5][5][200];
	// ---- Rewards
	char RewardType[5][200];
	int RewZCE[5][200];
	short RewItemCount[5][200];
	int RewItemType[5][200];
	int RewItemIndex[5][200];
	int RewItemLevel[5][200];
	unsigned char RewItemDur[5][200];
	char RewItemSkill[5][200];
	char RewItemLuck[5][200];
	char RewItemAddOpt[5][200];
	char RewItemExe[5][200];
	char RewItemAnc[5][200];
	unsigned char RewItemSocket[5][5][200];
};

class SQuest
{
public:
	SQuest(void);
	~SQuest(void);
	// ----
	void ProtocolManager(unsigned char * aRecv,int aIndex);
	void Init();
	void ReadQuestsFile(char Type);
	char ReturnQuestPos(char QuestType);
	// ----
	void Gens(int aIndex);
	void GyroRogerTraders(int aIndex);
	void OracleLayla(int aIndex);
	void PorterMercenary(int aIndex);
	void ElfSoldierQuest(int aIndex);
	void CheckQuest(int aIndex);
	void SelectQuest(unsigned char * aRecv,int aIndex);
	void TClick(unsigned char * aRecv,int aIndex);
	void GiveUpQuest(unsigned char * aRecv,int aIndex);
	void QuestAnswer(unsigned char * aRecv,int aIndex);
	void QuestFinish(unsigned char * aRecv,int aIndex);
	void MonsterKillManager(int aIndex,int MonsterIndex);
	bool CanReward(int aIndex,int Type,int SubType);
	int GetQuestIndex(int aIndex,unsigned char Type,unsigned char SubType, char QType);
	int GetQuestIndex2(int aIndex,unsigned char Type,unsigned char SubType, char QType);
	void IsRegister(int aIndex,unsigned char Type,unsigned char SubType, char QType);
	bool UseQuestScroll(int aIndex, char Type);
	// ----
	char Enabled;
	int QuestCount[6];
};

extern SQuest QuestNew;

#endif