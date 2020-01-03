//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Randomizer Structures & Functions					//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#pragma once

#include <vector>
#include <algorithm>

typedef std::vector<int>		RANDOM_VEC;
typedef RANDOM_VEC::iterator	RANDOM_VEC_ITER;

class Randomizer
{
public:
	Randomizer();
	~Randomizer();

	void Create( int iRandMax );
	void Destroy();
	int Rand();
	int Rand( int min, int max );

private:
	inline void		RandomShuffule();
	RANDOM_VEC		m_vecRandom;
	int				m_iRandMax;
	int				m_iGetCount;
};
