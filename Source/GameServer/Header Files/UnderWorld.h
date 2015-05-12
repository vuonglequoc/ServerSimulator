#ifndef _UNDERWORLD_H_
#define _UNDERWORLD_H_

#include "User.h"

void UnderWorld_EventStart(void * lpParam);

class CUnderworld
{
public:
	CUnderworld();
	~CUnderworld();
	// ----
	void Init();
	void Start();
	void End();
	void DeleteMonsters();
	void MonstersAdd();
	bool DropCheck(LPOBJ lpObj);
	// ----
	char Enabled;
	char Started;
	int DurationTime;
	int AuxDurationTime;
	short MonsterIndex[50];
	short MonsterCount;
	// ----
	unsigned char BossPosX;
	unsigned char BossPosY;
};

extern CUnderworld gUnderworld;

#endif