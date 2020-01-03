#ifndef PKSYSTEM_H
#define PKSYSTEM_H

#include "User.h"

class CPKSystem
{
public:
	CPKSystem(void);
	~CPKSystem(void);
	// ----
	void Init();
	void PKDownTime(LPOBJ lpObj, int TargetLevel);
	void KillPlayer(LPOBJ lpObj,LPOBJ lpTargetObj);
	// ----
	char Enabled;
	char ItemDrop;
	char IncreaseLevel;
	char HeroSystem;
	char MurderSystem;
	int PKTime;
};

extern CPKSystem gPKSystem;

#endif