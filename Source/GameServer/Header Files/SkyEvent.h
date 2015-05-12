#ifndef _SKYEVENT_H_
#define _SKYEVENT_H_

class CSkyEvent
{
public:
	CSkyEvent();
	~CSkyEvent();
	// ----
	void Init();
	// ----
	int Enabled;
};

extern CSkyEvent gSkyEvent;

#endif