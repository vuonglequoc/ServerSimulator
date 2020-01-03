//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU SkillDelay Functions & Structures					//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef SKILLDELAY_H
#define SKILLDELAY_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSkillDelay
{

public:

	CSkillDelay();
	virtual ~CSkillDelay();

	void Init();
	BOOL Check(WORD skill);

private:

	DWORD LastSkillUseTime[256];	// 4
};

#endif