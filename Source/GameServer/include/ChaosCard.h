#ifndef CHAOSCARD_H
#define CHAOSCARD_H

#include "User.h"

class CChaosCard
{
public:
	CChaosCard(void);
	~CChaosCard(void);
	// ----
	void ChaosCardMix(LPOBJ lpObj);
};

extern CChaosCard gChaosCard;

#endif