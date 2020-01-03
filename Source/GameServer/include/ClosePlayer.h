//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Close Player Structures & Functions					//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef CLOSEPLAYER_H
#define CLOSEPLAYER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Ccloseplayer
{
public:

	Ccloseplayer();
	virtual ~Ccloseplayer();

	void Load(char* filename);

private:

	
};
extern Ccloseplayer closeplayer;

#endif