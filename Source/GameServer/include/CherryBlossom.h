#ifndef _CHERRYBLOSSOM_H_
#define _CHERRYBLOSSOM_H_

#include "User.h"

class CCherryBlossom
{
public:
	CCherryBlossom(void);
	~CCherryBlossom(void);
	// ----
	void Init();
	void ReadFile();
	// ----
	unsigned char Enabled;
	// ----
	unsigned char ItemType[3][255];
	unsigned char ItemIndex[3][255];
	unsigned char Durability[3][255];
	unsigned char ItemLuck[3][255];
	unsigned char ItemSkill[3][255];
	unsigned char ItemExe[3][255];
	unsigned char ItemAnc[3][255];
	unsigned char ItemLevel[3][255];
	unsigned char ItemOpt[3][255];
	unsigned char ItemSocket[5][3][255];
	int ItemCount[3];
};

extern CCherryBlossom gCherryBlossom;

#endif