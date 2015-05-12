/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Project:		Zodea MU GameServer - Season 6
 * Developer:	RealCoderZ MU Development © 2011
 * Hint:		GM Manager For Zodea GameServer
 * Coded By:	WolF & mesosa
 */
#include "../Header Files/stdafx.h"
#include "../Header Files/GameMain.H"


CGMManager	cGMMng;
GMSTRUCT	GMSTR[50];
// -------------------------------------------------------------------------------------------------------------------------------------------------------
CGMManager::CGMManager()
{
	this->EnableGMManager = TRUE;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
CGMManager::~CGMManager()
{
	return;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CGMManager::Init()
{
	for( int n=0; n<MAX_GM_IN_ACCOUNT; n++ )
	{
		this->ManagerIndex[n] = -1;
	}
	// -----
	this->LoadGMMngFile( RFile.szRFileName[35] );
	// -----
	//if ( Unknown592 == false ) ExitProcess(0);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CGMManager::LoadGMMngFile(LPSTR filename)
{
	FILE *	OpFile;
	char	sLineTxt[255]	= {0};
	// -----
	GMMngCount	= 1;
	// -----
	if ( (OpFile = fopen(filename,"r")) == NULL )
	{
		EnableGMManager = FALSE;
		return;
	}
	// -----
	rewind(OpFile);
	// -----
	while ( fgets(sLineTxt, 255, OpFile) != NULL )
	{
		if ( sLineTxt[0] == '/' ) continue;  
		if ( sLineTxt[0] == ';' ) continue; 
		// -----
		sscanf(sLineTxt, "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", GM_Name, &Command[0], &Command[1],
			&Command[2], &Command[3], &Command[4], &Command[5], &Command[6], &Command[7], &Command[8], &Command[9], &Command[10], &Command[11],
			&Command[12], &Command[13], &Command[14], &Command[15], &Command[16], &Command[17], &Command[18], &Command[19], &Command[20],
			&Command[21], &Command[22], &Command[23], &Command[24], &Command[25], &Command[26], &Command[27]);
		// -----
		sprintf(GMSTR[GMMngCount].GMName,"%s",GM_Name);
		GMSTR[GMMngCount].ClearPK			= Command[0];
		GMSTR[GMMngCount].PostMSG			= Command[1];
		GMSTR[GMMngCount].CreateItems		= Command[2];
		GMSTR[GMMngCount].OnlineChar		= Command[3];
		GMSTR[GMMngCount].Fireworks			= Command[4];
		GMSTR[GMMngCount].SetPK				= Command[5];
		GMSTR[GMMngCount].AddPoints			= Command[6];
		GMSTR[GMMngCount].Disconnect		= Command[7];
		GMSTR[GMMngCount].SetZen			= Command[8];
		GMSTR[GMMngCount].AddZen			= Command[9];
		GMSTR[GMMngCount].Skin				= Command[10];
		GMSTR[GMMngCount].SpawnMob			= Command[11];
		GMSTR[GMMngCount].Status			= Command[12];
		GMSTR[GMMngCount].SetLevel			= Command[13];
		GMSTR[GMMngCount].SetLvlUpPoints	= Command[14];
		GMSTR[GMMngCount].AddBuff			= Command[15];
		GMSTR[GMMngCount].Summon			= Command[16];
		GMSTR[GMMngCount].Marriage			= Command[17];
		GMSTR[GMMngCount].MoveAll			= Command[18];
		GMSTR[GMMngCount].CreateSets		= Command[19];
		GMSTR[GMMngCount].BlockChat			= Command[20];
		GMSTR[GMMngCount].CharStatistics	= Command[21];
		GMSTR[GMMngCount].BlockCommands		= Command[22];
		GMSTR[GMMngCount].BanAccount		= Command[23];
		GMSTR[GMMngCount].BlockChar			= Command[24];
		GMSTR[GMMngCount].GMMove			= Command[25];
		GMSTR[GMMngCount].Trace				= Command[26];
		GMSTR[GMMngCount].DropEvent			= Command[27];
		GMMngCount++;
	}
	// -----
	rewind(OpFile);
	fclose(OpFile);
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
int CGMManager::AddGameMaster(char * name, int aIndex)
{
	for ( int n=0 ; n<MAX_GM_IN_ACCOUNT ; n++ )
	{
		if ( this->ManagerIndex[n] == -1 )
		{
			strcpy(this->szManagerName[n], name);
			this->ManagerIndex[n] = aIndex;
			return n;
		}
	}

	return -1;
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------
void CGMManager::DelGameMaster(char * name)
{
	for ( int n=0;n<MAX_GM_IN_ACCOUNT;n++)
	{
		if ( this->ManagerIndex[n] != -1 )
		{
			if ( strcmp(this->szManagerName[n], name) == 0 )
			{
				this->ManagerIndex[n] = -1;
				break;
			}
		}
	}
}
// -------------------------------------------------------------------------------------------------------------------------------------------------------