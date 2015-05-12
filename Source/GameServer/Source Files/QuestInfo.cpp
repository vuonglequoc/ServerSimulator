//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Quest Info Class
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------
#include "../Header Files/stdafx.h"
#include "../Header Files/QuestInfo.H"
#include "../Header Files/ReadScript.h"
#include "../Header Files/WzMemScript.H"
#include "../Header Files/LogProc.H"
#include "../Header Files/Main.H"
#include "../Header Files/GameMain.H"
#include "../Header Files/protocol.H"
#include "../Header Files/gObjMonster.H"
#include "../Header Files/DSProtocol.H"
#include "../Header Files/Party.h"
#include "../Header Files/Functions.H"
#include "../Header Files/SendProto.H"


unsigned char QuestBitMask[8];
CQuestInfo g_QuestInfo;
// ------------------------------
char sQuestString[4][50] = {"None", "Accept", "Clear", "None"};
// ------------------------------
CQuestInfo::CQuestInfo()
{
	QuestBitMask[0]=0xFC;
	QuestBitMask[1]=0xFC;
	QuestBitMask[2]=0xF3;
	QuestBitMask[3]=0xF3;
	QuestBitMask[4]=0xCF;
	QuestBitMask[5]=0xCF;
	QuestBitMask[6]=0x3F;
	QuestBitMask[7]=0x3F;

	this->Init();
}
// ------------------------------
CQuestInfo::~CQuestInfo()
{
	// -----
}
// ------------------------------
void CQuestInfo::Init()
{
	this->m_QuestCount = 0;
	memset(this->QuestInfo, -1, sizeof(this->QuestInfo));

	for( int i=0; i<MAX_QUEST_INFO; i++ )
	{
		this->QuestInfo[i].QuestConditionCount = 0;
		this->QuestInfo[i].QuestSubInfoCount = 0;
	}
}
// ------------------------------
BOOL CQuestInfo::LoadQuestInfo(char * filename)
{
	int Token;
	int n;

	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		CLog.MsgBox("load error %s", filename);
		return false;
	}

	this->Init();

	n = 0;

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}
		else
		{
			if ( Token == 1 )
			{
				int Type = (int)TokenNumber;

				if ( Type == 0 )
				{
					Token = GetToken();
					int iQuestIndex = (int)TokenNumber;
					this->QuestInfo[iQuestIndex].QuestIndex = (int)iQuestIndex;

					Token = GetToken();
					this->QuestInfo[iQuestIndex].QuestStartType = (int)TokenNumber;

					Token = GetToken();
					this->QuestInfo[iQuestIndex].QuestStartSubType = (int)TokenNumber;

					Token = GetToken();
					strcpy( this->QuestInfo[iQuestIndex].Name, TokenString);

					this->QuestInfo[iQuestIndex].QuestSubInfoCount = 0;

					while ( true )
					{
						int iSubInfoCount = this->QuestInfo[iQuestIndex].QuestSubInfoCount;

						Token = GetToken();

						if ( Token == 0 )
						{
							if ( strcmp("end", TokenString) == 0 )
							{
								break;
							}
						}
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].QuestType = (int)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedType = (int)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedSubType = (int)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ItemLevel = (int)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedNumber = (int)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedTargetMinLevel = (int)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedTargetMaxLevel = (int)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedDropRate = (int)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RewardType = (int)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RewardSubType = (int)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RewardCount = (int)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].LinkConditionIndex = (int)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextBeforeReg = (int)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextAfterReg = (int)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextCompleteQuest = (int)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextQuestClear = (int)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[0] = (BYTE)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[1] = (BYTE)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[2] = (BYTE)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[3] = (BYTE)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[4] = (BYTE)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[5] = (BYTE)TokenNumber;

						Token = GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[6] = (BYTE)TokenNumber;

						this->QuestInfo[iQuestIndex].QuestSubInfoCount++;

					
					}
					
					CLog.LogAdd(gQuest_Module,"[Quest] (%s) Load : Index %d, SubInfoCount %d", this->QuestInfo[iQuestIndex].Name, iQuestIndex, this->QuestInfo[iQuestIndex].QuestSubInfoCount );
					this->m_QuestCount++;
				}
				else if ( Type == 1 )
				{
					Token = GetToken();
					int iQuestIndex = (int)TokenNumber;

					while ( true )
					{
						int iFailInfoCount = this->QuestInfo[iQuestIndex].QuestConditionCount;
						Token = GetToken();

						if ( Token == 0 )
						{
							if ( strcmp("end", TokenString) == 0 )
							{
								break;
							}
						}
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].Index = (int)TokenNumber;

							Token = GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].NeedQuestIndex = (int)TokenNumber;

							Token = GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].MinLevel = (int)TokenNumber;

							Token = GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].MaxLevel = (int)TokenNumber;

							Token = GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].ReqStr = (int)TokenNumber;

							Token = GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].NeedZen = (int)TokenNumber;

							Token = GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].StartContext = (int)TokenNumber;

							this->QuestInfo[iQuestIndex].QuestConditionCount++;
					}
				}
				else
				{
					CLog.MsgBox("Quest Info Read Fail");
					fclose(SMDFile);
					return false;
				}
			}
		}
	}

	fclose(SMDFile);
	this->InitQuestItem();
	return true;
}
// ------------------------------
BOOL CQuestInfo::LoadQuestInfo(char* Buffer, int iSize)
{
	CWzMemScript WzMemScript;
	int Token;
	int n;

	WzMemScript.SetBuffer(Buffer, iSize);
	this->Init();

	n = 0;

	while ( true )
	{
		Token = WzMemScript.GetToken();

		if ( Token == 2 )
		{
			break;
		}
		else
		{
			if ( Token == 1 )
			{
				int Type = WzMemScript.GetNumber();

				if ( Type == 0 )
				{
					Token = WzMemScript.GetToken();
					int iQuestIndex = WzMemScript.GetNumber();
					this->QuestInfo[iQuestIndex].QuestIndex = iQuestIndex;

					Token = WzMemScript.GetToken();
					this->QuestInfo[iQuestIndex].QuestStartType = WzMemScript.GetNumber();

					Token = WzMemScript.GetToken();
					this->QuestInfo[iQuestIndex].QuestStartSubType = WzMemScript.GetNumber();

					Token = WzMemScript.GetToken();
					strcpy( this->QuestInfo[iQuestIndex].Name, WzMemScript.GetString());

					this->QuestInfo[iQuestIndex].QuestSubInfoCount = 0;

					while ( true )
					{
						int iSubInfoCount = this->QuestInfo[iQuestIndex].QuestSubInfoCount;

						Token = WzMemScript.GetToken();

						if ( Token == 0 )
						{
							if ( strcmp("end", WzMemScript.GetString()) == 0 )
							{
								break;
							}
						}
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].QuestType = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedType = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedSubType = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ItemLevel = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedNumber = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedTargetMinLevel = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedTargetMaxLevel = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].NeedDropRate = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RewardType = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RewardSubType = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RewardCount = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].LinkConditionIndex = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextBeforeReg = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextAfterReg = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextCompleteQuest = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].ContextQuestClear = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[0] = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[1] = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[2] = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[3] = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[4] = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[5] = WzMemScript.GetNumber();

						Token = WzMemScript.GetToken();
						this->QuestInfo[iQuestIndex].QuestSubInfo[iSubInfoCount].RequireClass[6] = WzMemScript.GetNumber();

						this->QuestInfo[iQuestIndex].QuestSubInfoCount++;

					
					}
					
					CLog.LogAdd(gQuest_Module, "[Quest] (%s) Load : Index %d, SubInfoCount %d", this->QuestInfo[iQuestIndex].Name,
						iQuestIndex, this->QuestInfo[iQuestIndex].QuestSubInfoCount );
					this->m_QuestCount++;
				}
				else if ( Type == 1 )
				{
					Token = WzMemScript.GetToken();
					int iQuestIndex = WzMemScript.GetNumber();

					while ( true )
					{
						int iFailInfoCount = this->QuestInfo[iQuestIndex].QuestConditionCount;
						Token = WzMemScript.GetToken();

						if ( Token == 0 )
						{
							if ( strcmp("end", WzMemScript.GetString()) == 0 )
							{
								break;
							}
						}	
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].Index = WzMemScript.GetNumber();

							Token = WzMemScript.GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].NeedQuestIndex = WzMemScript.GetNumber();

							Token = WzMemScript.GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].MinLevel = WzMemScript.GetNumber();

							Token = WzMemScript.GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].MaxLevel = WzMemScript.GetNumber();

							Token = WzMemScript.GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].ReqStr = WzMemScript.GetNumber();

							Token = WzMemScript.GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].NeedZen = WzMemScript.GetNumber();

							Token = WzMemScript.GetToken();
							this->QuestInfo[iQuestIndex].QuestCondition[iFailInfoCount].StartContext = WzMemScript.GetNumber();

							this->QuestInfo[iQuestIndex].QuestConditionCount++;
						
					}
				}
				else
				{
					CLog.MsgBox("Quest Info Read Fail");
					fclose(SMDFile);
					return false;
				}
			}
		}
	}

	this->InitQuestItem();
	return true;
}
// ------------------------------
void CQuestInfo::InitQuestItem()
{
	int questcount = this->GetQeustCount();
	int foundquest = 0;
	LPQUEST_INFO lpQuestInfo;
	LPQUEST_SUB_INFO lpSubInfo;

	for (int i=0; i<MAX_QUEST_INFO; i++)
	{
		lpQuestInfo = this->GetQuestInfo(i);

		if ( lpQuestInfo == NULL )
		{
			continue;
		}

		for ( int n=0; n<lpQuestInfo->QuestSubInfoCount; n++)
		{
			lpSubInfo = &lpQuestInfo->QuestSubInfo[n];

			if ( lpSubInfo != NULL )
			{
				if ( lpSubInfo->QuestType == 1 )
				{
					LPITEM_ATTRIBUTE p = &ItemAttribute[ITEMGET(lpSubInfo->NeedType, lpSubInfo->NeedSubType) ];
					p->QuestItem = TRUE;
					CLog.LogAdd(gQuest_Module, "[Quest] [SetQuestItem] %s", p->Name);
				}
				else if ( lpSubInfo->QuestType == 2 )
				{
					CLog.LogAdd(gQuest_Module, "[Quest] [SetQuestMonster]");
				}
			}
		}

		foundquest++;

		if ( foundquest == questcount )
		{
			return;
		}
	}
}
// ------------------------------
BOOL CQuestInfo::IsQuest(int QuestIndex)
{
	if ( QuestIndex < 0 || QuestIndex > MAX_QUEST_INFO )
	{
		return false;
	}

	if ( this->QuestInfo[QuestIndex].QuestIndex == -1 )
	{
		return false;
	}

	return true;
}
// ------------------------------
int CQuestInfo::GetQuestState(LPOBJ lpObj, int QuestIndex)
{
	if ( QuestIndex < 0 || QuestIndex > MAX_QUEST_INFO )
	{
		return FALSE;
	}

	int Index = (QuestIndex) /4;
	int Shift = (QuestIndex & 3) *2;
	return (lpObj->m_Quest[Index] >> Shift) &3;
}
// ------------------------------
BYTE CQuestInfo::GetQuestStateBYTE(LPOBJ lpObj, int QuestIndex)
{
	if ( QuestIndex < 0 || QuestIndex > MAX_QUEST_INFO )
	{
		return FALSE;
	}
	
	int index = (QuestIndex / 4);
	return lpObj->m_Quest[index];
}
// ------------------------------
struct PMSG_QUEST_DIALOG
{
	PBMSG_HEAD	head;
	unsigned char	Quest_ID;
	unsigned char	Dialog_ID;
};
// ------------------------------
BYTE CQuestInfo::SetQuestState(LPOBJ lpObj, int QuestIndex, int State)
{
	if ( QuestIndex < 0 || QuestIndex > MAX_QUEST_INFO )
	{
		CLog.LogAdd("ERROR 2 QUEST");
		return -1;
	}

	int QuestState = this->GetQuestState(lpObj, QuestIndex);

	if ( QuestState == 3 || QuestState == 0)
	{
		BYTE btCond = this->QuestRunConditionCheck(lpObj, QuestIndex);

		if ( btCond != 0 )
		{
			CLog.LogAdd("return 1 QUEST");
			return btCond;
		}

		this->QuestAccept(lpObj, QuestIndex);
		State = 1; /* State Stage */
		CLog.LogAdd("ACCEPT QUEST");
		CLog.LogAdd(gQuest_Module, "[Quest] AcceptQuest [%s][%s] (%s)", lpObj->AccountID, lpObj->Name, this->GetQuestInfo(QuestIndex)->Name);
	}
	else if ( QuestState == 1 )
	{
		BYTE btCond = this->QuestClearConditionCheck(lpObj, QuestIndex);

		if ( btCond != 0 )
		{
			CLog.LogAdd("return 2 QUEST");
			return btCond;
		}

		this->QuestClear(lpObj, QuestIndex);
		State = 2;/* State Stage */
		CLog.LogAdd("CLEAR QUEST");
		CLog.LogAdd(gQuest_Module, "[Quest] ClearQuest [%s][%s] (%s)", lpObj->AccountID, lpObj->Name, this->GetQuestInfo(QuestIndex)->Name);
	}
	else
	{
		CLog.LogAdd("ERROR QUEST");
		return -1;
	}

	int Index = (QuestIndex / 4);
	int Shift = (QuestIndex & 3) *2;
	lpObj->m_Quest[Index] &= QuestBitMask[Shift];
	lpObj->m_Quest[Index] |= (State&3) << Shift;
	return FALSE;
}
// ------------------------------
BYTE CQuestInfo::ReSetQuestState(LPOBJ lpObj, int QuestIndex)
{
	int State = 3;
	int Index = (QuestIndex/4);
	int Shift = (QuestIndex & 3) *2;
	lpObj->m_Quest[Index] &= QuestBitMask[Shift];
	lpObj->m_Quest[Index] |= (State&3) << Shift;

	return FALSE;
}
// ------------------------------
LPQUEST_INFO CQuestInfo::GetQuestInfo(int QuestIndex)
{
	if ( QuestIndex < 0 || QuestIndex > MAX_QUEST_INFO )
	{
		return FALSE;
	}
	
	if ( this->QuestInfo[QuestIndex].QuestIndex == -1 )
	{
		return NULL;
	}

	return &this->QuestInfo[QuestIndex];
}
// ------------------------------
LPQUEST_SUB_INFO CQuestInfo::GetSubquestInfo(LPOBJ lpObj, LPQUEST_INFO lpQuestInfo, int subquestindex)
{
	if ( lpQuestInfo == NULL )
	{
		return NULL;
	}

	if ( subquestindex < 0 || subquestindex > MAX_SUBINFO_QUEST )
	{
		return NULL;
	}

	LPQUEST_SUB_INFO lpSubInfo = &lpQuestInfo->QuestSubInfo[subquestindex];
	int Class = lpObj->Class;
	int ChangeUP = lpObj->ChangeUP;
	int ChangeUP2 = lpObj->ChangeUP2;
	int requireclass = lpSubInfo->RequireClass[Class];

	if (requireclass >= 3)
	{
		if(requireclass != (ChangeUP2+2))
		{
			return 0;
		}
	}
	else
	{
		if (requireclass > 1)
		{
			if ( requireclass != (ChangeUP+1) )
			{
				return 0;
			}
		}
	}

	return lpSubInfo;
}
// ------------------------------
LPQUEST_CONDITION CQuestInfo::GetQuestCondition(LPQUEST_INFO lpQuestInfo, int conditionindex)
{
	if ( lpQuestInfo == NULL )
	{
		return NULL;
	}

	if ( conditionindex < 0 || conditionindex > MAX_CONDITION_QUEST )
	{
		return NULL;
	}

	return &lpQuestInfo->QuestCondition[conditionindex];
}
// ------------------------------
BYTE CQuestInfo::QuestClearConditionCheck(LPOBJ lpObj, int QuestIndex)
{
	LPQUEST_INFO lpQuestInfo = this->GetQuestInfo(QuestIndex);

	if ( lpQuestInfo == NULL )
	{
		return -1;
	}

	int subquestcount = lpQuestInfo->QuestSubInfoCount;
	int concount = lpQuestInfo->QuestConditionCount;
	LPQUEST_SUB_INFO lpSubInfo;
	BOOL bFoundSubQuest = FALSE;
	

	for ( int subquest=0;subquest<subquestcount;subquest++)
	{
		lpSubInfo = this->GetSubquestInfo(lpObj, lpQuestInfo, subquest);

		if ( lpSubInfo != NULL )
		{
			bFoundSubQuest = TRUE;

			if ( lpSubInfo->QuestType == 1 )
			{
				int NumberItemQuestFound = gObjGetItemCountInIventory(lpObj->m_Index, lpSubInfo->NeedType, lpSubInfo->NeedSubType, lpSubInfo->ItemLevel);

				if ( NumberItemQuestFound < lpSubInfo->NeedNumber)
				{
					return 1;
				}
			}
			else if ( lpSubInfo->QuestType == 2)
			{
				int NumberMonsterKillCount = this->GetQuestKillCount(lpObj, lpSubInfo->NeedType);

				if ( NumberMonsterKillCount < lpSubInfo->NeedNumber)
				{
					return 1;
				}
			}
		}
	}

	if ( bFoundSubQuest == FALSE )
	{
		return -1;
	}

	return FALSE;
}
// ------------------------------
BYTE CQuestInfo::QuestRunConditionCheck(LPOBJ lpObj, int QuestIndex)
{
	LPQUEST_INFO lpQuestInfo = this->GetQuestInfo(QuestIndex);

	if ( lpQuestInfo == NULL )
	{
		return -1;
	}

	int subquestcount = lpQuestInfo->QuestSubInfoCount;
	int concount = lpQuestInfo->QuestConditionCount;
	LPQUEST_SUB_INFO lpSubInfo;
	LPQUEST_CONDITION lpCondition;
	BOOL bFoundSubQuest = FALSE;


	for ( int subquest=0;subquest<subquestcount;subquest++)
	{
		lpSubInfo = this->GetSubquestInfo(lpObj, lpQuestInfo, subquest);

		if ( lpSubInfo != NULL )
		{
			bFoundSubQuest = TRUE;

			for ( int i=0;i<concount;i++)
			{
				lpCondition = this->GetQuestCondition(lpQuestInfo, i);

				if ( lpCondition != NULL && lpCondition->Index == -1)
				{
					if ( this->CompareCondition(lpObj, lpCondition) == FALSE )
					{
						return lpCondition->StartContext;
					}
				}
				else if ( lpCondition != NULL && lpCondition->Index == lpSubInfo->LinkConditionIndex )
				{
					if ( this->CompareCondition(lpObj, lpCondition) == 0)
					{
						return lpCondition->StartContext;
					}
				}
				else if ( lpCondition == NULL )
				{
					return -1;
				}
			}

		}
	}

	if ( bFoundSubQuest == FALSE )
	{
		return -1;
	}

	return 0;
}
// ------------------------------
BOOL CQuestInfo::CompareCondition(LPOBJ lpObj, LPQUEST_CONDITION lpCondition)
{
	if ( lpCondition->NeedQuestIndex != -1 )
	{
		if ( this->GetQuestState(lpObj, lpCondition->NeedQuestIndex) != 2 )
		{
			return FALSE;
		}
	}

	if ( lpCondition->MinLevel != 0 )
	{
		if ( lpCondition->MinLevel > lpObj->Level )
		{
			return FALSE;
		}
	}

	if ( lpCondition->MaxLevel != 0 )
	{
		if ( lpCondition->MaxLevel < lpObj->Level )
		{
			return FALSE;
		}
	}

	if ( lpCondition->NeedZen > lpObj->Money )
	{
		return FALSE;
	}

	if ( lpCondition->ReqStr > lpObj->Strength + lpObj->AddStrength)
	{
		return FALSE;
	}

	return TRUE;

}
// ------------------------------
BOOL CQuestInfo::NpcTalk(LPOBJ lpNpc, LPOBJ lpObj)
{
	int questcount = this->GetQeustCount();
	int foundquest = 0;
	LPQUEST_INFO lpQuestInfo;
	int queststate = -1;
	int questindex = -1;

	for ( int i=0; i<MAX_QUEST_INFO; i++)
	{
		lpQuestInfo = this->GetQuestInfo(i);

		if ( lpQuestInfo == NULL )
		{
			continue;
		}

		if ( lpQuestInfo->QuestStartType == 2 ) // NPC Click
		{
			if ( lpNpc->Class == lpQuestInfo->QuestStartSubType)
			{
				if ( queststate == -1 )
				{
					questindex = lpQuestInfo->QuestIndex;
					queststate = this->GetQuestState(lpObj, lpQuestInfo->QuestIndex);
				}
				else if ( queststate == 2 )
				{
					for ( int n=0;n<lpQuestInfo->QuestSubInfoCount ;n++)
					{
						LPQUEST_SUB_INFO lpSubInfo = this->GetSubquestInfo(lpObj, lpQuestInfo, n);

						if ( lpSubInfo != NULL )
						{
							questindex = lpQuestInfo->QuestIndex;
							queststate = this->GetQuestState(lpObj, lpQuestInfo->QuestIndex);
						}
					}
				}
			}
		}

		foundquest++;

		if ( foundquest == questcount )
		{
			break;
		}
	}

	if ( queststate != -1 )
	{

		GCSendQuestInfo(lpObj->m_Index, questindex);
		lpObj->m_IfState.type = 10;
		lpObj->m_IfState.state = 0;
		lpObj->m_IfState.use = 1;
		return true;
	}

	return false;
}
// ------------------------------
bool CQuestInfo::MonsterItemDrop(LPOBJ lpObj)
{
	int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);

	if ( MaxHitUser == -1 )
	{
		return false;
	}

	int partycount = gParty.GetPartyCount(gObj[MaxHitUser].PartyNumber);

	if ( partycount > 0 )
	{
		return false;
	}

	LPOBJ lpTarget = &gObj[MaxHitUser];
	int questcount = this->GetQeustCount();
	int foundquest = 0;
	LPQUEST_INFO lpQuestInfo;
	LPQUEST_SUB_INFO lpSubInfo;
	int type;
	int level;
	int x;
	int y;
	float dur = 0;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;

	for ( int i=0; i<MAX_QUEST_INFO; i++)
	{
		lpQuestInfo = this->GetQuestInfo(i);

		if ( lpQuestInfo == NULL )
		{
			continue;
		}

		for ( int n=0; n<lpQuestInfo->QuestSubInfoCount; n++)
		{
			lpSubInfo = this->GetSubquestInfo(lpTarget, lpQuestInfo, n);
			
			if ( lpSubInfo != NULL )
			{
				if ( lpSubInfo->QuestType == 1 && lpSubInfo->NeedTargetMinLevel != -1 )
				{

					if ( partycount > 0 )
					{
						return false;
					}

					if ( lpObj->Level >= lpSubInfo->NeedTargetMinLevel)
					{
						if ( lpObj->Level <= lpSubInfo->NeedTargetMaxLevel )
						{
							if ( this->GetQuestState(lpTarget, lpQuestInfo->QuestIndex) == TRUE )
							{
								if ( (rand() % ITEM_QUEST_DROP_PROBABILITY) < lpSubInfo->NeedDropRate)
								{
									int itemcount = gObjGetItemCountInIventory(MaxHitUser, lpSubInfo->NeedType,
										lpSubInfo->NeedSubType, lpSubInfo->ItemLevel);

									if ( itemcount >= lpSubInfo->NeedNumber)
									{
										continue;
									}
									BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;
									dur = 0;
									x = lpObj->X;
									y = lpObj->Y;
									level = lpSubInfo->ItemLevel;
									type = ItemGetNumberMake(lpSubInfo->NeedType, lpSubInfo->NeedSubType);
									ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, (BYTE)dur, Option1,
										Option2, Option3, MaxHitUser, 0, 0, Socket);
									CLog.LogAdd(gQuest_Module, "[Quest] Quest Item Drop [%s]: [%s][%s] (%s) (%d,%d)", lpObj->Name,
										lpTarget->AccountID, lpTarget->Name, lpQuestInfo->Name, lpSubInfo->NeedType,
										lpSubInfo->NeedSubType);
									return true;
								}
							}
						}
					}
					if ( lpSubInfo->QuestType == 1 && lpSubInfo->NeedTargetMinLevel == -1 ) //Third Quest
				{

					if ( partycount > 0 )
					{
						return false;
					}

					if ( lpObj->Class == lpSubInfo->NeedTargetMaxLevel) //READS Monster Class on TargetMax Level Category (for Original Quest.txt)
					{
						if ( this->GetQuestState(lpTarget, lpQuestInfo->QuestIndex) == TRUE )
							{
								if ( (rand() % ITEM_QUEST_DROP_PROBABILITY) < lpSubInfo->NeedDropRate)
								{
									int itemcount = gObjGetItemCountInIventory(MaxHitUser, lpSubInfo->NeedType,
										lpSubInfo->NeedSubType, lpSubInfo->ItemLevel);

									if ( itemcount >= lpSubInfo->NeedNumber)
									{
										continue;
									}
									BYTE Socket[5];
	Socket[0] = 0xFF;
	Socket[1] = 0xFF;
	Socket[2] = 0xFF;
	Socket[3] = 0xFF;
	Socket[4] = 0xFF;
									dur = 0;
									x = lpObj->X;
									y = lpObj->Y;
									level = lpSubInfo->ItemLevel;
									type = ItemGetNumberMake(lpSubInfo->NeedType, lpSubInfo->NeedSubType);
									ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, (BYTE)dur, Option1,
										Option2, Option3, MaxHitUser, 0, 0, Socket);
									CLog.LogAdd(gQuest_Module, "[Season3 Quest] Quest Item Drop [%s]: [%s][%s] (%s) (%d,%d)", lpObj->Name,
										lpTarget->AccountID, lpTarget->Name, lpQuestInfo->Name, lpSubInfo->NeedType,
										lpSubInfo->NeedSubType);
									return true;
								}
							}
					}
				}
				if ( lpSubInfo->QuestType == 2 && lpSubInfo->NeedTargetMinLevel == -1 ) //Third Quest for Kill Count
				{
					if ( lpObj->Class == lpSubInfo->NeedTargetMaxLevel) //READS Monster Class on TargetMax Level Category (for Original Quest.txt)
					{
						if ( this->GetQuestState(lpTarget, lpQuestInfo->QuestIndex) == TRUE )
						{
							if( lpTarget->m_Quest[1] == 0xF6 && lpTarget->MapNumber == 41 )
							{
								if(lpObj->Class == 409 && lpTarget->m_Quest[30] < 10)
								{
									lpTarget->m_Quest[30] += 1;
									MsgOutput(lpTarget->m_Index, "[Quest] (%d/10) Balram(Hero)", lpTarget->m_Quest[30]);
								}

								if(lpObj->Class == 410 && lpTarget->m_Quest[31] < 10)
								{
									lpTarget->m_Quest[31] += 1;
									MsgOutput(lpTarget->m_Index, "[Quest] (%d/10) Death Spirit(Hero)", lpTarget->m_Quest[31]);
								}

								if(lpObj->Class == 411 && lpTarget->m_Quest[32] < 10)
								{
									lpTarget->m_Quest[32] += 1;
									lpTarget->m_Quest[34] += 1;
									MsgOutput(lpTarget->m_Index, "[Quest] (%d/10) Soram(Hero).", lpTarget->m_Quest[32]);
								}
							}
							if( lpTarget->m_Quest[1] == 0xDA && lpTarget->MapNumber == 42 )
							{
								if(lpObj->Class == 412 && lpTarget->m_Quest[34] < 1)
								{
									lpTarget->m_Quest[34] += 1;
									MsgOutput(lpTarget->m_Index, "[Quest] (%d/1) Dark Elf(Hero)", lpTarget->m_Quest[34]);
								}
							}
						}
					}
				}
			}
		}
		}

		foundquest++;

		if ( foundquest == questcount )
		{
			break;
		}
	}

	return false;
}
// ------------------------------
BOOL CQuestInfo::QuestAccept(LPOBJ lpObj, int QuestIndex)
{
	LPQUEST_INFO lpQuestInfo = this->GetQuestInfo(QuestIndex);

	if ( lpQuestInfo == NULL )
	{
		return 0xFF;
	}

	int subquestcount = lpQuestInfo->QuestSubInfoCount;
	int concount = lpQuestInfo->QuestConditionCount;
	LPQUEST_SUB_INFO lpSubInfo;
	LPQUEST_CONDITION lpCondition;
	int NeedZen = 0;

	if ( QuestIndex > 3 && QuestIndex < 6 ) //-- Third Quest Fix
	{
		//-- Begin
		if( lpQuestInfo->QuestSubInfoCount > 0 )
		{
			lpSubInfo = lpQuestInfo->QuestSubInfo;

			if ( lpSubInfo != NULL )
			{
				for ( int n = 0;n<concount;n++)
				{
					lpCondition = this->GetQuestCondition(lpQuestInfo, n);
					NeedZen += lpCondition->NeedZen;
				}
			}
		}

		if ( QuestIndex > 4 && QuestIndex < 7 ) //-- Third Quest Fix
		{

			if( lpObj->m_Quest[30] > 10)
			{
				lpObj->m_Quest[30] = 0;
			}
			if( lpObj->m_Quest[31] > 10 )
			{
				lpObj->m_Quest[31] = 0;
			}
			if( lpObj->m_Quest[32] > 10 )
			{
				lpObj->m_Quest[32] = 0;
			}
			if( lpObj->m_Quest[34] > 1 )
			{
				lpObj->m_Quest[34] = 0;
			}
			
			CLog.LogAdd(gQuest_Module, "[Quest] Set MonsterKillInfo [%s][%s] (%d/10, %d/10, %d/10, %d/1)", lpObj->AccountID, lpObj->Name, lpObj->m_Quest[30], lpObj->m_Quest[31], lpObj->m_Quest[32], lpObj->m_Quest[34]);

		}
		//-- END
	}
	else
	{
		for ( int subquest=0;subquest<subquestcount;subquest++)
		{
			lpSubInfo = this->GetSubquestInfo(lpObj, lpQuestInfo, subquest);

			if ( lpSubInfo != NULL )
			{
				for ( int n = 0;n<concount;n++)
				{
					lpCondition = this->GetQuestCondition(lpQuestInfo, n);
					NeedZen += lpCondition->NeedZen;
				}
			}
		}
	}

	lpObj->Money -= NeedZen;
	GCMoneySend(lpObj->m_Index, lpObj->Money);
	return true;
}
// ------------------------------
BOOL CQuestInfo::QuestClear(LPOBJ lpObj, int QuestIndex)
{
	LPQUEST_INFO lpQuestInfo = this->GetQuestInfo(QuestIndex);

	if ( lpQuestInfo == NULL )
	{
		return 0xFF;
	}

	int subquestcount = lpQuestInfo->QuestSubInfoCount;
	int concount = lpQuestInfo->QuestConditionCount;
	LPQUEST_SUB_INFO lpSubInfo;

	for ( int subquest=0;subquest<subquestcount;subquest++)
	{
		lpSubInfo = this->GetSubquestInfo(lpObj, lpQuestInfo, subquest);

		if ( lpSubInfo != NULL )
		{
			if ( lpSubInfo->RewardType == QUEST_COMPENSATION_CHANGEUP )
			{
				lpObj->LevelUpPoint += lpSubInfo->RewardCount;
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_STATUP, lpSubInfo->RewardCount );
				lpObj->ChangeUP = true;
				lpObj->DbClass |= 1;
				gObjMakePreviewCharSet(lpObj->m_Index);
				BYTE btClass = (lpObj->Class * 32) & 224 ;
				btClass |= (lpObj->ChangeUP * 16) & 16;
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_CHANGEUP, btClass);
				CLog.LogAdd(gQuest_Module, "[Quest] Quest Clear (%s) : [%s][%s] Stat(%d,%d), ChangeUp", lpQuestInfo->Name, lpObj->AccountID, lpObj->Name,
					lpObj->LevelUpPoint, lpSubInfo->RewardCount );
			}
			else if ( lpSubInfo->RewardType == QUEST_COMPENSATION_STATUP )
			{
				lpObj->LevelUpPoint += lpSubInfo->RewardCount;
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_STATUP, lpSubInfo->RewardCount );
				CLog.LogAdd(gQuest_Module, "[Quest] Quest Clear (%s) : [%s][%s] Stat(%d,%d)", lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, 
					lpObj->LevelUpPoint,lpSubInfo->RewardCount );
			}
			else if ( lpSubInfo->RewardType == QUEST_COMPENSATION_PLUSSSTAT )
			{
				int level = lpObj->Level - QUEST_MINLEVEL_PLUSSTAT;

				if ( level <= 0 )
				{
					level = 0;
				}

				lpObj->LevelUpPoint += level;
				lpObj->PlusStatQuestClear = true;
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_PLUSSSTAT, level );
				CLog.LogAdd(gQuest_Module, "[Quest] Quest Clear (%s) : [%s][%s] Stat(%d,%d), PlusStat",
					lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, lpObj->LevelUpPoint,
					level );
			}
			else if ( lpSubInfo->RewardType == QUEST_COMPENSATION_COMBOSKILL )
			{
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_COMBOSKILL, 0 );
				lpObj->ComboSkillquestClear = true;
				CLog.LogAdd(gQuest_Module, "[Quest] Quest Clear (%s) : [%s][%s] ComboSkill",
					lpQuestInfo->Name, lpObj->AccountID, lpObj->Name);
			}
			else if ( lpSubInfo->RewardType == QUEST_COMPENSATION_MASTERUP )
			{
				lpObj->LevelUpPoint += lpSubInfo->RewardCount;
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_STATUP, lpSubInfo->RewardCount);
				lpObj->ChangeUP = 2;
				BYTE btClass;
				btClass = (lpObj->Class * 32) + 24;

				if (lpObj->DbClass != 3 || lpObj->DbClass != 19 || lpObj->DbClass != 35 || lpObj->DbClass != 50 || lpObj->DbClass != 66 || lpObj->DbClass != 82 )
				{
					if (lpObj->DbClass == 48 || lpObj->DbClass == 64 || lpObj->DbClass == 96 )
					{
						lpObj->DbClass += 2;
					}
					else
					{
						lpObj->DbClass += 1;
					}
				}

				gObjMakePreviewCharSet(lpObj->m_Index);
				GCSendQuestPrize(lpObj->m_Index, QUEST_COMPENSATION_MASTERUP, btClass);
				CLog.LogAdd(gQuest_Module, "[Quest] Quest Clear (%s) : [%s][%s] Class:%d(%d), 3rd ChangeUp",
					lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, lpObj->DbClass, lpObj->DbClass-1 );

				if( lpObj->m_Quest[30] == 10)
				{
					lpObj->m_Quest[30] = 0;
				}
				if( lpObj->m_Quest[31] == 10 )
				{
					lpObj->m_Quest[31] = 0;
				}
				if( lpObj->m_Quest[32] == 10 )
				{
					lpObj->m_Quest[32] = 0;
				}
				if( lpObj->m_Quest[34] == 1 )
				{
					lpObj->m_Quest[34] = 0;
				}

				CLog.LogAdd(gQuest_Module, "[Quest] Quest Clear - MonsterKillCount Reset (%s) : [%s][%s]",
					lpQuestInfo->Name, lpObj->AccountID, lpObj->Name );

				CLog.LogAdd(gQuest_Module, "[Quest] Quest Clear (%s) : [%s][%s] Stat(%d,%d), Class:%d PlusStat",
					lpQuestInfo->Name, lpObj->AccountID, lpObj->Name, lpObj->LevelUpPoint, lpSubInfo->RewardCount, lpObj->DbClass );
			}

			if ( lpSubInfo->QuestType == 1 )
			{
				gObjDelteItemCountInInventory(lpObj->m_Index, lpSubInfo->NeedType, lpSubInfo->NeedSubType,
					lpSubInfo->NeedNumber);
			}
		}
	}
	return TRUE;
}
// ------------------------------
void CQuestInfo::QuestInfoSave(LPOBJ lpObj)
{
	int questcount = this->GetQeustCount();
	int foundquest = 0;
	LPQUEST_INFO lpQuestInfo;

	for ( int i = 0 ; i<MAX_QUEST_INFO;i++)
	{
		lpQuestInfo = this->GetQuestInfo(i);

		if ( lpQuestInfo == NULL )
		{
			continue;
		}

		CLog.LogAdd(gQuest_Module, "[Quest] QuestSave [%s][%s] index(%d) name(%s) state(%s)", lpObj->AccountID, lpObj->Name, i, lpQuestInfo->Name, sQuestString[this->GetQuestState(lpObj, i)]);
		foundquest++;

		if ( foundquest == questcount )
		{
			break;
		}
	}
	if (lpObj->m_Quest[0] == 0xF6 || lpObj->m_Quest[0] == 0xDA )
	{
		CLog.LogAdd(gQuest_Module, "[Quest] Save MonsterKillInfo [%s][%s] (%d/10, %d/10, %d/10, %d/1)", lpObj->AccountID, lpObj->Name, lpObj->m_Quest[30], lpObj->m_Quest[31], lpObj->m_Quest[32], lpObj->m_Quest[34]);
	}
}
// ------------------------------
int CQuestInfo::GetQuestKillCount(LPOBJ lpObj, int uClass)
{
	if (lpObj->m_Quest[1] == 0xF6 || lpObj->m_Quest[1] == 0xDA)
	{
		return lpObj->m_Quest[(uClass%100)-5];
	}
	else
	{
		return FALSE;
	}
}

// ------------------------------
// New Functions....
// ------------------------------
int CQuestInfo::QuestKillMonster(OBJECTSTRUCT* lpTargetObj, OBJECTSTRUCT* lpObj)//100% 1.00.90 Emulated 004AECB0
{
	if( lpObj->Connected < PLAYER_PLAYING )
	{
		if( lpObj->Live != 0 )
		{
			return 0;
		}
	}

	int iPartyNumber = lpObj->PartyNumber;

	if( iPartyNumber < 0 )
	{
		/*if( lpObj->m_ThirdQuestDBIndex == -1 )
		{
			return 0;
		}*/

		for( int n = 0; n < 5; n++ )
		{
			// Check if the target objects class is the quest condition mosnter
			/*if( lpTargetObj->Class == lpObj->m_ThirdQuestCondition[n].MonsterID )
			{
				// Check if the mosnter kill count is less than or equal to 50
				if( lpObj->m_ThirdQuestCondition[n].MonsterKillCount <= 50 )
				{
					// Increase the kill count
					lpObj->m_ThirdQuestCondition[n].MonsterKillCount += 1;
					break;
				}
			}*/
		}
	}
	else
	{
		int partynumber = lpObj->PartyNumber;
		int iPartyMember;
		OBJECTSTRUCT* lpPartyIndex;
		int iCalcDistance = 0;

		for( int iPartyCount = 0; iPartyCount < MAX_PARTY_MEMBER; iPartyCount++ )
		{
			iPartyMember = gParty.m_PartyInfo[partynumber].Number[iPartyCount];

			if( iPartyMember < 0 )
			{
				continue;
			}

			lpPartyIndex = &gObj[iPartyMember];

			if( lpPartyIndex->Connected < PLAYER_PLAYING )
			{
				if( lpPartyIndex->Live != 0 )
				{
					continue;
				}
			}

			if( lpObj->MapNumber != lpPartyIndex->MapNumber )
			{
				continue;
			}

			iCalcDistance = gObjCalDistance(lpObj, lpPartyIndex);

			if( iCalcDistance > 25 )
			{
				continue;
			}

			/*if( lpPartyIndex->m_ThirdQuestDBIndex == -1 )
			{
				continue;
			}

			for( int i = 0; i < 5; i++ ) 
			{
				// Check if the target objects class is the quest condition mosnter
				if( lpTargetObj->Class == lpPartyIndex->m_ThirdQuestCondition[i].MonsterID )
				{
					// Check if the mosnter kill count is less than or equal to 50
					if( lpPartyIndex->m_ThirdQuestCondition[i].MonsterKillCount <= 50 )
					{
						// Increase the kill count
						lpPartyIndex->m_ThirdQuestCondition[i].MonsterKillCount += 1;
						break;
					}
				}
			}*/
		}
	}

	return 1;
}

struct MONSTER_KILL_MSG 
{
	struct PBMSG_HEAD2 h;			// 00
	unsigned char Result;			// 03
	unsigned char m_QuestIndex;	// 05
	int m_QuestRequireKill[10];	// 08
};

int CQuestInfo::QuestKillMonsterInfo(int QuestIndex, int aIndex)//100% 1.00.90 Emulated
{
	MONSTER_KILL_MSG pMsg;

	C1SubHeadSet((unsigned char*)&pMsg, 0xA4, 0, sizeof(pMsg));
	pMsg.Result = 0;
	pMsg.m_QuestIndex = QuestIndex;

	OBJECTSTRUCT* lpObj = &gObj[aIndex];

	int iQuestState = this->GetQuestState(lpObj, QuestIndex);

	if( iQuestState != 1 )
	{
		return 1;
	}

	QUEST_INFO* iQuestInfo = this->GetQuestInfo(QuestIndex);

	if( iQuestInfo == 0 )
	{
		return 1;
	}

	int iSubInfoCount = iQuestInfo->QuestSubInfoCount;
	int iQuestConditionCount = iQuestInfo->QuestConditionCount;
	QUEST_SUB_INFO* iSubQuestInfo;

	int SubInfoCounter = 0;

	for( int n = 0; n < iSubInfoCount; n++ )
	{
		iSubQuestInfo = this->GetSubquestInfo(lpObj, iQuestInfo, n);

		if( iSubQuestInfo != 0 )
		{
			SubInfoCounter = 1;

			/*if( iSubQuestInfo->QuestType == 2 )
			{
				if( lpObj->m_NeedRequestUserQMK == 0 )
				{
					g_QuestMonsterKill.SendQuestKillInfo(lpObj);
					return 0;
				}

				if( lpObj->m_NeedRequestUserQMK == 1 )
				{
					if( lpObj->m_ThirdQuestDBIndex == -1 )
					{
						lpObj->m_NeedRequestUserQMK = 0;
						g_QuestMonsterKill.SendQuestKillInfo(lpObj);
						CLog.LogAdd(gQuest_Module, "[Quest] Error - Invalid MonsterKillInfo [%s][%s] (QuestIndex:%d/DBIndex:%d)", lpObj->AccountID, lpObj->Name, QuestIndex, lpObj->m_ThirdQuestDBIndex);
						return 0;
					}
				}

				pMsg.Result = 1;
				pMsg.m_QuestIndex = lpObj->m_ThirdQuestDBIndex;

				int iKillCount = 0;

				for( int i = 0; i < 5; i++ )
				{
					pMsg.m_QuestRequireKill[iKillCount] = lpObj->m_ThirdQuestCondition[i].MonsterID;
					iKillCount++;
					pMsg.m_QuestRequireKill[iKillCount] = lpObj->m_ThirdQuestCondition[i].MonsterKillCount;
					iKillCount++;
				}

				break;
			}*/
		}
	}

	if( pMsg.Result == 1 )
	{
		gSendProto.DataSend(aIndex, (unsigned char*)&pMsg, pMsg.h.size);
	}

	return 1;
}

struct THIRD_QUEST_NPC
{
	struct PBMSG_HEAD2 h;	// 00
	unsigned char Result;	// 04
};

// Used to teleport to barracks from werewolf - Review Here, Maybe we can change something
void CQuestInfo::QuestWereWolfMove(int aIndex)
{
	OBJECTSTRUCT* lpObj = &gObj[aIndex];

	if( lpObj == NULL )
	{
		return;
	}

	THIRD_QUEST_NPC pMsg;
	C1SubHeadSet((unsigned char*)&pMsg, 0xD0, 0x07, sizeof(pMsg));
	pMsg.Result = 0;

	int iQuestPrice = 3000000;
	int iQuestLevel = 350;

	if( lpObj->Level < 350 || lpObj->Money < iQuestPrice )
	{
		pMsg.Result = 0;
		gSendProto.DataSend(aIndex, (unsigned char*)&pMsg, pMsg.h.size);
		return;
	}

	int iGetQuestState = this->GetQuestState(lpObj, 5);

	if( iGetQuestState == 1 || iGetQuestState == 2 )
	{
		OBJECTSTRUCT* lpPartyObj;
		int iPartyNumber = lpObj->PartyNumber;
		int iPartyMember;
		int iCalcDistance;
		int iMoveGate = 0;

		if( iPartyNumber >= 0 )
		{
			for( int PartyCount = 0; PartyCount < MAX_PARTY_MEMBER; PartyCount++ )
			{
				iPartyMember = gParty.m_PartyInfo[iPartyNumber].Number[PartyCount];

				if( iPartyMember < 0 )
				{
					continue;
				}

				if( aIndex == iPartyMember )
				{
					continue;
				}

				lpPartyObj = &gObj[iPartyMember];

				if( lpPartyObj->Level < iQuestLevel )
				{
					continue;
				}

				if( lpObj->MapNumber == lpPartyObj->MapNumber )
				{
					iCalcDistance = gObjCalDistance(lpObj, lpPartyObj);

					if( iCalcDistance < 10 )
					{
						iMoveGate = gObjMoveGate(iPartyMember, 256);

						if( iMoveGate == 0 )
						{
							pMsg.Result = 0;
							gSendProto.DataSend(iPartyMember, (unsigned char*)&pMsg, pMsg.h.size);
						}
					}
				}
			}
		}

		iMoveGate = gObjMoveGate(aIndex, 256);

		if( iMoveGate != 0 )
		{
			lpObj->Money -= iQuestPrice;
			GCMoneySend(lpObj->m_Index, lpObj->Money);
			return;
		}

		pMsg.Result = 0;
		gSendProto.DataSend(aIndex, (unsigned char*)&pMsg, pMsg.h.size);
	}
}

// Moving to Refuge from gate Keeper - Review Here, Maybe we can change something
void CQuestInfo::QuestGateKeeperMove(int aIndex)
{
	OBJECTSTRUCT* lpObj = &gObj[aIndex];

	if( lpObj == NULL )
	{
		return;
	}

	THIRD_QUEST_NPC pMsg;
	C1SubHeadSet((unsigned char*)&pMsg, 0xD0, 0x08, sizeof(pMsg));
	pMsg.Result = 0;

	int iQuestLevel = 350;

	if( lpObj->Level < iQuestLevel )
	{
		pMsg.Result = 0;
		gSendProto.DataSend(aIndex, (unsigned char*)&pMsg, pMsg.h.size);
		return;
	}

	OBJECTSTRUCT* lpPartyObj;
	int iPartyNumber = lpObj->PartyNumber;
	int iPartyMember;
	int iCalcDistance;
	int iMoveGate = 0;

	if( iPartyNumber >= 0 )
	{
		for( int PartyCount = 0; PartyCount < MAX_PARTY_MEMBER; PartyCount++ )
		{
			iPartyMember = gParty.m_PartyInfo[iPartyNumber].Number[PartyCount];

			if( iPartyMember < 0 )
			{
				continue;
			}

			if( aIndex == iPartyMember )
			{
				continue;
			}

			lpPartyObj = &gObj[iPartyMember];

			if( lpPartyObj->Level < iQuestLevel )
			{
				continue;
			}

			if( lpObj->MapNumber == lpPartyObj->MapNumber )
			{
				iCalcDistance = gObjCalDistance(lpObj, lpPartyObj);

				if( iCalcDistance < 10 )
				{
					iMoveGate = gObjMoveGate(iPartyMember, 257);

					if( iMoveGate == 0 )
					{
						pMsg.Result = 0;
						gSendProto.DataSend(iPartyMember, (unsigned char*)&pMsg, pMsg.h.size);
						return;
					}
				}
			}
		}
	}

	iMoveGate = gObjMoveGate(aIndex, 257);

	if( iMoveGate == 0 )
	{
		pMsg.Result = 0;
		gSendProto.DataSend(aIndex, (unsigned char*)&pMsg, pMsg.h.size);
	}
}

int CQuestInfo::QuestItemGetRequest(int aIndex, unsigned short QuestItem, unsigned short Level)
{
	LPOBJ lpObj = &gObj[aIndex];
	QUEST_INFO* lpQuestInfo;
	QUEST_SUB_INFO* lpSubQuestInfo;
	int iSubQuestInfo;

	int iQuestState = 3;
	
	for (int i = 0; i < m_QuestCount; ++i)
	{
		iQuestState = GetQuestState(lpObj, i);

		if (iQuestState == 1)
		{
			lpQuestInfo = GetQuestInfo(i);

			if (lpQuestInfo == 0)
			{
				return 0;
			}

			iSubQuestInfo = lpQuestInfo->QuestSubInfoCount;

			for (int i = 0; i < iSubQuestInfo; ++i)
			{
				lpSubQuestInfo = GetSubquestInfo(lpObj, lpQuestInfo, i);

				if (lpSubQuestInfo == 0)
				{
					continue;
				}

				if (lpSubQuestInfo->QuestType != 1)
				{
					return 0;
				}

				if (QuestItem == ITEMGET(lpSubQuestInfo->NeedType, lpSubQuestInfo->NeedSubType)) 
				{
					if (Level == lpSubQuestInfo->ItemLevel)
					{
						int iItemCount = gObjGetItemCountInIventory(aIndex, QuestItem);

						if (iItemCount >= lpSubQuestInfo->NeedNumber)
						{
							CLog.LogAdd(gQuest_Module, "[Quest] Too many has quest items [%s][%s] (%d/%d)", lpObj->AccountID, 
								lpObj->Name, QuestItem, iItemCount);

							return 0;
						}

						return 1;
					}
				}
			}

			return 0;
		}
	}

	return 0;
}