#ifndef _HAPPYPOUCH_H_
#define _HAPPYPOUCH_H_

void HappyPouch_EventStart(void * lpParam);

class CHappyPouch
{
public:
	CHappyPouch();
	~CHappyPouch();
	// ----
	void Init();
	void Start();
	void Clear();
	void AddMonsters();
	void RemoveMonsters();
	// ----
	int Enabled;
	int Duration;
	int MaxGhost;
	int AuxDuration;
	int Started;
	int MonsterIndex[200];
	int MonsterCount;
};

extern CHappyPouch gHappyPouch;

#endif