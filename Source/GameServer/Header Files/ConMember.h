//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Connect Member Structures & Functions				//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef CONMEMBER_H
#define CONMEBMER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CConMember
{
public:

	CConMember();
	virtual ~CConMember();

	void Init();
	BOOL IsMember(LPSTR AccountID);
	void Load(const char* filename);

private:
	
	std::map<std::string, int> m_szAccount;

};



extern CConMember ConMember;

#endif