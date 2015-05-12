#ifndef _HALLOWEEN_H_
#define _HALLOWEEN_H_

#include "User.h"

class CHalloween
{
public:
	CHalloween();
	~CHalloween();
	// ----
	void Init();
	void Start();
	// ----
	int Enabled;
};

extern CHalloween gHalloween;

#endif