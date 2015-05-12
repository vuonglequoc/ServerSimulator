//---------------------------------------------------------------------------
// # Project:		HaRiBO MU GameServer - Supported Season 6
// # Company:		RealCoderZ MU Development © 2011
// # Description:	Game Map Class
// # Developed By:	WolF & M.E.S
//---------------------------------------------------------------------------
#ifndef MAPCLASS_H
#define MAPCLASS_H

#include "zzzitem.h"
#include "zzzpath.h"
#include "MapItem.h"

#define MAX_NUMBER_MAP 82
#define MAX_MAPITEM	300

#define MAX_MAP_RANGE(x) (((x)<0)?FALSE:((x)>MAX_NUMBER_MAP-1)?FALSE:TRUE)
#define MAX_ITEM_TYPE_RANGE(x) ( ((x)<0)?FALSE:((x)>MAX_MAPITEM-1)?FALSE:TRUE   )

enum MAP_INDEX
{
	MAP_INDEX_LORENCIA			= 0x0,
	MAP_INDEX_DUNGEON			= 0x1,
	MAP_INDEX_DEVIAS			= 0x2,
	MAP_INDEX_NORIA				= 0x3,
	MAP_INDEX_LOSTTOWER			= 0x4,
	MAP_INDEX_RESERVED			= 0x5,
	MAP_INDEX_BATTLESOCCER		= 0x6,
	MAP_INDEX_ATLANS			= 0x7,
	MAP_INDEX_TARKAN			= 0x8,
	MAP_INDEX_DEVILSQUARE		= 0x9,
	MAP_INDEX_ICARUS			= 0xa,
	MAP_INDEX_BLOODCASTLE1		= 0xb,
	MAP_INDEX_BLOODCASTLE2		= 0xc,
	MAP_INDEX_BLOODCASTLE3		= 0xd,
	MAP_INDEX_BLOODCASTLE4		= 0xe,
	MAP_INDEX_BLOODCASTLE5		= 0xf,
	MAP_INDEX_BLOODCASTLE6		= 0x10,
	MAP_INDEX_BLOODCASTLE7		= 0x11,
	MAP_INDEX_CHAOSCASTLE1		= 0x12,
	MAP_INDEX_CHAOSCASTLE2		= 0x13,
	MAP_INDEX_CHAOSCASTLE3		= 0x14,
	MAP_INDEX_CHAOSCASTLE4		= 0x15,
	MAP_INDEX_CHAOSCASTLE5		= 0x16,
	MAP_INDEX_CHAOSCASTLE6		= 0x17,
	MAP_INDEX_KALIMA1			= 0x18,
	MAP_INDEX_KALIMA2			= 0x19,
	MAP_INDEX_KALIMA3			= 0x1a,
	MAP_INDEX_KALIMA4			= 0x1b,
	MAP_INDEX_KALIMA5			= 0x1c,
	MAP_INDEX_KALIMA6			= 0x1d,
	MAP_INDEX_CASTLESIEGE		= 0x1e,
	MAP_INDEX_CASTLEHUNTZONE	= 0x1f,
	MAP_INDEX_DEVILSQUARE2		= 0x20,
	MAP_INDEX_AIDA				= 0x21,
	MAP_INDEX_CRYWOLF_FIRSTZONE	= 0x22,
	MAP_INDEX_CRYWOLF_SECONDZONE = 0x23,
	MAP_INDEX_KALIMA7			= 0x24,
	MAP_INDEX_KANTURU1			= 0x25,
	MAP_INDEX_KANTURU2			= 0x26,
	MAP_INDEX_KANTURU_BOSS		= 0x27,
	MAP_INDEX_SILENT			= 0x28,
	MAP_INDEX_BARRACK_BALGASS	= 0x29,
	MAP_INDEX_BALGASS_REFUGE	= 0x2a,
	MAP_INDEX_ILLUSION1			= 0x2d,
	MAP_INDEX_ILLUSION2			= 0x2e,
	MAP_INDEX_ILLUSION3			= 0x2f,
	MAP_INDEX_ILLUSION4			= 0x30,
	MAP_INDEX_ILLUSION5			= 0x31,
	MAP_INDEX_ILLUSION6			= 0x32,
	MAP_INDEX_ELBELAND			= 0x33,
	MAP_INDEX_BLOODCASTLE8		= 0x34,
	MAP_INDEX_CHAOSCASTLE7		= 0x35,
	MAP_INDEX_SWAMP_OF_CALMNESS = 0x38,
	MAP_INDEX_RAKLION			= 0x39,
	MAP_INDEX_RAKLION_BOSS		= 0x3a,
	MAP_INDEX_XMAS				= 0x3e,
	MAP_INDEX_VULCAN			= 0x3f,
	MAP_INDEX_VULCANROOM		= 0x40,
	MAP_INDEX_DOUBLEGOER1		= 65,
	MAP_INDEX_DOUBLEGOER2		= 66,
	MAP_INDEX_DOUBLEGOER3		= 67,
	MAP_INDEX_DOUBLEGOER4		= 68,
	MAP_INDEX_IMPERIAL1			= 69,
	MAP_INDEX_IMPERIAL2			= 70,
	MAP_INDEX_IMPERIAL3			= 71,
	MAP_INDEX_IMPERIAL4			= 72,
	MAP_INDEX_LORENMARKET		= 0x4F,
	MAP_INDEX_KARUTAN1			= 0x50,
	MAP_INDEX_KARUTAN2			= 0x51
};

class MapClass
{
public:
	// -----
	void SaveItemInfo();
	void GetLevelPos(short level, short& ox, short& oy);
	void GetMapPos(short Map, short& ox, short& oy);
	void GetRandomLengthPos(short& x, short& y, int length);
	void SearchStandAttr(short& x, short& y);
	BOOL AttrLoad(const char* filename);
	void LoadMapAttr(const char* filename, int MapNumber);
	BOOL CheckWall(int sx1, int sy1, int sx2, int sy2);
	BYTE CheckWall2(int sx1, int sy1, int sx2, int sy2);
	void ItemInit();
	int ItemGive(int aIndex, int item_num, bool bFailNotSend);
	int ItemDrop(int type, int level, float dur,  int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, DWORD number, int aIndex, int PetLevel, int PetExp, BYTE ItemEffectEx, BYTE * SocketOption);
	int MonsterItemDrop(int type, int level, float dur,  int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption,  int aIndex, DWORD number, BYTE ItemEffectEx, BYTE * SocketOption);
	int MoneyItemDrop(int money, int x, int y);
	void WeatherVariationProcess();
	BYTE GetWeather();
	void SetWeather(BYTE a_weather, BYTE a_variation);
	void StateSetDestroy();
	bool PathFinding2(int sx, int sy, int tx, int ty, PATH_t* a);
	bool PathFinding3(int sx, int sy, int tx, int ty, PATH_t* a);
	bool PathFinding4(int sx, int sy, int tx, int ty, PATH_t* a);
	void SetStandAttr(int x, int y);
	void ClearStandAttr(int x, int y);
	int GetStandAttr(int x, int y);
	BYTE GetAttr(int x, int y);

	void init()
	{
		this->m_attrbuf = 0;
		this->path = new PATH;
		this->m_width = 0;
		this->m_height = 0;
	}	// line : 134
	
	void free()
	{
		if ( this->m_attrbuf != 0 )
		{
			GlobalFree(this->m_attrbuf);
			this->m_attrbuf=0;
		}

		if ( this->path != 0 )
		{
			delete this->path;
			this->path = 0;
		}
	}

	MapClass();
	virtual ~MapClass();

public:

	BYTE m_Weather;
	BYTE m_WeatherVariation;
	DWORD m_WeatherTimer;
	DWORD m_NextWeatherTimer;
	LPBYTE m_attrbuf;
	int m_width;
	int m_height;
	PATH* path;
	class CMapItem m_cItem[MAX_MAPITEM];
	int m_ItemCount;
	RECT gRegenRect[MAX_NUMBER_MAP];
	int thisMapNumber;
};


bool	MapNumberCheck(int MapNumber);
bool	gObjPositionCheck(LPOBJ lpObj);
bool	gObjCheckTileArea(int aIndex, int x, int y, int dis);
bool	ExtentCheck(int x, int y, int w, int h);

#endif