// TMonsterAIRule.cpp: implementation of the TMonsterAIRule class.
//	GS-N	2.00.05	JPN	0x00562270	-	Completed
//////////////////////////////////////////////////////////////////////

#include "../Header Files/stdafx.h"
#include "../Header Files/TMonsterAIRule.h"


#include "../Header Files/ReadScript.h"

#include "../Header Files/LogProc.H"



BOOL TMonsterAIRule::s_bDataLoad = FALSE;
int TMonsterAIRule::s_iMonsterCurrentAIUnitTable[MAX_MONSTER_AI_RULE_TABLE];
TMonsterAIRuleInfo TMonsterAIRule::s_MonsterAIRuleInfoArray[MAX_MONSTER_AI_RULE_INFO];
int TMonsterAIRule::s_iMonsterAIRuleInfoCount;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMonsterAIRule::TMonsterAIRule()
{
	this->DelAllAIRule();
}

TMonsterAIRule::~TMonsterAIRule()
{
	return;
}




BOOL TMonsterAIRule::LoadData(const char* lpszFileName)
{
	TMonsterAIRule::s_bDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		CLog.MsgBox("[Monster AI Rule] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		SMDToken Token;
		SMDFile = fopen(lpszFileName, "r");

		if ( SMDFile == NULL )
		{
			CLog.MsgBox("[Monster AI Rule] - Can't Open %s ", lpszFileName);
			return FALSE;
		}

		TMonsterAIRule::DelAllAIRule();
		int iType = -1;

		while ( true )
		{
			Token = GetToken();

			if ( Token == END )
				break;

			iType = (int)TokenNumber;

			while ( true )
			{
				if ( iType == 0 )
				{
					int iAIRuleNumber = -1;
					int iMonsterClass = -1;
					TMonsterAIRuleInfo MonsterAIRuleInfo;

					Token = GetToken();

					if ( !strcmp("end", TokenString))
						break;

					iAIRuleNumber = (int)TokenNumber;

					Token = GetToken();
					iMonsterClass = (int)TokenNumber;

					Token = GetToken();
					memcpy(MonsterAIRuleInfo.m_szRuleDesc, TokenString, sizeof(MonsterAIRuleInfo.m_szRuleDesc));

					Token = GetToken();
					MonsterAIRuleInfo.m_iMonsterAIUnit = (int)TokenNumber;

					Token = GetToken();
					MonsterAIRuleInfo.m_iRuleCondition = (int)TokenNumber;

					Token = GetToken();
					MonsterAIRuleInfo.m_iWaitTime = (int)TokenNumber;

					Token = GetToken();
					MonsterAIRuleInfo.m_iContinuanceTime = (int)TokenNumber;

					Token = GetToken();
					MonsterAIRuleInfo.m_iMonth = (int)TokenNumber;

					Token = GetToken();
					MonsterAIRuleInfo.m_iDay = (int)TokenNumber;

					Token = GetToken();
					MonsterAIRuleInfo.m_iWeekDay = (int)TokenNumber;

					Token = GetToken();
					MonsterAIRuleInfo.m_iHour = (int)TokenNumber;

					Token = GetToken();
					MonsterAIRuleInfo.m_iMinute = (int)TokenNumber;

					if ( iAIRuleNumber  < 0 || iAIRuleNumber >= MAX_MONSTER_AI_RULE_INFO )
					{
						CLog.MsgBox("[Monster AI Rule] - AIRuleNumber(%d) Error (%s) File. ",
							iAIRuleNumber, lpszFileName);

						continue;
					}

					if ( iMonsterClass  < 0 || iMonsterClass >= MAX_MONSTER_AI_RULE_TABLE )
					{
						CLog.MsgBox("[Monster AI Rule] - MonsterClass(%d) Error (%s) File. ",
							iMonsterClass, lpszFileName);

						continue;
					}

					MonsterAIRuleInfo.m_iRuleNumber = iAIRuleNumber;
					MonsterAIRuleInfo.m_iMonsterClass = iMonsterClass;

					memcpy(&TMonsterAIRule::s_MonsterAIRuleInfoArray[TMonsterAIRule::s_iMonsterAIRuleInfoCount], 
						&MonsterAIRuleInfo,
						sizeof(MonsterAIRuleInfo));

					TMonsterAIRule::s_iMonsterAIRuleInfoCount++;
				}
			}
		}

		fclose(SMDFile);

		CLog.LogAddC(TColor.Red(), "[Monster AI Rule ] - %s file is Loaded", lpszFileName);

		TMonsterAIRule::s_bDataLoad = TRUE;
	}
	catch(DWORD)
	{
		CLog.MsgBox("[Monster AI Rule] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return FALSE;
}



BOOL TMonsterAIRule::DelAllAIRule()
{
	for ( int i=0;i<MAX_MONSTER_AI_RULE_INFO;i++)
	{
		TMonsterAIRule::s_MonsterAIRuleInfoArray[i].Reset();
	}

	for ( int j=0;j<MAX_MONSTER_AI_RULE_TABLE;j++)
	{
		TMonsterAIRule::s_iMonsterCurrentAIUnitTable[j] = 0;
	}

	TMonsterAIRule::s_iMonsterAIRuleInfoCount = 0;

	return FALSE;
}


int TMonsterAIRule::GetCurrentAIUnit(int iMonsterClass)
{
	if ( iMonsterClass < 0 || iMonsterClass >= MAX_MONSTER_AI_RULE_TABLE )
	{
		CLog.LogAdd("[Monster AI Rule] GetCurrentAIUnit() Error - (MonsterClass=%d) ", iMonsterClass);
		return 0;
	}

	return TMonsterAIRule::s_iMonsterCurrentAIUnitTable[iMonsterClass];
}




void TMonsterAIRule::MonsterAIRuleProc()
{
	int iChangedMonsterClassList[MAX_MONSTER_AI_RULE_INFO] = {-1};
	int iChangedAIUnitList[MAX_MONSTER_AI_RULE_INFO] = {0};
	int iChangedListCount = 0;
	
	for (int i=0;i<TMonsterAIRule::s_iMonsterAIRuleInfoCount;i++)
	{
		TMonsterAIRuleInfo & RuleInfo = TMonsterAIRule::s_MonsterAIRuleInfoArray[i];

		if ( RuleInfo.m_iMonsterClass < 0 || RuleInfo.m_iMonsterClass >= MAX_MONSTER_AI_RULE_TABLE )
			return;

		if ( RuleInfo.IsValid() )
		{
			iChangedMonsterClassList[iChangedListCount] = RuleInfo.m_iMonsterClass;
			iChangedAIUnitList[iChangedListCount] = RuleInfo.m_iMonsterAIUnit;
			iChangedListCount++;
		}
		else
		{
			TMonsterAIRule::s_iMonsterCurrentAIUnitTable[RuleInfo.m_iMonsterClass] = 0;
		}
	}

	for(int j=0;j<iChangedListCount;j++)
	{
		TMonsterAIRule::s_iMonsterCurrentAIUnitTable[iChangedMonsterClassList[j]] = iChangedAIUnitList[j];
	}
}
