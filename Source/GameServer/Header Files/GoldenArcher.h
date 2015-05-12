// ---------------------------------------------------------------- //
// Project:	APplE - Season 4 Episode II & Season 5 Project			//
// Coded by:	WolF & M.E.S										//
// Files:		GoldenArcher.H :: GoldenArcher.CPP					//
// Hint:		GoldenArcher Event									//
// ---------------------------------------------------------------- //

// --------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef GOLDENARCHER_H
#define GOLDENARCHER_H
// --------------------------------------------------------------------------------------------------------------------------------------------------
#include "StdAfx.H"
// --------------------------------------------------------------------------------------------------------------------------------------------------

class CGoldenArcher
{
public:
	CGoldenArcher();
	virtual ~CGoldenArcher();
	// ------
	void	Init(LPSTR szFileName);
	int		GetRenaCount(int PlayerID);
	void	PlayerDeleteQuestItem(int iIndex);
	void	NPCTalk_GoldenArcher(int PlayerID);
	void	RewardItem(int aIndex);
	void	ReadFile(std::vector<std::string> &buffer, const char *szFilename);
	int		ReqItem;
private:
	std::vector<std::string> ReadFiles;
};
// --------------------------------------------------------------------------------------------------------------------------------------------------
extern CGoldenArcher gGAEvent;
// --------------------------------------------------------------------------------------------------------------------------------------------------
#endif /* GOLDENARCHER_H */
// --------------------------------------------------------------------------------------------------------------------------------------------------