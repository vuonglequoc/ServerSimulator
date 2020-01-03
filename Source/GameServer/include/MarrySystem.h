#ifndef MARRYSYSTEM_H
#define MARRYSYSTEM_H

#include "User.h"

class CMarry
{
public:
	CMarry(void);
	~CMarry(void);
	// ----
	void Init();
	bool IsMarried(LPOBJ lpObj);
	bool AreMarried(LPOBJ lpObj,LPOBJ lpTargetObj);
	bool IsSameGender(LPOBJ lpObj,LPOBJ lpTargetObj);
	void MarryRequest(LPOBJ lpObj,LPOBJ lpTargetObj);
	void MarryAgree(LPOBJ lpObj);
	void MarryRefuse(LPOBJ lpObj);
	// ----
	char Enabled;
	char OnlyMarryDifGender;
	unsigned char MarryX[2];
	unsigned char MarryY[2];

};

extern CMarry gMarry;

#endif