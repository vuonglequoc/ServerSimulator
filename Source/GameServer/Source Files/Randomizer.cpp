//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 4								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU Randomizer Structures & Functions					//
// Decompilation By: Deathway											//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#include "../Header Files/stdafx.h"
#include <vector>
#include <algorithm>
#include "../Header Files/Randomizer.h"

Randomizer::Randomizer() 
{ 
	m_iRandMax = 0; 
	m_iGetCount = 0; 
	this->Create(0xFFFF); 
}

Randomizer::~Randomizer() 
{ 
	this->Destroy(); 
}

void Randomizer::Create( int iRandMax )
{
	m_iRandMax = iRandMax;
	for( int i = 1; i <= iRandMax; ++i )
	{
		m_vecRandom.push_back( i );
	}
	RandomShuffule();
}

void Randomizer::Destroy()
{
	m_vecRandom.clear();
}

inline int Randomizer::Rand()
{
	int retVal = m_vecRandom[m_iGetCount];
	if( ++m_iGetCount >= m_iRandMax ) 
	{	
		m_iGetCount = 0; 
		RandomShuffule(); 
	}
	return retVal;
}

int Randomizer::Rand( int min, int max )
{
	return ( Rand() % ( max + 1 - min ) ) + min;
}

void Randomizer::RandomShuffule()
{
	random_shuffle( m_vecRandom.begin(), m_vecRandom.end() );
}