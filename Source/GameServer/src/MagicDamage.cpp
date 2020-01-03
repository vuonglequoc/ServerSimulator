//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Class Magic Damage Information & Structures			//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "stdafx.h"
#include "MagicDamage.h"
#include "ReadScript.h"
#include "WzMemScript.h"
#include "LogProc.H"
#include "GameMain.h"
#include "User.h"
#include "MasterSkillSystem.h"


CMagicDamage MagicDamageC;
// -----------------------------------------------------------------------------------------------------------------------
CMagicDamage::CMagicDamage(void)
{
	return;
}
// -----------------------------------------------------------------------------------------------------------------------
CMagicDamage::~CMagicDamage(void)
{
	return;
}
// -----------------------------------------------------------------------------------------------------------------------
void CMagicDamage::Init()
{
	for ( int n=0; n< MAX_SKILL;n++)
	{
		this->m_Damage[n] = -1;
	}
}
// -----------------------------------------------------------------------------------------------------------------------
// Reformulated
void CMagicDamage::LogSkillList(char * filename)
{
	this->Init();

	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		CLog.MsgBox(lMsg.Get(MSGGET(1, 199)), filename);
		return;
	}

	int Token;
	int n=0;
	int number;
	char name[50];
	int requireLevel;
	int damage;
	int mana;
	int bp=0;
	int dis;
	int rEnergy;
	int iSkillType;
	int rLeadership;
	int rDelay;
	int attr1;
	int attr2;
	int iSkillUseType;
	int iSkillBrand;
	int iKillCount;
	int RequireStatus[MAX_REQ_SKILL_STATUS] = {0};
	BYTE RequireClass[MAX_CLASS_TYPE];

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}
		if ( Token == 1 )
		{
			number = TokenNumber;

			Token = GetToken();
			strcpy(name, TokenString);

			Token = GetToken();
			requireLevel = TokenNumber;

			Token = GetToken();
			damage = TokenNumber;

			Token = GetToken();
			mana = TokenNumber;

			Token = GetToken();
			bp = TokenNumber;

			Token = GetToken();
			dis = TokenNumber;

			Token = GetToken();
			rDelay = TokenNumber;

			Token = GetToken();
			rEnergy = TokenNumber;

			Token = GetToken();
			rLeadership = TokenNumber;

			Token = GetToken();
			attr1 = TokenNumber;

			Token = GetToken();
			attr2 = TokenNumber;

			Token = GetToken();
			iSkillUseType = TokenNumber;

			Token = GetToken();
			iSkillBrand = TokenNumber;

			Token = GetToken();
			iKillCount = TokenNumber;

			Token = GetToken();
			RequireStatus[0] = TokenNumber;

			Token = GetToken();
			RequireStatus[1] = TokenNumber;

			Token = GetToken();
			RequireStatus[2] = TokenNumber;

			Token = GetToken();
			RequireClass[0] = TokenNumber;

			Token = GetToken();
			RequireClass[1] = TokenNumber;

			Token = GetToken();
			RequireClass[2] = TokenNumber;

			Token = GetToken();
			RequireClass[3] = TokenNumber;

			Token = GetToken();
			RequireClass[4] = TokenNumber;

			Token = GetToken();
			RequireClass[5] = TokenNumber;

			Token = GetToken();
			RequireClass[6] = TokenNumber;

			Token = GetToken();
			iSkillType = TokenNumber;

			this->Set(name, number,damage, requireLevel, mana, bp, dis, attr1, attr2, RequireClass, rEnergy, rLeadership, rDelay, iSkillType);
			this->SetEx(number, iSkillUseType, iSkillBrand, iKillCount, RequireStatus);
			DefMagicInf[number].Set(number, 0);
		}
	}
	fclose(SMDFile);
}
// -----------------------------------------------------------------------------------------------------------------------
void CMagicDamage::LogSkillNameList(char* filename)
{
	int Token;
	int n;
	int number;
	char name[50];
	
	SMDFile = fopen(filename, "r");

	if ( SMDFile == NULL )
	{
		CLog.MsgBox(lMsg.Get(MSGGET(1, 199)), filename);
		return;
	}

	n=0;

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}

		if ( Token == 1 )
		{
			number = TokenNumber;

			Token = GetToken();
			strcpy(name, TokenString);

			strcpy(this->m_Name[number], name);
		}
	}

	fclose(SMDFile);
}
// -----------------------------------------------------------------------------------------------------------------------
// Reformulated
void CMagicDamage::LogSkillList(char* Buffer, int iSize)
{
	this->Init();
	CWzMemScript WzMemScript;

	WzMemScript.SetBuffer(Buffer, iSize);

	int Token;
	int n=0;
	int number;
	char name[50];
	int requireLevel;
	int damage;
	int mana;
	int bp=0;
	int dis;
	int rEnergy;
	int iSkillType;
	int rLeadership;
	int rDelay;
	int attr1;
	int attr2;
	int iSkillUseType;
	int iSkillBrand;
	int iKillCount;
	int RequireStatus[MAX_REQ_SKILL_STATUS] = {0};
	BYTE RequireClass[MAX_CLASS_TYPE];

	while ( true )
	{
		Token = WzMemScript.GetToken();

		if ( Token == 2 )
		{
			break;
		}
		if ( Token == 1 )
		{
			number = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			strcpy(name, WzMemScript.GetString() );

			Token = WzMemScript.GetToken();
			requireLevel = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			damage = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			mana = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			bp = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			dis = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			rDelay = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			rEnergy = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			rLeadership = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			attr1 = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			attr2 = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			iSkillUseType = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			iSkillBrand = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			iKillCount = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			RequireStatus[0] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			RequireStatus[1] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			RequireStatus[2] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			RequireClass[0] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			RequireClass[1] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			RequireClass[2] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			RequireClass[3] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			RequireClass[4] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			RequireClass[5] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			RequireClass[6] = WzMemScript.GetNumber();

			Token = WzMemScript.GetToken();
			iSkillType = WzMemScript.GetNumber();
			
			this->Set(name, number,damage, requireLevel, mana, bp, dis, attr1, attr2, RequireClass, rEnergy, rLeadership, rDelay, iSkillType);
			this->SetEx(number, iSkillUseType, iSkillBrand, iKillCount, RequireStatus);
			DefMagicInf[number].Set(number, 0);

		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
void CMagicDamage::LogSkillNameList(char* Buffer, int iSize)
{
	CWzMemScript WzMemScript;
	int Token;
	int n;
	int number;
	char name[50];
	
	WzMemScript.SetBuffer(Buffer, iSize);

	n=0;

	while ( true )
	{
		Token = WzMemScript.GetToken();

		if ( Token == 2 )
		{
			break;
		}

		if ( Token == 1 )
		{
			number = TokenNumber;	// #error Aply Deathway Fix

			Token = WzMemScript.GetToken();	
			strcpy(name, TokenString);	// #error Deathway Fix

			strcpy(this->m_Name[number], name);
		}
	}
}
// -----------------------------------------------------------------------------------------------------------------------
void CMagicDamage::SetEx(int iSkill, int iSkillUseType, int iSkillBrand, int iKillCount, int * pReqStatus)
{
	if ( iSkill <0 || iSkill > MAX_SKILL -1 )
	{
		CLog.MsgBox(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return;
	}

	if ( this->m_Damage[iSkill] == -1 )
	{
		CLog.MsgBox(lMsg.Get(MSGGET(2, 50)), __FILE__, __LINE__);
		return;
	}

	this->m_iSkillUseType[iSkill] = iSkillUseType;
	this->m_iSkillBrand[iSkill] = iSkillBrand;
	this->m_iKillCount[iSkill] = iKillCount;
	memcpy(this->m_iRequireStatus[iSkill], pReqStatus, sizeof(this->m_iRequireStatus[0]));	// #error Deathway Fix
}
// -----------------------------------------------------------------------------------------------------------------------
// Reformulated
void CMagicDamage::Set(char* name, int skill, int damage,  int rlevel,  int mana,  int bp,  int dis, int Attr1,  int Attr2, unsigned char* ReqClass, int rEnergy, int rLeadership, int rdelay, int iSkillType)
{
	if ( skill <0 || skill > MAX_SKILL -1 )
	{
		CLog.MsgBox(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return;
	}

	if ( this->m_Damage[skill] != -1 )
	{
		CLog.MsgBox(lMsg.Get(MSGGET(2, 50)), __FILE__, __LINE__);
		return;
	}

	strcpy(this->m_Name[skill], name);
	this->m_Damage[skill] = damage;
	this->m_rLevel[skill] = rlevel;
	this->m_Mana[skill] = mana;
	this->m_Distance[skill] = dis;
	this->m_RequireLeadership[skill] = rLeadership;
	this->m_Delay[skill] = rdelay;
	this->m_BrainPower[skill] = bp;
	this->m_Attr1[skill] = Attr1;
	this->m_Attr2[skill] = Attr2;
	this->m_iSkillType[skill] = iSkillType;
	memcpy(this->m_RequireClass[skill], ReqClass, sizeof(this->m_RequireClass[0]));
	this->m_RequireEnergy[skill] = (rEnergy * rlevel * 4) / 100 + 20; // Old
	//this->m_RequireEnergy[skill] = rEnergy; // New
}
// -----------------------------------------------------------------------------------------------------------------------
int CMagicDamage::SkillGet(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return this->m_Damage[0];
	}

	return this->m_Damage[skill];
}
// -----------------------------------------------------------------------------------------------------------------------
int CMagicDamage::SkillGetMana(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return this->m_Mana[0];
	}

	return this->m_Mana[skill];
}
// -----------------------------------------------------------------------------------------------------------------------
int CMagicDamage::SkillGetBP(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return FALSE;
	}

	return this->m_BrainPower[skill];
}
// -----------------------------------------------------------------------------------------------------------------------
int CMagicDamage::SkillGetRequireEnergy(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return -1;
	}

	return this->m_RequireEnergy[skill];
}
// -----------------------------------------------------------------------------------------------------------------------
BOOL CMagicDamage::SkillGetRequireClass(int aIndex, int skill)
{
	LPOBJ lpObj = &gObj[aIndex];
	// ----
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		//CLog.LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__); // Class Not Allowed to use Skill
		return -1;
	}

	if ( lpObj->Class < 0 || lpObj->Class > MAX_CLASS_TYPE-1 )
	{
		//CLog.LogAdd(lMsg.Get(MSGGET(1, 202)), __FILE__, __LINE__); // Class Type Error
		return -1;
	}

	int	requireclass = this->m_RequireClass[skill][lpObj->Class];
	
	if (requireclass >= 3 )
	{
		if ( gUser.gObjIsMasteringLevel(lpObj) == false )
		{
			return FALSE;
		}
	}
	else if (requireclass > 1)
	{
		if ( requireclass != (lpObj->ChangeUP+1) )
		{
			return FALSE;
		}
	}
	return TRUE;
}
// -----------------------------------------------------------------------------------------------------------------------
int CMagicDamage::GetSkillAttr1(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		return -1;
	}

	return this->m_Attr1[skill];
}
// -----------------------------------------------------------------------------------------------------------------------
int CMagicDamage::GetSkillAttr2(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		return -1;
	}

	return this->m_Attr2[skill];
}
// -----------------------------------------------------------------------------------------------------------------------
int CMagicDamage::CheckSkillAttr1(int skill, int attr)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		return FALSE;
	}

	return this->m_Attr1[skill];
}
// -----------------------------------------------------------------------------------------------------------------------
int CMagicDamage::CheckSkillAttr2(int skill, int attr)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		return FALSE;
	}

	return this->m_Attr2[skill];
}
// -----------------------------------------------------------------------------------------------------------------------
int CMagicDamage::GetskillRequireLeadership(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return -1;
	}

	return this->m_RequireLeadership[skill];
}
// -----------------------------------------------------------------------------------------------------------------------
int CMagicDamage::GetDelayTime(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return FALSE;
	}

	return this->m_Delay[skill];
}
// -----------------------------------------------------------------------------------------------------------------------
int CMagicDamage::GetSkillType(int iSkill)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return FALSE;
	}
	
	return this->m_iSkillType[iSkill];
}
// -----------------------------------------------------------------------------------------------------------------------
int CMagicDamage::SkillGetRequireLevel(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return -1;
	}

	return this->m_RequireLevel[skill];
}
// -----------------------------------------------------------------------------------------------------------------------
bool CMagicDamage::CheckStatus(int iSkill, int iGuildStatus)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return false;
	}

	if ( this->m_iSkillUseType[iSkill] == 0 )
	{
		return true;
	}

	if ( iGuildStatus == 0x80 )
	{
		if ( this->m_iRequireStatus[iSkill][0] == 1 )
		{
			return true;
		}
	}
	else if ( iGuildStatus == 0x40 )
	{
		if ( this->m_iRequireStatus[iSkill][1] == 1 )
		{
			return true;
		}
	}
	else if ( iGuildStatus == 0x20 )
	{
		if ( this->m_iRequireStatus[iSkill][2] == 1 )
		{
			return true;
		}
	}

	return false;
}
// -----------------------------------------------------------------------------------------------------------------------
bool CMagicDamage::CheckBrandOfSkill(int iSkill)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return false;
	}

	if ( this->m_iSkillBrand[iSkill] > 0 )
	{
		return true;
	}

	return false;
}
// -----------------------------------------------------------------------------------------------------------------------
int CMagicDamage::CheckKillCount(int iSkill, int iKillCount)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return -1;
	}

	return iKillCount - this->m_iKillCount[iSkill];
}
// -----------------------------------------------------------------------------------------------------------------------
int CMagicDamage::GetKillCount(int iSkill)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return -1;
	}

	return this->m_iKillCount[iSkill];
}
// -----------------------------------------------------------------------------------------------------------------------
int CMagicDamage::GetSkillDistance(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 201)), __FILE__, __LINE__);
		return -1;
	}

	return this->m_Distance[skill];
}
// -----------------------------------------------------------------------------------------------------------------------