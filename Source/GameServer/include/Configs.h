/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Project:		Zodea MU GameServer - Season 6
 * Developer:	RealCoderZ MU Development © 2011
 * Hint:		Main Configs For Zodea GameServer
 * Coded By:	WolF & mesosa
 */
#ifndef CONFIGS_HEADER
#define CONFIGS_HEADER

#include "Structures.H"


class MainConfig
{
public:
	MainConfig();
	virtual ~MainConfig();
	// -----
	void	Initialize();
	void	ReadConnectInfo	( LPSTR FileName );
	void	ReadCommandInfo ( LPSTR FileName );
};
extern MainConfig MCfg;



#endif