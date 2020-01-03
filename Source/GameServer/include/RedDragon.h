#ifndef _REDDRAGON_H_
#define _REDDRAGON_H_

void RedDragon_EventStart(void * lpParam);

class CRedDragon
{
public:
	CRedDragon();
	~CRedDragon();
	// ----
	void Init();
	void Run();
	void Start();
	void End();
	void DeleteMonsters();
	void AddMonsters();
	void SendInvasionEffect();
	// ----
	int Enabled;
	int Started;
	int DurationTime;
	int AuxDurationTime;
	int MaxDragons;
	int MonsterIndex[50];
	int MonsterCount;
	int MapNumber;
};

extern CRedDragon gRedDragon;

#endif