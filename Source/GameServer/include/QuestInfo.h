//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Quest Info Class
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------------------
#ifndef QUESTINFO_H
#define QUESTINFO_H
// -----------------------------------------------------------------------------------------------------------------------------------------
#include "../Test/CharacterInfo.h"
#include "user.h"

// -------------------------
#define gQuest_Module	"QUEST"
// -------------------------
#define MAX_QUEST_INFO 200
#define MAX_SUBINFO_QUEST 10
#define MAX_CONDITION_QUEST 10
#define ITEM_QUEST_DROP_PROBABILITY 1000
// -------------------------
//-- Quest Types
#define QUEST_COMPENSATION_STATUP 200
#define QUEST_COMPENSATION_CHANGEUP 201
#define QUEST_COMPENSATION_PLUSSSTAT 202
#define QUEST_COMPENSATION_COMBOSKILL 203
#define QUEST_COMPENSATION_MASTERUP	204

#define QUEST_MINLEVEL_PLUSSTAT 220
#define QUEST_STATE_SIZE 50
// -------------------------
typedef struct QUEST_SUB_INFO
{
	int QuestType;						// 00
	int NeedType;						// 04
	int NeedSubType;					// 08
	int ItemLevel;						// 0C
	int NeedNumber;						// 10
	int NeedTargetMinLevel;				// 14
	int NeedTargetMaxLevel;				// 18
	int NeedDropRate;					// 1C
	int RewardType;						// 20
	int RewardSubType;					// 24
	int RewardCount;					// 28
	int LinkConditionIndex;				// 2C
	int ContextBeforeReg;				// 30
	int ContextAfterReg;				// 34
	int ContextCompleteQuest;			// 38
	int ContextQuestClear;				// 3C
	BYTE RequireClass[MAX_CLASS_TYPE];	// 40

} QUEST_SUB_INFO, * LPQUEST_SUB_INFO;
// -------------------------
typedef struct QUEST_CONDITION
{
	int Index;							// 00
	int NeedQuestIndex;					// 04
	int MinLevel;						// 08
	int MaxLevel;						// 0C
	int ReqStr;							// 10
	int NeedZen;						// 14
	int StartContext;					// 18

} QUEST_CONDITION, * LPQUEST_CONDITION;
// -------------------------
typedef struct QUEST_INFO	
{
	char Name[50];											// 00
	int QuestIndex;											// 34
	int QuestStartType;										// 38
	int QuestStartSubType;									// 3C
	int QuestSubInfoCount;									// 40
	QUEST_SUB_INFO QuestSubInfo[MAX_SUBINFO_QUEST];			// 44
	int QuestConditionCount;								// 314
	QUEST_CONDITION QuestCondition[MAX_CONDITION_QUEST];	// 318

} QUEST_INFO, * LPQUEST_INFO;
// -------------------------
class CQuestInfo
{
public:

	CQuestInfo();
	virtual ~CQuestInfo();
	
	void Init();
	
	int IsQuest(int QuestIndex);
	int GetQeustCount(){return m_QuestCount;}
	int LoadQuestInfo(char* Buffer, int iSize);
	int LoadQuestInfo(char* filename);
	void InitQuestItem();
	LPQUEST_INFO GetQuestInfo(int QuestIndex);
	LPQUEST_SUB_INFO GetSubquestInfo(LPOBJ lpObj, LPQUEST_INFO lpQuestInfo, int subquestindex);
	LPQUEST_CONDITION GetQuestCondition(LPQUEST_INFO lpQuestInfo, int conditionindex);
	int GetQuestState(LPOBJ lpObj, int QuestIndex);
	BYTE GetQuestStateBYTE(LPOBJ lpObj, int QuestIndex);
	unsigned char SetQuestState(LPOBJ lpObj, int QuestIndex, int State);
	unsigned char ReSetQuestState(LPOBJ lpObj, int QuestIndex);
	unsigned char QuestClearConditionCheck(LPOBJ lpObj, int QuestIndex);
	unsigned char QuestRunConditionCheck(LPOBJ lpObj, int QuestIndex);
	int CompareCondition(LPOBJ lpObj, LPQUEST_CONDITION lpCondition);
	int NpcTalk(LPOBJ lpNpc, LPOBJ lpObj);
	bool MonsterItemDrop(LPOBJ lpObj);
	int QuestAccept(LPOBJ lpObj, int QuestIndex);
	int QuestClear(LPOBJ lpObj, int QuestIndex);
	void QuestInfoSave(LPOBJ lpObj);
	int GetQuestKillCount(LPOBJ lpObj, int uClass);

	//--- New Functions
	int QuestKillMonster(OBJECTSTRUCT* lpTargetObj, OBJECTSTRUCT* lpObj);
	int QuestKillMonsterInfo(int QuestIndex, int aIndex);
	void QuestWereWolfMove(int aIndex);
	void QuestGateKeeperMove(int aIndex);

	int QuestItemGetRequest(int aIndex, unsigned short QuestItem, unsigned short Level);

private:

	int m_QuestCount;						// 04
	QUEST_INFO QuestInfo[MAX_QUEST_INFO];	// 08
};
extern CQuestInfo g_QuestInfo;

#endif