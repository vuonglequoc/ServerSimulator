//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU SkillAdditionInfo Structures & Functions				//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "stdafx.h"
#include "SkillAdditionInfo.H"
#include "LogProc.H"
#include "GameMain.H"
#include "Defines.h"


CSkillAdditionInfo g_SkillAdditionInfo;
//-------------------------------------------------------------------------------------------------------------------------------------------
CSkillAdditionInfo::CSkillAdditionInfo()
{
	this->Init();
}
//-------------------------------------------------------------------------------------------------------------------------------------------
CSkillAdditionInfo::~CSkillAdditionInfo()
{
	return;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void CSkillAdditionInfo::Init()
{
	this->m_iInfinityArrowSkillTime				= 0;
	this->m_iInfinityArrowUseLevel				= 0;
	this->m_iInfinityArrowMPConsumptionPlus0	= 0;
	this->m_iInfinityArrowMPConsumptionPlus1	= 0;
	this->m_iInfinityArrowMPConsumptionPlus2	= 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------
BOOL CSkillAdditionInfo::Load(LPSTR lpszFileName)
{
	if( lpszFileName == NULL || strcmp(lpszFileName, "") == NULL )
	{
		CLog.LogAdd("[Skill Addition Info] - File load error : File Name Error");
		return FALSE;
	}

	this->m_iInfinityArrowSkillTime				= GetPrivateProfileInt(SKILL_INFO_HDR, "InfinityArrowSkillTime", 600, lpszFileName); 
	this->m_iInfinityArrowUseLevel				= GetPrivateProfileInt(SKILL_INFO_HDR, "InfinityArrowUseLevel", 220, lpszFileName); 
	this->m_iInfinityArrowMPConsumptionPlus0	= GetPrivateProfileInt(SKILL_INFO_HDR, "InfinityArraowMPConsumptionPlus0", 0, lpszFileName);
	this->m_iInfinityArrowMPConsumptionPlus1	= GetPrivateProfileInt(SKILL_INFO_HDR, "InfinityArraowMPConsumptionPlus1", 0, lpszFileName);
	this->m_iInfinityArrowMPConsumptionPlus2	= GetPrivateProfileInt(SKILL_INFO_HDR, "InfinityArraowMPConsumptionPlus2", 0, lpszFileName);


	CLog.LogAdd("[Skill Addition Info] [Infinity Arrow] Skill Time[%d], UseLevel[%d], Arrow MP Consumption +0[%d] +1[%d] +2[%d]",
		this->m_iInfinityArrowSkillTime, this->m_iInfinityArrowUseLevel,
		this->m_iInfinityArrowMPConsumptionPlus0, this->m_iInfinityArrowMPConsumptionPlus1,
		this->m_iInfinityArrowMPConsumptionPlus2);

	this->m_bFireScreamSkill					= GetPrivateProfileInt(SKILL_INFO_HDR,"FireScreamSkill", 1, lpszFileName);
	this->m_iFireScreamExplosionAttackDistance	= GetPrivateProfileInt(SKILL_INFO_HDR,"FireScreamExplosionAttackDistance", 1, lpszFileName);
	this->m_iFireScreamExplosionRate			= GetPrivateProfileInt(SKILL_INFO_HDR,"FireScreamExplosionRate", 300, lpszFileName);
	this->m_iFireScreamMaxAttackCountSameSerial = GetPrivateProfileInt(SKILL_INFO_HDR,"FireScreamMaxAttackCountSameSerial", 3, lpszFileName);
	this->m_iFireScreamSerialInitCount			= GetPrivateProfileInt(SKILL_INFO_HDR,"FireScreamSerialInitCount", 100, lpszFileName);
	this->m_bCheckFireScreamSerial				= GetPrivateProfileInt(SKILL_INFO_HDR,"CheckFireScreamSerial", 1, lpszFileName);


	CLog.LogAdd("[Skill Addition Info] [Fire Scream] Use(%d) : Explosion Distance(%d) - Rate(%d), MaxAttack_SameSerial(%d), SerialInitCount(%d), Check Serial(%d)",
		this->m_bFireScreamSkill, this->m_iFireScreamExplosionAttackDistance,
		this->m_iFireScreamExplosionRate, this->m_iFireScreamMaxAttackCountSameSerial,
		this->m_iFireScreamSerialInitCount, this->m_bCheckFireScreamSerial);

	return TRUE;
}
//-------------------------------------------------------------------------------------------------------------------------------------------