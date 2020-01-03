//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU CLogin Count Structures & Functions					//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef CLOGINCOUNT_H
#define CLOGINCOUNT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLoginCount
{

public:

	CLoginCount();
	~CLoginCount();

	void Init();
	void Add();
	void Delete();
	int Get();

private:

	int m_count;
};

#endif