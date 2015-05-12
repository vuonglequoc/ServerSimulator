//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Kanturu Maya Structure & Functions					//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef KANTURU_MAYA_H
#define KANTURU_MAYA_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKanturuMaya  
{
public:
	CKanturuMaya();
	virtual ~CKanturuMaya();

	void Init();
	void KanturuMayaAct_IceStorm(int iSkillUsingRate);
	void KanturuMayaAct_Hands();
	void SetMayaObjIndex(int iIndex){this->m_iMayaObjIndex = iIndex;}
private:

	int m_iMayaObjIndex;
	BOOL m_bMonsterKillCount;
	int m_iMayaSkillTime;
	int m_iIceStormCount;

};


#endif
