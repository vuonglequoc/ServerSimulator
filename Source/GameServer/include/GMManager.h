/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Project:		Zodea MU GameServer - Season 6
 * Developer:	RealCoderZ MU Development © 2011
 * Hint:		GM Manager For Zodea GameServer
 * Coded By:	WolF & mesosa
 */
#ifndef GM_MANAGER_HEADER
#define GM_MANAGER_HEADER

#include "StdAfx.H"


struct GMSTRUCT
{
	char	GMName[11];
	int		ClearPK;
	int		PostMSG;
	int		CreateItems;
	int		OnlineChar;
	int		Fireworks;
	int		SetPK;
	int		AddPoints;
	int		Disconnect;
	int		SetZen;
	int		AddZen;
	int		Skin;
	int		SpawnMob;
	int		Status;
	int		SetLevel;
	int		SetLvlUpPoints;
	int		AddBuff;
	int		Summon;
	int		Marriage;
	int		MoveAll;
	int		CreateSets;
	int		BlockChat;
	int		CharStatistics;
	int		BlockCommands;
	int		BanAccount;
	int		BlockChar;
	int		GMMove;
	int		Trace;
	int		DropEvent;
};
// -------------------------------------------------------------------------------------------------------------------------------------------------------
class CGMManager
{
public:
	CGMManager();
	virtual ~CGMManager();
	// -----
	void	Init();
	void	LoadGMMngFile(LPSTR filename);
	int		AddGameMaster(char* name, int aIndex);
	void	DelGameMaster(char* name);
	// -----
	char	EnableGMManager;
	int		GMMngCount;
	// -----
private:
	char	GM_Name[11];
	int		Command[28];
	char	szManagerName[5][11];
	int		ManagerIndex[5];
};
extern CGMManager	cGMMng;
extern GMSTRUCT		GMSTR[50];
// -------------------------------------------------------------------------------------------------------------------------------------------------------
#endif
// -------------------------------------------------------------------------------------------------------------------------------------------------------