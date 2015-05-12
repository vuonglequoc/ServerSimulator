#ifndef EVENTMANAGEMENT_H
#define EVENTMANAGEMENT_H

enum
{
	EVENT_ID_REDDRAGON		= 0,
	EVENT_ID_SKELETON		= 1,
	EVENT_ID_GOLDEN			= 2,
	EVENT_ID_WHITEMAGE		= 3,
	EVENT_ID_RAKLION		= 4,
	EVENT_ID_SANTA			= 5,
	EVENT_ID_HAPPYHOUR		= 6,
	EVENT_ID_BLUE			= 7,
	EVENT_ID_HAPPYCHAOS		= 8,
	EVENT_ID_HAPPYJEWEL		= 9,
	EVENT_ID_HAPPYPOUCH		= 10, 
	EVENT_ID_KILLER			= 11,	
	EVENT_ID_BANJO			= 12,  
	EVENT_ID_SUMMER			= 13,
	EVENT_ID_MOSS			= 14,
};

struct EVENT_ID_SETTINGS
{
	char EventKind;
	char EventMonth;
	char EventDayMonth;
	char EventDayWeek;
	char Hour;
	char Minute;
};

class CEventManagement
{
public:
	CEventManagement(void);
	~CEventManagement(void);
	// ----
	void Init();
	void Load();
	void Clear();
	void Run();
	void StartEvent(int eEventKind);
	void RegEvent(int eEventKind, void* lpEventObj);
	// ----
	char Enabled;
	int EventsCount;
};

extern CEventManagement gEventManager;

#endif