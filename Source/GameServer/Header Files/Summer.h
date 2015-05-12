#ifndef _SUMMER_H_
#define _SUMMER_H_

void Summer_EventStart(void * lpParam);

class CSummer
{
public:
	CSummer();
	~CSummer();
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

extern CSummer gSummer;

#endif