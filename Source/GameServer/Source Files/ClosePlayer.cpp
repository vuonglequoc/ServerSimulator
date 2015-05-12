//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Close Player Structures & Functions					//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "../Header Files/stdafx.h"
#include "../Header Files/ClosePlayer.h"
#include "../Header Files/closeplayer.h"
#include "../Header Files/ReadScript.h"
#include "../Header Files/LogProc.H"
#include "../Header Files/user.h"
#include "../Test/Network.H"
#include "../Header Files/GameMain.h"


Ccloseplayer closeplayer;

Ccloseplayer::Ccloseplayer()
{
	return;	
}

Ccloseplayer::~Ccloseplayer()
{
	return;
}

void Ccloseplayer::Load(char* filename)
{
	int Token; 
	int aIndex;
	char closeplayerid[50];
	SMDFile=fopen(filename, "r");

	if (SMDFile == 0)
	{
		CLog.LogAdd(lMsg.Get(MSGGET(1, 213)), filename);
	}
	else
	{
		aIndex=0;
		while ( true )
		{
			Token=GetToken();
			if (Token == 2)
			{
				break;
			}
			memset(closeplayerid, 0, sizeof(closeplayerid) );
			strcpy(&closeplayerid[0], TokenString);

			if ( strlen(closeplayerid) > 0 )
			{
				aIndex=gObjGetIndex(closeplayerid);	
				if ( aIndex >= 0 )
				{
					CLog.LogAdd(lMsg.Get(MSGGET(1, 191)), closeplayerid);
					gNet.CloseClient(aIndex);
				}
			}
		}
		fclose(SMDFile);
	}
}