//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU SkillAdditionInfo Structures & Functions				//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef SKILL_ADDITION_INFO_H
#define SKILL_ADDITION_INFO_H

#include "StdAfx.h"

#pragma once

class CSkillAdditionInfo
{
public:
	CSkillAdditionInfo();
	virtual ~CSkillAdditionInfo();
	// ------
	void	Init();
	int		Load(LPSTR lpszFileName);
	void	SetInfinityArrowSkillTime(int iValue){this->m_iInfinityArrowSkillTime = iValue;}
	int		GetInfinityArrowSkillTime(){return this->m_iInfinityArrowSkillTime;}
	void	SetInfinityArrowUseLevel(int iValue){this->m_iInfinityArrowUseLevel = iValue;}
	int		GetInfinityArrowUseLevel(){return this->m_iInfinityArrowUseLevel;}
	void	SetInfinityArrowMPConsumptionPlus0(int iValue){this->m_iInfinityArrowMPConsumptionPlus0 = iValue;}
	int		GetInfinityArrowMPConsumptionPlus0(){return this->m_iInfinityArrowMPConsumptionPlus0;}
	void	SetInfinityArrowMPConsumptionPlus1(int iValue){this->m_iInfinityArrowMPConsumptionPlus1 = iValue;}
	int		GetInfinityArrowMPConsumptionPlus1(){return this->m_iInfinityArrowMPConsumptionPlus1;}
	void	SetInfinityArrowMPConsumptionPlus2(int iValue){this->m_iInfinityArrowMPConsumptionPlus2 = iValue;}
	int		GetInfinityArrowMPConsumptionPlus2(){return this->m_iInfinityArrowMPConsumptionPlus2;}
	void	SetFireScreamSkill(int iValue){this->m_bFireScreamSkill = iValue;}
	BOOL	GetFireScreamSkill(){return this->m_bFireScreamSkill;}
	void	SetFireScreamExplosionAttackDistance(int iValue){this->m_iFireScreamExplosionAttackDistance = iValue;}
	int		GetFireScreamExplosionAttackDistance(){return this->m_iFireScreamExplosionAttackDistance;}
	void	SetFireScreamExplosionRate(int iValue){this->m_iFireScreamExplosionRate = iValue;}
	int		GetFireScreamExplosionRate(){return this->m_iFireScreamExplosionRate;}
	void	SetFireScreamMaxAttackCountSameSerial(int iValue){this->m_iFireScreamMaxAttackCountSameSerial = iValue;}
	int		GetFireScreamMaxAttackCountSameSerial(){return this->m_iFireScreamMaxAttackCountSameSerial;}
	void	SetFireScreamSerialInitCount(int iValue){this->m_iFireScreamSerialInitCount = iValue;}
	int		GetFireScreamSerialInitCount(){return this->m_iFireScreamSerialInitCount;}
	void	SetCheckFireScreamSkill(int iValue){this->m_bCheckFireScreamSerial = iValue;}
	BOOL	GetCheckFireScreamSkill(){return this->m_bCheckFireScreamSerial;}
	// ------
private:
	int		m_iInfinityArrowSkillTime;
	int		m_iInfinityArrowUseLevel;
	int		m_iInfinityArrowMPConsumptionPlus0;
	int		m_iInfinityArrowMPConsumptionPlus1;
	int		m_iInfinityArrowMPConsumptionPlus2;
	BOOL	m_bFireScreamSkill;
	int		m_iFireScreamExplosionAttackDistance;
	int		m_iFireScreamExplosionRate;
	int		m_iFireScreamMaxAttackCountSameSerial;
	int		m_iFireScreamSerialInitCount;
	BOOL	m_bCheckFireScreamSerial;
};
extern CSkillAdditionInfo g_SkillAdditionInfo;


#endif