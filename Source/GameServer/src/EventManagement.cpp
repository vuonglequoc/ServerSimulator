#include "stdafx.h"
#include "EventManagement.h"
#include "Defines.h"
#include "LogProc.H"
#include "GoldenEvent.h"
#include "RingAttackEvent.h"
#include "Raklion.h"
#include "Summer.h"
#include "SkyEvent.h"
#include "SantaEvent.h"
#include "UnderWorld.h"
#include "RedDragon.h"
#include "ReadScript.h"
#include "Notice.H"
#include "HappyPouch.h"
#include "HappyHour.h"
#include "BlueEvent.h"
#include "MossMerchant.h"

CEventManagement gEventManager;
EVENT_ID_SETTINGS EventSettings[1000];

CEventManagement::CEventManagement(void)
{
	return;
}

CEventManagement::~CEventManagement(void)
{
	return;
}

void CEventManagement::Clear()
{

}

void CEventManagement::Init()
{
	gEventManager.Enabled			= GetPrivateProfileInt(COMMON_HDR,"EnableEventManager",0,COMMON_FILE_PATH);

	if ( gEventManager.Enabled == 1 )
	{
		gEventManager.EventsCount = 0;
		gEventManager.Load();
		CLog.LogAdd("[EventManager]: Init -> Enabled. Events Count: %d",gEventManager.EventsCount);
	}
	else
	{
		CLog.LogAdd("[EventManager]: Init -> Disabled");
	}
}

void CEventManagement::Run()
{
	SYSTEMTIME t;
	GetLocalTime(&t);
	for ( int i=0;i<gEventManager.EventsCount;i++)
	{
		// ---- Month Check
		if ( EventSettings[i].EventMonth != -1 )
		{
			if ( EventSettings[i].EventMonth < 1 || EventSettings[i].EventMonth > 12 )
			{
				continue;
			}
			if ( EventSettings[i].EventMonth != t.wMonth )
			{
				continue;
			}
		}
		//---- Month Day Check
		if ( EventSettings[i].EventDayMonth != -1 )
		{
			if ( EventSettings[i].EventDayMonth < 1 || EventSettings[i].EventDayMonth > 31 )
			{
				continue;
			}
			if ( t.wMonth == 2 && EventSettings[i].EventDayMonth > 28 )
			{
				continue;
			}
			if ( EventSettings[i].EventDayMonth != t.wDay )
			{
				continue;
			}
		}
		// ---- Day of Week Check
		if ( EventSettings[i].EventDayWeek != -1 )
		{
			if ( EventSettings[i].EventDayWeek < 0 || EventSettings[i].EventDayWeek > 6 )
			{
				continue;
			}
			if ( EventSettings[i].EventDayWeek != t.wDayOfWeek )
			{
				continue;
			}
		}
		// ---- Hour Check
		if ( EventSettings[i].Hour == 24 )
		{
			EventSettings[i].Hour = 0;
		}
		if ( EventSettings[i].Hour < 0 || EventSettings[i].Hour > 23 )
		{
			continue;
		}
		if ( EventSettings[i].Hour != t.wHour )
		{
			continue;
		}
		// ---- Minute Check
		if ( EventSettings[i].Minute == 60 )
		{
			EventSettings[i].Minute = 0;
		}
		if ( EventSettings[i].Minute < 0 || EventSettings[i].Minute > 59 )
		{
			continue;
		}
		if ( EventSettings[i].Minute != t.wMinute )
		{
			continue;
		}
		// ---- Event Run
		gEventManager.StartEvent(EventSettings[i].EventKind);
	}
}

void CEventManagement::StartEvent(int eEventKind)
{
	switch ( eEventKind )
	{
	case EVENT_ID_REDDRAGON:
		gRedDragon.Start();
		break;
	case EVENT_ID_SKELETON:
		gUnderworld.Start();
		break;
	case EVENT_ID_GOLDEN:
		gGoldenEvent.Start();
		break;
	case EVENT_ID_WHITEMAGE:
		gRingAttackEvent.Start();
		break;
	case EVENT_ID_RAKLION:
		gRaklion.Start();
		break;
	case EVENT_ID_SANTA:
		gSantaEvent.Start();
		break;
	case EVENT_ID_HAPPYHOUR:
		break;
	case EVENT_ID_BLUE:
		gBlueEvent.Start();
		break;
	case EVENT_ID_HAPPYCHAOS:
		break;
	case EVENT_ID_HAPPYJEWEL:
		break;
	case EVENT_ID_SUMMER:
		gSummer.Start();
		break;
	case EVENT_ID_HAPPYPOUCH:
		gHappyPouch.Start();
		break;
	case EVENT_ID_MOSS:
		gMossMerchant.Start();
		break;
	}
}

void CEventManagement::Load()
{
	SMDFile = fopen(EVENT_MANAGEMENT, "r");

	if ( SMDFile == NULL )
	{
		return;
	}

	int Token;

	while ( true )
	{
		Token = GetToken();

		if ( Token == 2 )
		{
			break;
		}
		if ( Token == 1 )
		{
			EventSettings[gEventManager.EventsCount].EventKind = TokenNumber;

			Token = GetToken();
			EventSettings[gEventManager.EventsCount].EventMonth = TokenNumber;

			Token = GetToken();
			EventSettings[gEventManager.EventsCount].EventDayMonth = TokenNumber;

			Token = GetToken();
			EventSettings[gEventManager.EventsCount].EventDayWeek = TokenNumber;

			Token = GetToken();
			EventSettings[gEventManager.EventsCount].Hour = TokenNumber;

			Token = GetToken();
			EventSettings[gEventManager.EventsCount].Minute = TokenNumber;

			gEventManager.EventsCount++;
		}
	}
	fclose(SMDFile);
}