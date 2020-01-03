//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU SkillDelay Functions & Structures					//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "stdafx.h"
#include "SkillDelay.h"


CSkillDelay::CSkillDelay()
{
	this->Init();
}


CSkillDelay::~CSkillDelay()
{
	return;
}


void CSkillDelay::Init()
{
	memset(this->LastSkillUseTime, 0, sizeof(this->LastSkillUseTime) );
}


#pragma warning ( disable : 4101 )
int CSkillDelay::Check(WORD skill)
{
	int skilldelaytime;
	DWORD dwtime;
	return 1;
}
#pragma warning ( default : 4101 )