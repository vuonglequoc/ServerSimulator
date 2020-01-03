/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Project:		Zodea MU GameServer - Season 6
 * Developer:	RealCoderZ MU Development © 2011
 * Hint:		Main Structures For Zodea GameServer
 * Coded By:	WolF & mesosa
 */
#ifndef STRUCTURES_HEADER
#define STRUCTURES_HEADER

struct MAIN_STRUCT
{
	int		NumOfLoginAttemps;
	int		NormalPoint[7];
	int		SecondQuestPointPlus[4];
	int		MasterPointsPlus[7];
};
extern MAIN_STRUCT MSTR;

#endif