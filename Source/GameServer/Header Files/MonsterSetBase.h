//**********************************************************************//
// Project:	HaRiBO MU GameServer - Season 6								//
// Company: RealCoderZ MU Development © 2010							//
// Description: MU MonsterSetBase Functions & Structures				//
// Coded & Modified By:	WolF & M.E.S									//
//**********************************************************************//
#ifndef MONSTERSETBASE_H
#define MONSTERSETBASE_H

#include "user.h"


typedef struct MONSTER_POSITION
{
	unsigned char m_ArrangeType;
	unsigned short m_Type;
	unsigned char m_MapNumber;
	unsigned char m_Dis;
	unsigned char m_X;
	unsigned char m_Y;
	unsigned char m_Dir;
	unsigned char m_W;
	unsigned char m_H;

} MONSTER_POSITION, * LPMONSTER_POSITION;
class CMonsterSetBase
{

public:

	void LoadSetBase(char* Buffer, int iSize);
	void LoadSetBase(char* filename);

	CMonsterSetBase();
	virtual ~CMonsterSetBase();

	void SetBoxPosition(int TableNum, int mapnumber, int ax, int ay, int aw, int ah);
	BOOL GetPosition(int TableNum, short MapNumber, short &x, short &y);
	BOOL GetBoxPosition(int mapnumber, int ax, int ay, int aw, int ah, short &mx, short &my);


	//void SetBoxPosition(int TableNum, BYTE mapnumber, int ax, int ay, int aw, int ah);
	//int GetPosition(int TableNum, BYTE MapNumber, unsigned char& x, unsigned char& y);
	//int GetBoxPosition(int mapnumber, int ax, int ay, int aw, int ah, unsigned char& mx, unsigned char& my);
	

public:

  MONSTER_POSITION m_Mp[OBJ_MAXMONSTER];	// 4
  int m_Count;	// 10FE4


};

#endif