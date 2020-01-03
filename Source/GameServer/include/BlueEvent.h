#ifndef _BLUEEVENT_H_
#define _BLUEEVENT_H_

void BlueEvent_EventStart(void * lpParam);

class CBlueEvent
{
public:
	CBlueEvent(void);
	~CBlueEvent(void);
	// ----
	void Init();
	void Start();
	void Clear();
	void AddMonsters();
	void RemoveMonsters();
	// ----
	char Enabled;
	int Duration;
	int AuxDuration;
	char Started;
	short MonsterIndex[200];
	short MonsterCount;
};

extern CBlueEvent gBlueEvent;

#endif